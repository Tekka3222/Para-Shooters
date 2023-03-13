#pragma once
#include "Actor.h"
#include "AnimSpriteComponent.h"
#include "AnimInformation.h"
#include "Entity.h"
#include <utility>
#include <tuple>
#include <random>

//パーティクルの種類
#define PARTICLE_TYPE_DAMAGE 0
#define PARTICLE_TYPE_HEAL 1
#define PARTICLE_TYPE_RELOAD 2
#define PARTICLE_TYPE_REVERSE 3
//パーティクルの種類数
#define NUM_OF_PARTICLES 4
//パーティクルの種類ごとにおける時間経過で作成可能なパーティクル最大数とそのスケール
#define PARTICLE_TYPE_DAMAGE_INFO pair<int,double>(0, 0.05)
#define PARTICLE_TYPE_HEAL_INFO pair<int,double>(10, 0.04)
#define PARTICLE_TYPE_RELOAD_INFO pair<int,double>(10, 0.04)
#define PARTICLE_TYPE_REVERSE_INFO pair<int,double>(10, 0.04)
//パーティクルサイズの半分(BASIC_TEX_SIZE * "スケール" / 2で求められる)
#define PARTICLE_TYPE_DAMAGE_HALF (BASIC_TEX_SIZE * PARTICLE_TYPE_DAMAGE_INFO.second / 2)
#define PARTICLE_TYPE_HEAL_HALF (BASIC_TEX_SIZE * PARTICLE_TYPE_HEAL_INFO.second / 2)
#define PARTICLE_TYPE_RELOAD_HALF (BASIC_TEX_SIZE * PARTICLE_TYPE_RELOAD_INFO.second / 2)
#define PARTICLE_TYPE_REVERSE_HALF (BASIC_TEX_SIZE * PARTICLE_TYPE_REVERSE_INFO.second / 2)
//パーティクルの生存時間(秒)
#define PARTICLE_LIFETIME 0.3
//パーティクル生成のクールタイム
#define PARTICLE_COOLTIME 0.1
//パーティクルの移動スピード
#define PARTICLE_SPEED_X 0
#define PARTICLE_SPEED_Y (-ONEBOX)
//パーティクルを管理するpartsの管理
#define PARTS_LIFETIME 0
#define PARTS_MOVEDIRECTION 1
#define PARTS_VECTOR 2
#define PARTS_ACTOR 3
//パーティクルを管理
class Particles :public Actor {
public:
	Particles(Game* game, class Entity* entity, int type);
	~Particles();
	void UpdateActor(double delta) override;
	void CreateParticle(double midX, double midY);
	void Activate() { isActivated = true; }
	void Disable() { isActivated = false; }
	//0-100までの乱数生成器(パーティクルを生成したいエンティティのサイズに対する100％比率で位置を決定するため)
	mt19937 mt;
	uniform_int_distribution<> rand100;
private:
	class Entity* mEntity;
	int litsID;
	bool isActivated;
	int numOfParticle;
	int maxNumOfParticle;
	int particleType;
	double particleCool;
	double halfSize;
	int particleTexNum, particleTexNumRev;
	int particleDirection;
	//パーティクルと、(生存時間&移動方向&Entityの基準位置に対する変化ベクトル)の組
	vector<tuple<double, int, class Dir_Vector, class Actor*>> parts;
	
};