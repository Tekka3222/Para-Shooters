#pragma once
#include "Actor.h"
#include "AnimSpriteComponent.h"
#include "AnimInformation.h"
#include "Entity.h"
#include <utility>
//ステージのリザルト画面を管理
class Result :public Actor {
public:
	Result(Game* game, class Player* player);
	void UpdateActor(double delta) override;
	void CreateResult();
	void RecreateResult();
private:
	class Player* mPlayer;
	class Camera* mCamera;
	int score;
	int mapID, litsID;
	vector<SpriteComponent*> StageTime;
	vector<Actor*> GotShot_Actor, Kills_Actor, Score_Actor;
	vector<SpriteComponent*> GotShot, Kills, Score;
	void SetResultInfo(Actor* Display, int place, double gap, Dir_Vector first);
	SpriteComponent* SetChangeInfo(Actor* Display, int place, double gap, Dir_Vector first);
};