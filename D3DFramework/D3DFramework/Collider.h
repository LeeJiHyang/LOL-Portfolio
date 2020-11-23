#pragma once
#include "IComponent.h"

namespace PKH
{
    enum class ColliderType
    {
        None,
        Box,
        Sphere,
    };

    class Collider :
        public IComponent
    {
    public:
        explicit Collider(GameObject* owner);
        explicit Collider(const Collider& rhs);
        virtual ~Collider();

        virtual void Update();
        virtual void Render();
        // IComponent��(��) ���� ��ӵ�
        virtual IComponent* Clone() = 0;
        virtual void OnCollisionEnter(const Collider* other);
        virtual bool Raycast(Ray ray, RaycastHit* outHitInfo, float maxDistance) = 0;

    public:
        // getter
        Vector3 GetWorldPosition();
    public:
        // setter
        void SetColor(D3DCOLOR color);


        //----------------
        // ���
        //---------------
    public:
        // ���� ������
        Vector3 center;
        ColliderType type = ColliderType::None;
    protected:
        LPD3DXMESH pMesh = nullptr;
        DWORD fvf;
        Matrix world = Matrix::identity;
    };

}

