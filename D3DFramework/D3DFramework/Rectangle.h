#pragma once
#include "CustomMesh.h"

namespace PKH
{
    class Rectangle :
        public CustomMesh
    {
    public:
        Rectangle();
        virtual ~Rectangle();

        // CustomMesh��(��) ���� ��ӵ�

        virtual void Update() override;

        virtual IComponent* Clone() override;
    };
}
