#pragma once

#include "MathGeoLib/src/Geometry/AABB.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

#include <set>

class GameObject;
class CameraComponent;
class VertexBuffer;
class IndexBuffer;

class QuadtreeNode
{
public:
	QuadtreeNode(AABB limits);
	~QuadtreeNode();

	inline bool IsSlotAvailable() { return childs[0] == nullptr; }

	void Insert(GameObject* go);
	inline void InsertGO(GameObject* go) { gameObjects.push_back(go); }
	inline void Remove(std::vector<GameObject*>::const_iterator it) { gameObjects.erase(it); }
	void CreateChilds();
	void RedistributeChilds();
	void DebugDraw();

	inline const AABB& GetBox() const { return box; }
	inline const std::vector<GameObject*>& GetObjects() const { return gameObjects; }
	inline QuadtreeNode* GetChild(int index) const { return childs[index]; }

private:
	QuadtreeNode* childs[4];
	std::vector<GameObject*> gameObjects;
	AABB box;

	VertexBuffer* vbo;
	IndexBuffer* ebo;
};

class Quadtree
{
public:
	Quadtree();
	~Quadtree();

	void Create(AABB limits);
	void Clear();
	void Insert(GameObject* go);
	void Remove(GameObject* go);
	
	void Intersect(std::vector<GameObject*>& gos, Ray primitive);
	
	void Intersect(std::set<GameObject*>& gos, CameraComponent* frustum);
	void CollectGo(std::vector<GameObject*>& gos);
	void DebugDraw();

private:
	QuadtreeNode* root;
};