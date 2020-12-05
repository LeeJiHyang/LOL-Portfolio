#include "stdafx.h"
#include "StaticMesh.h"

PKH::StaticMesh::StaticMesh(GameObject* owner)
	:Mesh(owner)
{
}

PKH::StaticMesh::StaticMesh(const StaticMesh& rhs)
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
}

PKH::StaticMesh::~StaticMesh()
{
	for (ULONG i = 0; i < subsetCount; ++i)
		Safe_Release(&ppTextures[i]);

	Safe_Delete_Array(&ppTextures);
	ppTextures = nullptr;

	if (isClone == false)
	{
		Safe_Delete_Array(&pVertices);
		Safe_Delete_Array(&pIndices);
	}
	
	Safe_Release(&pSubset);
	Safe_Release(&pAdjacency);
	Safe_Release(&pOriginMesh);
	Safe_Release(&pMesh);

}

IComponent* PKH::StaticMesh::Clone()
{
	return new StaticMesh(*this);
}


HRESULT PKH::StaticMesh::LoadMesh(const WCHAR* pFilePath, const WCHAR* pFileName)
{
	WCHAR		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	auto device = RenderManager::GetDevice();

	//==============================
	// X���� �޽� �ε�
	//==============================
	RenderManager::LockDevice();
	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, device,
		&pAdjacency, &pSubset, NULL, &subsetCount, &pOriginMesh)))
	{
		RenderManager::UnlockDevice();
		return E_FAIL;
	}
	RenderManager::UnlockDevice();

	//==============================
	// FVF & �븻 ����
	//==============================
	ULONG	dwFVF = pOriginMesh->GetFVF();	// �޽��� ���� ���� FVF������ ������ �Լ�

	RenderManager::LockDevice();
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		// �븻 ���� ���� ���
		pOriginMesh->CloneMeshFVF(pOriginMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, device, &pMesh);
		// �븻���
		D3DXComputeNormals(pMesh, (ULONG*)pAdjacency->GetBufferPointer());
	}
	else
	{
		pOriginMesh->CloneMeshFVF(pOriginMesh->GetOptions(), dwFVF, device, &pMesh);
	}
	RenderManager::UnlockDevice();


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
	vertexSize = D3DXGetFVFVertexSize(dwFVF);
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

	return S_OK;
}

void PKH::StaticMesh::Render()
{
	if (gameObject == nullptr) return;

	auto device = RenderManager::GetDevice();
	RenderManager::LockDevice();

	device->SetTransform(D3DTS_WORLD, &gameObject->transform->world);

	device->SetRenderState(D3DRS_LIGHTING, false);
	// TODO : �ٲ���� �ø��
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	for (ULONG i = 0; i < subsetCount; ++i)
	{
		device->SetTexture(0, ppTextures[i]);
		pMesh->DrawSubset(i);
	}

	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	RenderManager::UnlockDevice();
}
