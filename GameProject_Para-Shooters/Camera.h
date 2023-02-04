#pragma once
#include "Actor.h"
#include "AnimSpriteComponent.h"
#include "AnimInformation.h"
#include "Entity.h"
#include <random>
#include <map>

#define SWIDTH 100
#define SHEIGHT 20
#define MAXHEIGHT 30
#define TEXWINDOWWIDTH 34 // 34
#define TEXWINDOWHEIGHT 20 // 20
#define ONEBOX 60.0
#define TEXWIDTH 512
#define TEXHEIGHT 512
#define SIZEX 32
#define SIZEY 32
#define DEFAULTHEIGHT 10
#define MOVECAMERAX 0.3
#define MOVECAMERAY 0.2
#define BACKROOM 2
#define ONESCAF 6
#define RESULTSIZE_CORRECTOR 512
#define BASIC_SCORE 100
#define INCREASE_SCORE 200

//ÉuÉçÉbÉNÇÃèÓïÒ
#define BLOCK_AIR 0
#define BLOCK_BOUND 1
#define BLOCK_WALL 2
#define BLOCK_DOOR_GROUND 3
#define BLOCK_DOOR_REVERSE 4

using namespace std;

class Camera :public Actor {
public:
	Camera(Game* game,class Player* player);
	void UpdateActor(double delta);
	static int GetMap(int x, int y) { if (x < 0 || x >= SWIDTH || y < 0 || y >= SHEIGHT) return (y == GetBound(x / ONESCAF)) ? 1 : 0; else return Rmap[y][x]; }
	static int GetBound(int x) { if (x < 0) return Bound[0]; else if (x >= SWIDTH / ONESCAF) return Bound[(SWIDTH-1) / ONESCAF]; else return Bound[x]; }
	static Dir_Vector GetCameraPosition() { return CPos; }
	void UpdateTex();
	void UpdateAllPos(double x, double y);
	void ResetWorld();
	void ReverseWorld();
	void CreateGameOver();
	int GetStageScore();
	double GetStageSumTime() const { return stageSumTime; }
	int GetLapKill() const { return lapKill; }
	int GetLeft() { return EntityList[1].size(); }
	static vector<class Entity*> GetEntity(int playerType) { return EntityList[playerType]; }
	bool revFlag;
	static int curStage;
private:
	void SetRandomMap();
	void RemoveAllTexture();
	void RemoveAllEnemies();
	void StopAllEnemies();
	void CreateEnemies();
	void CreateOneEnemy(int placement, int rand);
	void ActivateEnemies();
	void SetAllTexture();
	void CreateDisplayInfo();
	void SetInfo(Actor* Display, int place, double gap, Dir_Vector first);
	SpriteComponent* SetMoveInfo(Actor* Display, int place, double gap, Dir_Vector first, double scale);
	void SetLastInfo(Actor* Display, int place, double gap, Dir_Vector first);
	AnimSpriteComponent* SetChangeInfo(Actor* Display, int place, double gap, Dir_Vector first, double scale);
	void SetDisplayInfo();
	void SetUpTexture(double x, double y, int TexY, int texID, int start, int end);
	void SetUpAnimation(int x, int y, int& start, int& end);
	void ResetTextureX(int changeNum, int align, double TexX, double TexY, bool isRight);
	void ResetTextureY(int changeNum, int align, double TexX, double TexY, bool isDown);
	class Player* mPlayer;
	double curTextureX;
	double curTextureY;
	GLuint mapID, litsID, upgradeID;
	static int MaxLife;
	static int curLife;
	int lapKill;
	double stageSumTime;
	long long sumScore;
	static Dir_Vector CPos;
	const int CAMERA_WIDTH = 1920;
	const int CAMERA_HEIGHT = 1080;
	const int STAGE_WIDTH = SWIDTH;
	const int STAGE_HEIGHT = SHEIGHT;
	//5-14 Ground 15-24 Under
//0-AIr,1-Bound,2-Wall,3-Door,4-Rev_Door
	static vector<vector<int>> Rmap;
	static vector<int> Bound;
	vector < vector<Actor*>> BGActors;
	Actor* SunOrMoon;
	Actor* BulletSlash;
	vector<AnimSpriteComponent*> Life, Stage, Time, Kill, Left;
	vector<SpriteComponent*> Bullet, Max_Bullet, Heal;
	SpriteComponent* Heal_Pic;
	vector<Actor*> Bullet_Actor, Max_Bullet_Actor, Heal_Actor;
	static vector<vector<class Entity*>> EntityList;
};