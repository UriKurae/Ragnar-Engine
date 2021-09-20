#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 200.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	/*
	float angle = App->player->vehicle->vehicle->getChassisWorldTransform().getRotation().getAngle();

	vec3 cameraPosition = App->player->vehicle->GetPos() + vec3(10, 5, 10);//vec3(20 * cos(angle), 10, 20 * sin(angle));
	vec3 cameraTarget = App->player->vehicle->GetPos();

	Position = cameraPosition;
	LookAt(cameraTarget);
	*/



	// Mouse motion ----------------

	if (App->scene_intro->swapCamera == false)
	{
		Position.x = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 10 * App->player->vehicle->vehicle->getForwardVector().getX();
		Position.y = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 5 * App->player->vehicle->vehicle->getUpAxis();
		Position.z = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 10 * App->player->vehicle->vehicle->getForwardVector().getZ();
		//float x_value = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 10 * App->player->vehicle->vehicle->getForwardVector().getX();
		//float z_value = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 10 * App->player->vehicle->vehicle->getForwardVector().getZ();
	}
	else if (App->scene_intro->swapCamera == true)
	{
		/*Position.x = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 10 *App->player->vehicle->vehicle->getForwardVector().getX();
		Position.y = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 5 * App->player->vehicle->vehicle->getUpAxis();
		Position.z = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 8 * App->player->vehicle->vehicle->getForwardVector().getZ();

		LookAt(vec3(App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX(), App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 3, App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ()));
		*/
	}

	if (App->scene_intro->swapCameraTop)
	{
		// 159.46f,4.609f,-621.843f
		Position.x = 170.0f;
		Position.y = 6.0f;
		Position.z = -625.0f;
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	if (App->scene_intro->laps == 4)
	{
		Position.x = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 10 * App->player->vehicle->vehicle->getForwardVector().getX();
		Position.y = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 8 * App->player->vehicle->vehicle->getUpAxis();
		Position.z = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 10 * App->player->vehicle->vehicle->getForwardVector().getZ();
	}

	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT)
	{
		Position.x = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 10 * App->player->vehicle->vehicle->getForwardVector().getX();
		Position.z = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 15 * App->player->vehicle->vehicle->getForwardVector().getZ();
	}

	LookAt(vec3(App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX(), App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 2, App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ()));

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3& Position, const vec3& Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3& Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3& Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}