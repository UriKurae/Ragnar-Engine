#include "Application.h"
#include "ModuleCamera3D.h"

#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MathGeolib/src/Math/float4.h"

#include "Optick/include/optick.h"

#include "mmgr/mmgr.h"

ModuleCamera3D::ModuleCamera3D(bool startEnabled) : Module(startEnabled)
{
	name = "Camera3D";

	cameraFrustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	cameraFrustum.SetViewPlaneDistances(1.0f, 5.0f);
	cameraFrustum.SetPerspective(90.0f, 60.0f);
	cameraFrustum.SetFrame(float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, -1.0f), float3(0.0f, 1.0f, 0.0f));
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	DEBUG_LOG("Setting up the camera");
	bool ret = true;

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
	float3 newPos = cameraFrustum.Pos();

	// Inputs for the camera
	if (app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT)
	{
		int dX = -app->input->GetMouseXMotion();
		int dY = -app->input->GetMouseYMotion();

		if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT) newPos = cameraFrustum.Pos() + cameraFrustum.Front() * 9.0f * dt;
		if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT) newPos = cameraFrustum.Pos() + cameraFrustum.Front() * -9.0f * dt;

		if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) newPos = cameraFrustum.Pos() + cameraFrustum.WorldRight() * -9.0f * dt;
		if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT) newPos = cameraFrustum.Pos() + cameraFrustum.WorldRight() * 9.0f * dt;

		if (app->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT) newPos = cameraFrustum.Pos() + cameraFrustum.Up() * 9.0f * dt;
		if (app->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT) newPos = cameraFrustum.Pos() + cameraFrustum.Up() * -9.0f * dt;

		if (app->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT)
		{
			//GameObject* target = app->editor->GetSelected();

			if (dX != 0)
			{
				Quat rotateHorizontal;
				rotateHorizontal = rotateHorizontal.RotateY(math::DegToRad(-dX));
				rotateHorizontal.Normalize();

				
			}
			if (dY != 0)
			{
				Quat rotateVertical;
				rotateVertical = rotateVertical.RotateX(math::DegToRad(-dY));
				rotateVertical.Normalize();
				cameraFrustum.Transform(rotateVertical);
			}

			/*if (target != nullptr)
			{
				float3 distanceTarget = cameraFrustum.Pos() - target->GetComponent<TransformComponent>()->GetPosition();

				Quat quatX(cameraFrustum.WorldRight(), dY);
				Quat quatY(cameraFrustum.Up(), dX);
				
				distanceTarget = quatX.Transform(distanceTarget);
				distanceTarget = quatY.Transform(distanceTarget);
				cameraFrustum.SetPos(distanceTarget + target->GetComponent<TransformComponent>()->GetPosition());*/
				//LookAt(target->GetComponent<TransformComponent>()->GetPosition());
		
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_UP)
	{
		GameObject* target = app->editor->GetSelected();
		if (target != nullptr)
		{
			float3 maxPoint = target->GetAABB().maxPoint;
			float3 minPoint = target->GetAABB().minPoint;
			
			float3 h = (maxPoint - minPoint)/2;

			float angle = cameraFrustum.VerticalFov() / 2;

			float3 distance = h / Atan(angle);

			newPos.z = target->GetComponent<TransformComponent>()->GetPosition().z - distance.z;
			newPos.y = h.y;
		}
	}
	
	if (app->input->GetMouseZ() == 1) newPos.z -= 9.0f * dt;
	if (app->input->GetMouseZ() == -1) newPos.z += 9.0f * dt;

	cameraFrustum.SetPos(newPos);

	matrixViewFrustum = cameraFrustum.ViewMatrix();
	matrixProjectionFrustum = cameraFrustum.ProjectionMatrix();
	

	return true;
}

void ModuleCamera3D::LookAt(float3& target)
{
	float3 directionFrustum = target - cameraFrustum.Pos();
	directionFrustum.Normalize();

	float3x3 lookAt = float3x3::LookAt(cameraFrustum.Front(), directionFrustum, cameraFrustum.Up(), float3(0.0f, 1.0f, 0.0f));
	cameraFrustum.SetFront(lookAt.MulDir(cameraFrustum.Front()).Normalized());
	cameraFrustum.SetUp(lookAt.MulDir(cameraFrustum.Up()).Normalized());
}
