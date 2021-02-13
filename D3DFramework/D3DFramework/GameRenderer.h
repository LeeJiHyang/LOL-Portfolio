#pragma once
#include "Renderer.h"
namespace PKH
{
	class GameObject;
}
enum class RenderGroupID:int
{
	//deferred rendering ������ ������
	Skybox,
	Deferred,
	AlphablendForward,
	UI,
	HUD
};
class GameRenderer : public Renderer
{
public:
	static void Initialize();
	static GameRenderer* Instance();
	static void Destroy();
	static void Register(RenderGroupID groupId, PKH::GameObject* object);
	static void Unregister(RenderGroupID groupId, PKH::GameObject* object);
	static void Render();
	static void AddLight(const wchar_t* name, D3DLIGHT9 const& init);
	static D3DLIGHT9* GetLight(const wchar_t* name);
	static void RemoveLight(const wchar_t* name);
};