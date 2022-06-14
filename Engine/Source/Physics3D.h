#pragma once
#include "Module.h"

#include "Shapes.h"
#include "LinearMath/btIDebugDraw.h"

#define GRAVITY btVector3(0.0f, -10.0f, 0.0f)

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

class DebugDrawer;

class btCollisionShape;
class btDefaultMotionState;
class btTypedConstraint;

class btRigidBody;
class RigidBodyComponent;
class GameObject;
class Mesh;

enum class Axis {
	Y = 1,
	X = 2,
	Z = 3
};

class Physics3D : public Module
{
public:
	Physics3D(bool start_enabled = true);
	~Physics3D();

	bool Start() override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	bool LoadConstraints();

	btRigidBody* CollisionShape(const PCube& cube, RigidBodyComponent* component);
	btRigidBody* CollisionShape(const PSphere& sphere, RigidBodyComponent* component);
	btRigidBody* CollisionShape(const PCapsule& capsule, RigidBodyComponent* component);
	btRigidBody* CollisionShape(const PCylinder& cylinder, RigidBodyComponent* component, Axis axis = Axis::Y);
	btRigidBody* CollisionShape(const PPyramid& cone, RigidBodyComponent* component);
	btRigidBody* CollisionShape(const PPlane& plane, RigidBodyComponent* component);
	btRigidBody* CollisionShape(const std::shared_ptr<Mesh> mesh, RigidBodyComponent* component);

	btRigidBody* AddBody(btCollisionShape* colShape, btTransform startTransform, RigidBodyComponent* component);
	void DeleteBody(RigidBodyComponent* body, std::string name);
	void DesactivateCollision(btRigidBody* body);
	void ActivateCollision(btRigidBody* body);

	void AddConstraintP2P(btRigidBody& bodyA, btRigidBody& bodyB, const float3& anchorA, const float3& anchorB);
	void AddConstraintHinge(btRigidBody& bodyA, btRigidBody& bodyB, const float3& anchorA, const float3& anchorB, const float3& axisS, const float3& axisB, bool disable_collision = false);

	inline std::vector<RigidBodyComponent*> GetBodies() { return bodies; };
	inline std::vector<std::string> GetBodiesNames() { return bodiesNames; };
	inline bool& GetDebugMode() { return debug; };
	inline void SetDebugDrawing(bool value) { debug = value; }

	void DebugDraw();

	void SleepAllBodies();
	void ActiveAllBodies();

private:
	btDefaultCollisionConfiguration* collisionConfig = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btBroadphaseInterface* broadPhase = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld * world = nullptr;

	DebugDrawer* debugDraw = nullptr;

	std::vector<RigidBodyComponent*> bodies;
	std::vector<std::string> bodiesNames;

	bool debug = true;
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	// This use camelCase because use virtual functions of btIDebugDraw
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode = DBG_DrawWireframe;
	PLine line;
	Shape point;
};

