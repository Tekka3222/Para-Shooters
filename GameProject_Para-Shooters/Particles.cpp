#include "Particles.h"

Particles::Particles(Game* game, class Entity* entity, int type)
	:Actor(game)
	,mEntity(entity)
	,isActivated(false)
	,numOfParticle(0)
	,particleType(type)
	,particleCool(0.0)
	,particleDirection(1)
{
	//空のベクター
	parts = vector<tuple<double, int, class Dir_Vector, class Actor*>>();
	std::string ImURL = "images/";
	GLuint texID;
	glGenTextures(1, &texID);
	GetGame()->SetupTexture(&texID, ImURL + "litsmap.raw", TEXWIDTH, TEXHEIGHT);
	litsID = texID;
	glGenTextures(1, &texID);
	random_device rnd;
	mt.seed(rnd());
	uniform_int_distribution<> rand(0, 100);
	rand100 = rand;
	switch (type) {
	case PARTICLE_TYPE_DAMAGE:
		maxNumOfParticle = PARTICLE_TYPE_DAMAGE_INFO.first;
		SetScale(PARTICLE_TYPE_DAMAGE_INFO.second);
		halfSize = PARTICLE_TYPE_DAMAGE_HALF;
		particleTexNum = PARTICLE_DAMAGE;
		particleTexNumRev = PARTICLE_DAMAGE_REVERSE;
		break;
	case PARTICLE_TYPE_HEAL:
		maxNumOfParticle = PARTICLE_TYPE_HEAL_INFO.first;
		SetScale(PARTICLE_TYPE_HEAL_INFO.second);
		halfSize = PARTICLE_TYPE_HEAL_HALF;
		particleTexNum = PARTICLE_HEAL;
		particleTexNumRev = PARTICLE_HEAL_REVERSE;
		break;
	case PARTICLE_TYPE_RELOAD:
		maxNumOfParticle = PARTICLE_TYPE_RELOAD_INFO.first;
		SetScale(PARTICLE_TYPE_RELOAD_INFO.second);
		halfSize = PARTICLE_TYPE_RELOAD_HALF;
		particleTexNum = PARTICLE_RELOAD;
		particleTexNumRev = PARTICLE_RELOAD_REVERSE;
		break;
	case PARTICLE_TYPE_REVERSE:
		maxNumOfParticle = PARTICLE_TYPE_REVERSE_INFO.first;
		SetScale(PARTICLE_TYPE_REVERSE_INFO.second);
		halfSize = PARTICLE_TYPE_REVERSE_HALF;
		particleTexNum = PARTICLE_REVERSE;
		particleTexNumRev = PARTICLE_REVERSE_REVERSE;
		break;
	default:
		maxNumOfParticle = 0;
		halfSize = 0.0;
		particleTexNum = 0;
		particleTexNumRev = 0;
	}
}
//デストラクタ(partsの中身を全て廃棄する)
Particles::~Particles() {
	while (parts.size() > 0) {
		get<PARTS_ACTOR>(parts.back())->SetState(State::Dead);
		parts.pop_back();
	}
}

//ダメージ以外のパーティクルは、最大数分だけランダムな座標とタイミングで生成。(ダメージは、ダメージを受けた時に、弾と同じ座標に出現させる。)
void Particles::UpdateActor(double delta) {
	if (isActivated && numOfParticle < maxNumOfParticle && particleCool == 0) {
		Dir_Vector ePos = mEntity->GetAbsPPos();
		Dir_Vector cPos = Camera::GetCameraPosition();
		Dir_Vector relativePos = ePos - cPos;
		CreateParticle(relativePos.x + mEntity->getSize().x * (double)rand100(mt) / 100,relativePos.y + mEntity->getSize().y * (double)rand100(mt) / 100);

		particleCool += PARTICLE_COOLTIME;
		return;
	}
	if (parts.size() > 0) {
		auto iter = parts.begin();
		while (iter != parts.end()) {
			bool delFlag = false;
			if (get<PARTS_LIFETIME>(*iter) <= delta) {
				get<PARTS_ACTOR>(*iter)->SetState(State::Dead);
				delFlag = true;
				iter = parts.erase(iter);
			}
			else {
				get<PARTS_LIFETIME>(*iter) -= delta;
				if (particleType != PARTICLE_TYPE_DAMAGE) {
					Dir_Vector particleMovement = Dir_Vector(PARTICLE_SPEED_X * delta * get<PARTS_MOVEDIRECTION>(*iter), PARTICLE_SPEED_Y * delta * get<PARTS_MOVEDIRECTION>(*iter));
					get<PARTS_VECTOR>(*iter) = get<PARTS_VECTOR>(*iter) + particleMovement;
					get<PARTS_ACTOR>(*iter)->SetPosition(get<PARTS_VECTOR>(*iter) + mEntity->GetPosition());
				}
					
			}
			if (!delFlag) {
				iter++;
			}
		}
	}
	if (particleCool > 0.0) {
		if (particleCool < delta) {
			particleCool = 0.0;
			return;
		}
		else {
			particleCool -= delta;
		}
	}
}
//パーティクルを座標を指定し生成(座標はパーティクルの中心)
void Particles::CreateParticle(double midX, double midY){
	bool rev = mEntity->GetRev();
	Actor* Display = new Actor(GetGame());
	Display->SetScale(GetScale());
	Display->SetPosition(Dir_Vector(midX - halfSize, midY - halfSize));
	SpriteComponent* nDisp = new SpriteComponent(Display, GetGame()->GetShader(), 1200);
	if (rev) {
		nDisp->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, particleTexNumRev);
	}
	else {
		nDisp->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, particleTexNum);
	}
	parts.emplace_back(tuple<double, int, Dir_Vector, Actor*>{PARTICLE_LIFETIME, (rev) ? -1 : 1, (Display->GetPosition() - mEntity->GetPosition()), Display});
}
