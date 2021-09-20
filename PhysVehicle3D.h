#pragma once

#include "PhysBody3D.h"
#include "glmath.h"
#include "Primitive.h"

class btRaycastVehicle;
struct PhysBody3D;
class btQuaternion;
class btVector3;

struct Wheel
{
	vec3 connection; // origin of the ray. Must come from within the chassis
	vec3 direction; 
	vec3 axis;
	float suspensionRestLength; // max length for suspension in meters
	float radius;
	float width;
	bool front; // is front wheel ?
	bool drive; // does this wheel received engine power ?
	bool brake; // does breakes affect this wheel ?
	bool steering; // does this wheel turns ?
};

struct VehicleInfo
{
	~VehicleInfo();
	
	vec3 chassis_size;
	vec3 chassis_offset;

	vec3 front_chassis_size;
	vec3 front_chassis_offset;

	vec3 spoiler_size;
	vec3 spoiler_offset;

	vec3 front_wing_right_support_size;
	vec3 front_wing_right_support_offset;

	vec3 front_wing_left_support_size;
	vec3 front_wing_left_support_offset;

	vec3 front_wing_size;
	vec3 front_wing_offset;

	vec3 front_wing_right_size;
	vec3 front_wing_right_offset;

	vec3 front_wing_left_size;
	vec3 front_wing_left_offset;

	vec3 cockpit_size;
	vec3 cockpit_offset;

	vec3 back_cockpit_size;
	vec3 back_cockpit_offset;

	vec3 right_spoiler_support_size;
	vec3 right_spoiler_support_offset;

	vec3 left_spoiler_support_size;
	vec3 left_spoiler_support_offset;

	vec3 t_base_size;
	vec3 t_base_offset;

	vec3 t_up_size;
	vec3 t_up_offset;

	vec3 antenna_size;
	vec3 antenna_offset;

	int turbosLeft = 0;

	float mass;
	float suspensionStiffness; // default to 5.88 / 10.0 offroad / 50.0 sports car / 200.0 F1 car
	float suspensionCompression; // default to 0.83
	float suspensionDamping; // default to 0.88 / 0..1 0 bounces / 1 rigid / recommended 0.1...0.3
	float maxSuspensionTravelCm; // default to 500 cm suspension can be compressed
	float frictionSlip; // defaults to 10.5 / friction with the ground. 0.8 should be good but high values feels better (kart 1000.0)
	float maxSuspensionForce; // defaults to 6000 / max force to the chassis

	Wheel* wheels;
	int num_wheels;
};


struct PhysVehicle3D : public PhysBody3D
{
public:
	PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info);
	~PhysVehicle3D();

	Cube CreateCubeComponent(vec3 size, vec3 offset, Color color);

	void Render();
	void ApplyEngineForce(float force);
	void Brake(float force);
	void Handbrake(float force);
	void Turn(float degrees);
	float GetKmh() const;

	vec3 GetPos();


public:

	VehicleInfo info;
	btRaycastVehicle* vehicle;
	/*Cylinder* wheel;
	btQuaternion* q;
	Cube* chassis;
	btVector3* offset;
	Cube* cockpit;
	btVector3* cp_offset;
	Cube* leftSpoilerSupport;
	btVector3* lss_offset;
	Cube* rightSpoilerSupport;
	btVector3* rss_offset;
	Cube* spoiler;
	btVector3* s_offset;*/

	//p2List<Primitive*> carComponents;
};