#include "stdafx.h"
#include "DynamicMesh.h"

PKH::DynamicMesh::DynamicMesh(GameObject* owner)
	:Mesh(owner)
	, m_pRootFrame(nullptr)
	, m_pLoader(nullptr)
	, m_pAniCtrl(nullptr)
{
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
	Safe_Release(m_pAniCtrl);

	if (false == isClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}
	m_MeshContainerList.clear();
}

IComponent * PKH::DynamicMesh::Clone()
{
	return new DynamicMesh(*this);
}

HRESULT DynamicMesh::Ready_Meshes(const WCHAR* pFilePath, const WCHAR* pFileName)
{
	WCHAR		szFullPath[256] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

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

	Safe_Release(pAniCtrl);

	Matrix		matTemp;
	D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f));
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void DynamicMesh::Render_Meshes(void)
{
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


		for (ULONG i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			device->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

const D3DXFRAME_DERIVED* DynamicMesh::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

bool DynamicMesh::Is_AnimationSetEnd(void)
{
	return m_pAniCtrl->Is_AnimationSetEnd();
}

void DynamicMesh::Set_AnimationSet(const UINT& iIndex)
{
	m_pAniCtrl->Set_AnimationSet(iIndex);
}

void DynamicMesh::Play_Animation(const float& fTimeDelta)
{
	m_pAniCtrl->Play_Animation(fTimeDelta);

	Matrix		matTemp;
	D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f));
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);
}

void DynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const Matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);

}

void DynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
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
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

}


