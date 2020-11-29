#include "stdafx.h"
#include "MeshCollider.h"

PKH::MeshCollider::MeshCollider(GameObject* owner)
    :Collider(owner)
{
    // �����ϴ� �޽� ���� (���̳��� �޽��� ���� ������ ��� ����Ƿ� �̷� �ڵ尡 �Ұ���)
    StaticMesh* smesh = (StaticMesh*)owner->GetComponent<StaticMesh>();
    if (smesh != nullptr)
    {
        sharedMesh = smesh;
    }
    else
    {
        CustomMesh* cmesh = (CustomMesh*)owner->GetComponent<CustomMesh>();
        if (cmesh != nullptr)
        {
            sharedMesh = cmesh;
        }
    }
}

PKH::MeshCollider::MeshCollider(const MeshCollider& rhs)
    :Collider(rhs)
{
   
}

PKH::MeshCollider::~MeshCollider()
{
    sharedMesh = nullptr;
}

IComponent* PKH::MeshCollider::Clone()
{
    return new MeshCollider(*this);
}

bool PKH::MeshCollider::Raycast(Ray ray, RaycastHit* outHitInfo, float maxDistance)
{
    Mesh* sharedMesh = nullptr;
    StaticMesh* smesh = (StaticMesh*)gameObject->GetComponent<StaticMesh>();
    if (smesh != nullptr)
    {
        sharedMesh = smesh;
    }
    else
    {
        CustomMesh* cmesh = (CustomMesh*)gameObject->GetComponent<CustomMesh>();
        if (cmesh != nullptr)
        {
            sharedMesh = cmesh;
        }
    }
    if (sharedMesh == nullptr) return false;
    Vector3* vertexPositions = nullptr;
    DWORD* indices = nullptr;
    // ���ؽ� ���� ��������
    // �ε��� ���� ��������
    vertexPositions = sharedMesh->GetVertices();
    if (vertexPositions == nullptr) return false;
    ULONG vertexCount = sharedMesh->GetVertexCount();
    indices = sharedMesh->GetIndices();
    if (indices == nullptr)return false;
    ULONG triangleCount = sharedMesh->GetFaceCount();
    // 

    // TODO : (����)���ÿ��� �浹�˻縦 �ؾ� ��������...?
    Matrix matWorld = transform->world;

    float u, v, dist;
    DWORD index, ind;
    Vector3 v1;
    Vector3 v2;
    Vector3 v3;
    for (int i = 0; i < triangleCount; i++)
    {
        index = i * 3;
        ind = indices[index];
        v1 = vertexPositions[indices[index]];
        v2 = vertexPositions[indices[index + 1]];
        v3 = vertexPositions[indices[index + 2]];

        D3DXVec3TransformCoord(&v1, &v1, &matWorld);
        D3DXVec3TransformCoord(&v2, &v2, &matWorld);
        D3DXVec3TransformCoord(&v3, &v3, &matWorld);


        if (D3DXIntersectTri(&v1, &v2, &v3,
            &ray.origin, &ray.direction, &u, &v, &dist))
        {
            if (outHitInfo != nullptr)
            {
                // V1 + U(V2 - V1) + V(V3 - V1)
                outHitInfo->point = v1 + u * (v2 - v1) + v * (v3 - v1);
                outHitInfo->distance = dist;
                outHitInfo->collider = this;
            }

            return true;
        }
    }


    return false;
}
