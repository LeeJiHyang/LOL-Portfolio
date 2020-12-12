#pragma once
#include "Mesh.h"

namespace PKH
{
    struct AdjacencyInfo
    {
        DWORD a = ULONG_MAX;
        DWORD b = ULONG_MAX;
        DWORD c = ULONG_MAX;
    };

    

    class NavMesh : public Mesh
    {
    public:
        explicit NavMesh(GameObject* owner);
        explicit NavMesh(const NavMesh& rhs);
        virtual ~NavMesh();

    public:
        virtual IComponent* Clone() override;

    public:
        virtual Vector3* GetVertices() override { return this->pVertices; }
        virtual DWORD* GetIndices() override { return this->pIndices; }
        virtual ULONG GetVertexCount() override { return this->vertexCount; }
        virtual ULONG GetVertexSize() override { return this->vertexSize; }
        virtual ULONG GetFaceCount() override { return this->triangleCount; }

        AdjacencyInfo* GetAdjacencyInfo() { return this->pAdjacencyInfo; }
    public:
        HRESULT LoadMesh(const WCHAR* pFilePath, const WCHAR* pFileName);
    
        virtual void Render() override;



    private:
        LPD3DXMESH			pOriginMesh;	// ���� �ε� ������ �����ϴ� �޽� �İ�ü
        LPD3DXMESH			pMesh; // �븻 ������ �����Ͽ� ��ȯ��Ų �޽� �İ�ü

        LPD3DXBUFFER		pAdjacency; // ������ �̿��� ������ �����ϱ� ���� ù��° �ּҸ� �����Ϸ��� ��� ����
        LPD3DXBUFFER		pSubset; // ����� ���� == �ؽ����� ���� == ������ ����
        D3DXMATERIAL* pMaterial;
        ULONG				subsetCount;

        LPDIRECT3DTEXTURE9* ppTextures;

        // VertexSize (���� �޸� ũ��)
        ULONG				vertexSize = 0;
        ULONG				vertexCount = 0;
        Vector3* pVertices;// = nullptr;
        DWORD* pIndices;// = nullptr;
        ULONG triangleCount = 0;
        DWORD fvf;

        //=====================
        // �׺�޽� ����
        //=====================
        // ���������� ����
        AdjacencyInfo* pAdjacencyInfo = nullptr;
    };

}

