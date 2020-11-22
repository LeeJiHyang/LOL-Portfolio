#pragma once
#include "IComponent.h"

namespace PKH
{
    class Collider :
        public IComponent
    {
    public:
        explicit Collider(GameObject* owner);
        explicit Collider(const Collider& rhs);
        virtual ~Collider();

        virtual void Render();
        // IComponent을(를) 통해 상속됨
        virtual IComponent* Clone() = 0;
        virtual void OnCollisionEnter(const Collider* other) = 0;
        virtual bool Raycast(Ray ray, RaycastHit hitInfo, float maxDistance) = 0;

    public:
        // getter
    public:


        //----------------
        // 멤버
        //---------------
    public:
        // 로컬 포지션
        Vector3 center;
    protected:
        LPD3DXMESH pMesh = nullptr;
    };

}

