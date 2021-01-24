#pragma once
#include "IComponent.h"
#include "IComponent.h"
#include "NavMeshAgent.h"

using namespace PKH;

class Unit;

class NetPlayerController :
    public IComponent
{
public:
    explicit NetPlayerController(GameObject* owner);
    explicit NetPlayerController(const NetPlayerController& rhs);
    virtual ~NetPlayerController();

    virtual void Update() override;
    // IComponent��(��) ���� ��ӵ�
    virtual IComponent* Clone() override;

    void SetTargetMode(bool _mode);

    void ReqMove(Vector3 _dest, bool _search = false);
    void ReqAttack(Unit* _target);

public:
    Network* net = nullptr;
    PKH::NavMeshAgent* agent = nullptr;
    bool targetMode = false;
    Unit* unit = nullptr;
};

