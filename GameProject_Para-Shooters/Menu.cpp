#include "Menu.h"
#include <iostream>
#include <string>

Menu::Menu(Game* game, class Player* player)
	:Actor(game)
	, mPlayer(player)
	, mCamera(player->GetCamera())
	, cursor(0)
	, boughtTime(0.0)
	, currentStage(1)
	, sumOfHeals(0)
	, updateFlag(false)
	, checkFlag(false)
{
	std::string ImURL = "images/";
	GLuint texID;
	glGenTextures(1, &texID);
	GetGame()->SetupTexture(&texID, ImURL + "tilemap.raw", TEXWIDTH, TEXHEIGHT);
	mapID = texID;
	glGenTextures(1, &texID);
	GetGame()->SetupTexture(&texID, ImURL + "litsmap.raw", TEXWIDTH, TEXHEIGHT);
	litsID = texID;
	glGenTextures(1, &texID);
	GetGame()->SetupTexture(&texID, ImURL + "upgrademap.raw", TEXWIDTH, TEXHEIGHT);
	upgradeID = texID;
	upgrades = vector<std::pair<int,int>>(MAX_UPGRADES,std::pair<int,int>(1000, 0));
	cursorActor = new Actor(game);
	cursorActor->SetScale(0.12);
	cursorActor->SetPosition(Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 3));
	cursorTex = new SpriteComponent(cursorActor, GetGame()->GetShader(), 1200);
	cursorTex->SetTexture(upgradeID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, CURSOR);
	updateFlags = vector<bool>(MAX_UPGRADES, true);
	CreateMenu();
}

void Menu::UpdateActor(double delta) {
		cursorActor->SetPosition(Dir_Vector(Game::getWidth() * (2 + (cursor % 2) * 3) / 8 - ONEBOX / 2, ONEBOX * 3 * (((int)cursor / 2) + 1.0)));
		if (boughtTime > 0.0) {
			cursorTex->ChangeTexture(CURSOR_BOUGHT);
			if (boughtTime <= delta) {
				boughtTime = 0.0;
			}
			else {
				boughtTime -= delta;
			}
		}
		else {
			cursorTex->ChangeTexture(CURSOR);
		}
		if (updateFlag) {
			updateFlag = false;
			updateMenu();
		}
}

void Menu::ResetPrice() {
	if (currentStage != Camera::curStage) {
		currentStage = Camera::curStage;
		sumOfHeals = upgrades[ITEM_HEAL].second;
		checkFlag = true;
		updateFlag = true;
	}
	for (int i = 0; i < MAX_UPGRADES; i++) {
		upgrades[i].first = CalcPrice(i, upgrades[i].second);
	}
	if (checkFlag) {
		checkAbleToBuy();
	}
	return;
}
//価格の計算式
int Menu::CalcPrice(int type, int lv) {
	switch (type) {
	case ITEM_HEAL:
		return 1000 * static_cast<int>(std::pow(2.0, max(lv, sumOfHeals)));
	case UPGRADE_DAMAGE:
		return static_cast<int>(DAMAGE_PRICE_CORRECTOR * lv) + BASIC_PRICE;
	case UPGRADE_NUM_OF_BULLET:
		return static_cast<int>(NUM_OF_BULLET_PRICE_CORRECTOR * lv) + BASIC_PRICE;
	default:
		return static_cast<int>(BASIC_PRICE_CORRECTOR * lv) + BASIC_PRICE;
	}

	return 0;
}
//アップグレードアイテムが購入できるかどうか
void Menu::checkAbleToBuy() {
	int pScore = mPlayer->getScore();
	//Heal
	if (pScore >= upgrades[ITEM_HEAL].first && upgrades[ITEM_HEAL].second < MAX_ITEM) {
		upgradePic[ITEM_HEAL]->ChangeTexture(HEAL_TEX);
	}
	else {
		upgradePic[ITEM_HEAL]->ChangeTexture(HEAL_COMP_TEX);
	}
	//Damage
	if (pScore >= upgrades[UPGRADE_DAMAGE].first && upgrades[UPGRADE_DAMAGE].second < MAX_DAMAGE_LEVEL) {
		upgradePic[UPGRADE_DAMAGE]->ChangeTexture(DAMAGE_TEX);
	}
	else {
		upgradePic[UPGRADE_DAMAGE]->ChangeTexture(DAMAGE_COMP_TEX);
	}
	//Interval
	if (pScore >= upgrades[UPGRADE_INTERVAL].first && upgrades[UPGRADE_INTERVAL].second < MAX_LEVEL) {
		upgradePic[UPGRADE_INTERVAL]->ChangeTexture(INTERVAL_TEX);
	}
	else {
		upgradePic[UPGRADE_INTERVAL]->ChangeTexture(INTERVAL_COMP_TEX);
	}
	//numOfBullet
	if (pScore >= upgrades[UPGRADE_NUM_OF_BULLET].first && upgrades[UPGRADE_NUM_OF_BULLET].second < MAX_NUM_OF_BULLET_LEVEL) {
		upgradePic[UPGRADE_NUM_OF_BULLET]->ChangeTexture(NUM_OF_BULLET_TEX);
	}
	else {
		upgradePic[UPGRADE_NUM_OF_BULLET]->ChangeTexture(NUM_OF_BULLET_COMP_TEX);
	}
	//Reload
	if (pScore >= upgrades[UPGRADE_RELOAD].first && upgrades[UPGRADE_RELOAD].second < MAX_LEVEL) {
		upgradePic[UPGRADE_RELOAD]->ChangeTexture(RELOAD_TEX);
	}
	else {
		upgradePic[UPGRADE_RELOAD]->ChangeTexture(RELOAD_COMP_TEX);
	}
	//Shield
	if (pScore >= upgrades[UPGRADE_SHIELD].first && upgrades[UPGRADE_SHIELD].second < MAX_LEVEL) {
		upgradePic[UPGRADE_SHIELD]->ChangeTexture(SHIELD_TEX);
	}
	else {
		upgradePic[UPGRADE_SHIELD]->ChangeTexture(SHIELD_COMP_TEX);
	}
}
//メニューを作成
void Menu::CreateMenu() {
	ResetPrice();
	Actor* Display = NULL;
	double scale = (Game::getWidth() + ONEBOX) / RESULTSIZE_CORRECTOR;
	Actor* white = new Actor(GetGame());
	white->SetScale(scale);
	white->SetPosition(Dir_Vector((Game::getWidth() - scale * RESULTSIZE_CORRECTOR) / 2, -ONEBOX * 10));
	SpriteComponent* wAnim = new SpriteComponent(white, GetGame()->GetShader(), 1000);
	wAnim->SetTexture(mapID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, RESULT);
	//StageStore
	double length = 11;
	SetMenuInfo(Display, ALPHA_S, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, ALPHA_T, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, ALPHA_A, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, ALPHA_G, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, ALPHA_E, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, NULL_PIC, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, ALPHA_S, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, ALPHA_T, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, ALPHA_O, 8 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, ALPHA_R, 9 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetMenuInfo(Display, ALPHA_E, 10 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));

	//Heal+Possession row-3-left
	length = 8;
	upgradePic.emplace_back(SetUpgradeInfo(Display, HEAL_TEX, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 3)));
	SetMenuInfo(Display, ALPHA_P, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 3));
	SetMenuInfo(Display, ALPHA_O, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 3));
	SetMenuInfo(Display, ALPHA_S, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 3));
	SetMenuInfo(Display, ALPHA_S, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 3));
	SetMenuInfo(Display, COLON, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 3));
	
	Display = new Actor(GetGame());
	Heal_Actor_Lv.emplace_back(Display);
	Heal.emplace_back(SetChangeInfo(Display, 0, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 3)));
	//Heal Price row-4.5-left
	Display = new Actor(GetGame());
	Heal_Actor_Price.emplace_back(Display);
	Heal_Price.emplace_back(SetChangeInfo(Display, 0, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 4.5)));
	//Damage+Level row-3-right
	upgradePic.emplace_back(SetUpgradeInfo(Display, DAMAGE_TEX, 0 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 3)));
	SetMenuInfo(Display, ALPHA_L, 1 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 3));
	SetMenuInfo(Display, ALPHA_V, 2 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 3));
	SetMenuInfo(Display, COLON, 3 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 3));

	Display = new Actor(GetGame());
	Damage_Actor_Lv.emplace_back(Display);
	Damage.emplace_back(SetChangeInfo(Display, 0, 4 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 3)));
	//Damage Price row-4.5-right
	Display = new Actor(GetGame());
	Damage_Actor_Price.emplace_back(Display);
	Damage_Price.emplace_back(SetChangeInfo(Display, 0, 2 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 4.5)));
	//Interval+Level row-5-left
	upgradePic.emplace_back(SetUpgradeInfo(Display, INTERVAL_TEX, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 6)));
	SetMenuInfo(Display, ALPHA_L, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 6));
	SetMenuInfo(Display, ALPHA_V, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 6));
	SetMenuInfo(Display, COLON, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 6));

	Display = new Actor(GetGame());
	Interval_Actor_Lv.emplace_back(Display);
	Interval.emplace_back(SetChangeInfo(Display, 0, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 6)));
	//Interval Price row-6.5-left
	Display = new Actor(GetGame());
	Interval_Actor_Price.emplace_back(Display);
	Interval_Price.emplace_back(SetChangeInfo(Display, 0, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 6.5)));
	//numOfBullet+Level row-5-right
	upgradePic.emplace_back(SetUpgradeInfo(Display, NUM_OF_BULLET_TEX, 0 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 6)));
	SetMenuInfo(Display, ALPHA_L, 1 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 6));
	SetMenuInfo(Display, ALPHA_V, 2 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 6));
	SetMenuInfo(Display, COLON, 3 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 6));

	Display = new Actor(GetGame());
	NumOfBullet_Actor_Lv.emplace_back(Display);
	NumOfBullet.emplace_back(SetChangeInfo(Display, 0, 4 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 6)));
	//numOfBullet Price row-6.5-right
	Display = new Actor(GetGame());
	NumOfBullet_Actor_Price.emplace_back(Display);
	NumOfBullet_Price.emplace_back(SetChangeInfo(Display, 0, 2 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 6.5)));
	//reload+Level row-9-left
	upgradePic.emplace_back(SetUpgradeInfo(Display, RELOAD_TEX, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 9)));
	SetMenuInfo(Display, ALPHA_L, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 9));
	SetMenuInfo(Display, ALPHA_V, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 9));
	SetMenuInfo(Display, COLON, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 9));

	Display = new Actor(GetGame());
	Reload_Actor_Lv.emplace_back(Display);
	Reload.emplace_back(SetChangeInfo(Display, 0, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 9)));
	// reload Price row-10.5-left
	Display = new Actor(GetGame());
	Reload_Actor_Price.emplace_back(Display);
	Reload_Price.emplace_back(SetChangeInfo(Display, 0, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 4 - ONEBOX / 2, ONEBOX * 10.5)));
	//shield+Level row-9-right
	upgradePic.emplace_back(SetUpgradeInfo(Display, SHIELD_TEX, 0 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 9)));
	SetMenuInfo(Display, ALPHA_L, 1 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 9));
	SetMenuInfo(Display, ALPHA_V, 2 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 9));
	SetMenuInfo(Display, COLON, 3 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 9));

	Display = new Actor(GetGame());
	Shield_Actor_Lv.emplace_back(Display);
	Shield.emplace_back(SetChangeInfo(Display, 0, 4 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 9)));
	//shield Price row-10.5-right
	Display = new Actor(GetGame());
	Shield_Actor_Price.emplace_back(Display);
	Shield_Price.emplace_back(SetChangeInfo(Display, 0, 2 * ONEBOX, Dir_Vector(Game::getWidth() * 5 / 8 - ONEBOX / 2, ONEBOX * 10.5)));
	//ScorePoint row-12-mid
	length = 11;
	SetMenuInfo(Display, ALPHA_S, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, ALPHA_C, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, ALPHA_O, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, ALPHA_R, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, ALPHA_E, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, NULL_PIC, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, ALPHA_P, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, ALPHA_O, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, ALPHA_I, 8 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, ALPHA_N, 9 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetMenuInfo(Display, ALPHA_T, 10 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	//ScorePoint-Num row-13.5-mid 
	Display = new Actor(GetGame());
	Score_Actor.emplace_back(Display);
	Score.emplace_back(SetChangeInfo(Display, 0, 0, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 13.5)));
	//PressTToGo
	length = 16;
	SetMenuInfo(Display, ALPHA_P, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_R, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_E, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_S, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_S, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, NULL_PIC, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_T, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, NULL_PIC, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_T, 8 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_O, 9 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, NULL_PIC, 10 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_C, 11 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_L, 12 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_O, 13 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_S, 14 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetMenuInfo(Display, ALPHA_E, 15 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	checkFlag = true;
	updateFlag = true;
	ResetPrice();
	updateMenu();
}

void Menu::updateMenu() {
	//Heal
	if (updateFlags[ITEM_HEAL]) {
		ResetOneMenu(Heal_Price, Heal_Actor_Price, upgrades[ITEM_HEAL].first, true, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 4.0 - ONEBOX / 2, ONEBOX * 4.5));
		ResetOneMenu(Heal, Heal_Actor_Lv, upgrades[ITEM_HEAL].second, false, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 4.0 - ONEBOX / 2, ONEBOX * 3));
		updateFlags[ITEM_HEAL] = false;
	}
	//Damage
	if (updateFlags[UPGRADE_DAMAGE]) {
		ResetOneMenu(Damage_Price, Damage_Actor_Price, upgrades[UPGRADE_DAMAGE].first, true, 2 * ONEBOX, Dir_Vector(Game::getWidth() * 5.0 / 8 - ONEBOX / 2, ONEBOX * 4.5));
		ResetOneMenu(Damage, Damage_Actor_Lv, upgrades[UPGRADE_DAMAGE].second, false, 4 * ONEBOX, Dir_Vector(Game::getWidth() * 5.0 / 8 - ONEBOX / 2, ONEBOX * 3));
		updateFlags[UPGRADE_DAMAGE] = false;
	}
	//Interval
	if (updateFlags[UPGRADE_INTERVAL]) {
		ResetOneMenu(Interval_Price, Interval_Actor_Price, upgrades[UPGRADE_INTERVAL].first, true, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 4.0 - ONEBOX / 2, ONEBOX * 7.5));
		ResetOneMenu(Interval, Interval_Actor_Lv, upgrades[UPGRADE_INTERVAL].second, false, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 4.0 - ONEBOX / 2, ONEBOX * 6));
		updateFlags[UPGRADE_INTERVAL] = false;
	}
	//NumOfBullet
	if (updateFlags[UPGRADE_NUM_OF_BULLET]) {
		ResetOneMenu(NumOfBullet_Price, NumOfBullet_Actor_Price, upgrades[UPGRADE_NUM_OF_BULLET].first, true, 2 * ONEBOX, Dir_Vector(Game::getWidth() * 5.0 / 8 - ONEBOX / 2, ONEBOX * 7.5));
		ResetOneMenu(NumOfBullet, NumOfBullet_Actor_Lv, upgrades[UPGRADE_NUM_OF_BULLET].second, false, 4 * ONEBOX, Dir_Vector(Game::getWidth() * 5.0 / 8 - ONEBOX / 2, ONEBOX * 6));
		updateFlags[UPGRADE_NUM_OF_BULLET] = false;
	}
	//Reload
	if (updateFlags[UPGRADE_RELOAD]) {
		ResetOneMenu(Reload_Price, Reload_Actor_Price, upgrades[UPGRADE_RELOAD].first, true, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 4.0 - ONEBOX / 2, ONEBOX * 10.5));
		ResetOneMenu(Reload, Reload_Actor_Lv, upgrades[UPGRADE_RELOAD].second, false, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 4.0 - ONEBOX / 2, ONEBOX * 9));
		updateFlags[UPGRADE_RELOAD] = false;
	}
	//Shield
	if (updateFlags[UPGRADE_SHIELD]) {
		ResetOneMenu(Shield_Price, Shield_Actor_Price, upgrades[UPGRADE_SHIELD].first, true, 2 * ONEBOX, Dir_Vector(Game::getWidth() * 5.0 / 8 - ONEBOX / 2, ONEBOX * 10.5));
		ResetOneMenu(Shield, Shield_Actor_Lv, upgrades[UPGRADE_SHIELD].second, false, 4 * ONEBOX, Dir_Vector(Game::getWidth() * 5.0 / 8 - ONEBOX / 2, ONEBOX * 9));
		updateFlags[UPGRADE_SHIELD] = false;
	}
	//Score
	ResetOneMenu(Score, Score_Actor, mPlayer->getScore(), true, 0, Dir_Vector(Game::getWidth() / 2 - ONEBOX / 2, ONEBOX * 13.5));
}

void Menu::ResetOneMenu(vector<SpriteComponent*> &type, vector<Actor*> &parent, int level, bool moveFlag, double gap, Dir_Vector base) {
	string lv = to_string(level);
	int currentSize = type.size();
	int afterSize = lv.length();
	if (!moveFlag) {
		for (int i = 0; i < afterSize; i++) {
			if (i >= currentSize) {
				Actor* Display = new Actor(GetGame());
				parent.emplace_back(Display);
				type.emplace_back(SetChangeInfo(Display, lv[i] - '0', gap + i * ONEBOX, base));
			}
			else {
				type[i]->ChangeTexture(lv[i] - '0');
			}
		}
		while (afterSize < currentSize) {
			currentSize--;
			parent.back()->SetState(State::Dead);
			parent.pop_back();
			type.pop_back();
		}
	}
	else {
		for (int i = 0; i < afterSize; i++) {
			if (i >= currentSize) {
				Actor* Display = new Actor(GetGame());
				parent.emplace_back(Display);
				type.emplace_back(SetChangeInfo(Display, lv[i] - '0', gap + i * ONEBOX, base + Dir_Vector((1.0 - afterSize) * ONEBOX / 2.0, 0)));
			}
			else {//位置の調整
				parent[i]->SetPosition(base + Dir_Vector((1.0 - afterSize) * ONEBOX / 2,0) + Dir_Vector(gap + i * ONEBOX, 0));
				type[i]->ChangeTexture(lv[i] - '0');
			}
		}
		while (afterSize < currentSize) {
			currentSize--;
			parent.back()->SetState(State::Dead);
			parent.pop_back();
			type.pop_back();
		}
	}
}

SpriteComponent* Menu::SetChangeInfo(Actor* Display, int place, double gap, Dir_Vector first) {
	Display->SetScale(0.12);
	Display->SetPosition(first);
	SpriteComponent* nDisp = new SpriteComponent(Display, GetGame()->GetShader(), 1180);
	Display->SetPosition(Display->GetPosition() + Dir_Vector(gap, 0));
	nDisp->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, place);
	return nDisp;
}

void Menu::SetMenuInfo(Actor* Display, int place, double gap, Dir_Vector first) {
	Display = new Actor(GetGame());
	Display->SetScale(0.12);
	Display->SetPosition(first);
	SpriteComponent* nDisp = new SpriteComponent(Display, GetGame()->GetShader(), 1180);
	Display->SetPosition(Display->GetPosition() + Dir_Vector(gap, 0));
	nDisp->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, place);
}
SpriteComponent* Menu::SetUpgradeInfo(Actor* Display, int place, double gap, Dir_Vector first) {
	Display = new Actor(GetGame());
	Display->SetScale(0.12);
	Display->SetPosition(first);
	SpriteComponent* nDisp = new SpriteComponent(Display, GetGame()->GetShader(), 1180);
	Display->SetPosition(Display->GetPosition() + Dir_Vector(gap, 0));
	nDisp->SetTexture(upgradeID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, place);
	return nDisp;
}

void Menu::keyInput(GLFWwindow* mWindow, int key, int scancode, int action, int mods) {
	int sRight = glfwGetKey(mWindow, GLFW_KEY_RIGHT);
	int sD = glfwGetKey(mWindow, GLFW_KEY_D);
	if (sRight == GLFW_PRESS || sRight == GLFW_REPEAT || sD == GLFW_PRESS || sD == GLFW_REPEAT) {
		if (cursor % 2 == 0) {
			cursor++;
		}
		else {
			cursor--;
		}
	}
	int sLeft = glfwGetKey(mWindow, GLFW_KEY_LEFT);
	int sA = glfwGetKey(mWindow, GLFW_KEY_A);
	if (sLeft == GLFW_PRESS || sLeft == GLFW_REPEAT || sA == GLFW_PRESS || sA == GLFW_REPEAT) {
		if (cursor % 2 == 0) {
			cursor++;
		}
		else {
			cursor--;
		}
	}
	int sDown = glfwGetKey(mWindow, GLFW_KEY_DOWN);
	int sS = glfwGetKey(mWindow, GLFW_KEY_S);
	if (sDown == GLFW_PRESS || sDown == GLFW_REPEAT || sS == GLFW_PRESS || sS == GLFW_REPEAT) {
		cursor += 2;
		if (cursor >= MAX_UPGRADES) {
			cursor -= MAX_UPGRADES;
		}
	}
	int sUp = glfwGetKey(mWindow, GLFW_KEY_UP);
	int sW = glfwGetKey(mWindow, GLFW_KEY_W);
	if (sUp == GLFW_PRESS || sUp == GLFW_REPEAT || sW == GLFW_PRESS || sW == GLFW_REPEAT) {
		cursor -= 2;
		if (cursor < 0) {
			cursor += MAX_UPGRADES;
		}
	}		
	int buy = glfwGetKey(mWindow, GLFW_KEY_SPACE);
	if (buy == GLFW_PRESS || buy == GLFW_REPEAT) {
		buyItem();
	}
}
//アイテム/アップグレードを購入する
void Menu::buyItem() {
	int pScore = mPlayer->getScore();
	int level = upgrades[cursor].second;
	switch (cursor) {
	case ITEM_HEAL:
		if (upgrades[ITEM_HEAL].first <= pScore && level < MAX_ITEM && sumOfHeals < MAX_ITEM) {
			pScore -= upgrades[ITEM_HEAL].first;
			mPlayer->setScore(pScore);
			upgrades[ITEM_HEAL].second++;
			sumOfHeals++;
			upgrades[ITEM_HEAL].first = CalcPrice(ITEM_HEAL, level + 1);
			boughtTime = TIME_TO_BUY;
			updateFlag = true;
			checkAbleToBuy();
			updateFlags[ITEM_HEAL] = true;
		}
		break;
	case UPGRADE_DAMAGE:
		if (upgrades[UPGRADE_DAMAGE].first <= pScore && level < MAX_DAMAGE_LEVEL) {
			pScore -= upgrades[UPGRADE_DAMAGE].first;
			mPlayer->setScore(pScore);
			upgrades[UPGRADE_DAMAGE].second++;
			upgrades[UPGRADE_DAMAGE].first = CalcPrice(UPGRADE_DAMAGE, level + 1);
			boughtTime = TIME_TO_BUY;
			updateFlag = true;
			checkAbleToBuy();
			updateFlags[UPGRADE_DAMAGE] = true;
		}
		break;
	case UPGRADE_NUM_OF_BULLET:
		if (upgrades[UPGRADE_NUM_OF_BULLET].first <= pScore && level < MAX_NUM_OF_BULLET_LEVEL) {
			pScore -= upgrades[UPGRADE_NUM_OF_BULLET].first;
			mPlayer->setScore(pScore);
			upgrades[UPGRADE_NUM_OF_BULLET].second++;
			upgrades[UPGRADE_NUM_OF_BULLET].first = CalcPrice(UPGRADE_NUM_OF_BULLET, level + 1);
			boughtTime = TIME_TO_BUY;
			updateFlag = true;
			checkAbleToBuy();
			updateFlags[UPGRADE_NUM_OF_BULLET] = true;
		}
		break;
	default:
		if (upgrades[cursor].first <= pScore && level < MAX_LEVEL) {
			pScore -= upgrades[cursor].first;
			mPlayer->setScore(pScore);
			upgrades[cursor].second++;
			upgrades[cursor].first = CalcPrice(cursor, level + 1);
			boughtTime = TIME_TO_BUY;
			updateFlag = true;
			checkAbleToBuy();
			updateFlags[cursor] = true;
		}
	}
	return;
}
//回復を使用出来るかどうか確認し、できる場合は、ショップを含め情報を更新する。
bool Menu::healPlayer() {
	int ammount = upgrades[ITEM_HEAL].second;
	if (ammount > 0) {
		upgrades[ITEM_HEAL].second--;
		updateFlag = true;
		updateFlags[ITEM_HEAL] = true;
		return true;
	}
	return false;
}