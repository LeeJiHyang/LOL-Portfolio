#pragma once
#include "CustomMesh.h"

namespace PKH
{
    class Cube :
        public CustomMesh
    {
    public:
        Cube();
        virtual ~Cube();

        // CustomMesh��(��) ���� ��ӵ�

        virtual void Update() override;
        
        virtual IComponent* Clone() override;

    };
}
 