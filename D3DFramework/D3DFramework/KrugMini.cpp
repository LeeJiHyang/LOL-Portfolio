#include "stdafx.h"
#include "KrugMini.h"

KrugMini::KrugMini()
{
	transform->scale = { 0.012f, 0.012f, 0.012f, };
	transform->eulerAngles.y = D3DXToRadian(180.f);
	DynamicMesh* dmesh = RenderManager::CloneDynamicMesh(L"sru_krugmini");
	AddComponent(L"DynamicMesh", dmesh);
	anim->AttachToDynamicMesh(dmesh);

	anim->SetLoop((int)UnitState::IDLE1, true);
}

KrugMini::~KrugMini()
{
}
