#include "Quadtree.h"

#include "Globals.h"
#include "GameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "glew/include/GL/glew.h"

#include <stack>

#include "Profiling.h"

Quadtree::Quadtree() : root(nullptr)
{
}

Quadtree::~Quadtree()
{
	RELEASE(root);
}

void Quadtree::Create(AABB limits)
{
	root = new QuadtreeNode(limits);
}

void Quadtree::Clear()
{
	RELEASE(root);
}

void Quadtree::Insert(GameObject* go)
{
	if (root != nullptr)
	{
		if (go->GetAABB().Intersects(root->GetBox()))
		{
			root->Insert(go);
		}
	}
}

void Quadtree::Remove(GameObject* go)
{
	if (root != nullptr)
	{
		root->Remove(go);
	}
}

void Quadtree::Intersect(std::vector<GameObject*>& gos, Ray ray)
{
	if (root != nullptr) root->Intersect(gos, ray);
}

void Quadtree::CollectGo(std::vector<GameObject*>& gos)
{
	std::stack<QuadtreeNode*> stack;
	stack.push(root);

	while (!stack.empty())
	{
		QuadtreeNode* node = stack.top();
		for (int i = 0; i < node->GetObjects().size(); ++i)
			gos.push_back(node->GetObjects()[i]);

		stack.pop();

		for (int i = 0; i < 4; ++i)
			if (node->GetChild(i) != nullptr) stack.push(node->GetChild(i));
	}
}

void Quadtree::DebugDraw()
{
	if (root != nullptr)
	{
		root->DebugDraw();
	}
}

QuadtreeNode::QuadtreeNode(AABB limits) : box(limits)
{
	childs[0] = nullptr;
	childs[1] = nullptr;
	childs[2] = nullptr;
	childs[3] = nullptr;

	float3 corners[8];
	limits.GetCornerPoints(corners);

	unsigned int indices[24] =
	{
		0,1,
		1,3,
		3,2,
		2,0,

		1,5,
		4,6,
		7,3,

		6,7,
		6,2,

		7,5,
		4,5,

		4,0
	};

	vbo = new VertexBuffer(corners, sizeof(float3) * 8);
	ebo = new IndexBuffer(indices, 24);
}

QuadtreeNode::~QuadtreeNode()
{
	RELEASE(childs[0]);
	RELEASE(childs[1]);
	RELEASE(childs[2]);
	RELEASE(childs[3]);

	gameObjects.clear();

	RELEASE(vbo);
	RELEASE(ebo);
}

bool QuadtreeNode::IsSlotAvailable()
{
	return childs[0] == nullptr;
}

void QuadtreeNode::Insert(GameObject* go)
{
	if (IsSlotAvailable() && gameObjects.size() < 4)
	{
		gameObjects.push_back(go);
	}
	else
	{
		if (IsSlotAvailable()) CreateChilds();

		gameObjects.push_back(go);
		RedistributeChilds();
	}
}

void QuadtreeNode::Remove(GameObject* go)
{
	std::vector<GameObject*>::iterator it = std::find(gameObjects.begin(), gameObjects.end(), go);
	if (it != gameObjects.end())
		gameObjects.erase(it);

	if (IsSlotAvailable() == false)
	{
		for (int i = 0; i < 4; ++i)
			childs[i]->Remove(go);
	}
}

void QuadtreeNode::CreateChilds()
{
	float3 boxSize(box.Size());
	float3 childsSize(boxSize.x * 0.5f, boxSize.y, boxSize.z * 0.5f);

	float3 center(box.CenterPoint());
	float3 childCenter(center);
	AABB childAABB;

	// Upper left
	childCenter.x = center.x - (childsSize.x * 0.25f);
	childCenter.z = center.z + (childsSize.z * 0.25f);
	childAABB.SetFromCenterAndSize(childCenter, childsSize);
	childs[0] = new QuadtreeNode(childAABB);

	// Upper right
	childCenter.x = center.x + (childsSize.x * 0.25f);
	childCenter.z = center.z + (childsSize.z * 0.25f);
	childAABB.SetFromCenterAndSize(childCenter, childsSize);
	childs[1] = new QuadtreeNode(childAABB);

	// Bottom left
	childCenter.x = center.x - (childsSize.x * 0.25f);
	childCenter.z = center.z - (childsSize.z * 0.25f);
	childAABB.SetFromCenterAndSize(childCenter, childsSize);
	childs[2] = new QuadtreeNode(childAABB);

	// Bottom right
	childCenter.x = center.x + (childsSize.x * 0.25f);
	childCenter.z = center.z - (childsSize.z * 0.25f);
	childAABB.SetFromCenterAndSize(childCenter, childsSize);
	childs[3] = new QuadtreeNode(childAABB);
}

void QuadtreeNode::RedistributeChilds()
{
	for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end();)
	{
		GameObject* go = *it;
		AABB goAABB = go->GetAABB().MinimalEnclosingAABB();

		bool intersection[4];

		for (int i = 0; i < 4; ++i)
			intersection[i] = childs[i]->box.Intersects(goAABB);

		if (intersection[0] && intersection[1] && intersection[2] && intersection[3])
			++it;
		else
		{
			gameObjects.erase(it);
			for (int i = 0; i < 4; ++i)
				if (intersection[i]) childs[i]->Insert(go);
		}
	}
}

void QuadtreeNode::Intersect(std::vector<GameObject*>& gos, Ray ray)
{
	if (ray.Intersects(box))
	{
		for (std::vector<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			if (ray.Intersects((*it)->GetAABB()))
				gos.push_back(*it);
		}

		for (int i = 0; i < 4; ++i)
		{
			if (childs[i] != nullptr) childs[i]->Intersect(gos, ray);
		}
	}
}

void QuadtreeNode::CollectGo(std::vector<GameObject*>& gos)
{
	for (int i = 0; i < gameObjects.size(); ++i)
		gos.push_back(gameObjects[i]);

	if (childs[0] != nullptr)
	{
		for (int i = 0; i < 4; ++i)
			childs[i]->CollectGo(gos);
	}
}

void QuadtreeNode::DebugDraw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	vbo->Bind();
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	ebo->Bind();

	glDrawElements(GL_LINES, ebo->GetSize(), GL_UNSIGNED_INT, NULL);

	vbo->Unbind();
	ebo->Unbind();

	glDisableClientState(GL_VERTEX_ARRAY);

	if (!IsSlotAvailable())
	{
		for (int i = 0; i < 4; ++i)
			childs[i]->DebugDraw();
	}
}