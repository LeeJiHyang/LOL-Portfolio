#pragma once
#include "IComponent.h"
class PlayerController :
    public IComponent
{
public:
    PlayerController();
    virtual ~PlayerController();

    virtual void Update() override;
    // IComponent��(��) ���� ��ӵ�
    virtual IComponent* Clone() override;


};

