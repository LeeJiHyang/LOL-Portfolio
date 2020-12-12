#include "stdafx.h"
#include "DynamicMesh.h"
#include "HierarchyLoader.h"
#include "AnimationController.h"

PKH::DynamicMesh::DynamicMesh(GameObject* owner)
	:Mesh(owner)
	, m_pRootFrame(nullptr)
	, m_pLoader(nullptr)
	, m_pAniCtrl(nullptr)
{
	type = MeshType::DYNAMIC_MESH;
}

PKH::DynamicMesh::DynamicMesh(const DynamicMesh& rhs)
	:Mesh(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
{
	m_pAniCtrl = AnimationController::Create(*rhs.m_pAniCtrl);
}

PKH::DynamicMesh::~DynamicMesh()
{
	Safe_Release(&m_pAniCtrl);

	if (false == isClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(&m_pLoader);
	}
	m_MeshContainerList.clear();
}

IComponent * PKH::DynamicMesh::Clone()
{
	return new DynamicMesh(*this);
}

HRESULT DynamicMesh::LoadMesh(const WCHAR* pFilePath, const WCHAR* pFileName)
{
	WCHAR		szFullPath[256] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	device = RenderManager::GetDevice();
	m_pLoader = HierarchyLoader::Create(device, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	FAILED_CHECK_RETURN(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		device,
		m_pLoader,	// HierarchyLoader
		NULL,
		&m_pRootFrame,
		&pAniCtrl),	// AniCtrl
		E_FAIL);

	m_pAniCtrl = AnimationController::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);

	Safe_Release(&pAniCtrl);

	Matrix		matTemp;// = Matrix::identity;
	D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f));
	UpdateFrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);

	SetUpFrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void DynamicMesh::Render(void)
{
	if (gameObject == nullptr)return;
	
	//RenderManager::GetDevice()->SetTransform(D3DTS_WORLD, &gameObject->transform->world);

	Matrix		matTemp = gameObject->transform->world;
	//D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f));
	matTemp._41 = 0.f;
	matTemp._42 = 0.f;
	matTemp._43 = 0.f;
	//UpdateFrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &);
	UpdateFrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);

	for (auto& iter : m_MeshContainerList)
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

		device = RenderManager::GetDevice();
		RenderManager::LockDevice();

		device->SetTransform(D3DTS_WORLD, &gameObject->transform->world);
		device->SetRenderState(D3DRS_LIGHTING, false);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		for (ULONG i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			device->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		device->SetRenderState(D3DRS_LIGHTING, false);

		RenderManager::UnlockDevice();

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

const D3DXFRAME_DERIVED* DynamicMesh::GetFrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

bool DynamicMesh::IsAnimationSetEnd(void)
{
	return m_pAniCtrl->IsAnimationSetEnd();
}

void DynamicMesh::SetAnimationSet(const UINT& iIndex)
{
	m_pAniCtrl->SetAnimationSet(iIndex);
}

void DynamicMesh::PlayAnimation(const float& fTimeDelta)
{
	m_pAniCtrl->PlayAnimation(fTimeDelta);

	
}

void DynamicMesh::UpdateFrameMatrices(D3DXFRAME_DERIVED* pFrame, const Matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		UpdateFrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		UpdateFrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);

}

void DynamicMesh::SetUpFrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (ULONG i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			const char* pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED* pBone = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);

			pDerivedMeshContainer->ppFrameCombinedMatrix[i] = &pBone->CombinedTransformationMatrix;
		}

		m_MeshContainerList.push_back(pDerivedMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUpFrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUpFrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

}


