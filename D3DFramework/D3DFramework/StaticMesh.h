#pragma once
#include "Mesh.h"

namespace Engine
{
    class StaticMesh : public Mesh
    {
    public:
        explicit StaticMesh(GameObject* owner);
        explicit StaticMesh(const StaticMesh& rhs);
        virtual ~StaticMesh();

    public:
        virtual IComponent* Clone() override;

    public:
        virtual Vector3* GetVertices() override { return this->pVertices; }
        virtual DWORD* GetIndices() override { return this->pIndices; }
        virtual ULONG GetVertexCount() override { return this->vertexCount; }
        virtual ULONG GetVertexSize() override { return this->vertexSize; }
        virtual ULONG GetFaceCount() override { return this->triangleCount; }

        int GetSubsetCount();
        IDirect3DTexture9* GetSubsetTexture(int index);

    public:
        HRESULT LoadMesh(const WCHAR* pFilePath, const WCHAR* pFileName);
        virtual void RenderSubset(int index) override;

    private:
        LPD3DXMESH			pOriginMesh;	// ���� �ε� ������ �����ϴ� �޽� �İ�ü
        LPD3DXMESH			pMesh; // �븻 ������ �����Ͽ� ��ȯ��Ų �޽� �İ�ü

        LPD3DXBUFFER		pAdjacency; // ������ �̿��� ������ �����ϱ� ���� ù��° �ּҸ� �����Ϸ��� ��� ����
        LPD3DXBUFFER		pSubset; // ����� ���� == �ؽ����� ���� == ������ ����
        D3DXMATERIAL*       pMaterial;
        ULONG				subsetCount;

        LPDIRECT3DTEXTURE9* ppTextures;

        // VertexSize (���� �޸� ũ��)
        ULONG				vertexSize = 0;
        ULONG				vertexCount = 0;
        Vector3* pVertices;// = nullptr;
        DWORD* pIndices;// = nullptr;
        ULONG triangleCount = 0;
        DWORD fvf;



    };

}

