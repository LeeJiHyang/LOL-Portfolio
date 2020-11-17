#include "stdafx.h"
#include "TestScene.h"
#include "SkyBox.h"
#include "Environment.h"
#include "TestMan.h"

void TestScene::OnLoaded()
{
	Cursor::Show();
	//ObjectManager::GetInstance()->CreateObject<Light>();

	//SkyBox::Show();
	//SkyBox::SetTexture(TextureKey::SKY_U);
	Camera::GetInstance()->SetPosition(Vector3(0.f, 200.f ,-200.f));
	Camera::GetInstance()->transform->look = Vector3(0, 0, 1);


	
	GameObject* obj = ObjectManager::GetInstance()->CreateObject<Environment>();
	obj->transform->position = { 0,0,0 };

	ObjectManager::GetInstance()->CreateObject<TestMan>();
}

void TestScene::OnUnloaded()
{
	ObjectManager::DestroyAll();
}

void TestScene::Update()
{

}