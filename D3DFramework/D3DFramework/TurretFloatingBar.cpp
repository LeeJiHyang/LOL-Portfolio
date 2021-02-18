﻿#include "stdafx.h"
#include "TurretFloatingBar.h"
#include "Bar.h"

TurretFloatingBar::TurretFloatingBar()
{
	// 170,15
	// 11,6
	offset = { 0.f,3.5f, 0.f };
	hpBar->SetLocation(11, 6);
	mpBar->SetTexture(L"Empty");

	Texture* tex = hpBar->GetTexture();
	if (tex != nullptr)
	{
		int w = tex->GetSpriteWidth();
		int h = tex->GetSpriteHeight();
		float ratioW = 170.f / w;
		float ratioH = 15.f / h;
		hpBar->transform->scale = { ratioW,ratioH,1.f };
	}
	
	SetTexture(L"border_float (2)");
	SetSizeByTexture();
}

TurretFloatingBar::~TurretFloatingBar()
{
}
