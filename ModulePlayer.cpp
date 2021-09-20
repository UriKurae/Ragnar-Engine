#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 4.5);
	car.chassis_offset.Set(0, 1.5, 0);

	car.cockpit_size.Set(1.5, 0.7, 1.5);
	car.cockpit_offset.Set(0, 2.3, -0.5);

	car.back_cockpit_size.Set(1, 0.6, 0.7);
	car.back_cockpit_offset.Set(0, 2.2, -1.5);

	car.front_chassis_size.Set(1.3f, 0.7f, 2.5);
	car.front_chassis_offset.Set(0, 1.5f, 3.5);
	
	car.front_wing_right_support_size.Set(0.2, 0.5, 0.2);
	car.front_wing_right_support_offset.Set(-0.55f, 0.9, 4.5);

	car.front_wing_left_support_size.Set(0.2, 0.5, 0.2);
	car.front_wing_left_support_offset.Set(0.55, 0.9, 4.5);

	car.front_wing_size.Set(3.2, 0.15, 0.7);
	car.front_wing_offset.Set(0, 0.575, 4.75);

	car.front_wing_right_size.Set(0.1, 0.2, 0.7);
	car.front_wing_right_offset.Set(1.55, 0.75, 4.75);

	car.front_wing_left_size.Set(0.1, 0.2, 0.7);
	car.front_wing_left_offset.Set(-1.55, 0.75, 4.75);

	car.right_spoiler_support_size.Set(0.01, 0.95, 0.4);
	car.right_spoiler_support_offset.Set(0.8, 2.3, -2.3);

	car.left_spoiler_support_size.Set(0.01, 0.95, 0.4);
	car.left_spoiler_support_offset.Set(-0.8, 2.3, -2.3);

	car.spoiler_size.Set(2.8, 0.07, 0.8);
	car.spoiler_offset.Set(0, 2.8, -2.4);

	car.antenna_size.Set(0, 0.7, 0);
	car.antenna_offset.Set(0, 2.5, -0.8);

	car.t_base_size.Set(0.1, 0.2, 0.1);
	car.t_base_offset.Set(0, 2.7, 0);

	car.mass = 600.0f;
	car.suspensionStiffness = 20.83f;
	car.suspensionCompression = 1.0f;
	car.suspensionDamping = 9.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 40.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.9f;
	float wheel_radius = 1.0f;
	float wheel_width = 1.2;
	float suspensionRestLength = 1.4f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x * 0.9f;
	float half_length = car.chassis_size.z * 0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.6f, connection_height, half_length + 1);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius * 0.8f;
	car.wheels[0].width = wheel_width * 0.7f;
	car.wheels[0].front = true;
	car.wheels[0].drive = false;
	car.wheels[0].brake = true;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.6f, connection_height, half_length + 1);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius * 0.8;
	car.wheels[1].width = wheel_width * 0.7;
	car.wheels[1].front = true;
	car.wheels[1].drive = false;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f, connection_height, -half_length + wheel_radius * 0.5f);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = true;
	car.wheels[2].brake = false;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f, connection_height, -half_length + wheel_radius * 0.5f);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = true;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);

	ResetPlayer();

	vehicle->collision_listeners.add(App->scene_intro);

	vehicle->vehicle->getRigidBody()->setUserPointer(vehicle);

	initialPosition = vehicle->vehicle->getChassisWorldTransform().getOrigin();

	groundContact = true;
	allowFlip = false;

	turbo = 33.0f;
	turbosLeft = 0;

	maxAngularVelocity = btVector3(5, 2.3, 0);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = handbrake = 0.0f;

	CameraPosition();

	mat4x4 trans;
	vehicle->GetTransform(&trans);
	mat3x3 rot(trans);
	
	vec3 down(0, -1, 0);
	vec3 dest = rot * down;

	vec3 groundRaycast = App->physics->RayCast({ vehicle->GetPos().x, vehicle->GetPos().y + 1, vehicle->GetPos().z }, dest);
	float l = length(groundRaycast);
	
	if (l < 1.0f) groundContact = true;
	else
	{
		groundContact = false;
		vec3 force = vec3(0,-200,0);
		vehicle->Push(force.x, force.y, force.z);
	}

	vec3 up(0, 1, 0);
	vec3 upRotated = rot * up;
	vec3 upVector = App->physics->RayCast({ vehicle->GetPos().x, vehicle->GetPos().y + 1, vehicle->GetPos().z }, upRotated);
	float upVectorLength = length(upVector);

	if (App->scene_intro->laps != 4) HandleInputs(dt);

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN && App->scene_intro->laps == 4)
	{
		ResetPlayer();
		App->scene_intro->ResetScene();
	}
	
	if (App->scene_intro->laps == 4 && vehicle->GetKmh() > 0) acceleration = -MAX_ACCELERATION * 0.5f;
	if (App->scene_intro->laps == 4 && vehicle->GetKmh() > 2) brake = BRAKE_POWER;
	
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->Handbrake(handbrake);

	vehicle->info.turbosLeft = turbosLeft;

	vehicle->Render();

	return UPDATE_CONTINUE;
}

void ModulePlayer::HandleInputs(float dt)
{
	btVector3 currAngularVelocity = vehicle->vehicle->getRigidBody()->getAngularVelocity();

	if (App->scene_intro->startCountDown <= 0.0f)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && vehicle->GetKmh() < 320)
		{
			if (App->input->GetKey(SDL_SCANCODE_SPACE) != KEY_REPEAT)
			{				
				acceleration = MAX_ACCELERATION;	
			}

			if (groundContact == false && currAngularVelocity.getY() < maxAngularVelocity.getY())
				vehicle->vehicle->getRigidBody()->applyTorqueImpulse(RotateVecToLocal(100, 0, 0));
		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && turbo > 0.0f && vehicle->GetKmh() < 420)
		{
			if (App->input->GetKey(SDL_SCANCODE_SPACE) != KEY_REPEAT)
			{
				acceleration = MAX_ACCELERATION * 3.5f;
				turbo -= 25.0f * dt;
			}	
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			if(groundContact == false && abs(currAngularVelocity.getY()) < abs(maxAngularVelocity.getY()))
				vehicle->vehicle->getRigidBody()->applyTorqueImpulse(RotateVecToLocal(0, 100, 0));
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
			{
				if (vehicle->GetKmh() > 100)
				{
					turn += TURN_DEGREES / (vehicle->GetKmh() / 90);
				}
				else
					turn += TURN_DEGREES;
			}

			if (groundContact == false && abs(currAngularVelocity.getX()) < abs(maxAngularVelocity.getX()))
				vehicle->vehicle->getRigidBody()->applyTorqueImpulse(RotateVecToLocal(0, 0, -20));
		}


		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		{
			if (groundContact == false && abs(currAngularVelocity.getY()) < abs(maxAngularVelocity.getY()))
				vehicle->vehicle->getRigidBody()->applyTorqueImpulse(RotateVecToLocal(0, -100, 0));
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
			{
				if (vehicle->GetKmh() > 100)
				{
					turn -= TURN_DEGREES / (vehicle->GetKmh() / 90);
				}
				else
					turn -= TURN_DEGREES;
			}

			if (groundContact == false && abs(currAngularVelocity.getX()) < abs(maxAngularVelocity.getX()))
				vehicle->vehicle->getRigidBody()->applyTorqueImpulse(RotateVecToLocal(0, 0, 20));
		}


		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			handbrake = HANDBRAKE_POWER;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			if (vehicle->GetKmh() > 0)
				brake = BRAKE_POWER;

			else if (vehicle->GetKmh() > -80)
				acceleration = -MAX_ACCELERATION;

			else
				acceleration = -79;


			if (groundContact == false && currAngularVelocity.getY() < maxAngularVelocity.getY())
				vehicle->vehicle->getRigidBody()->applyTorqueImpulse(RotateVecToLocal(-100, 0, 0));
		}

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			if (App->scene_intro->checkpointList.getLast() != nullptr)
			{
				//mat4x4 torusTr = App->scene_intro->torusCheckpointList.getLast()->data->GetTransform();
				//torusTr.rotate(90, vec3(0, 1, 0));
				
				mat4x4 tr;
				App->scene_intro->checkpointList.getLast()->data->GetTransform(&tr);
				if (App->scene_intro->checkpointList.getLast()->data->name == "startcheckpoint" ||
					App->scene_intro->checkpointList.getLast()->data->name == "secondcheckpoint")
				{
					tr.rotate(90, vec3(0, 1, 0));
				}
				else if (App->scene_intro->checkpointList.getLast()->data->name == "thirdcheckpoint" ||
					App->scene_intro->checkpointList.getLast()->data->name == "fourthcheckpoint")
				{
					tr.rotate(180, vec3(0, 1, 0));
				}
				vehicle->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
				vehicle->SetTransform(&tr);

				btVector3 v = App->scene_intro->checkpointList.getLast()->data->GetPosition();
				vehicle->SetLinearVelocity(0, 0, 0);
				vehicle->SetPos(v.x(), v.y(), v.z());
			}
		}
	}
}

btVector3 ModulePlayer::RotateVecToLocal(float x, float y, float z)
{
	btVector3 v = btVector3(x, y, z);
	btMatrix3x3 rotation = vehicle->vehicle->getChassisWorldTransform().getBasis();
	btVector3 ret = rotation * v;
	return ret;
}

void ModulePlayer::ResetPlayer()
{
	vehicle->SetLinearVelocity(0.0f, 0.0f, 0.0f);
	btTransform tr;

	tr.setIdentity();
	btQuaternion quat;
	quat.setEuler(-67.5f, 0, 0);

	tr.setRotation(quat);

	vehicle->vehicle->getRigidBody()->setCenterOfMassTransform(tr);

	vehicle->SetPos(-164.75f, 1.026f, -19.035f);

	turn = 0.0f;
	acceleration = 0.0f;
	brake = 0.0f;
	handbrake = 0.0f;

	turbo = 33.0f;
	turbosLeft = 0;

	groundContact = true;

}

void ModulePlayer::CameraPosition()
{
	//if (App->scene_intro->swapCamera == false)
	//{
	//	App->camera->Position.x = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - 10 * App->player->vehicle->vehicle->getForwardVector().getX();
	//	App->camera->Position.y = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() + 5 * App->player->vehicle->vehicle->getUpAxis();
	//	App->camera->Position.z = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - 10 * App->player->vehicle->vehicle->getForwardVector().getZ();
	//	//float x_value = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 10 * App->player->vehicle->vehicle->getForwardVector().getX();
	//	//float z_value = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 10 * App->player->vehicle->vehicle->getForwardVector().getZ();

	//	App->camera->LookAt(vec3(App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX(), App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY(), App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ()));
	//}
	//else
	//{
	//	App->camera->Position.x = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() - App->player->vehicle->vehicle->getForwardVector().getX() - 1;
	//	App->camera->Position.y = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getY() - App->player->vehicle->vehicle->getUpAxis() + 4;
	//	App->camera->Position.z = App->player->vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() - App->player->vehicle->vehicle->getForwardVector().getZ();

	//	btVector3 lookat = RotateVecToLocal(vehicle->vehicle->getChassisWorldTransform().getOrigin().getX(), vehicle->vehicle->getChassisWorldTransform().getOrigin().getY(), vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ());
	//	App->camera->LookAt(vec3(lookat.getX(), lookat.getY(), lookat.getZ()));
	//}

}
