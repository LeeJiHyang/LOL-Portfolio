#pragma once
#include "GameObject.h"
class Wall :
    public GameObject
{
public:
    Wall();
    virtual ~Wall();

    // GameObject��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual void Release() override;
    virtual void Update() override;

};

