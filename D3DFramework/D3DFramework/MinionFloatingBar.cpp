﻿#include "stdafx.h"
#include "MinionFloatingBar.h"
#include "Bar.h"

MinionFloatingBar::MinionFloatingBar()
{
	// 64,5
	// 1,1
	offset = { 0.f,1.5f, 0.f };
	hpBar->SetLocation(1, 1);
	mpBar->SetTexture(L"Empty");
	
	Texture* tex = hpBar->GetTexture();
	if (tex != nullptr)
	{
		int w = tex->GetSpriteWidth();
		int h = tex->GetSpriteHeight();
		float ratioW = 64.f / w;
		float ratioH = 5.f / h;
		hpBar->transform->scale = { ratioW,ratioH,1.f };
	}
	
	SetTexture(L"border_float (6)");
	SetSizeByTexture();
}

MinionFloatingBar::~MinionFloatingBar()
{
}