#pragma once
#include "UI.h"
class Panel :
    public UI
{
public:
    Panel();
    virtual ~Panel();
    // UI��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual void Release() override;

};

