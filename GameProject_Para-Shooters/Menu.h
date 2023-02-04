#pragma once

#include "Actor.h"
#include "AnimSpriteComponent.h"
#include "AnimInformation.h"
#include "Entity.h"
#include <utility>

#define MAX_LEVEL 100
#define MAX_DAMAGE_LEVEL 9999
#define MAX_NUM_OF_BULLET_LEVEL 9984
#define DAMAGE_PRICE_CORRECTOR 99.9099
#define NUM_OF_BULLET_PRICE_CORRECTOR 100.060
#define BASIC_PRICE_CORRECTOR 998.999
#define BASIC_PRICE 1000
#define MAX_ITEM 10
#define MAX_UPGRADES 6
#define ITEM_HEAL 0
#define UPGRADE_DAMAGE 1 
#define UPGRADE_INTERVAL 2
#define UPGRADE_NUM_OF_BULLET 3
#define UPGRADE_RELOAD 4
#define UPGRADE_SHIELD 5 
#define UPGRADE_PLACEMENT_CORRECTOR 2
#define TIME_TO_BUY 0.3
class Menu :public Actor {
public:
	Menu(Game* game, class Player* player);
	void UpdateActor(double delta) override;
	bool healPlayer();
	void CreateMenu();
	void keyInput(GLFWwindow* mWindow, int key, int scancode, int action, int mods);
	int getUpgradeLevel(int i) { return upgrades[i].second; }
	void ResetPrice();
private:
	class Player* mPlayer;
	class Camera* mCamera;
	int score;
	int mapID, litsID, upgradeID;
	int cursor;
	double boughtTime;
	int currentStage;
	//1ステージで使える回復は10個まで)
	int sumOfHeals;
	bool updateFlag;
	bool checkFlag;
	vector<bool> updateFlags;
	vector<SpriteComponent*> StageTime;
	vector<Actor*> Damage_Actor_Price, Interval_Actor_Price, NumOfBullet_Actor_Price, Reload_Actor_Price, Shield_Actor_Price, Heal_Actor_Price;
	vector<SpriteComponent*> Damage_Price, Interval_Price, NumOfBullet_Price, Reload_Price, Shield_Price, Heal_Price;
	vector<Actor*> Damage_Actor_Lv, Interval_Actor_Lv, NumOfBullet_Actor_Lv, Reload_Actor_Lv, Shield_Actor_Lv, Heal_Actor_Lv;
	vector<SpriteComponent*> Damage, Interval, NumOfBullet, Reload, Shield, Heal;
	vector<Actor*> Score_Actor;
	vector<SpriteComponent*> Score;
	vector<std::pair<int,int>> upgrades;
	vector<SpriteComponent*> upgradePic;
	Actor* cursorActor;
	SpriteComponent* cursorTex;
	//動かない文字等の追加
	void SetMenuInfo(Actor* Display, int place, double gap, Dir_Vector first);
	//変更可能な文字等の追加
	SpriteComponent* SetChangeInfo(Actor* Display, int place, double gap, Dir_Vector first);
	//変更可能なアップグレードの追加
	SpriteComponent* SetUpgradeInfo(Actor* Display, int place, double gap, Dir_Vector first);
	int CalcPrice(int type, int lv);
	void buyItem();
	void updateMenu();
	void ResetOneMenu(vector<SpriteComponent*> &type, vector<Actor*> &parent, int level, bool moveFlag, double gap, Dir_Vector base);
	void checkAbleToBuy();
};