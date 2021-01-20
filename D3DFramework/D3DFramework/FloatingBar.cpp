#include "stdafx.h"
#include "FloatingBar.h"

FloatingBar::FloatingBar()
{
	offset = { 0,1.8f,-0.5f };
	offset2 = { -42,-9,0 };
	offsetMP = { -42,5,0 };

	scaleHP = { 0.97f,1.f,1.f };
	scaleMP = { 0.97f,0.3f,1.f };
	//mesh = (PKH::Rectangle*)AddComponent<PKH::Rectangle>(L"Mesh");
	//mesh->SetTexture(L"border_float (1)");
	//mesh->SetBlendMode(BlendMode::ALPHA_TEST);

	//Texture* tex = RenderManager::GetTexture(L"border_float (1)");
	//int w = tex->GetSpriteWidth();
	//int h = tex->GetSpriteHeight();
	//float scaleY = (float)h / w;
	//transform->scale = { 1,scaleY,1 };

	SetTexture(L"border_float (1)");
	textureKeyHP = L"bar_float (5)";
	textureKeyMP = L"bar_float (5)";
	////SetSizeByTexture();
	//SetSize(1000, 100);
	//mesh->SetBlendMode(BlendMode::ALPHA_BLEND);

	
}

FloatingBar::~FloatingBar()
{
	target = nullptr;
}

void FloatingBar::Initialize()
{
}

void FloatingBar::Release()
{
}

void FloatingBar::Update()
{
	if (target->IsDead())
	{
		target = nullptr;
	}

	if (target != nullptr)
	{
		Vector3 worldPos = target->transform->position + offset;
		worldPos = Camera::main->WorldToScreenPoint(worldPos);
		transform->position = worldPos;
	}
	
	//UI::Update();
	GameObject::Update();

	//Billboard();

	
}

void FloatingBar::Render()
{
	float ratioHP = 0.f;
	float ratioMP = 0.f;
	//UI::Render();
	RenderManager::DrawSprite(textureKey, *transform, 0);
	//RenderManager::DrawSprite(textureKey2, transform->position + offset2, 0);
	if (target != nullptr)
	{
		ratioHP = (float)target->hp / target->maxHp;
		ratioMP = (float)target->mp / target->maxMp;
	}
	if (ratioHP < 0.f)
	{
		ratioHP = 0.f;
	}
	if (ratioMP < 0.f)
	{
		ratioMP = 0.f;
	}
	RenderManager::DrawUIHorizontal(textureKeyHP, transform->position + offset2, scaleHP, 0, ratioHP);
	RenderManager::DrawUIHorizontal(textureKeyMP, transform->position + offsetMP, scaleMP, 0, ratioMP);
}

void FloatingBar::SetTarget(Unit* target)
{
	this->target = target;
}

void FloatingBar::SetTexture(const wstring& _textureKey)
{
	textureKey = _textureKey;
}

void FloatingBar::SetTextureHP(const wstring& _textureKey)
{
	textureKeyHP = _textureKey;
}

void FloatingBar::SetTextureMP(const wstring& _textureKey)
{
	textureKeyMP = _textureKey;
}
