#pragma once
#include "GameObject.h"
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
};

