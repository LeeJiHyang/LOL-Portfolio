#include "stdafx.h"
#include "SphereCollider.h"

using namespace PKH;

PKH::SphereCollider::SphereCollider(GameObject* owner)
    :Collider(owner)
{
    type = ColliderType::Sphere;
    LPD3DXMESH mesh;
    radius = 0.5f;
    auto device = RenderManager::GetDevice();
    RenderManager::LockDevice();
    D3DXCreateSphere(device, radius, 16, 16, &mesh, nullptr);

    fvf = mesh->GetFVF();
    if (!(fvf & D3DFVF_DIFFUSE))
    {
        mesh->CloneMeshFVF(mesh->GetOptions(), fvf |= D3DFVF_DIFFUSE, device, &pMesh);
    }
    else
    {
        mesh->CloneMeshFVF(mesh->GetOptions(), fvf, device, &pMesh);
    }

    void* pVertex = nullptr;
    pMesh->LockVertexBuffer(0, &pVertex);
    // ���� ���� ��ġ ã�� (�ʷϻ����� �ٲٷ���)
    D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
    ZeroMemory(decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

    pMesh->GetDeclaration(decl);

    BYTE offset = 0;
    for (int i = 0; i < MAX_FVF_DECL_SIZE; i++)
    {
        if (decl[i].Usage == D3DDECLUSAGE_COLOR)
        {
            offset = (BYTE)decl[i].Offset;
            break;
        }
    }
    int vertexCount = pMesh->GetNumVertices();
    int vertexSize = D3DXGetFVFVertexSize(fvf);
    for (int i = 0; i < vertexCount; ++i)
    {
        DWORD* pColor = ((DWORD*)(((BYTE*)pVertex) + (i * vertexSize + offset)));
        *pColor = D3DCOLOR_ARGB(255, 0, 255, 0);
    }

    pMesh->UnlockVertexBuffer();

    RenderManager::UnlockDevice();

    SetMeshInformation();
}

PKH::SphereCollider::SphereCollider(const SphereCollider& rhs)
    :Collider(rhs)
{
}

PKH::SphereCollider::~SphereCollider()
{
}

IComponent* PKH::SphereCollider::Clone()
{
    return new SphereCollider(*this);
}

bool PKH::SphereCollider::Raycast(Ray ray, RaycastHit* outHitInfo, float maxDistance)
{
    // c = center of Sphere
    Vector3 worldCenter = GetWorldPosition();
    // p0 - c
    Vector3 v = ray.origin - worldCenter;

    // �Ǻ���
    // -B +- sqrt(B*B - 4C) / 2
    // �ȸ����� �Ѵ� ����
    // �ȿ� �ִٰ� ������ ���,����
    // �ۿ� �ִٰ� ������ ���� ����,���
    // �����ϸ� �Ѵ� ���
    // ����� �����ϸ� �Ѵ� ����̰�, ���� ����

    // ����(t) = ������ġ(p0) + t * ��������(u) 
    // p(t) = p0 + tu

    // B = 2(u * (p0 -c))
    float b = 2.0f * Vector3::Dot(ray.direction, v);
    // C = (p0-c)*(p0-c) - r^2
    float c = Vector3::Dot(v, v) - (radius * radius);

    // �Ǻ���
    float discriminant = (b * b) - (4.0f * c);
    // ������ �� �׽�Ʈ
    if (discriminant < 0.f)
        return false;

    discriminant = sqrtf(discriminant);

    float t0 = (-b + discriminant) * 0.5f;
    float t1 = (-b - discriminant) * 0.5f;

    // �ذ� >= 0 �� ��� ����
    if (t0 >= 0.f || t1 >= 0.f)
    {
        if (outHitInfo != nullptr)
        {
            // origin�� ���� ����� t�� ����
            float nearT = min(fabsf(t0), fabsf(t1));
            // ����(t) = ������ġ(p0) + t * ��������(u) 
            // p(t) = p0 + tu
            outHitInfo->point = ray.origin + nearT * ray.direction;
            outHitInfo->distance = Vector3::Distance(ray.origin, outHitInfo->point);
            outHitInfo->collider = this;
        }
        return true;
    }


    return false;
}
