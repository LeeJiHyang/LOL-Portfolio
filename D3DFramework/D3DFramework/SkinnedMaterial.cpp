#include "stdafx.h"
#include "SkinnedMaterial.h"
//
//KST::SkinnedMeshMaterial::~SkinnedMeshMaterial()
//{
//
//}
//
//void KST::SkinnedMeshMaterial::Render()
//{
//	PKH::DynamicMesh* dmesh = static_cast<PKH::DynamicMesh*>(mesh);
//	dmesh->UpdateFrame();
//	int subsetCount = dmesh->GetSubsetCount();
//
//	IDirect3DDevice9* device = RenderManager::GetDevice();
//	device->SetTransform(D3DTS_WORLD, &gameObject->transform->world);
//	device->SetRenderState(D3DRS_LIGHTING, false);
//	std::list<D3DXMESHCONTAINER_DERIVED*> const& meshContainers = dmesh->GetMeshContainersRef();
//	for (auto& iter : meshContainers)
//	{
//		D3DXMESHCONTAINER_DERIVED* pMeshContainer = iter;
//
//		for (ULONG i = 0; i < pMeshContainer->dwNumBones; ++i)
//		{
//			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);
//		}
//
//		void* pSrcVtx = nullptr;
//		void* pDestVtx = nullptr;
//
//
//		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
//		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);
//
//		// ����Ʈ���� ��Ű���� �����ϴ� �Լ�(��Ű�� �� �ƴ϶� �ִϸ��̼� ���� ��, ������ ���� �������� ������ ���ÿ� �����ϱ⵵ ��)
//		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix,	// ���� ���� ��ȯ���� ���
//			nullptr,						// ���� ���·� �ǵ����� ���� ���� ���(������ �� ����� ������� ���ؼ� �־���� ������ �ȳ־��൵ ��� ����)
//			pSrcVtx,						// ������ �ʴ� ���� �޽��� ���� ����
//			pDestVtx);						// ��ȯ�� ������ ��� ���� �޽��� ���� ����
//		pMeshContainer->pOriMesh->UnlockVertexBuffer();
//		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
//
//		for (ULONG i = 0; i < pMeshContainer->NumMaterials; ++i)
//		{
//			device->SetTexture(0, pMeshContainer->ppTexture[i]);
//			this->material->SetSubset(i);
//			this->material->SetMesh(mesh)
//			pMeshContainer->MeshData.pMesh->DrawSubset(i);
//		}
//	}
//}
//
//void KST::SkinnedMeshMaterial::SetSubset(int )
//{
//}
//
//void KST::SkinnedMeshMaterial::SetMesh(PKH::Mesh* mesh)
//{
//	this->mesh = mesh;
//}
//
//void KST::SkinnedMeshMaterial::SetMaterial(Material* material)
//{
//	this->material = material;
//}
