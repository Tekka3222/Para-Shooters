#include "Entity.h"

Enemy::Enemy(Game* game, Player* target, Dir_Vector pos)
	: Entity(game)
	, mTarget(target)
	, changeInterval(0.0)
	, uniqueDistance(0.0)
	, isSleep(false)
{
		entityType = TYPE_ENEMY;
		health = ENEMY_HEALTH;
		shield = 1.0 * pow(SHIELD_PAR_STAGE, (Camera::curStage - 1.0));
		ePos = pos;
		mAnim = new AnimSpriteComponent(this, game->GetShader(), 140);
		mGun = new Gun(game, this);
		GLuint texID;
		std::string ImURL = "images/";
		int texwidth = BASIC_TEX_SIZE, texheight = BASIC_TEX_SIZE, sizex = TEX_SMALL, sizey = TEX_BIG, start = ENEMY_ACTIVE_STAND_RIGHT.first ,end = ENEMY_ACTIVE_STAND_RIGHT.second;
		glGenTextures(1, &texID);
		game->SetupTexture(&texID, ImURL + "character.raw", texwidth, texheight);
		bool cont = true;
		mAnim->SetAnimTextures(texID, texwidth, texheight, sizex, sizey, start, end, cont, 20);
		//Set random value to determine enemy's move
		random_device rnd;
		mt.seed(rnd());
		uniform_int_distribution<> rand(0, 100 - 1);
		rand100 = rand;
		uniqueDistance = ((double)(rand100(mt)) / 20) + 5;
}

void Enemy::UpdateActor(double delta) {
	Move(delta);
	Entity::UpdateActor(delta);
}

void Enemy::ReverseWorld() {
	whileRevFlag = true;
	Entity::ReverseWorld();
	mGun->ReverseWorld();
	whileRevFlag = false;
}

//プレイヤーとの水平距離distと100までの乱数によって行動を管理
void Enemy::Move(double delta) {
	if (!isActive) {
		return;
	}
	int rvalue = rand100(mt);
	Dir_Vector tPos = mTarget->GetAbsPPos();
	Dir_Vector ePos = GetAbsPPos();
	double dist = abs(tPos.x - ePos.x);
	if (dist > 11 * ONEBOX) {
		isSleep = true;
	}
	else {
		isSleep = false;
	}
	mRotSpeed = 0.0;
	if (isSleep) {
		if (changeInterval == 0.0) {
			changeInterval = 2.0;
			mRightAcc = 0.0;
			if (rvalue >= 66) {
				mRightAcc += ENEMY_ACC_SLEEP;
			}
			else if(rvalue >= 33){
				mRightAcc -= ENEMY_ACC_SLEEP;
			}
			if (rvalue <= 10) {
				revFlag = true;
			}
		}
	}
	else if(dist > uniqueDistance * ONEBOX){
		mRightAcc = 0.0;
		if (tPos.x < ePos.x) {
			mRightAcc -= ENEMY_ACC_ACTIVE;
		}
		else{
			mRightAcc += ENEMY_ACC_ACTIVE;
		}
	}
	else {
		mRightAcc = 0.0;
		if (tPos.x < ePos.x && GetFront() == Dir_Vector::Front) {
			mRightAcc -= ENEMY_ACC_ACTIVE;
		}
		else if (tPos.x >= ePos.x && GetFront() == Dir_Vector::Back){
			mRightAcc += ENEMY_ACC_ACTIVE;
		}
	}
	//Aim to the center of target
	tPos = tPos + Dir_Vector(ONEBOX / 2, 0);
	Dir_Vector eFront = GetGun()->GetFront();
	double difY = tPos.y - (eFront.y * ((tPos.x - ePos.x) / eFront.x) + ePos.y);
	if (!isSleep) {
		if (difY > 0) {
			mRotSpeed += ENEMY_ACC_ROTATE;
		}
		else {
			mRotSpeed -= ENEMY_ACC_ROTATE;
		}
		if (abs(difY) < ONEBOX && mGun->GetShotInterval() == 0.0) {
			shotflag = true;
		}
		else {
			shotflag = false;
		}
	}
	if (changeInterval == 0.0 && !isSleep) {
		changeInterval = 0.5;
		if (rvalue <= 33 && ((!GetRev() && difY > 2 * ONEBOX) || (GetRev() && difY < -2 * ONEBOX))) {
			revFlag = true;
		}
		if (mGun->GetAmmo() == 0) {
			reloadflag = true;
		}
	}
	if (changeInterval > 0) {
		if (changeInterval - delta < 0) {
			changeInterval = 0.0;
		}
		else {
			changeInterval -= delta;
		}
	}
}

void Enemy::SetCharacterTex() {
	if (isSleep) {
		if (!isReverse) {
			if (GetFront() == Dir_Vector::Front)
				mAnim->SetAnimRange(ENEMY_SLEEP_STAND_RIGHT.first, ENEMY_SLEEP_STAND_RIGHT.second);
			else
				mAnim->SetAnimRange(ENEMY_SLEEP_STAND_LEFT.first, ENEMY_SLEEP_STAND_LEFT.second);
		}
		else {
			if (GetFront() == Dir_Vector::Front)
				mAnim->SetAnimRange(ENEMY_SLEEP_STAND_RIGHT_REV.first, ENEMY_SLEEP_STAND_RIGHT_REV.second);
			else
				mAnim->SetAnimRange(ENEMY_SLEEP_STAND_LEFT_REV.first, ENEMY_SLEEP_STAND_LEFT_REV.second);
		}
	}
	else if (!isReverse) {
		if (GetFront() == Dir_Vector::Front) {
			if (isMoving && !isFalling)
				mAnim->SetAnimRange(ENEMY_ACTIVE_MOVE_RIGHT.first, ENEMY_ACTIVE_MOVE_RIGHT.second);
			else
				mAnim->SetAnimRange(ENEMY_ACTIVE_STAND_RIGHT.first, ENEMY_ACTIVE_STAND_RIGHT.second);
		}
		else {
			if (isMoving && !isFalling)
				mAnim->SetAnimRange(ENEMY_ACTIVE_MOVE_LEFT.first, ENEMY_ACTIVE_MOVE_LEFT.second);
			else
				mAnim->SetAnimRange(ENEMY_ACTIVE_STAND_LEFT.first, ENEMY_ACTIVE_STAND_LEFT.second);
		}
	}
	else {
		if (GetFront() == Dir_Vector::Front) {
			if (isMoving && !isFalling)
				mAnim->SetAnimRange(ENEMY_ACTIVE_MOVE_RIGHT_REV.first, ENEMY_ACTIVE_MOVE_RIGHT_REV.second);
			else
				mAnim->SetAnimRange(ENEMY_ACTIVE_STAND_RIGHT_REV.first, ENEMY_ACTIVE_STAND_RIGHT_REV.second);
		}
		else {
			if (isMoving && !isFalling)
				mAnim->SetAnimRange(ENEMY_ACTIVE_MOVE_LEFT_REV.first, ENEMY_ACTIVE_MOVE_LEFT_REV.second);
			else
				mAnim->SetAnimRange(ENEMY_ACTIVE_STAND_LEFT_REV.first, ENEMY_ACTIVE_STAND_LEFT_REV.second);
		}
	}
	if (isMoving && !isFalling) {
		mAnim->SetAnimFPS(abs(mRightSpeed) / ANIMFPS_CORRECTOR);
	}
}