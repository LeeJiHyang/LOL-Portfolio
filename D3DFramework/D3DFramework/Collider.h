#pragma once
#include "IComponent.h"

namespace PKH
{
    class Collider :
        public IComponent
    {
    public:
        // IComponent��(��) ���� ��ӵ�
        virtual IComponent* Clone() = 0;
        virtual void OnCollisionEnter(const Collider* other) = 0;
    };

}

