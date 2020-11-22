#pragma once
#include "IComponent.h"
class PlayerController :
    public IComponent
{
public:
    explicit PlayerController(GameObject* owner);
    explicit PlayerController(const PlayerController& rhs);
    virtual ~PlayerController();

    virtual void Update() override;
    // IComponent��(��) ���� ��ӵ�
    virtual IComponent* Clone() override;


};

