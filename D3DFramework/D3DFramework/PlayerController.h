#pragma once
#include "IComponent.h"
#include "NavMeshAgent.h"

using namespace PKH;

class Unit;

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

    void SetTargetMode(bool _mode);

public:
    PKH::NavMeshAgent* agent = nullptr;
    bool targetMode = false;
    Unit* unit = nullptr;
};

