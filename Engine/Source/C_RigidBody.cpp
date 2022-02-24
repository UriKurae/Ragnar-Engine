#include "C_RigidBody.h"
#include "Application.h"
#include "Globals.h"

#include "Physics3D.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

#include "ImGui/imgui.h"
#include "IconsFontAwesome5.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "Math/float3x3.h"

#include "ModuleScene.h"

RigidBodyComponent::RigidBodyComponent(GameObject* obj, CollisionType type, float mass, bool isKinematic) : Component(), collisionType(type), mass(mass), isKinematic(isKinematic)
{
	owner = obj;
	this->type = ComponentType::RIGID_BODY;
	SetCollisionType(type);
	// Calculate offset CM
	if (owner->GetComponent<MeshComponent>())
	{
		float3 posBody = body->getCenterOfMassPosition();
		float3 posObj = owner->GetComponent<TransformComponent>()->GetGlobalTransform().Col3(3);
		offset = posBody - posObj;
		offset = quatRotate(body->getOrientation().inverse(), offset);
	}	
}

RigidBodyComponent::~RigidBodyComponent()
{
	if (body != nullptr)
	{
		app->physics->DeleteBody(this, owner->name);

		constraintBodies.clear();
	}	
}

void RigidBodyComponent::SetBoundingBox()
{
	float3 pos, radius, size;

	if (owner->GetComponent<MeshComponent>() == nullptr)
	{
		pos = owner->GetComponent<TransformComponent>()->GetGlobalTransform().Col3(3);
		radius = { 1,1,1 };
		size = { 1,1,1 };
	}
	else
	{
		OBB obb = owner->GetOOB();
		pos = obb.CenterPoint();
		radius = obb.r;
		size = obb.Size();
	}		
	
	switch (collisionType)
	{
	case CollisionType::BOX:
		box.FromRS(owner->GetComponent<TransformComponent>()->GetRotation(), {1,1,1});
		box.SetPos(pos);
		box.size = size;
		break;
	case CollisionType::SPHERE:
		sphere.FromRS(owner->GetComponent<TransformComponent>()->GetRotation(), { 1,1,1 });
		sphere.SetPos(pos);
		sphere.radius = radius.MaxElement();
		break;
	case CollisionType::CAPSULE:
		capsule.FromRS(owner->GetComponent<TransformComponent>()->GetRotation(), { 1,1,1 });
		capsule.SetPos(pos);
		capsule.radius = radius.MaxElementXZ();
		capsule.height = size.y;
		break;
	case CollisionType::CYLINDER:
		cylinder.FromRS(owner->GetComponent<TransformComponent>()->GetRotation(), { 1,1,1 });
		cylinder.SetPos(pos);
		cylinder.radius = radius.MaxElementXZ();
		cylinder.height = size.y;
		break;
	case CollisionType::CONE:
		cone.FromRS(owner->GetComponent<TransformComponent>()->GetRotation(), { 1,1,1 });
		cone.SetPos(pos);
		cone.radius = radius.MaxElementXZ();
		cone.height = size.y;
		break;
	case CollisionType::STATIC_PLANE:
		plane.FromRS(owner->GetComponent<TransformComponent>()->GetRotation(), { 1,1,1 });
		plane.SetPos(pos);
		break;
	default:
		break;
	}
}

bool RigidBodyComponent::Update(float dt)
{
	if (app->scene->GetGameState() == GameState::PLAYING)
	{
		if (body->getActivationState() == 1 || body->getActivationState() == 3)
		{
			float3 f = quatRotate(body->getOrientation(), offset);
			float4x4 CM2 = float4x4::FromTRS(body->getCenterOfMassPosition() - f, body->getWorldTransform().getRotation(), owner->GetComponent<TransformComponent>()->GetScale());
			owner->GetComponent<TransformComponent>()->SetTransform(CM2);
		}
	}

	return true;
}

void RigidBodyComponent::UpdateCollision()
{
	if (app->scene->GetGameState() != GameState::PLAYING)
	{
		OBB obb = owner->GetOOB();

		float3 f = quatRotate(owner->GetComponent<TransformComponent>()->GetRotation(), offset);
		btTransform t;
		t.setBasis(float3x3::FromQuat(owner->GetComponent<TransformComponent>()->GetRotation()));
		t.setOrigin(owner->GetComponent<TransformComponent>()->GetGlobalTransform().Col3(3) + f);
		
		body->setWorldTransform(t);
	}
}

void RigidBodyComponent::OnEditor()
{
	// Important condition: body != nullptr 
	// Because OnStop the body don't exist but OnPlay can modify her parameters
	if (ImGui::CollapsingHeader(ICON_FA_ATOM" Rigid Body", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Checkbox("Active    ", &active))
		{
			if (active) app->physics->ActivateCollision(body);
			else app->physics->DesactivateCollision(body);
		}
		ImGui::SameLine();
		static const char* collisions[] = { "Box", "Sphere", "Capsule", "Cylinder", "Cone", "Plane" };
		
		ImGui::PushItemWidth(85);
		int currentCollision = (int)collisionType;
		if (ImGui::Combo("Collision Type", &currentCollision, collisions, 6))
		{
			collisionType = (CollisionType)currentCollision;
			SetCollisionType(collisionType);		
		}
		ImGui::PopItemWidth();

		ImVec2 winSize = ImGui::GetWindowSize();
		winSize.x -= 130;

		ImGui::Text("Mass:        "); ImGui::SameLine();
		ImGui::PushItemWidth(winSize.x);
		if (ImGui::DragFloat("##Mass", &mass, 0.1f, 0.0f, INFINITE))
		{
			if(body->isStaticObject() && mass != 0.0f)
				CreateBody();
			if (mass != 0.f)
			{
				btVector3 inertia;
				body->getCollisionShape()->calculateLocalInertia(mass, inertia);
				body->setMassProps(mass, inertia);
			}
		}
		ImGui::PopItemWidth();

		ImGui::Text("Friction:    "); ImGui::SameLine();
		ImGui::PushItemWidth(winSize.x);
		if (ImGui::DragFloat("##Friction", &friction, 0.05f, 0.f, 1.f) && body)
			body->setFriction(friction);
		ImGui::PopItemWidth();

		ImGui::Text("Restitution: "); ImGui::SameLine();
		ImGui::PushItemWidth(winSize.x);
		if (ImGui::DragFloat("##Restitution", &restitution, 0.1f, 0.f, 1.f) && body)
			body->setRestitution(restitution);
		ImGui::PopItemWidth();

		if(ImGui::Checkbox("Use Gravity", &useGravity))
		{
			if (!useGravity && !isKinematic)
				SetAsStatic();
			else CreateBody();
		}
		if (ImGui::Checkbox("Is Kinematic", &isKinematic))
		{
			if (isKinematic)
			{
				body->setCollisionFlags(body->getFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
				if(app->scene->GetGameState() == GameState::PLAYING)
					body->setActivationState(DISABLE_DEACTIVATION);
			}
			else CreateBody();
		}

		Combos();		
	}
}

void RigidBodyComponent::Combos()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 colors = style.Colors[ImGuiCol_Border];

	ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));
	if (ImGui::CollapsingHeader("Collision mesh"))
	{
		ImGui::PushStyleColor(ImGuiCol_Border, colors);
		EditCollisionMesh();
		ImGui::PopStyleColor();
		ImGui::Separator();
	}
	if (ImGui::CollapsingHeader("Constraints"))
	{
		ImGui::PushStyleColor(ImGuiCol_Border, colors);

		ImGui::Text("Freeze Position:");
		if (ImGui::DragFloat3("##FreezePosition", &movementConstraint.x, 0.05f, 0.f, 1.f) && body)
			body->setLinearFactor(movementConstraint);

		ImGui::Text("Freeze Rotation:");
		if (ImGui::DragFloat3("##FreezeRotation", &rotationConstraint.x, 0.05f, 0.f, 1.f) && body)
			body->setAngularFactor(rotationConstraint);

		ImGui::NewLine();
		ImGui::Text("Add P2P:"); ImGui::SameLine();
		ImGui::PushItemWidth(150);
		if (ImGui::BeginCombo("##AddConstraint", "Select body"))
		{
			for (int i = 0; i < app->physics->GetBodiesNames().size(); i++)
			{
				if (body != app->physics->GetBodies().at(i)->body)
				{
					if (ImGui::Selectable(app->physics->GetBodiesNames().at(i).c_str()))
					{
						constraintBodies.push_back(app->physics->GetBodies().at(i));
						app->physics->GetBodies().at(i)->constraintBodies.push_back(this);
						AddConstraintP2P(app->physics->GetBodies().at(i));
					}
				}				
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		ImGui::Text("List of constraint P2P:");
		if (constraintBodies.size() == 0) ImGui::TextColored(ImVec4(1.00f, 1.00f, 1.00f, 0.50f),"-List is empty");
		char* name = "";
		for (int i = 0; i < constraintBodies.size(); i++)
		{
			//Subtitute of imgui_stdlib function. Copy the information of the given string to a char*.
			strncpy(name, constraintBodies.at(i)->owner->name.c_str(), sizeof(char*) * 1024);
			ImGui::InputText("##Name", name, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
		}

		ImGui::PopStyleColor();
		ImGui::Separator();
	}
	if (ImGui::CollapsingHeader("Damping"))
	{
		ImGui::PushStyleColor(ImGuiCol_Border, colors);

		ImGui::Text("Linear Damping:");
		if (ImGui::DragFloat("##LinearDamping", &linearDamping, 0.05f, 0.f, 1.f) && body)
			body->setDamping(linearDamping, angularDamping);

		ImGui::Text("Angular Damping:");
		if (ImGui::DragFloat("##AngularDamping", &angularDamping, 0.05f, 0.f, 1.f) && body)
			body->setDamping(linearDamping, angularDamping);

		ImGui::PopStyleColor();
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void RigidBodyComponent::AddConstraintP2P(RigidBodyComponent* const& val)
{	
	btVector3 center;
	float r1, r2;
	body->getCollisionShape()->getBoundingSphere(center, r1);
	val->GetBody()->getCollisionShape()->getBoundingSphere(center, r2);
	app->physics->AddConstraintP2P(*body, *val->GetBody(), float3(r1, r1, r1), float3(r2, r2, r2));
}

void RigidBodyComponent::SetCollisionType(CollisionType type)
{
	collisionType = type;
	SetBoundingBox();
	CreateBody();
	ResetLocalValues();
}

void RigidBodyComponent::ResetLocalValues()
{
	LOG(LogType::L_NORMAL, "%f, %f, %f",box.size[0], box.size[1], box.size[2]);
	box.size = { 1,1,1 };
	capsule.radius = 1;
	capsule.height = 2;
	cylinder.radius = 1;
	cylinder.height = 2;
	cone.radius = 1;
	cone.height = 2;
}

void RigidBodyComponent::EditCollisionMesh()
{
	switch (collisionType)
	{
	case CollisionType::BOX:
		ImGui::Text("Box Size: ");
		if (ImGui::DragFloat3("##Box", &box.size.x, 0.1f, 0.1f, INFINITE)) editMesh = true;
		break;
	case CollisionType::SPHERE:
		ImGui::Text("Radius: "); ImGui::SameLine();
		if (ImGui::DragFloat("##Radius", &sphere.radius, 0.1f, 0.1f, INFINITE)) editMesh = true;
		break;
	case CollisionType::CAPSULE:
		ImGui::Text("Radius: "); ImGui::SameLine();
		if (ImGui::DragFloat("##Radius", &capsule.radius, 0.1f, 0.1f, INFINITE)) editMesh = true;
		ImGui::Text("Height: "); ImGui::SameLine();
		if (ImGui::DragFloat("##Height", &capsule.height, 0.1f, 0.1f, INFINITE)) editMesh = true;
		break;
	case CollisionType::CYLINDER:
		ImGui::Text("Radius: "); ImGui::SameLine();
		if (ImGui::DragFloat("##Radius", &cylinder.radius, 0.1f, 0.1f, INFINITE)) editMesh = true;
		ImGui::Text("Height: "); ImGui::SameLine();
		if (ImGui::DragFloat("##Height", &cylinder.height, 0.1f, 0.1f, INFINITE)) editMesh = true;
		break;
	case CollisionType::CONE:
		ImGui::Text("Radius: "); ImGui::SameLine();
		if (ImGui::DragFloat("##Radius", &cone.radius, 0.1f, 0.1f, INFINITE)) editMesh = true;
		ImGui::Text("Height: "); ImGui::SameLine();
		if (ImGui::DragFloat("##Height", &cone.height, 0.1f, 0.1f, INFINITE)) editMesh = true;
		break;
	case CollisionType::STATIC_PLANE:
		ImGui::Text("Normal: ");
		if (ImGui::DragFloat3("##Normal", &plane.normal.x, 0.1f, 0.0f, 1))
		{
			if (plane.normal.x == 0.0f && plane.normal.y == 0.0f && plane.normal.z == 0.0f) plane.normal.y = 0.1f;
			editMesh = true;
		}
		ImGui::Text("Constant: "); ImGui::SameLine();
		if (ImGui::DragFloat("##Constant", &plane.constant, 0.1f)) editMesh = true;
		break;
	default:
		break;
	}

	if (editMesh)
	{
		switch (body->getCollisionShape()->getShapeType())
		{
		case BOX_SHAPE_PROXYTYPE:
			static_cast<btBoxShape*>(body->getCollisionShape())->setLocalScaling(box.size);
			break;
		case SPHERE_SHAPE_PROXYTYPE:
			static_cast<btSphereShape*>(body->getCollisionShape())->setUnscaledRadius(sphere.radius);
			break;
		case CAPSULE_SHAPE_PROXYTYPE:
			static_cast<btCapsuleShape*>(body->getCollisionShape())->setLocalScaling(btVector3(capsule.radius, capsule.height * 0.5f, 0.0f));
			break;
		case CYLINDER_SHAPE_PROXYTYPE:
			static_cast<btCylinderShape*>(body->getCollisionShape())->setLocalScaling(btVector3(cylinder.radius, cylinder.height * 0.5f, 0.0f));
			break;
		case CONE_SHAPE_PROXYTYPE:
			static_cast<btConeShape*>(body->getCollisionShape())->setLocalScaling(btVector3(cone.radius, cone.height * 0.5f, cone.radius));
			break;
		case STATIC_PLANE_PROXYTYPE:
			CreateBody();
			break;
		default:
			break;
		}
		editMesh = false;
	}
}

float4x4 RigidBodyComponent::btScalarTofloat4x4(btScalar* transform)
{
	float4x4 newTransform;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			newTransform[j][i] = transform[k];
			k++;
		}
	}

	return newTransform;
}

void RigidBodyComponent::CreateBody()
{
	if (body != nullptr)
		app->physics->DeleteBody(this, owner->name);

	switch (collisionType)
	{
	case CollisionType::BOX:
		body = app->physics->CollisionShape(box, this);
		break;
	case CollisionType::SPHERE:
		body = app->physics->CollisionShape(sphere, this);
		break;
	case CollisionType::CAPSULE:
		body = app->physics->CollisionShape(capsule, this);
		break;
	case CollisionType::CYLINDER:
		body = app->physics->CollisionShape(cylinder, this);
		break;
	case CollisionType::CONE:
		body = app->physics->CollisionShape(cone, this);
		break;
	case CollisionType::STATIC_PLANE:
		body = app->physics->CollisionShape(plane, this);
		break;
	default:
		break;
	}

	if (body != nullptr)
	{
		body->setFriction(friction);
		body->setRestitution(restitution);
		body->setLinearFactor(movementConstraint);
		body->setAngularFactor(rotationConstraint);
		body->setDamping(linearDamping, angularDamping);
	}
}

void RigidBodyComponent::SetMass(float mass)
{
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
	body->setMassProps(mass, localInertia);
	this->mass = mass;
}

void RigidBodyComponent::SetAsStatic()
{
	useGravity = false;
	isKinematic = false;
	mass = 0.0f;
	SetCollisionType(collisionType);
}

bool RigidBodyComponent::OnLoad(JsonParsing& node)
{
	active = node.GetJsonBool("Active");

	//Collision dimensions
	collisionType = (CollisionType)(int)node.GetJsonNumber("Collision Type");
	switch (collisionType)
	{
	case CollisionType::BOX:
		box.size = node.GetJson3Number(node, "Size");
		break;
	case CollisionType::SPHERE:
		sphere.radius = node.GetJsonNumber("Radius");
		sphere.sectors = node.GetJsonNumber("Sectors");
		sphere.stacks = node.GetJsonNumber("Stacks");
		break;
	case CollisionType::CAPSULE:
		capsule.radius = node.GetJsonNumber("Radius");
		capsule.height = node.GetJsonNumber("Height");
		break;
	case CollisionType::CYLINDER:
		cylinder.radius = node.GetJsonNumber("Radius");
		cylinder.height = node.GetJsonNumber("Height");
		cylinder.sectorCount = node.GetJsonNumber("Sectors");
		break;
	case CollisionType::CONE:
		cone.radius = node.GetJsonNumber("Radius");
		cone.height = node.GetJsonNumber("Height");
		break;
	case CollisionType::STATIC_PLANE:
		plane.normal = node.GetJson3Number(node, "Normal");
		plane.constant = node.GetJsonNumber("Constant");
		break;
	default:
		break;
	}

	//Collision physics
	useGravity = node.GetJsonBool("Gravity");
	isKinematic = node.GetJsonBool("Kinematic");
	mass = node.GetJsonNumber("Mass");
	friction = node.GetJsonNumber("Friction");
	restitution = node.GetJsonNumber("Restitution");
	offset = node.GetJson3Number(node, "Offset");

	//Constrains
	movementConstraint = node.GetJson3Number(node, "MovementConstraint");
	rotationConstraint = node.GetJson3Number(node, "RotationConstraint");

	//Damping
	linearDamping = node.GetJsonNumber("LinearDamping");
	angularDamping = node.GetJsonNumber("AngularDamping");

	return true;
}

bool RigidBodyComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);

	//Collision dimensions
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Collision Type", (int)collisionType);
	switch (collisionType)
	{
	case CollisionType::BOX:
		file.SetNewJson3Number(file, "Size", box.size);
		break;
	case CollisionType::SPHERE:
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Radius", sphere.radius);
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Sectors", sphere.sectors);
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Stacks", sphere.stacks);
		break;
	case CollisionType::CAPSULE:
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Radius", capsule.radius);
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Height", capsule.height);
		break;
	case CollisionType::CYLINDER:
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Radius", cylinder.radius);
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Height", cylinder.height);
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Sectors", cylinder.sectorCount);
		break;
	case CollisionType::CONE:
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Radius", cone.radius);
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Height", cone.height);
		break;
	case CollisionType::STATIC_PLANE:
		file.SetNewJson3Number(file, "Normal", plane.normal);
		file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Constant", plane.constant);
		break;
	default:
		break;
	}

	//Collision physics
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Gravity", useGravity);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Kinematic", isKinematic);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Mass", mass);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Friction", friction);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Restitution", restitution);
	file.SetNewJson3Number(file, "Offset", offset);

	//Constrains
	file.SetNewJson3Number(file, "MovementConstraint", movementConstraint);
	file.SetNewJson3Number(file, "RotationConstraint", rotationConstraint);

	//Damping
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "LinearDamping", linearDamping);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "AngularDamping", angularDamping);


	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Active", active);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}