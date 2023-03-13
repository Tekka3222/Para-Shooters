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
	if (isActive && !whileRevFlag) {
		Move(delta);
	}
	Entity::UpdateActor(delta);
}

void Enemy::ReverseWorld() {
	Entity::ReverseWorld();
	mGun->ReverseWorld();
}

//プレイヤーとの水平距離distと100までの乱数によって行動を管理
void Enemy::Move(double delta) {
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
	//索敵モード
	if (isSleep) {
		if (changeInterval == 0.0) {
			changeInterval = ENEMY_INTERVAL_WHILE_SLEEP;
			mRightAcc = 0.0;
			if (rvalue <= PROB_MOVE_RIGHT_SLEEP) {
				mRightAcc += ENEMY_ACC_SLEEP;
			}
			else if(rvalue <= PROB_MOVE_RIGHT_SLEEP + PROB_MOVE_LEFT_SLEEP){
				mRightAcc -= ENEMY_ACC_SLEEP;
			}
			if (rvalue <= PROB_REVERSE_SLEEP) {
				goRevFlag = true;
			}
		}
	}
	//発見モード
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
	Dir_Vector eFront = GetGun()->GetFront();
	double difY = 0;
	//銃口の向きが真上や、真下の場合eFront.yの値を用いてdifYを計算する(ただし、実際には存在しないy座標上の距離なので、x方向の距離で代用する。）
	if (abs(eFront.x) <= DELTA) {
		difY = -eFront.y * abs(tPos.x - ePos.x);
	}
	else {
		difY = tPos.y - (eFront.y * ((tPos.x - ePos.x) / eFront.x) + ePos.y);
	}
	if (!isSleep) {
		if (difY > 0) {
			mRotSpeed += ENEMY_ACC_ROTATE;
		}
		else {
			mRotSpeed -= ENEMY_ACC_ROTATE;
		}
		Dir_Vector cPos = Camera::GetCameraPosition();
		//カメラの中に敵が存在する場合で、かつプレイヤーに弾が当たる場合に弾を発射する。
		Dir_Vector relativePos = ePos - cPos;
		if (abs(difY) < ONEBOX / 2 && mGun->GetShotInterval() == 0.0 && (relativePos.x < Game::getWidth() && 0 < relativePos.x + sizeX && relativePos.y < Game::getHeight() && 0 < relativePos.y + sizeY)) {
			shotflag = true;
		}
		else {
			shotflag = false;
		}
	}
	if (changeInterval == 0.0 && !isSleep) {
		changeInterval = ENEMY_INTERVAL;
		if (rvalue <= PROB_REVERSE && ((!GetRev() && difY > 2 * ONEBOX) || (GetRev() && difY < -2 * ONEBOX))) {
			goRevFlag = true;
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
//敵キャラクターのテクスチャを設定。
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
//反転動作中の敵キャラクターのテクスチャを設定
void Enemy::SetCharacterTexRev() {
	if (!isReverse) {
		if (isSleep) {
			mAnim->SetAnimRange(ENEMY_REV_NOR_SLEEP.first, ENEMY_REV_NOR_SLEEP.second);
		}
		else {
			mAnim->SetAnimRange(ENEMY_REV_NOR.first, ENEMY_REV_NOR.second);
		}

	}
	else {
		if (isSleep) {
			mAnim->SetAnimRange(ENEMY_REV_REV_SLEEP.first, ENEMY_REV_REV_SLEEP.second);
		}
		else {
			mAnim->SetAnimRange(ENEMY_REV_REV.first, ENEMY_REV_REV.second);
		}

	}

	mAnim->SetAnimFPS(20);
}