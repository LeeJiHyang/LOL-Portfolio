#pragma once
#include "IScene.h"

using namespace PKH;

class Label;

class TestScene :
    public IScene
{
    // IScene��(��) ���� ��ӵ�
    virtual void OnLoaded() override;
    virtual void OnUnloaded() override;
    virtual void Update() override;

    void CreateMinionCaster();

public:
    Label* testLabel = nullptr;
    Unit* testUnit = nullptr;
    UIManager* UIManager = nullptr;
};
