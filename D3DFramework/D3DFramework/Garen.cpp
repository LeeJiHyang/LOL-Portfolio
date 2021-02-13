#include "stdafx.h"
#include "Garen.h"
#include "Animation.h"
#include "GameRenderer.h"
Garen::Garen()
{
	transform->scale = { 0.014f, 0.014f, 0.014f, };
	transform->eulerAngles.y = D3DXToRadian(180.f);
	DynamicMesh* dmesh = RenderManager::CloneDynamicMesh(L"garen");
	AddComponent(L"DynamicMesh", dmesh);
	dmesh->renderGroupID = RenderGroupID::Deferred;
	GameRenderer::Register(dmesh);
	anim->AttachToDynamicMesh(dmesh);

	// �� ������
	faceCircleTexkey = L"garen_circle";
	faceSquareTexkey = L"garen_square";
	// ��ų ������
	qTexKey = L"garen_q";
	wTexKey = L"garen_w";
	eTexKey = L"garen_e1";
	rTexKey = L"garen_r";
	passiveTexKey = L"garen_passive";

	// ����
	SetHP(620.f);
	SetMP(100.f);
	SetHPRegen(8.f);
	SetMPRegen(0.f);
	SetAttackDamage(66.f);
	SetAttackPerSec(0.625f);
	SetAttackRange(1.75f);
	SetMovementSpeed(3.4f);
	SetArmor(36.f);
	SetMagicResistance(32.1f);



}

Garen::~Garen()
{
	DynamicMesh* dmesh = (DynamicMesh*)GetComponent(L"DynamicMesh");
	GameRenderer::Unregister(dmesh);
}

void Garen::Initialize()
{
}

void Garen::Release()
{
}

void Garen::Update()
{
	Champion::Update();
}

void Garen::Spell1()
{
	printf("���� Q\n");
}

void Garen::Spell2()
{
}

void Garen::Spell3()
{
}

void Garen::Spell4()
{
}
