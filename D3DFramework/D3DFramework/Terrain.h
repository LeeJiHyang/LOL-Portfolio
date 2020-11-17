#pragma once
#include "CustomMesh.h"

namespace PKH
{
    class Terrain :
        public CustomMesh
    {
    public:
        Terrain();
        virtual ~Terrain();

        // CustomMesh��(��) ���� ��ӵ�

        virtual void Update() override;

        virtual IComponent* Clone() override;

        void LoadHeightMap(const string& filePath);
        bool GetYFromPoint(float* _outY, float _x, float _z);


    private:
        // ��������
        void SetNormalVector();

    private:
        int width = dfTERRAIN_WIDTH;
        int height = dfTERRAIN_WIDTH;

        Vector3* vertexInfo = nullptr;
    };


}
