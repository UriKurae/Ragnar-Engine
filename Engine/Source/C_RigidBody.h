#pragma once
#include "Component.h"
#include "Shapes.h"

class GameObject;
class btRigidBody;
struct PhysVehicle3D;

enum class CollisionType
{
	BOX = 0,
	SPHERE = 1,
	CAPSULE = 2,
	CYLINDER = 3,
	CONE = 4,
	STATIC_PLANE = 5,
};

class RigidBodyComponent : public Component
{
public:
	RigidBodyComponent(GameObject* obj, CollisionType type = CollisionType::BOX, float mass = 1.0f,  bool isKinematic = false);
	virtual ~RigidBodyComponent();

	void SetBoundingBox();
	bool Update(float dt) override;
	void UpdateCollision();
	void OnEditor() override;

	void Combos();

	void AddConstraintP2P(RigidBodyComponent* const& val);

	void SetCollisionType(CollisionType type);
	void ResetLocalValues();
	void EditCollisionMesh();

	float4x4 btScalarTofloat4x4(btScalar* transform);

	void CreateBody();
	float GetMass() { return mass; };
	void SetMass(float mass);
	btRigidBody* GetBody() { return body; };
	CollisionType GetCollisionType() { return collisionType; };

	void SetAsStatic();

	bool useGravity = true;
	bool isKinematic = false;
	std::vector<RigidBodyComponent*> constraintBodies;
	std::vector<int> bodiesUIDs;

private:
	btRigidBody* body = nullptr;
	CollisionType collisionType = CollisionType::BOX;

	float mass = 1.0f;
	float friction = 1.0f;
	float restitution = 0.2f;
	float3 offset = { 0,0,0 };

	// Block the movement and rotation
	float3 movementConstraint = float3::one;
	float3 rotationConstraint = float3::one;

	// Having linear damping at zero means objects will keep moving until friction slows them down.
	float linearDamping = 0.0f;
	float angularDamping = 0.0f;

	// Figures
	PCube box;
	PSphere sphere;
	PCapsule capsule;
	PCylinder cylinder;
	PPyramid cone;
	PPlane plane;

	bool editMesh = false;
};
