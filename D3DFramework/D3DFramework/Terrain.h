#pragma once
#include "Mesh.h"

namespace PKH
{
    class Terrain :
        public Mesh
    {
    public:
        Terrain();
        virtual ~Terrain();

        // Mesh��(��) ���� ��ӵ�

        virtual void Update() override;

        virtual IComponent* Clone() override;

        int width = 129;
        int height = 129;
    };


}
