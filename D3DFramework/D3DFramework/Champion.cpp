#include "stdafx.h"
#include "Champion.h"
#include "FloatingBar.h"
#include "FloatingHPBar.h"
#include "Collider.h"

Champion::Champion()
{
	bar = (FloatingBar*)ObjectManager::GetInstance()->CreateObject<FloatingBar>(Layer::UI);
	bar->SetTarget(this);
	hpBar = (FloatingHPBar*)ObjectManager::GetInstance()->CreateObject<FloatingHPBar>(Layer::UI);
	hpBar->SetTarget(this);
}

Champion::~Champion()
{
	bar = nullptr;
}

void Champion::Initialize()
{
	Unit::Initialize();

}

void Champion::Release()
{
	Unit::Release();
}

void Champion::Update()
{
	Unit::Update();
}

void Champion::OnCollisionEnter(Collider* target)
{
	if (dynamic_cast<Unit*>(target->gameObject))
	{
		Unit* unit = (Unit*)target->gameObject;
		if (unit->GetState() == UnitState::RUN)
		{
			unit->PushedOut(this);
		}
		
	}
}

void Champion::SetTeam(Team _team)
{
	Unit::SetTeam(_team);

	if (_team == Team::BLUE)
	{
		bar->SetTextureHP(L"bar_float (5)");
	}
	else
	{
		bar->SetTextureHP(L"bar_float (2)");
	}
}
