﻿#include "stdafx.h"
#include "OrderMinionCaster.h"

OrderMinionCaster::OrderMinionCaster()
{
	transform->scale = { 0.012f, 0.012f, 0.012f, };
	transform->eulerAngles.y = D3DXToRadian(180.f);
	DynamicMesh* dmesh = RenderManager::CloneDynamicMesh(L"order_minion_caster");
	AddComponent(L"DynamicMesh", dmesh);

	KST::SkinnedMeshRenderer* renderer = new KST::SkinnedMeshRenderer(this);
	renderer->SetMesh(dmesh);
	AddComponent(L"renderer", renderer);

	anim->AttachToDynamicMesh(dmesh);
}

OrderMinionCaster::~OrderMinionCaster()
{
}
