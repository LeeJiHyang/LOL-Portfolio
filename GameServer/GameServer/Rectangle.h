#pragma once
#include "CustomMesh.h"

namespace PKH
{
    class Rectangle : public CustomMesh
    {
    public:
        explicit Rectangle(GameObject* owner);
        explicit Rectangle(const Rectangle& rhs);
        virtual ~Rectangle();

        // CustomMesh��(��) ���� ��ӵ�

        virtual void Update() override;

        virtual IComponent* Clone() override;

        // CustomMesh��(��) ���� ��ӵ�
        virtual void CreateCustomMesh() override;
    };
}
