#pragma once
#include "Actor.h"
#include "AnimSpriteComponent.h"
#include "Entity.h"
#include "Game.h"

//�����̌����ɑ΂���p�x���/����
#define MAXROTATE_N 180.0
#define MINROTATE_N 80.0
#define MAXROTATE_R 100.0
#define MINROTATE_R 0.0
//�e�̐���
#define BULLETSPEED 1200
#define BASIC_BULLETDAMAGE 5
#define BASIC_NUM_OF_BULLET 15
#define RELOAD_TIME 2.0
#define ANGLE_CORRECTOR 90
//�e��A���e�B�N���𒆐S�ɍ��킹�邽�߂̒�����
#define BULLET_SIZE 0.05
#define BULLET_SPAWN_ADJUSTER (ONEBOX * 3 / 4)
#define MID_ADJUSTER Dir_Vector(BASIC_TEX_SIZE * BULLET_SIZE / 2, BASIC_TEX_SIZE * BULLET_SIZE / 2)
#define RETICLE_SIZE 0.12
#define RETICLE_REVERSE_SIZE 0.06
#define RETICLE_MID_ADJUSTER Dir_Vector(BASIC_TEX_SIZE * RETICLE_SIZE / 2,BASIC_TEX_SIZE * RETICLE_SIZE / 2)
#define RETICLE_REVERSE_MID_ADJUSTER Dir_Vector(BASIC_TEX_SIZE * RETICLE_REVERSE_SIZE / 2,BASIC_TEX_SIZE * RETICLE_REVERSE_SIZE / 2)
#define RETICLE_SIZE_DIFFERENT_ADJUSTER (RETICLE_MID_ADJUSTER - RETICLE_REVERSE_MID_ADJUSTER)
//���]�ɉ������e�̕␳�l
#define GUN_ADJUSTER_FRONT_X 0 
#define GUN_ADJUSTER_BACK_X (ONEBOX / 2)
#define GUN_ADJUSTER_REVERSE_PLAYER_Y (ONEBOX * 5 / 8)
#define GUN_ADJUSTER_NORMAL_PLAYER_Y (ONEBOX / 3)
#define GUN_ADJUSTER_REVERSE_ENEMY_Y (ONEBOX / 2)
#define GUN_ADJUSTER_NORMAL_ENEMY_Y (ONEBOX * 7 / 16)
//�e�Ɋւ���A�b�v�O���[�h
#define PLAYER_BASIC_SHOT_INTERVAL 0.5
#define ENEMY_BASIC_INTERVAL_CORRECTOR 1.0
#define DECREASE_INTERVAL_PAR_STAGE 0.98
#define INCREASE_DAMAGE_PAR_STAGE 0.3
#define INCREASE_DAMAGE_PAR_LEVEL 0.5
#define DECREASE_RELOAD_PAR_LEVEL 0.95
#define DECREASE_INTERVAL_PAR_LEVEL 0.97
//�v���C���[��G�̎��e�̏��
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
//�e���甭�˂����e�̏��
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