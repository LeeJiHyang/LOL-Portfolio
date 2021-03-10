﻿#include "stdafx.h"
#include "ScorePanel.h"
#include "Champion.h"
#include "Label.h"

ScorePanel* pScorePanel = nullptr;
std::wstring dragonCountTex[(UINT)Dragon::END] = {
	L"scoreboard_dragoncount_wind",
	L"scoreboard_dragoncount_fire",
	L"scoreboard_dragoncount_earth",
	L"scoreboard_dragoncount_water",
	L"scoreboard_dragoncount_break"
};

std::wstring dragonSoulTex1[(UINT)Dragon::END] = {
	L"scoreboard_dragonsimbol_wind_1",
	L"scoreboard_dragonsimbol_fire_1",
	L"scoreboard_dragonsimbol_earth_1",
	L"scoreboard_dragonsimbol_water_1"
};

std::wstring dragonSoulTex2[(UINT)Dragon::END] = {
	L"scoreboard_dragonsimbol_wind_2",
	L"scoreboard_dragonsimbol_fire_2",
	L"scoreboard_dragonsimbol_earth_2",
	L"scoreboard_dragonsimbol_water_2"
};

ScorePanel::ScorePanel()
{
	int screenW = MainGame::GetWidth();
	int screenH = MainGame::GetHeight();

	auto mainpanelTex = RenderManager::GetTexture(L"scoreboard_mainpanel");

// 용 시간
	//UI* MobTimePanel = CreateChild(L"scoreboard_mobtimepanel", Vector2(655, -10));

// 메인
	UI* mainPanel = CreateChild(L"scoreboard_mainpanel", Vector2((screenW * 0.5f) - (mainpanelTex->GetSpriteWidth() * 0.5f), (screenH * 0.5f) - (mainpanelTex->GetSpriteHeight() * 0.5f)));

// 용
	dragonSoulUI = mainPanel->CreateChild(L"scoreboard_dragonsimbol_default", Vector2(460, 10));
	dragonSoulUI->transform->scale = { 0.75f, 0.75f, 0.75f };

	// 용 카운트
	for (int i = 0; i < 4; ++i) {
		dragonCountUI[(UINT)Team::BLUE][i] = mainPanel->CreateChild(L"", Vector2(400 - (46 * i), 20));
		dragonCountUI[(UINT)Team::BLUE][i]->transform->scale = { 0.75f, 0.75f, 0.75f };

		dragonCountUI[(UINT)Team::RED][i] = mainPanel->CreateChild(L"", Vector2(543 + (46 * i), 20));
		dragonCountUI[(UINT)Team::RED][i]->transform->scale = { 0.75f, 0.75f, 0.75f };
	}

	// 용추가 예시
	// 한 팀이 4개가 되면 자동으로 나머지 빈 자리는 부셔지는 아이콘이 들어감
	//AddDragon(Dragon::INFERNAL, Team::BLUE);
	//AddDragon(Dragon::OCEAN, Team::RED);
	//AddDragon(Dragon::OCEAN, Team::RED);
	//SetDragonSoul(Dragon::OCEAN); // 3용 이후 용 결정될 때 호출
	//AddDragon(Dragon::OCEAN, Team::RED);
	//AddDragon(Dragon::OCEAN, Team::RED);
	//SetDragonSoulBuf(); // 4용 버프 적용될 때 호출

// 라벨
	turretCountLabel[(UINT)Team::BLUE] = new Label(15);
	mainPanel->AddChild<Label>(L"BlueTurretCount", turretCountLabel[(UINT)Team::BLUE]);
	turretCountLabel[(UINT)Team::BLUE]->SetLocation(381, 91);
	turretCountLabel[(UINT)Team::BLUE]->SetText(0);
	turretCountLabel[(UINT)Team::BLUE]->align = Label::Align::Right;
	turretCountLabel[(UINT)Team::BLUE]->valign = Label::VAlign::Middle;
	turretCountLabel[(UINT)Team::BLUE]->foreColor = D3DCOLOR_ARGB(255, 78, 157, 222);

	turretCountLabel[(UINT)Team::RED] = new Label(15);
	mainPanel->AddChild<Label>(L"RedTurretCount", turretCountLabel[(UINT)Team::RED]);
	turretCountLabel[(UINT)Team::RED]->SetLocation(588, 91);
	turretCountLabel[(UINT)Team::RED]->SetText(0);
	turretCountLabel[(UINT)Team::RED]->align = Label::Align::Left;
	turretCountLabel[(UINT)Team::RED]->valign = Label::VAlign::Middle;
	turretCountLabel[(UINT)Team::RED]->foreColor = D3DCOLOR_ARGB(255, 223, 60, 50);

	killCountLabel[(UINT)Team::BLUE] = new Label(20);
	mainPanel->AddChild<Label>(L"BlueKillCount", killCountLabel[(UINT)Team::BLUE]);
	killCountLabel[(UINT)Team::BLUE]->SetLocation(466, 91);
	killCountLabel[(UINT)Team::BLUE]->SetText(0);
	killCountLabel[(UINT)Team::BLUE]->align = Label::Align::Right;
	killCountLabel[(UINT)Team::BLUE]->valign = Label::VAlign::Middle;
	killCountLabel[(UINT)Team::BLUE]->foreColor = D3DCOLOR_ARGB(255, 78, 157, 222);

	killCountLabel[(UINT)Team::RED] = new Label(20);
	mainPanel->AddChild<Label>(L"RedKillCount", killCountLabel[(UINT)Team::RED]);
	killCountLabel[(UINT)Team::RED]->SetLocation(497, 91);
	killCountLabel[(UINT)Team::RED]->SetText(0);
	killCountLabel[(UINT)Team::RED]->align = Label::Align::Left;
	killCountLabel[(UINT)Team::RED]->valign = Label::VAlign::Middle;
	killCountLabel[(UINT)Team::RED]->foreColor = D3DCOLOR_ARGB(255, 223, 60, 50);

// 챔피언 정보
	for (int i = (UINT)Team::BLUE; i <= (UINT)Team::RED; ++i) {
		for (int j = 0; j < 5; ++j) {
			championScoreUI[i].push_back(mainPanel->CreateChild(L"Champion", Vector2(9 + (475 * (i - (UINT)Team::BLUE)), 120 + (63 * j))));
			UI* champPanel = championScoreUI[i][j];

			// 배경
			champPanel->CreateChild(L"scoreboard_dead", Vector2(0, 2));
			champPanel->CreateChild(L"scoreboard_playermark", Vector2(0, 3));
			champPanel->CreateChild(L"scoreboard_mia", Vector2(6, 3));

			Label* DeadCount = new Label(20);
			champPanel->AddChild<Label>(L"DeadCount", DeadCount);
			DeadCount->SetLocation(15, 29);
			DeadCount->SetText(40);
			DeadCount->align = Label::Align::Center;
			DeadCount->valign = Label::VAlign::Middle;
			DeadCount->foreColor = D3DCOLOR_ARGB(255, 255, 0, 0);

			// 챔피언 초상화
			UI* portrait = champPanel->CreateChild(L"Portrait", Vector2(80, 10));
			portrait->transform->scale = { 0.35f, 0.35f, 1.f };

			// 챔피언 초상화 테두리
			champPanel->AddChild<UI>(L"ChampBorder", new UI(L"", Vector2(76, 10)));

			// 레벨
			Label* levelLabel = champPanel->AddChild<Label>(L"LevelLabel", new Label(12));
			levelLabel->SetLocation(115, 47);
			levelLabel->SetText(18);
			levelLabel->align = Label::Align::Center;
			levelLabel->valign = Label::VAlign::Middle;

			// 소환사 주문
			UI* spell1 = champPanel->CreateChild(L"summoner_heal", Vector2(45, 11));
			UI* spell2 = champPanel->CreateChild(L"summoner_flash", Vector2(45, 34));
			spell1->transform->scale = { 0.3125f, 0.3125f, 1.f };
			spell2->transform->scale = { 0.3125f, 0.3125f, 1.f };

			// cs
			Label* csLabel = champPanel->AddChild<Label>(L"CSLabel", new Label(12));
			csLabel->SetLocation(156, 29);
			csLabel->SetText(0);
			csLabel->align = Label::Align::Center;
			csLabel->valign = Label::VAlign::Middle;

			// KDA
			Label* kdaLabel = champPanel->AddChild<Label>(L"KDALabel", new Label(12));
			kdaLabel->SetLocation(219, 29);
			kdaLabel->SetText(L"%d//%d//%d", 0, 0, 0);
			kdaLabel->align = Label::Align::Center;
			kdaLabel->valign = Label::VAlign::Middle;

			// 닉네임
			Label* nameLabel = champPanel->AddChild<Label>(L"NameLabel", new Label(12));
			nameLabel->SetLocation(262, 11);
			//nameLabel->SetText(L"박경훈봇");
			nameLabel->align = Label::Align::Left;
			nameLabel->valign = Label::VAlign::Middle;

			// 아이템
			vector<UI*> itemslot;
			for (int k = 0; k < 7; ++k)
			{
				//auto slot = champPanel->CreateChild(L"scoreboard_itemempty", Vector2(260 + (29 * k), 29));
				auto slot = champPanel->AddChild<UI>(L"item", new UI(L"scoreboard_itemempty", Vector2(260 + (29 * k), 29)));
				itemslot.push_back(slot);
				slot->transform->scale = { 0.4218f, 0.4218f, 1.f };
			}
			itemslot[0]->SetTexture(L"1004_class_t1_faeriecharm");

			championScoreUI[i][j]->Hide();
		}
	}
}

ScorePanel::~ScorePanel()
{
}

ScorePanel* ScorePanel::GetInstance()
{
	if (pScorePanel == nullptr)
	{
		pScorePanel = new ScorePanel();
	}
	return pScorePanel;
}

void ScorePanel::DestroyInstance()
{
	if (pScorePanel)
	{
		delete pScorePanel;
		pScorePanel = nullptr;
	}
}

void ScorePanel::Update()
{
	if (Input::GetKeyDown('O'))
	{
		visible = !visible;
		visible ? Show() : Hide();
	}

	if (visible)
	{
		for (int i = (UINT)Team::BLUE; i <= (UINT)Team::RED; ++i) {
			size_t j;
			for (j = 0; j < championInfo[i].size(); j++)
			{
				auto* ui = championScoreUI[i][j];
				auto* champ = championInfo[i][j];

				// 사망 표시
				if (champ->IsDead())	ui->children[L"scoreboard_dead"]->Show();
				else					ui->children[L"scoreboard_dead"]->Hide();

				// 사망 카운트
				ui->children[L"DeadCount"]->Hide();

				// 미아 표시
				ui->children[L"scoreboard_mia"]->Hide();

				// 챔피언 초상화 테두리
				if (champ->IsDead())
					dynamic_cast<UI*>(ui->children[L"ChampBorder"])->SetTexture(L"scoreboard_champborder_dead");
				else
					dynamic_cast<UI*>(ui->children[L"ChampBorder"])->SetTexture(L"scoreboard_champborder_alive");
			
				// 레벨
				dynamic_cast<Label*>(ui->children[L"LevelLabel"])->SetText(1);

				// CS
				dynamic_cast<Label*>(ui->children[L"CSLabel"])->SetText(0);

				// KDA
				dynamic_cast<Label*>(ui->children[L"KDALabel"])->SetText(L"%d/%d/%d", 0, 0, 0);

				// 아이템


			}
			for (; j < 5; j++)
			{
				championScoreUI[i][j]->Hide();
			}
		}
	}
}

void ScorePanel::AddChampion(Champion* _champ, bool _isPlayer/* = false*/)
{
	// 5인
	if (championInfo[(UINT)_champ->GetTeam()].size() >= 5) return;
	
	// 중복 확인
	for (auto Champ : championInfo[(UINT)_champ->GetTeam()]) {
		if (Champ->GetID() == _champ->GetID()) return;
	}

	UINT index = championInfo[(UINT)_champ->GetTeam()].size();

	championInfo[(UINT)_champ->GetTeam()].push_back(_champ);

	UI* ui = championScoreUI[(UINT)_champ->GetTeam()][index];

	dynamic_cast<UI*>(ui->children[L"Portrait"])->SetTexture(_champ->faceCircleTexkey);
	dynamic_cast<Label*>(ui->children[L"NameLabel"])->SetText(_champ->nickname);

	if (_isPlayer) {

	}
	else {
		ui->DeleteChild(L"scoreboard_playermark");
	}
}

void ScorePanel::AddDragon(Dragon _dragon, Team _team)
{
	int size = dragonCount[(UINT)_team].size();
	int index = size;
	if (size == 4) return;

	dragonCount[(UINT)_team].push_back(_dragon);
	dragonCountUI[(UINT)_team][index]->SetTexture(dragonCountTex[(UINT)_dragon]);

	if (index == 3) {
		for (int i = dragonCount[(UINT)Team::BLUE].size(); i < 4; ++i)
			dragonCountUI[(UINT)Team::BLUE][i]->SetTexture(dragonCountTex[(UINT)Dragon::BREAK]);
		for (int i = dragonCount[(UINT)Team::RED].size(); i < 4; ++i)
			dragonCountUI[(UINT)Team::RED][i]->SetTexture(dragonCountTex[(UINT)Dragon::BREAK]);
	}
}

void ScorePanel::SetDragonSoul(Dragon _dragon)
{
	dragonSoulUI->SetTexture(dragonSoulTex1[(UINT)_dragon]);
	dragonSoul = _dragon;
}

void ScorePanel::SetDragonSoulBuf()
{
	dragonSoulUI->SetTexture(dragonSoulTex2[(UINT)dragonSoul]);
}
