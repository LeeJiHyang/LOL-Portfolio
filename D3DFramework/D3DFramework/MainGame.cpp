#include "stdafx.h"
#include "MainGame.h"
#include "TestScene.h"
#include "SkyBox.h"
#include "TitleScene.h"
#include "LoadingScene.h"

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

	// �ٸ� ��� �Ŵ��� �ʱ�ȭ
	TimeManager::GetInstance();
	TimeManager::SetFPS(300);
	
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
	Frustum::GetInstance();

	LoadUISprite();

	
	//SkillManager::GetInstance();

	// ���ε�
	SceneManager::LoadScene<LoadingScene>();
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
	
	CollisionManager::GetInstance()->Update();
	ObjectManager::PostUpdate();
	SoundManager::Update();

	if (!TimeManager::SkipFrame())
	{
		RenderManager::Clear();

		SkyBox::GetInstance()->Render();
		//ObjectManager::PreRender();
		ObjectManager::Render();
		//ObjectManager::PostRender();


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
	// ���ҽ� �ε�
	//RenderManager::LoadSprite(TextureKey::UI_CURSOR, L"Texture\\UI\\Cursor.png");
	RenderManager::LoadSprite(TextureKey::GRASS, L"Resource\\Texture\\grassTexture.png");




}										
