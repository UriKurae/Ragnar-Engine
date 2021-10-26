#include "Application.h"
#include "ModuleCamera3D.h"

#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include "Optick/include/optick.h"

#include "mmgr/mmgr.h"

ModuleCamera3D::ModuleCamera3D(bool startEnabled) : Module(startEnabled)
{
	name = "Camera3D";

	CalculateViewMatrix();

	x = Vec3(1.0f, 0.0f, 0.0f);
	y = Vec3(0.0f, 1.0f, 0.0f);
	z = Vec3(0.0f, 0.0f, 1.0f);

	position = Vec3(0.0f, 0.0f, 5.0f);
	reference = Vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	DEBUG_LOG("Setting up the camera");
	bool ret = true;
	// Ruben : Setup de la camara frustum
	cameraFrustum = new Frustum();
	cameraFrustum->SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	cameraFrustum->SetViewPlaneDistances(1.0f, 300.0f);
	cameraFrustum->SetFrame(math::vec(0.0f, 0.0f, 0.0f), math::vec(0.0f, 0.0, 1.0f), math::vec(0.0f, 1.0f, 0.0f));
	cameraFrustum->SetPerspective(90.0f, 59.0f);
	

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	DEBUG_LOG("Cleaning camera");

	return true;
}

bool ModuleCamera3D::LoadConfig(JsonParsing& node)
{
	return true;
}

bool ModuleCamera3D::SaveConfig(JsonParsing& node) const
{
	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Update(float dt)
{
	OPTICK_EVENT("Updating Camera 3D");
	Vec3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT)
		speed = 25.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_R) == KeyState::KEY_REPEAT) newPos.y += speed;
	if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_REPEAT) newPos.y -= speed;

	if (app->editor->GetSelected())
	{
		cameraFrustum->SetFront(app->editor->GetSelected()->GetComponent<TransformComponent>()->GetPosition());
		
	}

	// Ruben : Aqui muevo la camaraFrustum con el Translate que da mathgeolib frustum
	if ((app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT) && (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT))
	{
		cameraFrustum->Translate(math::vec(0.0f, 0.0f, speed * 9.0f));

		DEBUG_LOG("CAMERA IS %f %f %f", cameraFrustum->Pos().x, cameraFrustum->Pos().y, cameraFrustum->Pos().z);
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT && (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT))
	{
		cameraFrustum->Translate(math::vec(0.0f, 0.0f, -speed * 9.0f));
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT && (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT)) cameraFrustum->Translate(math::vec(speed * 9.0f, 0.0f, 0.0f));
	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT && (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT)) cameraFrustum->Translate(math::vec(-speed * 9.0f, 0.0f, 0.0f));
	
	if (app->input->GetMouseZ() == 1) newPos -= z * speed * 9.0f;
	if (app->input->GetMouseZ() == -1) newPos += z * speed * 9.0f;

	position += newPos;
	reference += newPos;

	if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_UP)
	{
		float3 maxPoint = app->editor->GetSelected()->GetAABB().maxPoint;
		float3 minPoint = app->editor->GetSelected()->GetAABB().minPoint;

		float3 result = (maxPoint - minPoint) / 2;
		
	}

	// Mouse motion ----------------

	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT)
	{
		int dX = -app->input->GetMouseXMotion();
		int dY = -app->input->GetMouseYMotion();

		float sensitivity = 0.25f;

		position -= reference;

		if (dX != 0)
		{
			float deltaX = (float)dX * sensitivity;

			x = Rotate(x, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			y = Rotate(y, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			z = Rotate(z, deltaX, Vec3(0.0f, 1.0f, 0.0f));
		}

		if (dY != 0)
		{
			float DeltaY = (float)dY * sensitivity;

			y = Rotate(y, DeltaY, x);
			z = Rotate(z, DeltaY, x);

			if (y.y < 0.0f)
			{
				z = Vec3(0.0f, z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				y = Cross(z, x);
			}
		}

		position = reference + z * Length(position);

		if (app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT && app->editor->GetSelected() != nullptr)
		{
			Vec3 target;
			target.x = app->editor->GetSelected()->GetComponent<TransformComponent>()->GetPosition().x;
			target.y = app->editor->GetSelected()->GetComponent<TransformComponent>()->GetPosition().y;
			target.z =app->editor->GetSelected()->GetComponent<TransformComponent>()->GetPosition().z;
			LookAt(target);
		}
	}

	// Recalculate matrix -------------
	//CalculateViewMatrix();

	// Ruben : Aqui probe a calcular las matrices a cada frame.
	/*projectionMatrix = cameraFrustum->ComputeProjectionMatrix();
	frustumViewMatrix = cameraFrustum->ComputeViewMatrix();*/
	

	return true;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const Vec3 &pos, const Vec3 &ref, bool rotateAroundReference)
{
	this->position = pos;
	this->reference = ref;

	z = Normalize(position - reference);
	x = Normalize(Cross(Vec3(0.0f, 1.0f, 0.0f), z));
	y = Cross(z, x);

	if(!rotateAroundReference)
	{
		this->reference = this->position;
		this->position += z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const Vec3 &spot)
{
	reference = spot;

	z = Normalize(position - reference);
	x = Normalize(Cross(Vec3(0.0f, 1.0f, 0.0f), z));
	y = Cross(z, x);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const Vec3 &movement)
{
	position += movement;
	reference += movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &viewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	viewMatrix = Mat4x4(x.x, y.x, z.x, 0.0f, x.y, y.y, z.y, 0.0f, x.z, y.z, z.z, 0.0f, -Dot(x, position), -Dot(y, position), -Dot(z, position), 1.0f);
	viewMatrixInverse = Inverse(viewMatrix);
}