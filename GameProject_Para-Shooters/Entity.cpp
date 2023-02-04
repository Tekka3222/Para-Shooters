#include "Entity.h"

Entity::Entity(Game* game)
	:Actor(game)
	, isReverse(false)
	, ePos(ONEBOX * 1.0, ONEBOX * 8.0)
	, health(MAX_HEALTH)
	, entityType(TYPE_NONE)
	, rev_count(0.0)
	, revFlag(false)
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
}

void Entity::UpdateActor(double delta) {
	Actor::UpdateActor(delta);
	if (health <= 0) {
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
	if (revFlag) {
		revFlag = false;
		if (!isReverse) {
			if (Camera::GetMap(static_cast<int>(floor(pos.x / ONEBOX)), static_cast<int>(floor((pos.y + sizeY - 1)) / ONEBOX + 1)) != BLOCK_BOUND ||
				Camera::GetMap(static_cast<int>(floor((pos.x + sizeX - 1) / ONEBOX)), static_cast<int>(floor((pos.y + sizeY - 1) / ONEBOX + 1))) != BLOCK_BOUND) {
				return;
			}
		}
		else {
			if (Camera::GetMap(static_cast<int>(floor(pos.x / ONEBOX)), static_cast<int>(floor(pos.y / ONEBOX - 1))) != BLOCK_BOUND ||
				Camera::GetMap(static_cast<int>(floor((pos.x + sizeX - 1) / ONEBOX)), static_cast<int>(floor(pos.y / ONEBOX - 1))) != BLOCK_BOUND) {
				return;
			}
		}
		ReverseWorld();
		rev_count += 0.3;
		return;
	}
	if (rev_count > 0) {
		if (rev_count - delta < 0.0) {
			rev_count = 0.0;
		}
		else {
			rev_count -= delta;
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


void Entity::ReverseWorld() {
	Dir_Vector pos = ePos;
	if (!isReverse) {
		pos.y += BORDERSIZE * ONEBOX;
	}
	else {
		pos.y -= BORDERSIZE * ONEBOX;
	}

	ePos = pos;
	mFallAcc *= -1;
	mRightAcc = 0;
	mRightSpeed = 0;
	isMoving = false;
	isReverse = !isReverse;
	numOfReverse++;
	SetCharacterTex();
}

void Entity::SetCharacterTex() {
}
