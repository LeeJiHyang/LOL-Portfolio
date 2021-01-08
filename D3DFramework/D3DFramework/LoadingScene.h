#pragma once
#include "IScene.h"

class LoadingChampScreen;

class LoadingScene :
    public IScene
{
public:
    // IScene��(��) ���� ��ӵ�
    virtual void OnLoaded() override;
    virtual void OnUnloaded() override;
    virtual void Update() override;
    
    void LoadResources();

    static void Count();

    void NetProc();
    void PacketProc(CPacket* pPacket);

    void ReqTest();
    void ResTest(CPacket* pack);

    void ReqEnterGame();
    void ResEnterGame(CPacket* pack);
    void ReqLoading(int percent);
    void ResLoading(CPacket* pack);
    void ReqCompleteLoading();
    void ResCompleteLoading(CPacket* pack);

    void CreateChampPanel(int borderW, int borderH, int padding);
    
public:
    bool isConnected = false;
    float connectDelay = 0.3f;
    float connectTick = 0.f;

    bool isEnterGame = false;

    bool isCompleteLoading = false;

    int loadPercent = 0;

    LoadingChampScreen* screens[10] = {};
};

