#pragma once
#include "Actor.h"
#include "AnimSpriteComponent.h"
#include "Entity.h"
#include "Game.h"

//é©ï™ÇÃå¸Ç´Ç…ëŒÇ∑ÇÈäpìxè„å¿/â∫å¿
#define MAXROTATE_N 180.0
#define MINROTATE_N 80.0
#define MAXROTATE_R 100.0
#define MINROTATE_R 0.0
//BulletInformation
#define BULLETSPEED 1200
#define HALFBULLET Dir_Vector(15,15)
#define BASIC_BULLETDAMAGE 5
#define BASIC_NUM_OF_BULLET 15
#define RELOAD_TIME 2.0
#define ANGLE_CORRECTOR 90
#define PLAYER_BASIC_SHOT_INTERVAL 0.5
#define ENEMY_BASIC_INTERVAL_CORRECTOR 1.0
#define DECREASE_INTERVAL_PAR_STAGE 0.98
#define INCREASE_DAMAGE_PAR_STAGE 0.3
#define INCREASE_DAMAGE_PAR_LEVEL 0.5
#define DECREASE_RELOAD_PAR_LEVEL 0.95
#define DECREASE_INTERVAL_PAR_LEVEL 0.97

class Gun :public Actor {
public:
	Gun(Game* game, class Entity* entity);
	void UpdateActor(double delta);
	void ReverseWorld();
	void ResetWorld();
	int GetAmmo() { return leftingammo; }
	int GetMaxAmmo() { return maxAmmo; }
	double GetDamage() { return shotDamage; }
	double GetShotInterval() { return shotinterval; }
	void changeDirection(double rotate);
	void shot(double delta);
	void RotateGun(double rotate);
	void Reload(double delta);
	bool isReloading() { return reloading; }
private:
	class Entity* mEntity;
	int entityType;
	bool revFlag;
	bool fFlag;
	double shotinterval;
	double intervalTime;
	int leftingammo;
	int maxAmmo;
	bool reloading;
	double reloadTime;
	double reloadCompleteTime;
	double shotDamage;
	AnimSpriteComponent* mAnim;
	Actor* mDest;
	AnimSpriteComponent* cDest;
	Dir_Vector sholder;
	void ReflectUpgrade();
};

class Bullet : public Actor {
public:
	Bullet(Game* game, class Entity* entity, class Gun* gun, int entityType);
	void UpdateActor(double delta);
	void checkHit(Dir_Vector& before, Dir_Vector current);
private:
	static int bnum;
	Dir_Vector AbsPos;
	class Entity* mEntity;
	class Gun* mGun;
	int entityType;
	bool isHit;
	double hittimer;
	double shotDamage;
	class AnimSpriteComponent* mAnim;
};