#pragma once
#include "UI.h"
class LoadingBackGround :
    public UI
{
public:
    LoadingBackGround();
    virtual ~LoadingBackGround();
    // UI��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual void Release() override;

    virtual void Update() override;
};

