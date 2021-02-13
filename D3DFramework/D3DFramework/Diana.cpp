#include "stdafx.h"
#include "Diana.h"
#include "GameRenderer.h"
Diana::Diana()
{
	transform->scale = { 0.015f, 0.015f, 0.015f, };
	transform->eulerAngles.y = D3DXToRadian(180.f);
	DynamicMesh* dmesh = RenderManager::CloneDynamicMesh(L"diana");
	AddComponent(L"DynamicMesh", dmesh);
	dmesh->renderGroupID = RenderGroupID::Deferred;
	GameRenderer::Register(dmesh);
	anim->AttachToDynamicMesh(dmesh);

	faceCircleTexkey = L"diana_circle";
	faceSquareTexkey = L"diana_square";
	// ��ų ������
	qTexKey = L"diana_q";
	wTexKey = L"diana_w";
	eTexKey = L"diana_e";
	rTexKey = L"diana_r";
	passiveTexKey = L"diana_passive";

	SetHP(570.f);
	SetHPRegen(7.5f);
	SetMP(375.f);
	SetMPRegen(8.f);
	SetAttackDamage(57.f);
	SetAttackPerSec(0.625f);
	SetArmor(31.f);
	SetMagicResistance(32.1f);
	SetAttackRange(1.5f);
	SetMovementSpeed(3.45f);
}

Diana::~Diana()
{
	DynamicMesh* dmesh = (DynamicMesh*)GetComponent(L"DynamicMesh");
	GameRenderer::Unregister(dmesh);
}

void Diana::Initialize()
{
}

void Diana::Release()
{
}

void Diana::Update()
{
	Champion::Update();
}
