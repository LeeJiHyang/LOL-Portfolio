#include "stdafx.h"
#include "MainGame.h"
#include "TestScene.h"
#include "SkyBox.h"
#include "TitleScene.h"

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

void PKH::MainGame::Initialize()
{
	Network::GetInstance();
	Network::Initialize();
	LobbyWindow::GetInstance();
	Random::InitState();

	// 다른 모든 매니저 초기화
	TimeManager::GetInstance();
	TimeManager::SetFPS(60.f);
	
	RenderManager::GetInstance();
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

	LoadUISprite();

	
	//SkillManager::GetInstance();

	// 씬로드
	SceneManager::LoadScene<TestScene>();
}

void PKH::MainGame::Update()
{
	// 1. 인풋 먼저
	InputManager::Update();

	// 2. 씬 매니저 업데이트
	SceneManager::Update();

	// 3. Player 업데이트

	// 4. Obj 업데이트
	ObjectManager::Update();

	Camera::GetInstance()->Update();
	SkyBox::GetInstance()->Update();
	
	CollisionManager::GetInstance()->Update();
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
	// 다른 모든 매니저 해제
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


void PKH::MainGame::LoadUISprite()
{
	// 리소스 로드
	//RenderManager::LoadSprite(TextureKey::UI_CURSOR, L"Texture\\UI\\Cursor.png");
	RenderManager::LoadSprite(TextureKey::GRASS, L"Resource\\Texture\\grassTexture.png");




}										
