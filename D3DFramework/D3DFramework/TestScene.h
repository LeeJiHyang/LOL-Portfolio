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

public:
    Label* testLabel = nullptr;
};
