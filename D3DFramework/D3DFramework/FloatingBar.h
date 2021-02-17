﻿#pragma once
#include "GameObject.h"
#include "Unit.h"
using namespace PKH;

class Bar;
class Label;

class FloatingBar : public UI
{
public:
    FloatingBar();
    virtual ~FloatingBar();

    virtual void Update() override;


    void SetTarget(Unit* target);
    void SetTextureHP(const wstring& _key);
    void SetTextureMP(const wstring& _key);
    void SetNickname(const wstring& _nick);

protected:
    Unit* target = nullptr;

    Vector3 offset;
    wstring textureKey;

    Bar* hp = nullptr;
    Bar* mp = nullptr;
};
