#include "CameraComponent.h"
#include "Application.h"
#include "Globals.h"

#include "ModuleScene.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

#include "ModuleEditor.h"
#include "GameView.h"

#include "TransformComponent.h"
#include "MeshComponent.h"

#include "Math/float3x3.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Profiling.h"

#include <time.h>
//#include <stdlib.h>

CameraComponent::CameraComponent(GameObject* own, TransformComponent* trans) : horizontalFov(DegToRad(90.0f)), verticalFov(0.0f), nearPlane(1.0f), farPlane(100.0f), transform(trans), currentRotation(0,0,0,1), currentScreenHeight(SCREEN_HEIGHT), currentScreenWidth(SCREEN_WIDTH), vbo(nullptr), ebo(nullptr)
{
	type = ComponentType::CAMERA;
	owner = own;
	camera.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	camera.SetViewPlaneDistances(nearPlane, farPlane);
	CalculateVerticalFov(horizontalFov, currentScreenWidth, currentScreenHeight);
	camera.SetPerspective(horizontalFov, verticalFov);
	camera.SetFrame(float3(0.0f,0.0f, 0.0f), float3(0.0f, 0.0f, 1.0f), float3(0.0f, 1.0f, 0.0f));

	/*owner->GetParent()->RemoveChild(owner);

	defTarget = new GameObject();
	defTarget->SetName("cameraController");

	defTarget->SetParent(app->scene->GetRoot());
	app->scene->GetRoot()->AddChild(defTarget);

	TransformComponent* comp = new TransformComponent(defTarget);
	defTarget->AddComponent(comp);

	owner->SetParent(defTarget);
	defTarget->AddChild(owner);*/

	srand(time(NULL));

	CompileBuffers();
}

CameraComponent::~CameraComponent()
{
	RELEASE(vbo);
	RELEASE(ebo);
}

void CameraComponent::OnEditor()
{
	ImGui::PushID(this);

	if (ImGui::CollapsingHeader(ICON_FA_CAMERA" Camera"))
	{
		ImGui::Text("Field of view");
		ImGui::SameLine();
		static float horizontalFovEditor = RadToDeg(horizontalFov);
		if (ImGui::DragFloat("", &horizontalFovEditor, 1.0f, 1.0f, 179.0f))
		{
			horizontalFov = DegToRad(horizontalFovEditor);
			UpdateFov();
			CompileBuffers();
		}

		ImGui::Text("Clipping planes");

		ImGui::Text("Near");
		ImGui::SameLine();
		ImGui::PushID("NearPlane");
		if (ImGui::DragFloat("", &nearPlane, 0.5f, 0.1f)) SetPlanes();
		ImGui::PopID();

		ImGui::PushID("farPlane");
		ImGui::Text("Far");
		ImGui::SameLine();
		if (ImGui::DragFloat("", &farPlane, 0.5f, 0.1f)) SetPlanes();
		ImGui::PopID();

		ImGui::Text("- - - - MOVEMENT - - - -");

		if (ImGui::Checkbox("RightClickRotation", &rightClickRot)) {}
		ImGui::SameLine();
		if (ImGui::Checkbox("WheelClickMovement", &rightClickRot)) {}

		if (ImGui::Checkbox("ArrowRotation", &arrowRot)) {}
		ImGui::SameLine();
		if (ImGui::Checkbox("WASD-Movement", &WASDMov)) {}

		if (ImGui::Checkbox("BorderMovement", &borderMov)) {}

		if (ImGui::Button("Switch camera movement"))
		{
			if (freeMovement)
			{
				freeMovement = false;
				followTarget = true;
			}

			else
			{
				freeMovement = true;
				followTarget = false;
				//transform->SetRotation(Quat::identity);
			}
		}

		if (freeMovement)
		{
			ImGui::Text("Movement: free");
			ImGui::DragFloat("movementSpeed", &movementSpeed, 0.002f, 0.01f, 1.0f);
		}

		else if (followTarget)
		{
			ImGui::Text("Movement: target-locked");


			ImGui::Text("Target:");
			ImGui::SameLine();
			ImGui::Button(target != nullptr ? target->GetName() : "none");
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* go = ImGui::AcceptDragDropPayload("HierarchyItem");
				if (go)
				{
					uint uuid = *(const uint*)(go->Data);
					target = app->scene->GetGoByUuid(uuid);
				}
				ImGui::EndDragDropTarget();
			}
		}

		if (ImGui::DragFloat("verticalAngle", &verticalAngle, 0.01f, 0.0f)) {}
		if (ImGui::Checkbox("lockVerticalAngle", &lockVerticalAngle)) {}

		if (ImGui::DragFloat("rotationSpeed", &rotationSpeed, 0.001f, 0.0f)) {}
		if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.0f)) {}

		ImGui::Text("- - - - SHAKE - - - -");

		if (ImGui::DragFloat("shakeStrength", &shakeStrength, 0.1f, 0.0f)) {}
		if (ImGui::DragFloat("shakeDuration", &shakeDuration, 0.1f, 0.0f)) {}

		if (ImGui::Button("<")) if (smooth > 0) smooth--;
		ImGui::SameLine();
		ImGui::Button("Smoothness Type");
		ImGui::SameLine();
		if (ImGui::Button(">")) if (smooth < 3) smooth++;

		if (smooth == 0) ImGui::Text("Smoothnes:: none");
		if (smooth == 1) ImGui::Text("Smoothnes:: in-out");
		if (smooth == 2) ImGui::Text("Smoothnes:: in");
		if (smooth == 3) ImGui::Text("Smoothnes:: out");
		//if (ImGui::Checkbox("smoothShake", &smoothShake)) {}
		if (ImGui::Button("Shake")) {
			RequestShake(shakeStrength, shakeDuration);
		}
		ComponentOptions(this);
		ImGui::Separator();
	}
	ImGui::PopID();
}

bool CameraComponent::Update(float dt)
{
	// This is bad!
	if (targetUID != 0)
	{
		target = app->scene->GetGoByUuid(targetUID);
		targetUID = 0;
	}

	float4 viewport = app->editor->GetGameView()->GetBounds();
	camera.SetOrthographic(viewport.z / zoom, viewport.w / zoom);
	zoom = Clamp(zoom + app->input->GetMouseZ(), zoomMin, zoomMax);

	camera.SetPos(transform->GetPosition());

	if (target && app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_DOWN)
	{
		fixingToTarget ^= true;
	}
	if (target&&fixingToTarget)
	{
		float3 pos_ = target->GetComponent<TransformComponent>()->GetPosition() - camera.Pos();
		camera.Translate(pos_ * dt);
		if (pos_.IsZero(0.001)) fixingToTarget = false;
	}

	if (shake)
	{
		Shake(dt);
	}
	
	if (!CompareRotations(currentRotation, transform->GetRotation()))
	{
		currentRotation = transform->GetRotation();
	
		float3 newUp = float3::unitY;
		float3 newFront = float3::unitZ;
		
		newUp = transform->GetRotation() * newUp;
		
		newUp.Normalize();
		newFront = transform->GetRotation() * newFront;
		newFront.Normalize();

		float3::Orthonormalize(newUp, newFront);
		
		camera.SetUp(newUp);
		camera.SetFront(newFront);
	}


	bool mouseDragRight;
	if(rightClickRot) mouseDragRight = (app->input->GetMouseButton(3) == KeyState::KEY_REPEAT);
	float horizontalDrag = 0;
	if(arrowRot || rightClickRot) horizontalDrag = app->input->GetMouseXMotion();

	// -------------MOVEMENT---------------
	if ((arrowRot && app->input->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_REPEAT) || (rightClickRot && mouseDragRight && horizontalDrag > 1)) horizontalAngle -= rotationSpeed;
	if ((arrowRot && app->input->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_REPEAT) || (rightClickRot && mouseDragRight && horizontalDrag < -1)) horizontalAngle += rotationSpeed;
	if (!lockVerticalAngle)
	{
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_REPEAT) verticalAngle -= rotationSpeed;
		if (app->input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_REPEAT) verticalAngle += rotationSpeed;
		verticalAngle = Clamp(verticalAngle, -179.9f, -0.1f);
	}

	if (freeMovement)
	{
		float3 pos = defTarget->GetComponent<TransformComponent>()->GetPosition();
		bool mouseDragMid = (app->input->GetMouseButton(2) == KeyState::KEY_REPEAT);
		//int horizontalDrag = app->input->GetMouseXMotion();
		float verticalDrag = app->input->GetMouseYMotion();
		int dragThreshold = 0;
		int wx, wy;
		SDL_GetWindowPosition(app->window->window, &wx, &wy);

		if ((WASDMov && app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
			|| (rightClickRot && mouseDragMid && horizontalDrag > dragThreshold)
			|| (borderMov && app->input->GetMouseX() < wx+2)) {
			pos.x += movementSpeed * horizontalDrag / zoom / 2 * sin(DEGTORAD * (horizontalAngle + 90));
			pos.z += movementSpeed * horizontalDrag / zoom / 2 * cos(DEGTORAD * (horizontalAngle + 90));
		}
		if ((WASDMov && app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
			|| (rightClickRot && mouseDragMid && horizontalDrag < -dragThreshold)
			|| (borderMov && app->input->GetMouseX() > *app->window->GetWindowWidth() + wx-2)) {
			pos.x -= movementSpeed * -horizontalDrag / zoom / 2 * sin(DEGTORAD * (horizontalAngle + 90));
			pos.z -= movementSpeed * -horizontalDrag / zoom / 2 * cos(DEGTORAD * (horizontalAngle + 90));
		}
		if ((WASDMov && app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
			|| (rightClickRot && mouseDragMid && verticalDrag < -dragThreshold)
			|| (borderMov && app->input->GetMouseY() > *app->window->GetWindowHeight() + wy-2)) {
			pos.x -= movementSpeed * -verticalDrag / zoom * sin(DEGTORAD * horizontalAngle);
			pos.z -= movementSpeed * -verticalDrag / zoom * cos(DEGTORAD * horizontalAngle);
		}
		if ((WASDMov && app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
			|| (rightClickRot && mouseDragMid && verticalDrag > dragThreshold)
			|| (borderMov && app->input->GetMouseY() < wy+2)) {
			pos.x += movementSpeed * verticalDrag / zoom * sin(DEGTORAD * horizontalAngle);
			pos.z += movementSpeed * verticalDrag / zoom * cos(DEGTORAD * horizontalAngle);
		}
		defTarget->GetComponent<TransformComponent>()->SetPosition(float3(pos.x, 0, pos.z));
	}

	float3 targetPos = float3(0, 0, 0);
	if(freeMovement) targetPos = defTarget->GetComponent<TransformComponent>()->GetPosition();
	else if(target) targetPos = target->GetComponent<TransformComponent>()->GetPosition();
	float3 newPos = targetPos;

	// offset
	newPos.z += radius * sin(DEGTORAD * verticalAngle) * cos(DEGTORAD * horizontalAngle);
	newPos.x += radius * sin(DEGTORAD * verticalAngle) * sin(DEGTORAD * horizontalAngle);
	newPos.y += radius * cos(DEGTORAD * verticalAngle);

	float3 directionFrustum = targetPos - newPos;
	directionFrustum.Normalize();

	float3x3 lookAt = float3x3::LookAt(camera.Front(), directionFrustum, camera.Up(), float3(0.0f, 1.0f, 0.0f));
	camera.SetFront(lookAt.MulDir(camera.Front()).Normalized());
	camera.SetUp(lookAt.MulDir(camera.Up()).Normalized());

	transform->SetRotation(lookAt.ToQuat());
	transform->SetPosition(newPos);

	matrixProjectionFrustum = camera.ComputeProjectionMatrix();
	matrixViewFrustum = camera.ComputeViewMatrix();

	return true;
}

void CameraComponent::Draw(CameraComponent* gameCam)
{
	glPushMatrix();

	glMultMatrixf(transform->GetGlobalTransform().Transposed().ptr());
	glEnableClientState(GL_VERTEX_ARRAY);
	vbo->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	ebo->Bind();
	glLineWidth(2.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glDrawElements(GL_LINES, ebo->GetCount(), GL_UNSIGNED_INT, NULL);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
	vbo->Unbind();
	ebo->Unbind();
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void CameraComponent::SetPlanes()
{
	camera.SetViewPlaneDistances(nearPlane, farPlane);
	//CompileBuffers();
}

void CameraComponent::CalculateVerticalFov(float horizontalFovRadians, float width, float height)
{
	verticalFov = 2 * Atan((Tan(horizontalFovRadians / 2)) * (height / width));
	camera.SetVerticalFovAndAspectRatio(verticalFov, (width / height));
	currentScreenHeight = height;
	currentScreenWidth = width;
}

void CameraComponent::UpdateFovAndScreen(float width, float height)
{
	verticalFov = 2 * Atan((Tan(horizontalFov / 2)) * (height / width));
	camera.SetVerticalFovAndAspectRatio(verticalFov, (width / height));
	currentScreenHeight = height;
	currentScreenWidth = width;
}

void CameraComponent::UpdateFov()
{
	verticalFov = 2 * Atan((Tan(horizontalFov / 2)) * (currentScreenHeight / currentScreenWidth));
	camera.SetVerticalFovAndAspectRatio(verticalFov, (currentScreenWidth / currentScreenHeight));
}

void CameraComponent::SetNewFov(float horizontalFovRadians)
{
	horizontalFov = horizontalFovRadians;
}

void CameraComponent::CompileBuffers()
{
	// Configure buffers
	float3 corners[8];
	camera.GetCornerPoints(corners);
	unsigned int indices[24] =
	{
		0,1,
		1,3,
		3,2,
		2,0,

		1,5,
		4,6,
		7,3,

		6,7,
		6,2,

		7,5,
		4,5,

		4,0
	};

	if (vbo)
	{
		vbo->Unbind();
		RELEASE(vbo);
	}
	ebo = new IndexBuffer(indices, 24);
	vbo = new VertexBuffer(corners, sizeof(float3) * 8);
	ebo->Unbind();
	vbo->Unbind();
}

bool CameraComponent::CompareRotations(Quat& quat1, Quat& quat2)
{
	if (quat1.x == quat2.x && quat1.y == quat2.y && quat1.z == quat2.z && quat1.w == quat2.w) return true;

	return false;
}

bool CameraComponent::OnLoad(JsonParsing& node)
{
	nearPlane = node.GetJsonNumber("Near Plane");
	farPlane = node.GetJsonNumber("Far Plane");
	verticalFov = node.GetJsonNumber("Vertical Fov");
	horizontalFov = node.GetJsonNumber("Horizontal Fov");
	camera.SetPos(node.GetJson3Number(node, "Camera Pos"));

	// MOVEMENT
	zoom = node.GetJsonNumber("Zoom");
	freeMovement = node.GetJsonBool("Free Movement");
	if (!freeMovement) followTarget = true;
	defTarget = owner->GetParent();
	//defTarget->GetComponent<TransformComponent>()->SetPosition(node.GetJson3Number(node, "Default Target Pos"));
	movementSpeed = node.GetJsonNumber("Movement Speed");
	targetUID = node.GetJsonNumber("Target Pos");
	// ANGLES
	verticalAngle = node.GetJsonNumber("Vertical Angle");
	lockVerticalAngle = node.GetJsonBool("Lock Vertical Angle");
	rotationSpeed = node.GetJsonNumber("Rotation Speed");
	radius = node.GetJsonNumber("Radius");
	horizontalAngle = node.GetJsonNumber("Horizontal Angle");
	// CONTROLS
	rightClickRot = node.GetJsonBool("Right Click Rotation");
	if (!rightClickRot) midClickMov = false;
	arrowRot = node.GetJsonBool("Arrow Rotation");
	WASDMov = node.GetJsonBool("WASD Movement");
	borderMov = node.GetJsonBool("Border Movement");
	// SHAKE
	shakeStrength = node.GetJsonNumber("Shake Strength");
	shakeDuration = node.GetJsonNumber("Shake Duration");
	smooth = node.GetJsonNumber("Shake Smooth");

	return true;
}

bool CameraComponent::OnSave(JsonParsing& node, JSON_Array* array)
{
	JsonParsing file = JsonParsing();

	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Near Plane", nearPlane);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Far Plane", farPlane);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Vertical Fov", verticalFov);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Horizontal Fov", horizontalFov);
	file.SetNewJson3Number(file, "Camera Pos", camera.Pos());
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Type", (int)type);
	// MOVEMENT
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Zoom", zoom);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Free Movement", freeMovement);
	file.SetNewJson3Number(file, "Default Target Pos", defTarget->GetComponent<TransformComponent>()->GetPosition());
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Movement Speed", movementSpeed);
	if(target) file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Target Pos", target->GetUUID());
	// ANGLES
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Vertical Angle", verticalAngle);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Lock Vertical Angle", lockVerticalAngle);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Rotation Speed", rotationSpeed);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Radius", radius);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Horizontal Angle", horizontalAngle);
	// CONTROLS
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Right Click Rotation", rightClickRot);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Arrow Rotation", arrowRot);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "WASD Movement", WASDMov);
	file.SetNewJsonBool(file.ValueToObject(file.GetRootValue()), "Border Movement", borderMov);
	// SHAKE
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Shake Strength", shakeStrength);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Shake Duration", shakeDuration);
	file.SetNewJsonNumber(file.ValueToObject(file.GetRootValue()), "Shake Smooth", smooth);

	node.SetValueToArray(array, file.GetRootValue());

	return true;
}

int CameraComponent::ContainsAaBox(const AABB& boundingBox)
{
	if (boundingBox.IsFinite())
	{
		if (camera.Contains(boundingBox))
		{
			return 1;
		}
		else if (camera.Intersects(boundingBox))
		{
			return 2;
		}
		return 0;
	}
	return -1;
}

void CameraComponent::RequestShake(float strength, float duration)
{
	shakeStrength = strength;
	shakeDuration = duration;
	if (smooth != 0) currentStrength = 0;
	else currentStrength = strength;
	shake = true;
	originalPos = transform->GetPosition();
}

void CameraComponent::Shake(float dt)
{
	// Exponential
	if (smooth != 0)
	{
		if ((elapsedTime < shakeDuration / 2 && smooth == 1) || (smooth == 2 && elapsedTime < shakeDuration))
		{
			if (currentStrength < shakeStrength)
			{
				currentStrength = shakeStrength * elapsedTime * elapsedTime;
			}
			else currentStrength = shakeStrength;
		}
		else if ((currentStrength > 0 && elapsedTime < shakeDuration && smooth == 1) || (elapsedTime < shakeDuration && smooth == 3))
		{
			currentStrength = shakeStrength * (shakeDuration - elapsedTime) * (shakeDuration - elapsedTime);
		}
		else currentStrength = 0;
	}

	if (elapsedTime < shakeDuration)
	{
		float x = -currentStrength + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (currentStrength - -currentStrength)));
		float y = -currentStrength + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (currentStrength - -currentStrength)));
		float z = -currentStrength + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (currentStrength - -currentStrength)));
		float3 lastPos = transform->GetPosition();
		lastPos.x += x;
		lastPos.y += y;
		lastPos.z += z;
		camera.SetPos(lastPos);
		elapsedTime += dt;
	}
	else
	{
		shake = false;
		elapsedTime = 0.0f;
		camera.SetPos(originalPos);
	}
}