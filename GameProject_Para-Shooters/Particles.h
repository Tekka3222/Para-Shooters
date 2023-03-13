#pragma once
#include "Actor.h"
#include "AnimSpriteComponent.h"
#include "AnimInformation.h"
#include "Entity.h"
#include <utility>
#include <tuple>
#include <random>

//�p�[�e�B�N���̎��
#define PARTICLE_TYPE_DAMAGE 0
#define PARTICLE_TYPE_HEAL 1
#define PARTICLE_TYPE_RELOAD 2
#define PARTICLE_TYPE_REVERSE 3
//�p�[�e�B�N���̎�ސ�
#define NUM_OF_PARTICLES 4
//�p�[�e�B�N���̎�ނ��Ƃɂ����鎞�Ԍo�߂ō쐬�\�ȃp�[�e�B�N���ő吔�Ƃ��̃X�P�[��
#define PARTICLE_TYPE_DAMAGE_INFO pair<int,double>(0, 0.05)
#define PARTICLE_TYPE_HEAL_INFO pair<int,double>(10, 0.04)
#define PARTICLE_TYPE_RELOAD_INFO pair<int,double>(10, 0.04)
#define PARTICLE_TYPE_REVERSE_INFO pair<int,double>(10, 0.04)
//�p�[�e�B�N���T�C�Y�̔���(BASIC_TEX_SIZE * "�X�P�[��" / 2�ŋ��߂���)
#define PARTICLE_TYPE_DAMAGE_HALF (BASIC_TEX_SIZE * PARTICLE_TYPE_DAMAGE_INFO.second / 2)
#define PARTICLE_TYPE_HEAL_HALF (BASIC_TEX_SIZE * PARTICLE_TYPE_HEAL_INFO.second / 2)
#define PARTICLE_TYPE_RELOAD_HALF (BASIC_TEX_SIZE * PARTICLE_TYPE_RELOAD_INFO.second / 2)
#define PARTICLE_TYPE_REVERSE_HALF (BASIC_TEX_SIZE * PARTICLE_TYPE_REVERSE_INFO.second / 2)
//�p�[�e�B�N���̐�������(�b)
#define PARTICLE_LIFETIME 0.3
//�p�[�e�B�N�������̃N�[���^�C��
#define PARTICLE_COOLTIME 0.1
//�p�[�e�B�N���̈ړ��X�s�[�h
#define PARTICLE_SPEED_X 0
#define PARTICLE_SPEED_Y (-ONEBOX)
//�p�[�e�B�N�����Ǘ�����parts�̊Ǘ�
#define PARTS_LIFETIME 0
#define PARTS_MOVEDIRECTION 1
#define PARTS_VECTOR 2
#define PARTS_ACTOR 3
//�p�[�e�B�N�����Ǘ�
class Particles :public Actor {
public:
	Particles(Game* game, class Entity* entity, int type);
	~Particles();
	void UpdateActor(double delta) override;
	void CreateParticle(double midX, double midY);
	void Activate() { isActivated = true; }
	void Disable() { isActivated = false; }
	//0-100�܂ł̗���������(�p�[�e�B�N���𐶐��������G���e�B�e�B�̃T�C�Y�ɑ΂���100���䗦�ňʒu�����肷�邽��)
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
	//�p�[�e�B�N���ƁA(��������&�ړ�����&Entity�̊�ʒu�ɑ΂���ω��x�N�g��)�̑g
	vector<tuple<double, int, class Dir_Vector, class Actor*>> parts;
	
};