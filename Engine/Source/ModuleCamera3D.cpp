#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	CalculateViewMatrix();

	x = vec3(1.0f, 0.0f, 0.0f);
	y = vec3(0.0f, 1.0f, 0.0f);
	z = vec3(0.0f, 0.0f, 1.0f);

	position = vec3(0.0f, 0.0f, 5.0f);
	reference = vec3(0.0f, 0.0f, 0.0f);
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
UpdateStatus ModuleCamera3D::Update(float dt)
{
	vec3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT)
		speed = 8.0f * dt;

	if (app->input->GetKey(SDL_SCANCODE_R) == KeyState::KEY_REPEAT) newPos.y += speed;
	if (app->input->GetKey(SDL_SCANCODE_F) == KeyState::KEY_REPEAT) newPos.y -= speed;

	if (app->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT) newPos -= z * speed;
	if (app->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT) newPos += z * speed;


	if (app->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) newPos -= x * speed;
	if (app->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT) newPos += x * speed;

	position += newPos;
	reference += newPos;

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

			x = rotate(x, deltaX, vec3(0.0f, 1.0f, 0.0f));
			y = rotate(y, deltaX, vec3(0.0f, 1.0f, 0.0f));
			z = rotate(z, deltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dY != 0)
		{
			float DeltaY = (float)dY * sensitivity;

			y = rotate(y, DeltaY, x);
			z = rotate(z, DeltaY, x);

			if (y.y < 0.0f)
			{
				z = vec3(0.0f, z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				y = cross(z, x);
			}
		}

		position = reference + z * length(position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UpdateStatus::UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &pos, const vec3 &ref, bool rotateAroundReference)
{
	this->position = pos;
	this->reference = ref;

	z = normalize(position - reference);
	x = normalize(cross(vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	if(!rotateAroundReference)
	{
		this->reference = this->position;
		this->position += z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &spot)
{
	reference = spot;

	z = normalize(position - reference);
	x = normalize(cross(vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &movement)
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
	viewMatrix = mat4x4(x.x, y.x, z.x, 0.0f, x.y, y.y, z.y, 0.0f, x.z, y.z, z.z, 0.0f, -dot(x, position), -dot(y, position), -dot(z, position), 1.0f);
	viewMatrixInverse = inverse(viewMatrix);
}