#pragma once

#include "MathGeoLib/src/Geometry/AABB.h"
#include "MathGeoLib/src/Geometry/Frustum.h"

class GameObject;
class VertexBuffer;
class IndexBuffer;

class QuadtreeNode
{
public:
	QuadtreeNode(AABB limits);
	~QuadtreeNode();

	bool IsSlotAvailable();

	void Insert(GameObject* go);
	void Remove(GameObject* go);
	void CreateChilds();
	void RedistributeChilds();
	void Intersect(std::vector<GameObject*>& gos, Ray ray);
	void CollectGo(std::vector<GameObject*>& gos);
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
	void Intersect(std::vector<GameObject*>& gos, Ray ray);
	void CollectGo(std::vector<GameObject*>& gos);
	void DebugDraw();

private:
	QuadtreeNode* root;
};