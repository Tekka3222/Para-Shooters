#pragma once
#include "Actor.h"
#include "AnimSpriteComponent.h"
#include "AnimInformation.h"
#include "Camera.h"
#include "Gun.h"
#include "Menu.h"
#include "Particles.h"
#include <random>

//キャラクターごとのType
#define TYPE_NONE -1
#define TYPE_PLAYER 0
#define TYPE_ENEMY 1
#define CHARACTER_TEX_TYPE_NOR 0
#define CHARACTER_TEX_TYPE_REV 1
//プレイヤーの挙動
#define PLAYERSIZE_X ONEBOX * 2;
#define PLAYERSIZE_Y ONEBOX
#define MAX_SPEED 500
#define MAX_FALL 900
#define DEC_SPEED 1000
#define FALL_ACC 800.0
#define PLAYER_ACC 1000.0
#define PLAYER_ACC_ROTATE 90
#define PLAYER_SPEED_REVERSE 1000
#define ENEMY_ACC_SLEEP 350.0
#define ENEMY_ACC_ACTIVE 700.0
#define ENEMY_ACC_ROTATE 67.5
#define ENEMY_SPEED_REVERSE 800
#define ANIMFPS_CORRECTOR 25

#define BASIC_REVERCE_COOLDOWN 0.3
#define DEPTH_ADJUSTER 5
//ワールドの情報(1マスのサイズ
#define ONEBOX 60.0
//体力に関する宣言
#define MAX_HEALTH 250
#define HEAL_HEALTH 50
#define ENEMY_HEALTH 15
#define MAX_NUM_OF_HEAL 10
#define BASIC_SHIELD 1.0
#define SHIELD_PAR_UPGRADE 0.934
#define SHIELD_PAR_STAGE 0.97
#define HEAL_INTERVAL 1.0
//敵のAIにおける確率(％)
#define PROB_REVERSE 70
#define PROB_REVERSE_SLEEP 15
#define PROB_MOVE_RIGHT_SLEEP 33
#define PROB_MOVE_LEFT_SLEEP 33
//敵の行動を受け付けるまでのインターバル
#define ENEMY_INTERVAL 0.5
#define ENEMY_INTERVAL_WHILE_SLEEP 1.0
//極小値
#define DELTA 0.001


//キャラクター共通のふるまいを定義
class Entity : public Actor {
public:
	Entity(Game* game);
	virtual void UpdateActor(double delta);
	Dir_Vector getSize() { return Dir_Vector(sizeX, sizeY); }
	void setSize(int sx, int sy) { sizeX = sx; sizeY = sy; }
	void setActive() { isActive = true; }
	void setNoActive() { isActive = false; }
	int GetEntityType() { return entityType; }
	class Gun* GetGun() { return mGun; }
	int GetHealth() { return health; }
	void Damage(double damage, double midX, double midY);
	void killEnemy() { kill++; }
	bool GetRev() { return isReverse; }
	int GetKill() { return kill; }
	int GetNoReverse() { return numOfReverse; }
	int getNoDamage() { return numOfDamage; }
	double GetRightSpeed() const { return mRightSpeed; }
	double GetFallSpeed() const { return mFallSpeed; }
	Dir_Vector GetAbsPPos() const { return ePos; }
	virtual void ReverseWorld();
	bool whileReverse() { return whileRevFlag; }
	class Particles* GetParticle(int type) { return mParticles[type]; }
protected:
	virtual void SetCharacterTex();
	virtual void SetCharacterTexRev();
	class AnimSpriteComponent* mAnim;
	class Gun* mGun;
	int health;
	int entityType;
	bool isReverse;
	Dir_Vector ePos;
	double mRightSpeed;
	double mFallSpeed;
	double mRightAcc;
	double mFallAcc;
	bool isMoving;
	bool isFalling;
	bool shotflag;
	bool reloadflag;
	bool isActive;
	double shield;
	double mRotSpeed;
	double sizeX, sizeY;
	int kill;
	int numOfReverse, numOfDamage;
	bool goRevFlag;
	bool whileRevFlag;
	double revLength, currentRevLength, revCool;
	double beforeRevPosY;
	vector<class Particles*> mParticles;
};

//プレイヤーの挙動を管理
class Player : public Entity {
public:
	Player(class Game* game);
	void UpdateActor(double delta) override;
	class Camera* GetCamera() const { return mCam; }
	void ReverseWorld() override;
	void ResetWorld();
	bool checkGoal();
	void addScore(int score) { playerScore += score; }
	int getScore() const { return playerScore; }
	void setScore(int score) { playerScore = score; }
	void setResultFlag(bool flag) { resultFlag = flag; }
	bool getResultFlag() { return resultFlag; }
	void keyInput(GLFWwindow* mWindow, int key, int scancode, int action, int mods);
private:
	void SetCharacterTex() override;
	void SetCharacterTexRev() override;
	void ReflectUpgrade();
	class Camera* mCam;
	bool checkG;
	bool finFlag;
	bool resultFlag;
	int numOfHeal;
	int playerScore;
	double checkGoalInterval;
	double healInterval;
};

//敵の挙動を管理
class Enemy : public Entity {
public:
	Enemy(class Game* game, class Player* target, Dir_Vector pos);
	void UpdateActor(double delta) override;
	void ReverseWorld() override;
	void Move(double delta);
private:
	void SetCharacterTex() override;
	void SetCharacterTexRev() override;
	std::mt19937 mt;
	std::uniform_int_distribution<> rand100;
	Player* mTarget;
	bool isSleep;
	double changeInterval;
	double uniqueDistance;
};