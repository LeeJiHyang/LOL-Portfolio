#pragma once
#include "Mesh.h"

namespace PKH
{
	class DynamicMesh : public Mesh
	{
	public:
		explicit DynamicMesh(GameObject* owner);
		explicit DynamicMesh(const DynamicMesh& rhs);
		virtual ~DynamicMesh();
		// Mesh��(��) ���� ��ӵ�
		virtual IComponent * Clone() override;
	public:
		HRESULT			Ready_Meshes(const WCHAR* pFilePath, const WCHAR* pFileName);
		void			Render_Meshes(void);
		const  D3DXFRAME_DERIVED* Get_FrameByName(const char* pFrameName);
		bool							Is_AnimationSetEnd(void);
	public:
		void			Set_AnimationSet(const UINT& iIndex);
		void			Play_Animation(const float& fTimeDelta);

	private:

		// ��� ������ ��ȸ�ϸ鼭 ������ ���� �ִ� TransformationMatrix�� pParentMatrix�� �����Ͽ� CombinedTransformationMatrix�� ������ִ� �Լ�
		void			Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const Matrix* pParentMatrix);

		// CombinedTransformationMatrix�� �ּҰ��� ���ͼ� ä���ִ� �Լ�
		void			SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

	private:
		D3DXFRAME* m_pRootFrame;
		HierarchyLoader* m_pLoader;
		AnimationController* m_pAniCtrl;
		list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;

	};
}


