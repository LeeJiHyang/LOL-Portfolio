#include "stdafx.h"
#include "NavMeshMap.h"
#include "MeshCollider.h"
#include "GameRenderer.h"
using namespace PKH;

PKH::NavMeshMap::NavMeshMap()
{
	isVisible = false;
	transform->scale = { 0.1f,0.1f, 0.1f };
	transform->Update();

	//transform->eulerAngles.y = D3DXToRadian(180.f);
	navMesh = RenderManager::CloneNavMesh(L"summoner_rift_nav");
	AddComponent(L"NavMesh", navMesh);
	AddComponent<MeshCollider>(L"MeshCollider");

	pPathFinder = new Astar();
	//SetNavigationInfo();
	LoadNavigationInfo();
	navMesh->renderGroupID = RenderGroupID::AlphablendForward;
	GameRenderer::Register(navMesh);
}

PKH::NavMeshMap::~NavMeshMap()
{
	delete pPathFinder;
	pPathFinder = nullptr;
	GameRenderer::Unregister(navMesh);
}

void PKH::NavMeshMap::Initialize()
{
}

void PKH::NavMeshMap::Release()
{
}

void PKH::NavMeshMap::Update()
{
	GameObject::Update();

	if (InputManager::GetKeyDown('P'))
	{
		isVisible = !isVisible;
	}
}

void PKH::NavMeshMap::SetNavigationInfo()
{
	DWORD triangleCount = navMesh->GetFaceCount();
	Vector3* pVertices = navMesh->GetVertices();
	DWORD* pIndices = navMesh->GetIndices();
	AdjacencyInfo* pAdjacencyInfo = navMesh->GetAdjacencyInfo();
	// ���� �ﰢ�� ������ ���ؼ�
	// �ﰢ���� ����
	// 
	for (DWORD i = 0; i < triangleCount; i++)
	{
		DWORD index = i * 3;
		Vector3 a = pVertices[pIndices[index]];
		Vector3 b = pVertices[pIndices[index + 1]];
		Vector3 c = pVertices[pIndices[index + 2]];

		
		// ���� �߽�
		Vector3 barycentricCoord;
		barycentricCoord = a + b + c;
		barycentricCoord.x /= 3.f;
		barycentricCoord.y /= 3.f;
		barycentricCoord.z /= 3.f;

		//����� �ٲ�� ��
		D3DXVec3TransformCoord(&barycentricCoord, &barycentricCoord, &transform->localMatrix);

		PathFinder::Node* node = new PathFinder::Node();
		node->position = barycentricCoord;
		pPathFinder->AddNode(i,node);
	}

	for (DWORD i = 0; i < triangleCount; i++)
	{
		if (pAdjacencyInfo[i].a != ULONG_MAX)
			pPathFinder->LinkNode(i, pAdjacencyInfo[i].a);
		if (pAdjacencyInfo[i].b != ULONG_MAX)
			pPathFinder->LinkNode(i, pAdjacencyInfo[i].b);
		if (pAdjacencyInfo[i].c != ULONG_MAX)
			pPathFinder->LinkNode(i, pAdjacencyInfo[i].c);

	}
	
}

void PKH::NavMeshMap::LoadNavigationInfo()
{
	map<int, vector<int>> adjacencyInfo;

	FileManager::SetDirectory("Data/Nav/Nav.dat");
	FileManager::OpenFile("rb");

	int lastID = 0;
	FileManager::ReadFile(&lastID, sizeof(int), 1);
	int count = 0;
	FileManager::ReadFile(&count, sizeof(int), 1);

	for (int i = 0; i < count; i++)
	{
		PathFinder::Node* node = new PathFinder::Node();
		
		int id;
		FileManager::ReadFile(&id, sizeof(int), 1);
		FileManager::ReadFile(&node->position.x, sizeof(float), 1);
		FileManager::ReadFile(&node->position.y, sizeof(float), 1);
		FileManager::ReadFile(&node->position.z, sizeof(float), 1);

		int adjCount = 0;
		FileManager::ReadFile(&adjCount, sizeof(int), 1);
		for (int j = 0; j < adjCount; j++)
		{
			int adj;
			FileManager::ReadFile(&adj, sizeof(int), 1);
			adjacencyInfo[id].push_back(adj);
		}

		pPathFinder->AddNode(id, node);
	}

	for (auto iter : adjacencyInfo)
	{
		int srcID = iter.first;
		int adjCount = iter.second.size();
		for (int i = 0; i < adjCount; i++)
		{
			int destID = iter.second[i];
			pPathFinder->LinkNode(srcID, destID);

		}
	}
	

	FileManager::CloseFile();
}

bool PKH::NavMeshMap::Search(const Vector3& start, const Vector3& dest)
{
	return pPathFinder->Search(start, dest);
}

PathFinder::Node* PKH::NavMeshMap::GetResultNode()
{
	return pPathFinder->GetResultNode();
}

list<PathFinder::Node*>* PKH::NavMeshMap::GetPath()
{
	return pPathFinder->GetPath();
}
