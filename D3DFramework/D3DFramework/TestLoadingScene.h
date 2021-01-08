#pragma once
#include "IScene.h"

class LoadingChampScreen;

class TestLoadingScene :
    public IScene
{
    // IScene��(��) ���� ��ӵ�
    virtual void OnLoaded() override;
    virtual void OnUnloaded() override;
    virtual void Update() override;

    void LoadResources();

    static void Count();

    void CreateChampPanel(int borderW, int borderH, int padding);

    LoadingChampScreen* screens[10] = {};
};

