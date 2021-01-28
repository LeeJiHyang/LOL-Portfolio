#pragma once
#include "GameObject.h"
#include "HUD.h"
class PlayerInfoPanel : public GameObject
{
private:
	PlayerInfoPanel();
	virtual ~PlayerInfoPanel();

public:
	static PlayerInfoPanel* GetInstance();
	static void DestroyInstance();

	// UI��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Release() override;

	virtual void Update() override;
	virtual void Render() override;


public:
	int screenW = 1280;
	int screenH = 720;

	Vector3 faceBorderPos;
	Vector3 mainPanelPos;
	Vector3 miniPanelPos;
	Vector3 invenPanelPos;

	Texture* mainPanelTex = nullptr;
	Texture* miniPanelTex = nullptr;
	Texture* faceBorderTex = nullptr;
	Texture* invenPanelTex = nullptr;

	UI* mainPanel = nullptr;
	UI* miniPanel = nullptr;
	UI* invenPanel = nullptr;
	UI* faceBorder = nullptr;
};

