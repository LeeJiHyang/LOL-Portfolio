﻿#pragma once
#include "Panel.h"

class LoadingBorder;
class LoadingChampPanel;
class LoadingPanel;
class UI_Spell;
class Label;

class LoadingChampScreen :
    public Panel
{
public:
    LoadingChampScreen();
    virtual ~LoadingChampScreen();
    // GameObject을(를) 통해 상속됨
    virtual void Initialize() override;
    virtual void Release() override;
    virtual void Update() override;

    void SetChampion(ChampionType type);
    void SetSpell1(SpellType type);
    void SetSpell2(SpellType type);

public:
    LoadingBorder* border = nullptr;
    LoadingChampPanel* champ = nullptr;
    LoadingPanel* mask = nullptr;
    UI_Spell* spell1 = nullptr;
    UI_Spell* spell2 = nullptr;
    Label* champName = nullptr;
    Label* nickName = nullptr;
    Label* progressLabel = nullptr;
};

