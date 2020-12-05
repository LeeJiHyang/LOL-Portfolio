#include "stdafx.h"
#include "TerrainMesh.h"

PKH::TerrainMesh::TerrainMesh(GameObject* owner)
    :Mesh(owner)
{
}

PKH::TerrainMesh::TerrainMesh(const TerrainMesh& rhs)
	: Mesh(rhs)
	, pAdjacency(rhs.pAdjacency)
	, pSubset(rhs.pSubset)
	, pMesh(rhs.pMesh)
	, pOriginMesh(rhs.pOriginMesh)
	, pMaterial(rhs.pMaterial)
	, subsetCount(rhs.subsetCount)
	, vertexCount(rhs.vertexCount)
	, vertexSize(rhs.vertexSize)
	, triangleCount(rhs.triangleCount)
	, pVertices(rhs.pVertices)
	, pIndices(rhs.pIndices)
	, subsetBoxArray(rhs.subsetBoxArray)
	, vertexBuffer(rhs.vertexBuffer)
	, indexBuffer(rhs.indexBuffer)
	, pAttributeTable(rhs.pAttributeTable)
{
	ppTextures = new LPDIRECT3DTEXTURE9[rhs.subsetCount];

	for (ULONG i = 0; i < rhs.subsetCount; ++i)
	{
		ppTextures[i] = rhs.ppTextures[i];
		Safe_AddRef(&ppTextures[i]);
	}

	Safe_AddRef(&pMesh);
	Safe_AddRef(&pOriginMesh);
	Safe_AddRef(&pAdjacency);
	Safe_AddRef(&pSubset);
	Safe_AddRef(&vertexBuffer);
	Safe_AddRef(&indexBuffer);
}

PKH::TerrainMesh::~TerrainMesh()
{
	for (ULONG i = 0; i < subsetCount; ++i)
		Safe_Release(&ppTextures[i]);

	Safe_Delete_Array(&ppTextures);
	ppTextures = nullptr;

	if (isClone == false)
	{
		Safe_Delete_Array(&pVertices);
		Safe_Delete_Array(&pIndices);
		Safe_Delete_Array(&subsetBoxArray);
		Safe_Delete_Array(&pAttributeTable);
	}

	Safe_Release(&pSubset);
	Safe_Release(&pAdjacency);
	Safe_Release(&pOriginMesh);
	Safe_Release(&pMesh);
	Safe_Release(&vertexBuffer);
	Safe_Release(&indexBuffer);
}

IComponent* PKH::TerrainMesh::Clone()
{
    return new TerrainMesh(*this);
}

HRESULT PKH::TerrainMesh::LoadMesh(const WCHAR* pFilePath, const WCHAR* pFileName)
{
	WCHAR		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	auto device = RenderManager::GetDevice();

	//==============================
	// X���� �޽� �ε�
	//==============================
	RenderManager::LockDevice();
	if (FAILED(D3DXLoadMeshFromXW(szFullPath, D3DXMESH_MANAGED, device,
		&pAdjacency, &pSubset, NULL, &subsetCount, &pOriginMesh)))
	{
		RenderManager::UnlockDevice();
		return E_FAIL;
	}
	RenderManager::UnlockDevice();

	
	
	//==============================
	// FVF & �븻 ����
	//==============================
	fvf = pOriginMesh->GetFVF();	// �޽��� ���� ���� FVF������ ������ �Լ�

	RenderManager::LockDevice();
	if (!(fvf & D3DFVF_NORMAL))
	{
		// �븻 ���� ���� ���
		pOriginMesh->CloneMeshFVF(pOriginMesh->GetOptions(), fvf |= D3DFVF_NORMAL, device, &pMesh);
		// �븻���
		D3DXComputeNormals(pMesh, (ULONG*)pAdjacency->GetBufferPointer());
	}
	else
	{
		pOriginMesh->CloneMeshFVF(pOriginMesh->GetOptions(), fvf, device, &pMesh);
	}
	RenderManager::UnlockDevice();

	HRESULT res = pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(), NULL, NULL, NULL);

	//==============================
	// ���ؽ����� ������ ���� ����
	//==============================
	void* pVertex = nullptr;

	vertexCount = pMesh->GetNumVertices();	// �޽��� ���� ������ ������ ��ȯ
	pVertices = new Vector3[vertexCount];

	pMesh->LockVertexBuffer(0, &pVertex);

	// ���� ���� �� position�� ��ġ ã��
	D3DVERTEXELEMENT9			Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	pMesh->GetDeclaration(Decl);

	UCHAR byOffset = 0;

	for (ULONG i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (UCHAR)Decl[i].Offset;
			break;
		}
	}
	// FVF ������ ���� ������ ũ�⸦ ��ȯ�ϴ� �Լ�
	vertexSize = D3DXGetFVFVertexSize(fvf);
	// ���ؽ��� ������������ ����
	for (ULONG i = 0; i < vertexCount; ++i)
	{
		pVertices[i] = *((Vector3*)(((UCHAR*)pVertex) + (i * vertexSize + byOffset)));
	}

	pMesh->UnlockVertexBuffer();

	//==============================
	// �ε��� ���� ����
	//==============================

	this->triangleCount = pMesh->GetNumFaces();
	int indexCount = triangleCount * 3;
	pIndices = new DWORD[indexCount];

	// TODO : �ε���16 or 32 ã�� �ڵ�
	// �ε��� ���� ����
	LPDIRECT3DINDEXBUFFER9 pIB;
	pMesh->GetIndexBuffer(&pIB);

	D3DINDEXBUFFER_DESC desc;
	pIB->GetDesc(&desc);
	D3DFORMAT format = desc.Format;
	UINT indexSize = 2; // INDEX16
	if (format == D3DFORMAT::D3DFMT_INDEX16)
	{
		WORD* dummyIndices = nullptr;
		pMesh->LockIndexBuffer(0, (void**)&dummyIndices);
		for (int i = 0; i < indexCount; i++)
		{
			WORD idx = dummyIndices[i];
			pIndices[i] = idx;
		}
		pMesh->UnlockIndexBuffer();
	}
	else
	{
		DWORD* dummyIndices = nullptr;
		pMesh->LockIndexBuffer(0, (void**)&dummyIndices);
		for (int i = 0; i < indexCount; i++)
		{
			DWORD idx = dummyIndices[i];
			pIndices[i] = idx;
		}
		pMesh->UnlockIndexBuffer();
	}

	//==============================
	// Attribute ID ã�� & Bounding Box Setting
	//==============================

	subsetBoxArray = new BoundingBox[subsetCount];
	for (int i = 0; i < subsetCount; i++)
	{
		// �ּҰ� �ʱ�ȭ 
		subsetBoxArray[i].minPos.x = FLT_MAX;
		subsetBoxArray[i].minPos.y = FLT_MAX;
		subsetBoxArray[i].minPos.z = FLT_MAX;
	}

	pAttributeTable = new D3DXATTRIBUTERANGE[subsetCount];
	memset(pAttributeTable, 0, sizeof(D3DXATTRIBUTERANGE)* subsetCount);
	attributeCount = subsetCount;

	DWORD* attributeBuffer = 0;

	pMesh->LockAttributeBuffer(0, &attributeBuffer);

	//attribute buffer�� read/write
	DWORD attID = 0;
	int oldID = -1;

	for (int i = 0; i < triangleCount; i++)
	{
		
		attID = attributeBuffer[i];
		DWORD index = i * 3;

		pAttributeTable[attID].AttribId = attID;
		pAttributeTable[attID].FaceCount++;
		if (oldID != attID)
		{
			pAttributeTable[attID].FaceStart = i;
			pAttributeTable[attID].VertexStart = i * 3;
			oldID = attID;
		}
		pAttributeTable[attID].VertexCount += 3;
		
			

		// ������� �ִ� x,y,z ������ ���ϱ�
		subsetBoxArray[attID].maxPos.x = max(subsetBoxArray[attID].maxPos.x, pVertices[pIndices[index]].x);
		subsetBoxArray[attID].maxPos.x = max(subsetBoxArray[attID].maxPos.x, pVertices[pIndices[index + 1]].x);
		subsetBoxArray[attID].maxPos.x = max(subsetBoxArray[attID].maxPos.x, pVertices[pIndices[index + 2]].x);

		subsetBoxArray[attID].maxPos.y = max(subsetBoxArray[attID].maxPos.y, pVertices[pIndices[index]].y);
		subsetBoxArray[attID].maxPos.y = max(subsetBoxArray[attID].maxPos.y, pVertices[pIndices[index + 1]].y);
		subsetBoxArray[attID].maxPos.y = max(subsetBoxArray[attID].maxPos.y, pVertices[pIndices[index + 2]].y);

		subsetBoxArray[attID].maxPos.z = max(subsetBoxArray[attID].maxPos.z, pVertices[pIndices[index]].z);
		subsetBoxArray[attID].maxPos.z = max(subsetBoxArray[attID].maxPos.z, pVertices[pIndices[index + 1]].z);
		subsetBoxArray[attID].maxPos.z = max(subsetBoxArray[attID].maxPos.z, pVertices[pIndices[index + 2]].z);

		// ������� �ִ� x,y,z ������ ���ϱ�
		subsetBoxArray[attID].minPos.x = min(subsetBoxArray[attID].minPos.x, pVertices[pIndices[index]].x);
		subsetBoxArray[attID].minPos.x = min(subsetBoxArray[attID].minPos.x, pVertices[pIndices[index + 1]].x);
		subsetBoxArray[attID].minPos.x = min(subsetBoxArray[attID].minPos.x, pVertices[pIndices[index + 2]].x);
		subsetBoxArray[attID].minPos.y = min(subsetBoxArray[attID].minPos.y, pVertices[pIndices[index]].y);
		subsetBoxArray[attID].minPos.y = min(subsetBoxArray[attID].minPos.y, pVertices[pIndices[index + 1]].y);
		subsetBoxArray[attID].minPos.y = min(subsetBoxArray[attID].minPos.y, pVertices[pIndices[index + 2]].y);
		subsetBoxArray[attID].minPos.z = min(subsetBoxArray[attID].minPos.z, pVertices[pIndices[index]].z);
		subsetBoxArray[attID].minPos.z = min(subsetBoxArray[attID].minPos.z, pVertices[pIndices[index + 1]].z);
		subsetBoxArray[attID].minPos.z = min(subsetBoxArray[attID].minPos.z, pVertices[pIndices[index + 2]].z);


		
	}

	pMesh->UnlockAttributeBuffer();

	//==============================
	// �Ӽ� ���̺� ����
	//==============================
	

	// ����´� ���������ǰ� �ݰ� ���ϱ�
	for (int i = 0; i < subsetCount; i++)
	{
		subsetBoxArray[i].center =  subsetBoxArray[i].minPos + (subsetBoxArray[i].maxPos - subsetBoxArray[i].minPos) * 0.5f;
		subsetBoxArray[i].radius = Vector3(subsetBoxArray[i].maxPos - subsetBoxArray[i].center).Length();

		
	}

	DWORD attCount;
	D3DXATTRIBUTERANGE a;
	pMesh->GetAttributeTable(&a, &attCount);

	//==============================
	// ��Ƽ���� & �ؽ�ó ���� ����
	//==============================
	// �޽��� ���� ���� ���� �� ù ��° �ּҸ� ��ȯ�Ͽ� ����
	pMaterial = (D3DXMATERIAL*)pSubset->GetBufferPointer();

	ppTextures = new LPDIRECT3DTEXTURE9[subsetCount];

	for (ULONG i = 0; i < subsetCount; ++i)
	{
		WCHAR	szFileName[256] = L"";

		lstrcpy(szFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP,
			0,
			pMaterial[i].pTextureFilename,
			strlen(pMaterial[i].pTextureFilename),
			szFileName,
			256);

		lstrcat(szFullPath, szFileName);

		HRESULT res = E_FAIL;
		RenderManager::LockDevice();
		res = D3DXCreateTextureFromFile(device, szFullPath, &ppTextures[i]);
		if (res != S_OK)
		{
			RenderManager::UnlockDevice();
			return E_FAIL;
		}


		RenderManager::UnlockDevice();
	}

	pMesh->GetVertexBuffer(&vertexBuffer);
	pMesh->GetIndexBuffer(&indexBuffer);


	return S_OK;
}

void PKH::TerrainMesh::Render()
{
	if (gameObject == nullptr) return;
	
	auto device = RenderManager::GetDevice();
	RenderManager::LockDevice();

	device->SetTransform(D3DTS_WORLD, &gameObject->transform->world);

	device->SetStreamSource(0, vertexBuffer, 0, vertexSize);
	device->SetFVF(fvf);
	device->SetIndices(indexBuffer);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	device->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	device->SetRenderState(D3DRS_LIGHTING, false);
	// TODO : �ٲ���� �ø��
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	int cullcount = 0;
	for (ULONG i = 0; i < subsetCount; ++i)
	{
		Vector3 worldCenter;
		D3DXVec3TransformCoord(&worldCenter, &subsetBoxArray[i].center, &gameObject->transform->world);
		if (Frustum::Intersect(&worldCenter, subsetBoxArray[i].radius) == false)
		{
			cullcount++;
			continue;
		}
		device->SetTexture(0, ppTextures[i]);
		//pMesh->DrawSubset(i);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, pAttributeTable[i].FaceStart * 3, pAttributeTable[i].FaceCount);
	}

	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	RenderManager::UnlockDevice();
}
