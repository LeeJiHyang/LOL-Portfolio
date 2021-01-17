#include "stdafx.h"
#include "MainGame.h"
#include "TestScene.h"
#include "SkyBox.h"
#include "TitleScene.h"
#include "LoadingScene.h"
#include "TestLoadingScene.h"

using namespace PKH;

PKH::MainGame* pMainGame = nullptr;

PKH::MainGame::MainGame()
{
}

PKH::MainGame::~MainGame()
{
}

PKH::MainGame* MainGame::GetInstance()
{
    if (pMainGame == nullptr)
    {
        pMainGame = new MainGame;
    }
    
    return pMainGame;
}

void PKH::MainGame::Destroy()
{
    if (pMainGame)
    {
        delete pMainGame;
        pMainGame = nullptr;
    }
}

void PKH::MainGame::Initialize(int screenW, int screenH)
{
	pMainGame->width = screenW;
	pMainGame->height = screenH;
	
	Network::GetInstance();
	Network::Initialize();
	//LobbyWindow::GetInstance();
	Random::InitState();

	// �ٸ� ��� �Ŵ��� �ʱ�ȭ
	TimeManager::GetInstance();
	TimeManager::SetFPS(300);
	
	RenderManager::GetInstance();
	RenderManager::Initialize(screenW, screenH);

	LoadUISprite();

	ObjectManager::GetInstance();
	CollisionManager::GetInstance();
	LightManager::GetInstance();
	InputManager::GetInstance();
	SkyBox::GetInstance();

	//RenderManager::GetInstance();

	SceneManager::GetInstance();

	Camera::GetInstance();
	FileManager::GetInstance();
	Cursor::GetInstance();

	SoundManager::GetInstance()->Initialize();
	LoadManager::GetInstance();
	Frustum::GetInstance();
	NavNodeManager::GetInstance();

	

	
	//SkillManager::GetInstance();

	// ���ε�
	SceneManager::LoadScene<TestLoadingScene>();
	//SceneManager::LoadScene<LoadingScene>();
	//SceneManager::LoadScene<TestScene>();
}

void PKH::MainGame::Update()
{
	// 1. ��ǲ ����
	InputManager::Update();

	// 2. �� �Ŵ��� ������Ʈ
	SceneManager::Update();

	// 3. Player ������Ʈ

	// 4. Obj ������Ʈ
	ObjectManager::Update();

	Camera::GetInstance()->Update();
	SkyBox::GetInstance()->Update();
	Frustum::Update();
	
	//CollisionManager::GetInstance()->Update();
	ObjectManager::PostUpdate();
	SoundManager::Update();

	if (!TimeManager::SkipFrame())
	{
		RenderManager::Clear();

		SkyBox::GetInstance()->Render();
		ObjectManager::PreRender();
		ObjectManager::Render();
		ObjectManager::PostRender();


		RenderManager::Present();

		
	}
}

void PKH::MainGame::Release()
{
	// �ٸ� ��� �Ŵ��� ����
	LoadManager::Destroy();
	SceneManager::Destroy();
	TimeManager::Destroy();
	ObjectManager::Destroy();
	CollisionManager::Destroy();
	SkyBox::Destroy();
	//RenderManager::Release();
	RenderManager::Destroy();
	InputManager::Destroy();
	Camera::Destroy();
	FileManager::Destroy();
	LightManager::Destroy();
	SoundManager::Destroy();
	Cursor::Destroy();
	LobbyWindow::Destroy();
	Network::Destroy();
	Frustum::Destroy();
	NavNodeManager::Destroy();
}

void PKH::MainGame::Pause()
{
}

void PKH::MainGame::Resume()
{
}

void PKH::MainGame::Shutdown()
{
	PostQuitMessage(0);
}

int PKH::MainGame::GetWidth()
{
	return pMainGame->width;
}

int PKH::MainGame::GetHeight()
{
	return pMainGame->height;
}


void PKH::MainGame::LoadUISprite()
{
	// ���ҽ� �ε�
	//RenderManager::LoadSprite(TextureKey::UI_CURSOR, L"Texture\\UI\\Cursor.png");
	//RenderManager::LoadSprite(TextureKey::GRASS, L"Resource\\Texture\\grassTexture.png");
	RenderManager::LoadSprite(L"Resource\\Texture\\", L"Empty.png");
	RenderManager::LoadSprite(L"Resource\\UI\\cursor\\", L"hover_precise.tga");
	RenderManager::LoadSprite(L"Resource\\UI\\cursor\\", L"singletarget.tga");
	//�ε���
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"srbackground.dds");

	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"loadingscreen_spinner_atlas.dds",8,4);
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"loadingFrameBlue.png");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"loadingFrameRed.png");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"loadScreen.png");

	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"ahriloadscreen.dds");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"amumuloadscreen.dds");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"blitzcrankloadscreen.dds");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"dariusloadscreen.dds");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"dianaloadscreen.dds");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"garenloadscreen.dds");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"jinxloadscreen.dds");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"leesinloadscreen.dds");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"leonaloadscreen.dds");
	RenderManager::LoadSprite(L"Resource\\UI\\loading\\", L"missfortuneloadscreen.dds");

	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_barrier.dds");
	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_boost.dds");
	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_exhaust.dds");
	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_flash.dds");
	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_haste.dds");
	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_heal.dds");
	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_ignite.dds");
	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_mana.dds");
	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_smite.dds");
	RenderManager::LoadSprite(L"Resource\\Spell\\icon\\", L"summoner_teleport.dds");
	
	// �ΰ���
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_big1.png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_big2.png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_big3.png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_big4.png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_exp.png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (4).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (5).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (6).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (7).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (8).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (9).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_float (10).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_small (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_small (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_small (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_small (4).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_tip (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"bar_tip (2).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_float (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_float (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_float (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_float (4).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_float (5).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_float (6).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_float (7).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_float (8).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_skill (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_skill (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_skill (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_skill (4).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_skill (5).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"border_skill (6).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"button_gold (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"button_gold (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"button_gold (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"button_gold (4).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"button_skillup (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"button_skillup (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"button_skillup (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"button_skillup (4).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"hud_icon (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"hud_icon (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"hud_icon (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"hud_icon (4).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"map11.png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"minimapBorder.png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"panel (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"panel (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"panel (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"panel (4).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"panel (5).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (4).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (5).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (6).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (7).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (8).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (9).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (10).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (11).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (12).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (13).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (14).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (15).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_icon (16).png");

	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_panel (1).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_panel (2).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_panel (3).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_panel (4).png");
	RenderManager::LoadSprite(L"Resource\\UI\\HUD\\", L"stat_panel (5).png");


}										
