#include "stdafx.h"
#include "GameRenderer.h"
#include "GameRendererImpl.h"
static GameRendererImpl* g_instance = nullptr;
void GameRenderer::Initialize()
{
	if (g_instance == nullptr)
	{
		g_instance = new GameRendererImpl();
	}
}

GameRenderer* GameRenderer::Instance()
{
	return g_instance;
}

void GameRenderer::Destroy()
{
	if (g_instance != nullptr)
	{
		delete g_instance;
		g_instance = nullptr;
	}
}

void GameRenderer::Register(RenderGroupID groupId, PKH::GameObject* object)
{
	g_instance->Register(groupId, object);
}

void GameRenderer::Unregister(RenderGroupID groupId, PKH::GameObject* object)
{
	g_instance->Unregister(groupId, object);
}

void GameRenderer::Render()
{
	g_instance->Render();
}

void GameRenderer::AddLight(const wchar_t* name, D3DLIGHT9 const& init)
{
	g_instance->AddLight(name, init);

}

D3DLIGHT9* GameRenderer::GetLight(const wchar_t* name)
{
	return g_instance->GetLight(name);
}

void GameRenderer::RemoveLight(const wchar_t* name)
{
	g_instance->RemoveLight(name);
}
