#pragma once
#include "IComponent.h"
class PlayerController :
    public IComponent
{
public:
    PlayerController();
    virtual ~PlayerController();

    // IComponent��(��) ���� ��ӵ�
    virtual IComponent* Clone() override;


};

