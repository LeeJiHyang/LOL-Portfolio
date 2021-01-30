#pragma once
#include "Mesh.h"

namespace PKH
{
	class HierarchyLoader;
	class AnimationController;
	
	class DynamicMesh : public Mesh
	{
	public:
		explicit DynamicMesh(GameObject* owner);
		explicit DynamicMesh(const DynamicMesh& rhs);
		virtual ~DynamicMesh();
		// Mesh��(��) ���� ��ӵ�
		virtual IComponent * Clone() override;
	public:
		HRESULT			LoadMesh(const WCHAR* pFilePath, const WCHAR* pFileName);
		virtual void Render(void) override;
		const  D3DXFRAME_DERIVED* GetFrameByName(const char* pFrameName);
		bool			IsAnimationSetEnd(void);
	public:
		void			SetAnimationSet(const UINT& iIndex);
		void			SetAnimationSet(const char* name);
		void			PlayAnimation(const float& fTimeDelta);

		bool			GetAnimationIndex(UINT* outIndex, const char* name);
		double			GetPeriod(const UINT& index);
	private:

		// ��� ������ ��ȸ�ϸ鼭 ������ ���� �ִ� TransformationMatrix�� pParentMatrix�� �����Ͽ� CombinedTransformationMatrix�� ������ִ� �Լ�
		void			UpdateFrameMatrices(D3DXFRAME_DERIVED* pFrame, const Matrix* pParentMatrix);

		// CombinedTransformationMatrix�� �ּҰ��� ���ͼ� ä���ִ� �Լ�
		void			SetUpFrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

		void			CloneFrame(D3DXFRAME_DERIVED* pRoot, D3DXFRAME** ppOutCloneFrame);
		void			DeleteFrame(D3DXFRAME* pRoot);
	public:
		// TODO : �̰͵� ��������� �� ��찡 �������?
		virtual ULONG GetVertexCount() override { return 0; }
		virtual ULONG GetVertexSize() override { return 0; }
		virtual Vector3* GetVertices() override { return nullptr; }
		virtual DWORD* GetIndices() override { return nullptr; }
		virtual ULONG GetFaceCount() override { return 0; }

	private:
		D3DXFRAME* m_pRootFrame;
		HierarchyLoader* m_pLoader;
		AnimationController* m_pAniCtrl;
		list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;

		map<string, UINT> m_AnimKeys;
	public:
		float animSpeed = 0.f;
		bool stopFlag = false;

	};
}


