#include "Result.h"
#include <iostream>
#include <string>

Result::Result(Game* game, class Player* player)
	:Actor(game)
	,mPlayer(player)
	,mCamera(player->GetCamera())
{
	std::string ImURL = "images/";
	GLuint texID;
	glGenTextures(1, &texID);
	GetGame()->SetupTexture(&texID, ImURL + "tilemap.raw", TEXWIDTH, TEXHEIGHT);
	mapID = texID;
	glGenTextures(1, &texID);
	GetGame()->SetupTexture(&texID, ImURL + "litsmap.raw", TEXWIDTH, TEXHEIGHT);
	litsID = texID;
	CreateResult();
}

void Result::UpdateActor(double delta) {
	if (mPlayer->getResultFlag()) {
		mPlayer->setResultFlag(false);
		RecreateResult();
	}
}

void Result::CreateResult() {
	Actor* Display = NULL;
	double scale = (Game::getWidth() + ONEBOX) / RESULTSIZE_CORRECTOR;
	Actor* white = new Actor(GetGame());
	white->SetScale(scale);
	white->SetPosition(Dir_Vector((Game::getWidth() - scale * RESULTSIZE_CORRECTOR) / 2, -ONEBOX * 10));
	SpriteComponent* wAnim = new SpriteComponent(white, GetGame()->GetShader(), 1000);
	wAnim->SetTexture(mapID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, 11);
	//StageResult
	double length = 11;
	SetResultInfo(Display, ALPHA_S, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_T, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_A, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_G, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_E, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, NULL_PIC, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_R, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_E, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_S, 8 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_U, 9 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_L, 10 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetResultInfo(Display, ALPHA_T, 11 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	//GotShot
	length = 8;
	SetResultInfo(Display, ALPHA_G, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetResultInfo(Display, ALPHA_O, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetResultInfo(Display, ALPHA_T, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetResultInfo(Display, NULL_PIC, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetResultInfo(Display, ALPHA_S, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetResultInfo(Display, ALPHA_H, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetResultInfo(Display, ALPHA_O, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetResultInfo(Display, ALPHA_T, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	//GotShot_Num
	Display = new Actor(GetGame());
	GotShot_Actor.emplace_back(Display);
	GotShot.emplace_back(SetChangeInfo(Display, 0, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX / 2, ONEBOX * 4.5)));
	//Kills
	length = 5;
	SetResultInfo(Display, ALPHA_K, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 6));
	SetResultInfo(Display, ALPHA_I, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 6));
	SetResultInfo(Display, ALPHA_L, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 6));
	SetResultInfo(Display, ALPHA_L, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 6));
	SetResultInfo(Display, ALPHA_S, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 6));
	//Kills_Num//7.5
	Display = new Actor(GetGame());
	Kills_Actor.emplace_back(Display);
	Kills.emplace_back(SetChangeInfo(Display, 0, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX / 2, ONEBOX * 7.5)));
	//StageTime
	length = 10;
	SetResultInfo(Display, ALPHA_S, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetResultInfo(Display, ALPHA_T, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetResultInfo(Display, ALPHA_A, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetResultInfo(Display, ALPHA_G, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetResultInfo(Display, ALPHA_E, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetResultInfo(Display, NULL_PIC, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetResultInfo(Display, ALPHA_T, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetResultInfo(Display, ALPHA_I, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetResultInfo(Display, ALPHA_M, 8 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetResultInfo(Display, ALPHA_E, 9 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	//Time_Num//10.5
	length = 8;
	SetResultInfo(Display, COLON, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 10.5));
	SetResultInfo(Display, COLON, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 10.5));
	for (int i = 0; i < 6; i++) {
		Display = new Actor(GetGame());
		StageTime.emplace_back(SetChangeInfo(Display, 0, i * ONEBOX + (i / 2) * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 10.5)));
	}
	//StageScore
	length = 11;
	SetResultInfo(Display, ALPHA_S, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, ALPHA_T, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, ALPHA_A, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, ALPHA_G, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, ALPHA_E, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, NULL_PIC, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, ALPHA_S, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, ALPHA_C, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, ALPHA_O, 8 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, ALPHA_R, 9 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetResultInfo(Display, ALPHA_E, 10 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	//Score_Num//13.5
	Display = new Actor(GetGame());
	Score_Actor.emplace_back(Display);
	Score.emplace_back(SetChangeInfo(Display, 0, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX / 2, ONEBOX * 13.5)));
	//PressEnterToGo
	length = 17;
	SetResultInfo(Display, ALPHA_P, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_R, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_E, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_S, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_S, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, NULL_PIC, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_E, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_N, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_T, 8 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_E, 9 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_R, 10 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, NULL_PIC, 11 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_T, 12 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_O, 13 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, NULL_PIC, 14 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_G, 15 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetResultInfo(Display, ALPHA_O, 16 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
}

void Result::RecreateResult() {
	Actor* Display = NULL;
	score = mCamera->GetStageScore();
	int damageTime = mPlayer->getNoDamage();
	int NumOfKill = mPlayer->GetKill() - mCamera->GetLapKill();
	double stageSumTime = mCamera->GetStageSumTime();
	//GotShot
	int basicSize = GotShot.size();
	string changeStr = to_string(damageTime);
	int afterSize = changeStr.length();
	for (int i = 0; i < afterSize;i++) {
		if (i >= basicSize) {
			Display = new Actor(GetGame());
			GotShot_Actor.emplace_back(Display);
			GotShot.emplace_back(SetChangeInfo(Display, changeStr[i] - '0', i * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * afterSize / 2, ONEBOX * 4.5)));
		}
		else {
			GotShot_Actor[i]->SetPosition(Dir_Vector(Game::getWidth() / 2 - ONEBOX * afterSize / 2, ONEBOX * 4.5) + Dir_Vector(i * ONEBOX,0));
			GotShot[i]->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, changeStr[i] - '0');
		}
	}
	while(afterSize < basicSize) {
		basicSize--;
		GotShot_Actor.back()->SetState(State::Dead);
		GotShot_Actor.pop_back();
		GotShot.pop_back();
	}
	//Kills
	basicSize = Kills.size();
	changeStr = to_string(NumOfKill);
	afterSize = changeStr.length();
	for (int i = 0; i < afterSize; i++) {
		if (i >= basicSize) {
			Display = new Actor(GetGame());
			Kills_Actor.emplace_back(Display);
			Kills.emplace_back(SetChangeInfo(Display, changeStr[i] - '0', i * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * afterSize / 2, ONEBOX * 7.5)));
		}
		else {
			Kills_Actor[i]->SetPosition(Dir_Vector(Game::getWidth() / 2 - ONEBOX * afterSize / 2, ONEBOX * 7.5) + Dir_Vector(i * ONEBOX, 0));
			Kills[i]->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, changeStr[i] - '0');
		}
	}
	while (afterSize < basicSize) {
		basicSize--;
		Kills_Actor.back()->SetState(State::Dead);
		Kills_Actor.pop_back();
		Kills.pop_back();
	}
	//Time
	int min, sec, mil;
	min = static_cast<int>(stageSumTime) / 60;
	sec = static_cast<int>(stageSumTime) % 60;
	mil = static_cast<int>(stageSumTime * 100) % 100;
	if (min > 99) {
		min = 99;
	}
	StageTime[0]->ChangeTexture(min / 10);
	StageTime[1]->ChangeTexture(min % 10);
	StageTime[2]->ChangeTexture(sec / 10);
	StageTime[3]->ChangeTexture(sec % 10);
	StageTime[4]->ChangeTexture(mil / 10);
	StageTime[5]->ChangeTexture(mil % 10);

	//Score
	basicSize = Score.size();
	changeStr = to_string(score);
	afterSize = changeStr.length();
	for (int i = 0; i < afterSize; i++) {
		if (i >= basicSize) {
			Display = new Actor(GetGame());
			Score_Actor.emplace_back(Display);
			Score.emplace_back(SetChangeInfo(Display, changeStr[i] - '0', i * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * afterSize / 2, ONEBOX * 13.5)));
		}
		else {
			Score_Actor[i]->SetPosition(Dir_Vector(Game::getWidth() / 2 - ONEBOX * afterSize / 2, ONEBOX * 13.5) + Dir_Vector(i * ONEBOX, 0));
			Score[i]->ChangeTexture(changeStr[i] - '0');
		}
	}
	while (afterSize < basicSize) {
		basicSize--;
		Score_Actor.back()->SetState(State::Dead);
		Score_Actor.pop_back();
		Score.pop_back();
	}
}


SpriteComponent* Result::SetChangeInfo(Actor* Display, int place, double gap, Dir_Vector first) {
	Display->SetScale(0.12);
	Display->SetPosition(first);
	SpriteComponent* nDisp = new SpriteComponent(Display, GetGame()->GetShader(), 1180);
	Display->SetPosition(Display->GetPosition() + Dir_Vector(gap, 0));
	nDisp->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, place);
	return nDisp;
}

void Result::SetResultInfo(Actor* Display, int place, double gap, Dir_Vector first) {
	Display = new Actor(GetGame());
	Display->SetScale(0.12);
	Display->SetPosition(first);
	SpriteComponent* nDisp = new SpriteComponent(Display, GetGame()->GetShader(), 1180);
	Display->SetPosition(Display->GetPosition() + Dir_Vector(gap, 0));
	nDisp->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, place);
}