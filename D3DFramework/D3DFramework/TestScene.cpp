#include "stdafx.h"
#include "TestScene.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Cube.h"
#include "SkyBox.h"
#include "Player.h"
#include "CubeMan.h"
#include "Monster.h"

void TestScene::OnLoaded()
{
	ObjectManager::GetInstance()->CreateObject<Player>();
	ObjectManager::GetInstance()->CreateObject<CubeMan>();
	ObjectManager::GetInstance()->CreateObject<Monster>();
}

void TestScene::OnUnloaded()
{
	ObjectManager::DestroyAll();
}

void TestScene::Update()
{

}
