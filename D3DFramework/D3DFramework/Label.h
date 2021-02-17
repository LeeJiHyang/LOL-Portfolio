﻿#pragma once
#include "UI.h"


class Label :
    public UI
{
public:
    enum class Align
    {
        Left,
        Center,
        Right
    };
public:

    Label();
    Label(int fontSize);
    virtual ~Label();

    virtual void Initialize() override;
    virtual void Release() override;
    virtual void Update() override;
private:
    void MakeFont(int fontSize = 20);

public:
    Label::Align align = Label::Align::Left;

    LPD3DXFONT	pFont = nullptr;
    LPD3DXSPRITE pSprite = nullptr;
    LPDIRECT3DDEVICE9 pDevice = nullptr;
    RECT rcDraw = {};

    wstring text;
    D3DCOLOR foreColor = D3DCOLOR_ARGB(255, 254, 254, 254);
};

