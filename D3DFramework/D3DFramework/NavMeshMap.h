#pragma once
#include "GameObject.h"

namespace PKH
{
    class NavMeshMap :
        public GameObject
    {
    public:
        NavMeshMap();
        virtual ~NavMeshMap();

        // GameObject��(��) ���� ��ӵ�
        virtual void Initialize() override;
        virtual void Release() override;
        virtual void Update() override;

    private:
        // �޽��� ���������� ��带 ����(old version)
        void SetNavigationInfo();
        // ����� �����ͷ� ��带 ����(new version)
        void LoadNavigationInfo();
    public:
        bool Search(const Vector3& start, const Vector3& dest);
        PathFinder::Node* GetResultNode();

    private:
        NavMesh* navMesh = nullptr;
        PathFinder* pPathFinder = nullptr;
    };
}


