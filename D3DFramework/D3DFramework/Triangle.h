#pragma once
#include "CustomMesh.h"

namespace PKH
{
    class Triangle :
        public CustomMesh
    {
    public:
        Triangle();
        virtual ~Triangle();

        // CustomMesh��(��) ���� ��ӵ�

        virtual void Update() override;

		virtual IComponent* Clone() override;
    };


}