#pragma once
#include "Collider.h"

namespace PKH
{
    class BoxCollider :
        public Collider
    {
    public:
        explicit BoxCollider(GameObject* owner);
        explicit BoxCollider(const BoxCollider& rhs);
        virtual ~BoxCollider();

        virtual IComponent* Clone() override;
        virtual bool Raycast(Ray ray, RaycastHit* outHitInfo, float maxDistance) override;



    public:
        // ������ ����
        Vector3 size;
    };


}
