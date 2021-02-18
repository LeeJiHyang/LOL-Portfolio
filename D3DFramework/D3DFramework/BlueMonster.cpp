﻿#include "stdafx.h"
#include "BlueMonster.h"
BlueMonster::BlueMonster()
{
	transform->scale = { 0.016f, 0.016f, 0.016f, };
	transform->eulerAngles.y = D3DXToRadian(180.f);
	DynamicMesh* dmesh = RenderManager::CloneDynamicMesh(L"sru_blue");
	AddComponent(L"DynamicMesh", dmesh);
	anim->AttachToDynamicMesh(dmesh);

	KST::SkinnedMeshRenderer* renderer = new KST::SkinnedMeshRenderer(this);
	renderer->SetMesh(dmesh);
	AddComponent(L"renderer", renderer);

	anim->SetLoop((int)UnitState::IDLE1, true);
}

BlueMonster::~BlueMonster()
{
}
