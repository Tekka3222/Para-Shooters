#include "Entity.h"

//敵味方共通の動きなどを定義する親クラス
Entity::Entity(Game* game)
	:Actor(game)
	, isReverse(false)
	, ePos(ONEBOX * 1.0, ONEBOX * 8.0)
	, health(MAX_HEALTH)
	, entityType(TYPE_NONE)
	, goRevFlag(false)
	, revCool(0.0)
	, revLength(0.0)
	, currentRevLength(0.0)
	, beforeRevPosY(0.0)
	, whileRevFlag(false)
	, isFalling(false)
	, mRightSpeed(0.0)
	, mRightAcc(0.0)
	, mFallSpeed(0.0)
	, mFallAcc(FALL_ACC)
	, isMoving(false)
	, isActive(false)
	, shotflag(false)
	, reloadflag(false)
	, mRotSpeed(0.0)
	, kill(0)
	, sizeX(ONEBOX)
	, sizeY(ONEBOX * 2.0)
	, shield(BASIC_SHIELD)
	, numOfReverse(0)
	, numOfDamage(0)
{
	mAnim = NULL;
	mGun = NULL;
	for (int i = 0; i < NUM_OF_PARTICLES; i++) {
		Particles *particle = new Particles(game, this, i);
		mParticles.emplace_back(particle);
	}
}

void Entity::UpdateActor(double delta) {
	Actor::UpdateActor(delta);
	if (health <= 0) {
		return;
	}
	if (whileRevFlag) {
		double speed = (entityType == TYPE_PLAYER) ? PLAYER_SPEED_REVERSE : ENEMY_SPEED_REVERSE;
		//方向によってspeedを反転する
		speed *= (revLength > 0) ? 1 : -1;
		currentRevLength += speed * delta;
		if (abs(revLength) <= abs(currentRevLength)) {
			Dir_Vector pos = ePos;
			pos.y = beforeRevPosY + revLength;
			ePos = pos;
			Dir_Vector cPos = Camera::GetCameraPosition();
			Dir_Vector relativePos(pos.x - cPos.x, pos.y - cPos.y);
			SetPosition(relativePos);
			SetCharacterTex();
			revLength = 0;
			mFallAcc *= -1;
			isReverse = !isReverse;
			whileRevFlag = false;
			mParticles[PARTICLE_TYPE_REVERSE]->Activate();
		}
		else {
			Dir_Vector pos = ePos;
			pos.y = beforeRevPosY + currentRevLength;
			ePos = pos;
			Dir_Vector cPos = Camera::GetCameraPosition();
			Dir_Vector relativePos(pos.x - cPos.x, pos.y - cPos.y);
			SetPosition(relativePos);
			SetCharacterTexRev();
		}
		return;
	}
	Dir_Vector pos = ePos;
	double maxS = MAX_SPEED;
	if (entityType != 0) {
		maxS /= 2;
	}
	if (abs(mRightSpeed + mRightAcc * delta) > maxS)
		mRightSpeed = (mRightAcc > 0) ? maxS : -maxS;
	else if (mRightAcc == 0) {
		if (abs(mRightSpeed) <= maxS * delta) {
			mRightSpeed = 0;
		}
		else {
			mRightSpeed -= (mRightSpeed > 0) ? DEC_SPEED * delta : -DEC_SPEED * delta;
		}
	}
	else {
		mRightSpeed += mRightAcc * delta;
	}
	if (mRightAcc < 0) {
		SetFront(Dir_Vector::Back);
	}
	else if (mRightAcc > 0) {
		SetFront(Dir_Vector::Front);
	}
	if (abs(mRightSpeed) > 0) {
		isMoving = true;
	}
	else {
		isMoving = false;
	}
	SetCharacterTex();
	if (goRevFlag && revCool == 0.0) {
		goRevFlag = false;
		if (isFalling) {
			return;
		}
		else {
			ReverseWorld();
			return;
		}
	}
	if (revCool > 0) {
		if (revCool - delta < 0.0) {
			revCool = 0.0;
			mParticles[PARTICLE_TYPE_REVERSE]->Disable();
		}
		else {
			revCool -= delta;
		}
		if (revCool <= PARTICLE_LIFETIME) {
			mParticles[PARTICLE_TYPE_REVERSE]->Disable();
		}
	}
	double moved = pos.x;
	if (!isReverse) {
		if (Camera::GetMap(static_cast<int>(floor(moved / ONEBOX)), static_cast<int>(floor((pos.y + sizeY - 1) / ONEBOX + 1))) == BLOCK_AIR &&
			Camera::GetMap(static_cast<int>(floor((moved + sizeX - 1) / ONEBOX)), static_cast<int>(floor((pos.y + sizeY - 1) / ONEBOX + 1))) == BLOCK_AIR) {
			isFalling = true;
		}
	}
	else {
		if (Camera::GetMap(static_cast<int>(floor((pos.x + mRightSpeed * delta) / ONEBOX)), static_cast<int>(floor(pos.y / ONEBOX - 1))) == BLOCK_AIR &&
			Camera::GetMap(static_cast<int>(floor((pos.x + mRightSpeed * delta + ONEBOX - 1) / ONEBOX)), static_cast<int>(floor(pos.y / ONEBOX - 1))) == BLOCK_AIR) {
			isFalling = true;
		}
	}

	if (isFalling) {
		if (abs(mFallSpeed + mFallAcc * delta) > MAX_FALL) {
			mFallSpeed = (mFallAcc > 0) ? MAX_FALL : -MAX_FALL;
		}
		else {
			mFallSpeed += mFallAcc * delta;
		}
	}
	else {
		mFallSpeed = 0;
	}
	double before = pos.x;
	if (static_cast<int>(floor(moved / ONEBOX + 1)) <= 0) {
		mRightSpeed = 0;
		pos.x = 0;
	}
	else if (static_cast<int>(floor((moved + sizeX - 1) / ONEBOX)) >= SWIDTH) {
		mRightSpeed = 0;
		pos.x = (double)(SWIDTH - 1) * ONEBOX;
	}
	else {
		double before = pos.x;
		pos.x += mRightSpeed * delta;
		int pBottomLeft = Camera::GetMap(static_cast<int>(floor(pos.x / ONEBOX)), static_cast<int>(floor((pos.y + sizeY - 1) / ONEBOX)));
		int pBottomRight = Camera::GetMap(static_cast<int>(floor((pos.x + sizeX - 1) / ONEBOX)), static_cast<int>(floor((pos.y + sizeY - 1) / ONEBOX)));
		int pTopLeft = Camera::GetMap(static_cast<int>(floor(pos.x / ONEBOX)), static_cast<int>(floor(pos.y / ONEBOX)));
		int pTopRight = Camera::GetMap(static_cast<int>(floor((pos.x + sizeX - 1) / ONEBOX)), static_cast<int>(floor(pos.y / ONEBOX)));
		if (!isReverse) {
			if (pBottomLeft == BLOCK_BOUND || pBottomLeft == BLOCK_WALL || pBottomRight == BLOCK_BOUND || pBottomRight == BLOCK_WALL){
				mRightSpeed = 0;
				pos.x = before;
			}
		}
		else {
			if (pTopLeft == BLOCK_BOUND || pTopLeft == BLOCK_WALL || pTopRight == BLOCK_BOUND || pTopRight == BLOCK_WALL) {
				mRightSpeed = 0;
				pos.x = before;
			}
		}
	}
	if (isFalling) {
		pos.y += mFallSpeed * delta;
		int pBottomLeft = Camera::GetMap(static_cast<int>(floor(pos.x / ONEBOX)), static_cast<int>(floor((pos.y + sizeY - 1) / ONEBOX)));
		int pBottomRight = Camera::GetMap(static_cast<int>(floor((pos.x + sizeX - 1) / ONEBOX)), static_cast<int>(floor((pos.y + sizeY - 1) / ONEBOX)));
		int pTopLeft = Camera::GetMap(static_cast<int>(floor(pos.x / ONEBOX)), static_cast<int>(floor(pos.y / ONEBOX)));
		int pTopRight = Camera::GetMap(static_cast<int>(floor((pos.x + sizeX - 1) / ONEBOX)), static_cast<int>(floor(pos.y / ONEBOX)));
		if (!isReverse) {
			if (pBottomLeft == BLOCK_BOUND || pBottomLeft == BLOCK_WALL || pBottomRight == BLOCK_BOUND || pBottomRight == BLOCK_WALL) {
				isFalling = false;
				pos.y = static_cast<int>(floor((pos.y + sizeY - 1) / ONEBOX - 2)) * ONEBOX;
			}
		}
		else {
			if (pTopLeft == BLOCK_BOUND || pTopLeft == BLOCK_WALL || pTopRight == BLOCK_BOUND || pTopRight == BLOCK_WALL) {
				isFalling = false;
				pos.y = static_cast<int>(floor(pos.y / ONEBOX + 1)) * ONEBOX;
			}
		}
	}
	ePos = pos;
	Dir_Vector cPos = Camera::GetCameraPosition();
	Dir_Vector relativePos(pos.x - cPos.x, pos.y - cPos.y);
	SetPosition(relativePos);
	mGun->changeDirection(mRotSpeed * delta);
	if (reloadflag || mGun->isReloading()) {
		reloadflag = false;
		mGun->Reload(delta);
	}
	if (shotflag) {
		mGun->shot(delta);
	}
}

//現在位置がBorderの時の挙動を定義
void Entity::ReverseWorld() {
	Dir_Vector pos = ePos;
	int depth = Camera::GetDepth(static_cast<int>(floor(pos.x / ONEBOX)));
	int altDepth = Camera::GetDepth(static_cast<int>(floor((pos.x + sizeX - 1) / ONEBOX)));
	pos.x = static_cast<int>(floor(ePos.x / ONEBOX)) * ONEBOX;
	if (depth < altDepth) {
		depth = altDepth;
		pos.x = static_cast<int>(floor((ePos.x + sizeX - 1) / ONEBOX)) * ONEBOX;
		ePos = pos;
		Dir_Vector cPos = Camera::GetCameraPosition();
		Dir_Vector relativePos(pos.x - cPos.x, pos.y - cPos.y);
		SetPosition(relativePos);
	}
	else if(depth != altDepth){
		ePos = pos;
		Dir_Vector cPos = Camera::GetCameraPosition();
		Dir_Vector relativePos(pos.x - cPos.x, pos.y - cPos.y);
		SetPosition(relativePos);
	}
	
	if (!isReverse) {
		revLength += depth * ONEBOX + sizeY;
	}
	else {
		revLength -= depth * ONEBOX + sizeY;
	}
	whileRevFlag = true;
	//反転クールダウンの設定(深さを定数倍したものの平方根に、基本料をかけたもの)
	revCool = sqrt(depth * DEPTH_ADJUSTER) * BASIC_REVERCE_COOLDOWN;
	mRightAcc = 0;
	mRightSpeed = 0;
	isMoving = false;
	currentRevLength = 0.0;
	beforeRevPosY = pos.y;
	numOfReverse++;
	SetCharacterTexRev();
}

void Entity::SetCharacterTexRev() {

}

void Entity::SetCharacterTex() {
}

//キャラクターにダメージを与える。
void Entity::Damage(double damage, double midX, double midY) { 
	health -= max(static_cast<int>(damage * shield), 1); 
	numOfDamage++; mParticles[PARTICLE_TYPE_DAMAGE]->CreateParticle(midX, midY); 
}