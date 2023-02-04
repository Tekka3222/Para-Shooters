#include "Gun.h"

Bullet::Bullet(Game* game, Entity* entity, Gun* gun, int entityType)
	:Actor(game)
	, mEntity(entity)
	, mGun(gun)
	, entityType(entityType)
	, isHit(false)
	, hittimer(0.0)
	, shotDamage(mGun->GetDamage())
{
	bnum++;
	Dir_Vector pBul = mGun->GetPosition() + mGun->GetFront() * ONEBOX * 3 / 4 + HALFBULLET;
	Dir_Vector pCam = Camera::GetCameraPosition();
	SetPosition(pBul);
	AbsPos = Dir_Vector(pBul + pCam + HALFBULLET);
	SetFront(mGun->GetFront());
	SetScale(0.05);
	mAnim = new AnimSpriteComponent(this, game->GetShader(), 200);
	GLuint texID;
	std::string ImURL = "images/";
	int texwidth = BASIC_TEX_SIZE, texheight = BASIC_TEX_SIZE, sizex = TEX_BIG, sizey = TEX_BIG, start = PLAYER_BULLET, end = PLAYER_BULLET;
	if (entityType == 0)
		start = PLAYER_BULLET, end = PLAYER_BULLET;
	else
		start = ENEMY_BULLET, end = ENEMY_BULLET;
	glGenTextures(1, &texID);
	game->SetupTexture(&texID, ImURL + "litsmap.raw", texwidth, texheight);
	bool cont = true;
	mAnim->SetAnimTextures(texID, texwidth, texheight, sizex, sizey, start, end, cont);
}

int Bullet::bnum = 0;

void Bullet::UpdateActor(double delta) {
	if (isHit) {
		hittimer += delta;
		//“–‚½‚Á‚½’e‚Í0.2•bŒã‚ÉÁ–Å‚·‚é
		if (hittimer > 0.2) {
			SetState(State::Dead);
		}
		return;
	}
	Dir_Vector pCam = Camera::GetCameraPosition();
	double nx = AbsPos.x + GetFront().x * BULLETSPEED * delta - pCam.x;
	double ny = AbsPos.y + GetFront().y * BULLETSPEED * delta - pCam.y;
	if ((nx < 0 || nx > TEXWINDOWWIDTH * ONEBOX) ||
		(ny < 0 || ny > TEXWINDOWHEIGHT * ONEBOX)) {
		this->SetState(State::Dead);
		return;
	}
	else {
		checkHit(AbsPos,(Dir_Vector(nx,ny) + pCam));
	}
	AbsPos.x += GetFront().x * BULLETSPEED * delta;
	AbsPos.y += GetFront().y * BULLETSPEED * delta;
	SetPosition(Dir_Vector(nx, ny)-HALFBULLET);
}

void Bullet::checkHit(Dir_Vector& before, Dir_Vector current){
	if (entityType == 0) {
		vector<Entity*> entities = Camera::GetEntity(TYPE_ENEMY);
		for (auto& e : entities) {
			if (e->GetState() != State::Active)
				continue;
			Dir_Vector ePos = e->GetAbsPPos();
			Dir_Vector size = e->getSize();
			if (Dir_Vector::CheckHit(before, current, Dir_Vector(ePos.x, ePos.y), Dir_Vector(ePos.x, ePos.y + size.y)) ||
				Dir_Vector::CheckHit(before, current, Dir_Vector(ePos.x, ePos.y), Dir_Vector(ePos.x + size.x, ePos.y)) ||
				Dir_Vector::CheckHit(before, current, Dir_Vector(ePos.x, ePos.y + size.y), Dir_Vector(ePos.x + size.x, ePos.y + size.y)) ||
				Dir_Vector::CheckHit(before, current, Dir_Vector(ePos.x + size.x, ePos.y), Dir_Vector(ePos.x + size.x, ePos.y + size.y))) {
				e->Damage(mGun->GetDamage());
				if (e->GetHealth() <= 0) {
					mEntity->killEnemy();
				}
				isHit = true;
				mAnim->SetAnimRange(PLAYER_BULLET_HIT, PLAYER_BULLET_HIT);
				SetScale(0.1);
			}
		}
	}
	else {
		vector<Entity*> entities = Camera::GetEntity(TYPE_PLAYER);
		for (auto& e : entities) {
			Dir_Vector ePos = e->GetAbsPPos();
			Dir_Vector size = e->getSize();
			if (Dir_Vector::CheckHit(before, current, Dir_Vector(ePos.x, ePos.y), Dir_Vector(ePos.x, ePos.y + size.y)) ||
				Dir_Vector::CheckHit(before, current, Dir_Vector(ePos.x, ePos.y), Dir_Vector(ePos.x + size.x, ePos.y)) ||
				Dir_Vector::CheckHit(before, current, Dir_Vector(ePos.x, ePos.y + size.y), Dir_Vector(ePos.x + size.x, ePos.y + size.y)) ||
				Dir_Vector::CheckHit(before, current, Dir_Vector(ePos.x + size.x, ePos.y), Dir_Vector(ePos.x + size.x, ePos.y + size.y))) {
				e->Damage(mGun->GetDamage());
				isHit = true;
				mAnim->SetAnimRange(ENEMY_BULLET_HIT, ENEMY_BULLET_HIT);
				SetScale(0.1);
			}
		}
	}
}