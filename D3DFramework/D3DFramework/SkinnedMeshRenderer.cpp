#include "stdafx.h"
#include "SkinnedMeshRenderer.h"
#include <wrl.h>
using namespace Microsoft::WRL;


KST::SkinnedMeshRenderer::SkinnedMeshRenderer(PKH::GameObject* owner):
	Renderer(owner, RendererType::Forward)
{

}

void KST::SkinnedMeshRenderer::Render()
{
	IDirect3DDevice9* device = RenderManager::GetDevice();
	ComPtr<IDirect3DSurface9> surface;
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface);
	device->SetRenderTarget(0, surface.Get());

	device->SetTransform(D3DTS_WORLD, &gameObject->transform->worldMatrix );
	device->SetRenderState(D3DRS_LIGHTING, false);
	std::list<D3DXMESHCONTAINER_DERIVED*> const& meshContainers = this->mesh->GetMeshContainersRef();
	mesh->UpdateFrame();
	for (auto& iter : meshContainers)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = iter;

		for (ULONG i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * (*pMeshContainer->ppFrameCombinedMatrix[i]);
		}

		void* pSrcVtx = nullptr;
		void* pDestVtx = nullptr;


		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		// ����Ʈ���� ��Ű���� �����ϴ� �Լ�(��Ű�� �� �ƴ϶� �ִϸ��̼� ���� ��, ������ ���� �������� ������ ���ÿ� �����ϱ⵵ ��)
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix,	// ���� ���� ��ȯ���� ���
			nullptr,						// ���� ���·� �ǵ����� ���� ���� ���(������ �� ����� ������� ���ؼ� �־���� ������ �ȳ־��൵ ��� ����)
			pSrcVtx,						// ������ �ʴ� ���� �޽��� ���� ����
			pDestVtx);						// ��ȯ�� ������ ��� ���� �޽��� ���� ����
		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		for (ULONG i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			device->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
}

void KST::SkinnedMeshRenderer::SetMesh(DynamicMesh* mesh)
{
	this->mesh = mesh;
}
