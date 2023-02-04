#include "Gun.h"

Gun::Gun(Game* game, Entity* entity)
	:Actor(game)
	, mEntity(entity)
	, revFlag(false)
	, leftingammo(BASIC_NUM_OF_BULLET)
	, maxAmmo(BASIC_NUM_OF_BULLET)
	, shotinterval(0.0)
	, intervalTime(PLAYER_BASIC_SHOT_INTERVAL)
	, reloadTime(0.0)
	, reloading(false)
	, entityType(entity->GetEntityType())
	, reloadCompleteTime(RELOAD_TIME)
	, shotDamage(BASIC_BULLETDAMAGE)
{
	if (entityType == TYPE_PLAYER) {
		SetPosition(mEntity->GetPosition() + Dir_Vector(ONEBOX / 2, ONEBOX));
	}
	else {
		SetPosition(mEntity->GetPosition() + Dir_Vector(ONEBOX / 2, ONEBOX * 3 / 4));
		//�ŏ��̃X�e�[�W��1�Ȃ̂ōŏ�BASIC�ɂȂ�悤�ɒ����A���̌�̓X�e�[�W���i�ނ��ƂɃC���^�[�o�������炷
		intervalTime += max(ENEMY_BASIC_INTERVAL_CORRECTOR * pow(DECREASE_INTERVAL_PAR_STAGE, (Camera::curStage - 1.0)), 0.0);
		//10�X�e�[�W�Ɉ��_���[�W������
		shotDamage += (Camera::curStage - 1.0) * INCREASE_DAMAGE_PAR_STAGE;
	}
	SetFront(mEntity->GetFront());
	SetScale(0.12);
	SetRotate(-90);
	if (GetFront() == Dir_Vector::Front) {
		fFlag = true;
	}
	else {
		fFlag = false;
	}
	if(entityType == TYPE_PLAYER)
		mAnim = new AnimSpriteComponent(this, game->GetShader(), 150);
	else
		mAnim = new AnimSpriteComponent(this, game->GetShader(), 140);
	GLuint texID;
	std::string ImURL = "images/";
	int texwidth = BASIC_TEX_SIZE, texheight = BASIC_TEX_SIZE, sizex = TEX_SMALL, sizey = TEX_BIG, start = PLAYER_GUN_RIGHT, end = PLAYER_GUN_RIGHT;
	if (entityType == TYPE_PLAYER) {
		start = PLAYER_GUN_RIGHT, end = PLAYER_GUN_RIGHT;
	}
	else if (entityType == TYPE_ENEMY) {
		start = ENEMY_GUN_RIGHT, end = ENEMY_GUN_RIGHT;
	}
	glGenTextures(1, &texID);
	game->SetupTexture(&texID, ImURL + "character.raw", texwidth, texheight);
	bool cont = true;
	mAnim->SetAnimTextures(texID, texwidth, texheight, sizex, sizey, start, end, cont);

	if (entityType == TYPE_PLAYER) {
		mDest = new Actor(game);
		mDest->SetScale(0.12);
		mDest->SetPosition(mEntity->GetPosition() + GetFront() * ONEBOX * 10 + Dir_Vector(ONEBOX / 2, ONEBOX / 2));
		cDest = new AnimSpriteComponent(mDest, game->GetShader(), 150);
		texwidth = BASIC_TEX_SIZE, texheight = BASIC_TEX_SIZE, sizex = TEX_BIG, sizey = TEX_BIG, start = RETICLE_NOR, end = RETICLE_NOR;
		glGenTextures(1, &texID);
		game->SetupTexture(&texID, ImURL + "litsmap.raw", texwidth, texheight);
		cont = true;
		cDest->SetAnimTextures(texID, texwidth, texheight, sizex, sizey, start, end, cont);

		sholder = Dir_Vector(ONEBOX / 3, 0);
	}
	else {
		mDest = NULL;
		cDest = NULL;
		sholder = Dir_Vector(ONEBOX / 3, 0);
	}
}
void Gun::UpdateActor(double delta) {
	if (mEntity->GetState() == State::Dead) {
		SetState(State::Dead);
		mDest->SetState(State::Dead);
		return;
	}
	sholder = Dir_Vector(ONEBOX / 3, 0);
	sholder.rotate(GetRotate() + ANGLE_CORRECTOR);
	if (leftingammo == 0 && entityType == TYPE_PLAYER) {
		cDest->SetAnimRange(RETICLE_RELOAD, RETICLE_RELOAD);
	}
	else if (entityType == TYPE_PLAYER) {
		cDest->SetAnimRange(RETICLE_NOR, RETICLE_NOR);
	}
	if (entityType == TYPE_PLAYER) {
		ReflectUpgrade();
		Dir_Vector tmp = GetPosition() + GetFront() * ONEBOX * 10;
		Dir_Vector gPos = GetPosition();
		double rat = 1.0;
		double width = (double)Game::getWidth();
		double height = (double)Game::getHeight();
		if (tmp.x > width - ONEBOX - HALFBULLET.x / 2) {
			rat = (width - ONEBOX - HALFBULLET.x / 2 - gPos.x) / (tmp.x - gPos.x);
		}
		if (tmp.x < 0) {
			rat = gPos.x / (gPos.x - tmp.x);
		}
		if (tmp.y > height - ONEBOX - HALFBULLET.y / 2) {
			if (rat > (height - ONEBOX - HALFBULLET.y / 2 - gPos.y) / (tmp.y - gPos.y))
				rat = (height - ONEBOX - HALFBULLET.y / 2 - gPos.y) / (tmp.y - gPos.y);
		}
		if (tmp.y < 0) {
			if (rat > gPos.y / (gPos.y - tmp.y))
				rat = gPos.y / (gPos.y - tmp.y);
		}
		mDest->SetPosition(GetPosition() + GetFront() * ONEBOX * 10 * rat);
	}
	if (fFlag && mEntity->GetFront() == Dir_Vector::Back) {
		SetRotate(-GetRotate());
		Dir_Vector tmp = Dir_Vector::Front;
		tmp.rotate(GetRotate() + ANGLE_CORRECTOR);
		SetFront(tmp);
		sholder = Dir_Vector(-sholder.x,sholder.y);
		fFlag = false;
	}
	else if (!fFlag && mEntity->GetFront() == Dir_Vector::Front) {
		SetRotate(-GetRotate());
		Dir_Vector tmp = Dir_Vector::Front;
		tmp.rotate(GetRotate() + ANGLE_CORRECTOR);
		SetFront(tmp);
		sholder = Dir_Vector(-sholder.x, sholder.y);
		fFlag = true;
	}
	double xdif, ydif;
	//xdif��ydif���L�����N�^�[�̌��̈ʒu�ɍ����悤�ɕ␳
	if (fFlag)
		xdif = 0;
	else
		xdif = ONEBOX / 2;
	if (entityType == TYPE_PLAYER) {
		if (revFlag)
			ydif = ONEBOX * 5 / 8;
		else
			ydif = ONEBOX / 3;
	}
	else {
		if (revFlag)
			ydif = ONEBOX / 2;
		else
			ydif = ONEBOX * 7 / 16;
	}
	SetPosition(mEntity->GetPosition() + Dir_Vector(xdif, ydif) + sholder);
	if (shotinterval > 0.0) {
		if (shotinterval - delta < 0.0) {
			shotinterval = 0;
		}
		else {
			shotinterval -= delta;
		}
	}
}

void Gun::shot(double delta) {
	if (shotinterval == 0.0 && leftingammo > 0 && !(mEntity->whileReverse())) {
		shotinterval = intervalTime;
		leftingammo--;
		new Bullet(GetGame(), mEntity, this, entityType);
	}
}

void Gun::ReverseWorld() {
	if (mEntity->GetFront() == Dir_Vector::Front) {
		Dir_Vector f = GetFront();
		f.rotate(-180 - GetRotate() * 2);
		SetFront(f);
		SetRotate(-180 - GetRotate());
		sholder = Dir_Vector(sholder.x, -sholder.y);
	}
	else {
		Dir_Vector f = GetFront();
		f.rotate(180 - GetRotate() * 2);
		SetFront(f);
		SetRotate(180 - GetRotate());
		sholder = Dir_Vector(sholder.x, -sholder.y);
		
	}
	revFlag = !revFlag;
}

void Gun::changeDirection(double rotate) {
	if (!fFlag) {
		rotate *= -1;
	}
	if (!revFlag) {
		if (fFlag) {
			if (entityType == TYPE_PLAYER)
				mAnim->SetAnimRange(PLAYER_GUN_RIGHT, PLAYER_GUN_RIGHT);
			else
				mAnim->SetAnimRange(ENEMY_GUN_RIGHT, ENEMY_GUN_RIGHT);
			if (GetRotate() + rotate < -MAXROTATE_N) {//-180
				Dir_Vector f = Dir_Vector::Front;
				f.rotate(-MAXROTATE_N + ANGLE_CORRECTOR);
				SetFront(f);
				SetRotate(-MAXROTATE_N);
				return;
			}
			else if (GetRotate() + rotate > -MINROTATE_N) {//-80
				Dir_Vector f = Dir_Vector::Front;
				f.rotate(-MINROTATE_N + ANGLE_CORRECTOR);
				SetFront(f);
				SetRotate(-MINROTATE_N);
				return;
			}
		}
		else {
			if (entityType == TYPE_PLAYER)
				mAnim->SetAnimRange(PLAYER_GUN_LEFT, PLAYER_GUN_LEFT);
			else
				mAnim->SetAnimRange(ENEMY_GUN_LEFT, ENEMY_GUN_LEFT);
			if (GetRotate() + rotate < MINROTATE_N) {//80
				Dir_Vector f = Dir_Vector::Front;
				f.rotate(MINROTATE_N + ANGLE_CORRECTOR);
				SetFront(f);
				SetRotate(MINROTATE_N);
				return;
			}
			else if (GetRotate() + rotate > MAXROTATE_N) {//180
				Dir_Vector f = Dir_Vector::Front;
				f.rotate(MAXROTATE_N + ANGLE_CORRECTOR);
				SetFront(f);
				SetRotate(MAXROTATE_N);
				return;
			}
		}
	}
	else {
		if (fFlag) {
			if (entityType == TYPE_PLAYER)
				mAnim->SetAnimRange(PLAYER_GUN_RIGHT_REV, PLAYER_GUN_RIGHT_REV);
			else
				mAnim->SetAnimRange(ENEMY_GUN_RIGHT_REV, ENEMY_GUN_RIGHT_REV);
			if (GetRotate() + rotate < -MAXROTATE_R) {//-100
				Dir_Vector f = Dir_Vector::Front;
				f.rotate(-MAXROTATE_R + ANGLE_CORRECTOR);
				SetFront(f);
				SetRotate(-MAXROTATE_R);
				return;
			}
			else if (GetRotate() + rotate > -MINROTATE_R) {//0
				Dir_Vector f = Dir_Vector::Front;
				f.rotate(-MINROTATE_R + ANGLE_CORRECTOR);
				SetFront(f);
				SetRotate(-MINROTATE_R);
				return;
			}
		}
		else {
			if (entityType == TYPE_PLAYER)
				mAnim->SetAnimRange(PLAYER_GUN_LEFT_REV, PLAYER_GUN_LEFT_REV);
			else
				mAnim->SetAnimRange(ENEMY_GUN_LEFT_REV, ENEMY_GUN_LEFT_REV);
			if (GetRotate() + rotate < MINROTATE_R) {//0
				Dir_Vector f = Dir_Vector::Front;
				f.rotate(MINROTATE_R + ANGLE_CORRECTOR);
				SetFront(f);
				SetRotate(MINROTATE_R);
				return;
			}
			else if (GetRotate() + rotate > MAXROTATE_R) {//100
				Dir_Vector f = Dir_Vector::Front;
				f.rotate(MAXROTATE_R + ANGLE_CORRECTOR);
				SetFront(f);
				SetRotate(MAXROTATE_R);
				return;
			}
		}
	}
	Dir_Vector f = GetFront();
	f.rotate(rotate);
	SetFront(f);
	RotateGun(rotate);
	return;
}

void Gun::ResetWorld() {
	revFlag = false;
	fFlag = true;
	shotinterval = 0.0;
	leftingammo = maxAmmo;
	SetFront(Dir_Vector::Front);
	SetRotate(-90);
	sholder = Dir_Vector(ONEBOX / 3, 0);
}

void Gun::RotateGun(double rotate) {
	sholder.rotate(rotate);
	SetRotate(GetRotate() + rotate);
}

void Gun::Reload(double delta) {
	if (!reloading) {
		reloading = true;
		leftingammo = 0;
		reloadTime = 0.0;
	}
	if (reloadTime < reloadCompleteTime) {
		reloadTime += delta;
		return;
	}
	leftingammo = maxAmmo;
	reloading = false;
}

void Gun::ReflectUpgrade() {
	//���x���ɉ����ăC���^�[�o�������炷
	intervalTime = max(PLAYER_BASIC_SHOT_INTERVAL * pow(DECREASE_INTERVAL_PAR_LEVEL,GetGame()->getMenu()->getUpgradeLevel(UPGRADE_INTERVAL)), 0.01);
	//���x���ɉ����ă_���[�W������
	shotDamage = BASIC_BULLETDAMAGE + GetGame()->getMenu()->getUpgradeLevel(UPGRADE_DAMAGE) * (double)INCREASE_DAMAGE_PAR_LEVEL;
	maxAmmo = BASIC_NUM_OF_BULLET + GetGame()->getMenu()->getUpgradeLevel(UPGRADE_NUM_OF_BULLET);
	reloadCompleteTime = max(RELOAD_TIME * pow(DECREASE_RELOAD_PAR_LEVEL, GetGame()->getMenu()->getUpgradeLevel(UPGRADE_RELOAD)) , 0.01);
}


