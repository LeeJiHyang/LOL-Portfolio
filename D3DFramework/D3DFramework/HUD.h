#pragma once
#include "UI.h"

class HUD :
    public UI
{
    // UI��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual void Release() override;
};

