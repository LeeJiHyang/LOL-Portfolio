#pragma once
#include "Mesh.h"

namespace PKH
{
    class StaticMesh : public Mesh
    {
    public:
        StaticMesh();
        StaticMesh(const StaticMesh& rhs);
        virtual ~StaticMesh();

    public:
        virtual IComponent* Clone() override;

    public:
        const ULONG& GetVertexCount() { return this->vertexCount; }
        const ULONG& GetVertexSize() { return this->vertexSize; }
        const Vector3* GetVertices() { return this->pVertices; }

    public:
        HRESULT LoadMesh(const WCHAR* pFilePath, const WCHAR* pFileName);
        void Render();

    private:
        LPD3DXMESH			pOriginMesh;	// ���� �ε� ������ �����ϴ� �޽� �İ�ü
        LPD3DXMESH			pMesh; // �븻 ������ �����Ͽ� ��ȯ��Ų �޽� �İ�ü

        LPD3DXBUFFER		pAdjacency; // ������ �̿��� ������ �����ϱ� ���� ù��° �ּҸ� �����Ϸ��� ��� ����
        LPD3DXBUFFER		pSubset; // ����� ���� == �ؽ����� ���� == ������ ����
        D3DXMATERIAL*       pMaterial;
        ULONG				subsetCount;

        // VertexSize (���� �޸� ũ��)
        ULONG				vertexSize;
        ULONG				vertexCount;
        Vector3*            pVertices;

        LPDIRECT3DTEXTURE9* ppTextures;
    };

}

