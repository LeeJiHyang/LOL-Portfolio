#pragma once
#include "IComponent.h"
class Collider :
    public IComponent
{
    // IComponent��(��) ���� ��ӵ�
    virtual IComponent* Clone() override;
};

