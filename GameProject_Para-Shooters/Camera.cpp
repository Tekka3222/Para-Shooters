#include "Camera.h"

Camera::Camera(Game* game, Player* player)
	:Actor(game)
	, mPlayer(player)
	, revFlag(false)
	, BGActors(vector<vector<Actor*>>(TEXWINDOWHEIGHT))
	, curTextureX(0)
	, curTextureY(0)
	, Life(vector<AnimSpriteComponent*>(5))
	, lapKill(0)
	, stageSumTime(0.0)
	, sumScore(0)
{
	std::string ImURL = "images/";
	GLuint texID;
	glGenTextures(1, &texID);
	GetGame()->SetupTexture(&texID, ImURL + "tilemap.raw", TEXWIDTH, TEXHEIGHT);
	mapID = texID;
	glGenTextures(1, &texID);
	GetGame()->SetupTexture(&texID, ImURL + "litsmap.raw", TEXWIDTH, TEXHEIGHT);
	litsID = texID;
	glGenTextures(1, &texID);
	GetGame()->SetupTexture(&texID, ImURL + "upgrademap.raw", TEXWIDTH, TEXHEIGHT);
	upgradeID = texID;
	SetRandomMap();
	SetAllTexture();
	EntityList[0].emplace_back(player);
	CreateEnemies();
	CreateDisplayInfo();
}

vector<int> Camera::Bound(SWIDTH / ONESCAF + 1, 0);
vector<vector<int>> Camera::Rmap(SHEIGHT, vector<int>(SWIDTH, 0));
Dir_Vector Camera::CPos(0.0, 0.0);
int Camera::curStage = 1;
int Camera::MaxLife = MAX_HEALTH;
int Camera::curLife = 4;
vector<vector<Entity*>> Camera::EntityList(2);

void Camera::ResetWorld() {
	RemoveAllTexture();
	RemoveAllEnemies();
	SunOrMoon->SetState(State::Dead);
	revFlag = false;
	lapKill = mPlayer->GetKill();
	stageSumTime = 0;
	SetRandomMap();
	SetAllTexture();
	CreateEnemies();
	curStage++;
}

void Camera::ReverseWorld() {
	if (!revFlag) {
		SunOrMoon->SetState(State::Dead);
		int start = MOON.first, end = MOON.second;
		SunOrMoon = new Actor(GetGame());
		SunOrMoon->SetPosition(Dir_Vector((1920.0 - 100.0), (1080.0 - 100)));
		SunOrMoon->SetScale(0.2);
		AnimSpriteComponent* mSpr = new AnimSpriteComponent(SunOrMoon, GetGame()->GetShader(), 125);
		mSpr->SetAnimTextures(mapID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, start, end, true);
	}
	else {
		SunOrMoon->SetState(State::Dead);
		int start = SUN.first, end = SUN.second;
		SunOrMoon = new Actor(GetGame());
		SunOrMoon->SetPosition(Dir_Vector::Zero);
		SunOrMoon->SetScale(0.2);
		AnimSpriteComponent* mSpr = new AnimSpriteComponent(SunOrMoon, GetGame()->GetShader(), 125);
		mSpr->SetAnimTextures(mapID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, start, end, true);
	}
	revFlag = !revFlag;
}
void Camera::UpdateActor(double delta) {
	Dir_Vector pPos = mPlayer->GetAbsPPos();
	Dir_Vector cPos = GetPosition();
	CPos = cPos;
	double gWidth = (double)GetGame()->getWidth();
	double gHeight = (double)GetGame()->getHeight();
	if (abs(pPos.x - cPos.x - gWidth / 2.0) > gWidth / 2.0 * MOVECAMERAX) {
		if (pPos.x - cPos.x - gWidth / 2.0 <= 0) {
			cPos.x -= gWidth / 2.0 * (1 - MOVECAMERAX) - (pPos.x - cPos.x);
		}
		else {
			cPos.x += pPos.x - cPos.x - gWidth / 2.0 * ( 1 + MOVECAMERAX );
		}
	}
	if (abs(pPos.y - cPos.y - gHeight / 2.0) > gHeight / 2.0 * MOVECAMERAY) {
		if (pPos.y - cPos.y - gHeight / 2.0 <= 0) {
			cPos.y -= gHeight / 2.0 * ( 1 - MOVECAMERAY ) - (pPos.y - cPos.y);
		}
		else {
			cPos.y += pPos.y - cPos.y - gHeight / 2.0 * (1 + MOVECAMERAY);
		}
	}
	if (cPos.x < 0) {
		cPos.x = 0;
	}
	if (cPos.x > SWIDTH * ONEBOX - Game::getWidth())
		cPos.x = SWIDTH * ONEBOX - Game::getWidth();
	stageSumTime += delta;
	SetPosition(cPos);
	SetDisplayInfo();
	UpdateTex();
	ActivateEnemies();
}

void Camera::ActivateEnemies() {
	auto itr = EntityList[1].begin();
	while (itr != EntityList[1].end()) {
		if ((*itr)->GetHealth() <= 0) {
			(*itr)->SetState(State::Dead);
			(*itr)->GetGun()->SetState(State::Dead);
			itr = EntityList[1].erase(itr);
			if (itr == EntityList[1].end()) {
				return;
			}
		}
		Dir_Vector pCam = GetPosition();
		Dir_Vector eAbsPos = (*itr)->GetAbsPPos();
		if (pCam.x < eAbsPos.x && eAbsPos.x < pCam.x + Game::getWidth() && pCam.y < eAbsPos.y && eAbsPos.y < pCam.y + Game::getHeight())
			(*itr)->setActive(true);
		else
			(*itr)->setActive(false);
		itr++;
	}
}

void Camera::SetRandomMap() {
	for (int i = 0; i < SHEIGHT; i++) {
		for (int j = 0; j < SWIDTH; j++) {
			Rmap[i][j] = 0;
		}
	}
	random_device rnd;
	mt19937 mt;
	mt.seed(rnd());
	uniform_int_distribution<> rand20(0, SHEIGHT - 1);
	Bound[0] = DEFAULTHEIGHT;
	Bound[1] = DEFAULTHEIGHT;
	for (int i = 2; i < SWIDTH / ONESCAF + 1; i++) {
		Bound[i] = rand20(mt);
	}
	for (int i = 0; i < SHEIGHT; i++) {
		for (int j = 0; j < SWIDTH; j++) {
			//Creating Bounds
			if (i == GetBound(j / ONESCAF)) {
				Rmap[i][j] = BLOCK_BOUND;
			}
			//Creating Walls
			if (j % ONESCAF == 0 && j >= ONESCAF * 2) {
				if (GetBound(j / ONESCAF) > GetBound(j / ONESCAF - 1)) {
					if (GetBound(j / ONESCAF - 1) <= i && i <= GetBound(j / ONESCAF)) {
						Rmap[i][j] = BLOCK_WALL;
					}
				}
				if (GetBound(j / ONESCAF) < GetBound(j / ONESCAF - 1)) {
					if (GetBound(j / ONESCAF - 1) >= i && i >= GetBound(j / ONESCAF)) {
						Rmap[i][j] = BLOCK_WALL;
					}
				}
			}
		}
	}
	//Creating Doors
	int dx;
	for (int i = 0; i < SHEIGHT; i++) {
		if ((SWIDTH - (ONESCAF * 2)) % ONESCAF == 0)
			dx = SWIDTH - 2;
		else {
			dx = SWIDTH - 1;
		}
		int y = GetBound(dx / ONESCAF);
		if (y <= 10) {
			Rmap[y + 1][dx] = BLOCK_DOOR_REVERSE;
		}
		else {
			Rmap[y - 1][dx] = BLOCK_DOOR_GROUND;
		}
	}
	for (int i = 0; i < TEXWINDOWHEIGHT; i++) {
		while (BGActors[i].size() > 0) {
			BGActors[i].back()->SetState(State::Dead);
			BGActors[i].pop_back();
		}
	}
	BGActors = vector<vector<Actor*>>(TEXWINDOWHEIGHT);
	curTextureX = 0;
	curTextureY = 0;
}

void Camera::SetAllTexture() {
	SetPosition(Dir_Vector(0.0, 0.0));
	curTextureX = 0;
	curTextureY = 0;
	int start = 0, end = 0;
	for (int i = 0; i < TEXWINDOWHEIGHT; i++) {
		for (int j = 0; j < TEXWINDOWWIDTH; j++) {
			SetUpAnimation(j, i, start, end);
			SetUpTexture((double)j * ONEBOX, (double)i * ONEBOX, i, mapID, start, end);
		}
	}
	start = SUN.first, end = SUN.second;
	SunOrMoon = new Actor(GetGame());
	SunOrMoon->SetScale(0.2);
	AnimSpriteComponent* mSpr = new AnimSpriteComponent(SunOrMoon, GetGame()->GetShader(), 125);
	mSpr->SetAnimTextures(mapID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, start, end, true);
}

void Camera::CreateEnemies() {
	random_device rnd;
	mt19937 mt;
	mt.seed(rnd());
	uniform_int_distribution<> rand16_100(16, SWIDTH - 1);
	map<int, bool> EPlace;
	int numOfEnemies = static_cast<int>(floor(curStage * 20 / 30 + 5));
	if (numOfEnemies > 30){
		numOfEnemies = 30;
	}
	while((int)EPlace.size() < numOfEnemies){
		EPlace[rand16_100(mt)] = true;
	}
	for (int i = 0; i < SWIDTH; i++) {
		if (EPlace[i])
			CreateOneEnemy(i,rand16_100(mt));
	}
}

void Camera::CreateOneEnemy(int placement, int rand) {
	int bound = GetBound(placement / ONESCAF);
	if (bound > GetBound((placement - 1) / ONESCAF))
		bound = GetBound((placement - 1) / ONESCAF);
	Dir_Vector firstPosition((double)placement * ONEBOX,0);
	firstPosition.y = ((double)bound - 2) * ONEBOX;
	Enemy* oneEnemy = new Enemy(GetGame(), mPlayer, firstPosition);
	oneEnemy->SetPosition(firstPosition);
	oneEnemy->SetScale(0.24);
	EntityList[1].emplace_back(oneEnemy);

}

void Camera::RemoveAllTexture() {
	for (int i = 0; i < TEXWINDOWHEIGHT; i++) {
		while (BGActors[i].size() > 0) {
			BGActors[i].back()->SetState(State::Dead);
			BGActors[i].pop_back();
		}
	}
}

void Camera::RemoveAllEnemies() {
	while ((int)EntityList[1].size() > 0) {
		Entity* del = EntityList[1].back();
		del->SetState(State::Dead);
		del->GetGun()->SetState(State::Dead);
		EntityList[1].pop_back();
	}
}

void Camera::SetUpTexture(double x, double y, int TexY, int texID, int start, int end) {
	Actor* BG = new Actor(GetGame());
	BG->SetPosition(Dir_Vector(x,y));
	BG->SetScale(0.12);
	BG->SetState(State::Paused);
	if (start == end) {
		SpriteComponent* mSpr = new SpriteComponent(BG, GetGame()->GetShader(), 100);
		mSpr->SetTexture(texID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, start);
	}
	else {
		AnimSpriteComponent* mSpr = new AnimSpriteComponent(BG, GetGame()->GetShader(), 100);
		mSpr->SetAnimTextures(texID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, start, end, true);
	}
	BGActors[TexY].emplace_back(BG);
}

void Camera::SetUpAnimation(int x, int y, int& start, int& end) {
	if (GetMap(x, y) == BLOCK_BOUND) {
		start = BOUND;
		end = BOUND;
	}
	else if (GetMap(x, y) == BLOCK_DOOR_GROUND) {
		start = WHITE_DOOR;
		end = WHITE_DOOR;
	}
	else if (GetMap(x, y) == BLOCK_DOOR_REVERSE) {
		start = BLACK_DOOR;
		end = BLACK_DOOR;
	}
	else if (GetMap(x, y) == BLOCK_WALL) {
		if (GetBound(x / ONESCAF) >= GetBound(x / ONESCAF - 1)) {
			start = BW_WALL;
			end = BW_WALL;
		}
		else if (GetBound(x / ONESCAF) <= GetBound(x / ONESCAF - 1)) {
			start = WB_WALL;
			end = WB_WALL;
		}
	}
	else if (GetMap(x, y) == BLOCK_AIR) {
		if (GetBound(x / ONESCAF) >= y) {
			start = WHITE_AIR;
			end = WHITE_AIR;
		}
		else if (GetBound(x / ONESCAF) <= y) {
			start = BLACK_AIR;
			end = BLACK_AIR;
		}
	}
}

void Camera::UpdateTex() {
	int placement = 0;
	Dir_Vector cPos = GetPosition();
	int tX = static_cast<int>(floor(cPos.x / ONEBOX));
	int tY = static_cast<int>(floor(cPos.y / ONEBOX));
	int ctX = static_cast<int>(floor (curTextureX / ONEBOX));
	int ctY = static_cast<int>(floor (curTextureY / ONEBOX));
	bool upFlag = false;
	double xdif = 0.0, ydif = 0.0;
	if (tX != ctX) {
		if (tX > ctX) {
			int changeNum = tX - ctX;
			for (int i = 0; i < TEXWINDOWHEIGHT; i++) {
				ResetTextureX(changeNum, i, curTextureX,curTextureY, true);
			}
		}
		else {
			int changeNum = ctX - tX;
			for (int i = 0; i < TEXWINDOWHEIGHT; i++) {
				ResetTextureX(changeNum, i, curTextureX, curTextureY, false);
			}
		}
	}
	if (cPos.x != curTextureX) {
		xdif = cPos.x - curTextureX;
		curTextureX = cPos.x;
		UpdateAllPos(-xdif, 0);
	}
	if (tY != ctY) {
		if (tY > ctY) {
			int changeNum = tY - ctY;
			for (int i = 0; i < changeNum; i++) {
				ResetTextureY(changeNum, i, curTextureX, curTextureY, true);
			}
		}
		else {
			int changeNum = ctY - tY;
			for (int i = 0; i < changeNum; i++) {
				ResetTextureY(changeNum, i, curTextureX, curTextureY, false);
			}
		}
	}
	if (cPos.y != curTextureY) {
		ydif = cPos.y - curTextureY;
		curTextureY = cPos.y;
		UpdateAllPos(0,-ydif);
	}
}

void Camera::UpdateAllPos(double x, double y) {
	for (int i = 0; i < TEXWINDOWHEIGHT; i++) {
		for (auto& a : BGActors[i]) {
			Dir_Vector tmp = a->GetPosition();
			a->SetPosition(Dir_Vector(tmp.x + x, tmp.y + y));
		}
	}

}

void Camera::ResetTextureX(int changeNum, int align, double TexX, double TexY, bool isRight){
	if (isRight) {
		for (int i = 0;i<changeNum; i++) {
			BGActors[align][0]->SetState(State::Dead);
			BGActors[align].erase(BGActors[align].begin());
			int start = 0, end = 0;
			int placeX = static_cast<int>(floor(TexX / ONEBOX)) + TEXWINDOWWIDTH + i;
			int placeY = static_cast<int>(floor(TexY / ONEBOX)) + align;
			SetUpAnimation(placeX,placeY, start, end); 
			SetUpTexture((double)placeX * ONEBOX - TexX, (double)placeY * ONEBOX - TexY, align, mapID, start, end);
		}
	}
	else {
		for (int i = 0; i < changeNum; i++) {
			BGActors[align].back()->SetState(State::Dead);
			BGActors[align].pop_back();
			int start = 0, end = 0;
			int placeX = static_cast<int>(floor(TexX / ONEBOX)) - (i + 1);
			int placeY = static_cast<int>(floor(TexY / ONEBOX)) + align;
			//cout << "cnum" << i << "x" << (double)placeX * ONEBOX - TexX+1 << "y" << placeY * ONEBOX - TexY << endl;
			SetUpAnimation(placeX, placeY, start, end);
			SetUpTexture(((double)placeX * ONEBOX - TexX),((double)placeY * ONEBOX - TexY), align, mapID, start, end);
			rotate(BGActors[align].rbegin(), BGActors[align].rbegin() + 1, BGActors[align].rend());
		}

	}
}

void Camera::ResetTextureY(int changeNum, int align, double TexX, double TexY, bool isDown) {
	if (isDown) {
		for (int i = 0; i < TEXWINDOWWIDTH; i++) {
			BGActors[0].back()->SetState(State::Dead);
			BGActors[0].pop_back();
		}
		rotate(BGActors.begin(), BGActors.begin() + 1, BGActors.end());
		for (int i = 0; i < TEXWINDOWWIDTH; i++) {
			int start = 0, end = 0;
			int placeX = static_cast<int>(floor(TexX / ONEBOX)) + i;
			int placeY = static_cast<int>(floor(TexY / ONEBOX)) + TEXWINDOWHEIGHT + align;
			SetUpAnimation(placeX, placeY , start, end);
			SetUpTexture((double)placeX * ONEBOX - TexX, (double)placeY * ONEBOX - TexY,TEXWINDOWHEIGHT - 1, mapID, start, end);
		}
	}
	else {
		for (int i = 0; i < TEXWINDOWWIDTH; i++) {
			BGActors[TEXWINDOWHEIGHT - 1].back()->SetState(State::Dead);;
			BGActors[TEXWINDOWHEIGHT - 1].pop_back();
		}
		rotate(BGActors.rbegin(), BGActors.rbegin() + 1, BGActors.rend());
		for (int i = 0; i < TEXWINDOWWIDTH; i++) {
			int start = 0, end = 0;
			int placeX = static_cast<int>(floor(TexX / ONEBOX)) + i;
			int placeY = static_cast<int>(floor(TexY / ONEBOX)) - (align + 1);
			SetUpAnimation(placeX, placeY, start, end);
			SetUpTexture((double)placeX * ONEBOX - TexX, (double)placeY * ONEBOX - TexY, 0, mapID, start, end);
		}
	}
}

void Camera::CreateDisplayInfo() {
	Actor* Display = NULL;
	//STAGE
	SetInfo(Display, ALPHA_S, 0 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, 0));
	SetInfo(Display, ALPHA_T, 1 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, 0));
	SetInfo(Display, ALPHA_A, 2 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, 0));
	SetInfo(Display, ALPHA_G, 3 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, 0));
	SetInfo(Display, ALPHA_E, 4 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, 0));
	Stage.emplace_back(SetChangeInfo(Display, curStage, 0, Dir_Vector(ONEBOX * 5, 0), 0.06));
	//TIME
	SetInfo(Display, ALPHA_T, 0 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2));
	SetInfo(Display, ALPHA_I, 1 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2));
	SetInfo(Display, ALPHA_M, 2 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2));
	SetInfo(Display, ALPHA_E, 3 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2));
	//COLON OF TIME
	SetInfo(Display, COLON, 2 * ONEBOX / 2, Dir_Vector(ONEBOX * 9 / 2, ONEBOX / 2));
	SetInfo(Display, COLON, 5 * ONEBOX / 2, Dir_Vector(ONEBOX * 9 / 2, ONEBOX / 2));
	for (int i = 0; i < 6; i++) {
		Time.emplace_back(SetChangeInfo(Display, 0, i * ONEBOX / 2 + (i / 2) * ONEBOX / 2, Dir_Vector(ONEBOX * 9 / 2, ONEBOX / 2), 0.06));
	}
	//Kill
	SetInfo(Display, ALPHA_K, 0 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX));
	SetInfo(Display, ALPHA_I, 1 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX));
	SetInfo(Display, ALPHA_L, 2 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX));
	SetInfo(Display, ALPHA_L, 3 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX));
	Kill.emplace_back(SetChangeInfo(Display, 0, 5 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX), 0.06));
	//Bullets
	Display = new Actor(GetGame());
	BulletSlash = Display;
	SetMoveInfo(Display, SLASH, 3 * ONEBOX, Dir_Vector(ONEBOX * 2.0, Game::getHeight() - ONEBOX * 1.5), 0.12);
	Display = new Actor(GetGame());
	Max_Bullet_Actor.emplace_back(Display);
	Max_Bullet.emplace_back(SetMoveInfo(Display, 1, 10 * ONEBOX / 2, Dir_Vector(ONEBOX * 2.0, Game::getHeight() - ONEBOX * 2.0), 0.06));
	Display = new Actor(GetGame());
	Max_Bullet_Actor.emplace_back(Display);
	Max_Bullet.emplace_back(SetMoveInfo(Display, 5, 11 * ONEBOX / 2, Dir_Vector(ONEBOX * 2.0, Game::getHeight() - ONEBOX * 2.0), 0.06));
	Display = new Actor(GetGame());
	Bullet_Actor.emplace_back(Display);
	Bullet.emplace_back(SetMoveInfo(Display, 1, 0 * ONEBOX, Dir_Vector(ONEBOX * 2.0, Game::getHeight() - ONEBOX * 2.5), 0.12));
	Display = new Actor(GetGame());
	Bullet_Actor.emplace_back(Display);
	Bullet.emplace_back(SetMoveInfo(Display, 5, 1 * ONEBOX, Dir_Vector(ONEBOX * 2.0, Game::getHeight() - ONEBOX * 2.5), 0.12));
	//Life
	for (int i = 0; i < 5; i++) {
		Life[i] = SetChangeInfo(Display, FULL_NOR_HEART, i * ONEBOX, Dir_Vector(Game::getWidth() - ONEBOX * (5.0), 0), 0.12);
	}
	//LEFT
	SetInfo(Display, ALPHA_L, 0 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2 * 3));
	SetInfo(Display, ALPHA_E, 1 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2 * 3));
	SetInfo(Display, ALPHA_F, 2 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2 * 3));
	SetInfo(Display, ALPHA_T, 3 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2 * 3));
	Left.emplace_back(SetChangeInfo(Display, EntityList[1].size(), 5 * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2 * 3), 0.06));
	//Heal
	Display = new Actor(GetGame());
	Display->SetScale(0.12);
	Display->SetPosition(Dir_Vector(Game::getWidth() - ONEBOX * 4, Game::getHeight() - ONEBOX * 2.5));
	Heal_Pic = new SpriteComponent(Display, GetGame()->GetShader(), 180);
	Heal_Pic->SetTexture(upgradeID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, HEAL_COMP_TEX);
	Display = new Actor(GetGame());
	Heal_Actor.emplace_back(Display);
	Heal.emplace_back(SetMoveInfo(Display, 0, 0, Dir_Vector(ONEBOX * 2.0 + Game::getWidth() - ONEBOX * 4, Game::getHeight() - ONEBOX * 2.5), 0.12));
}

void Camera::SetInfo(Actor* Display, int place, double gap, Dir_Vector first) {
	Display = new Actor(GetGame());
	Display->SetScale(0.06);
	Display->SetPosition(first);
	SpriteComponent* nDisp = new SpriteComponent(Display, GetGame()->GetShader(), 180);
	Display->SetPosition(Display->GetPosition() + Dir_Vector(gap, 0));
	nDisp->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, place);
}

AnimSpriteComponent* Camera::SetChangeInfo(Actor* Display, int place, double gap, Dir_Vector first, double scale) {
	Display = new Actor(GetGame());
	Display->SetScale(scale);
	Display->SetPosition(first);
	AnimSpriteComponent* nDisp = new AnimSpriteComponent(Display, GetGame()->GetShader(), 180);
	Display->SetPosition(Display->GetPosition() + Dir_Vector(gap, 0));
	nDisp->SetAnimTextures(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, place, place, true);
	return nDisp;
}

void Camera::SetDisplayInfo() {
	//SetStage
	if (std::pow(10, Stage.size()) <= curStage) {
		Actor* Display = NULL;
		Stage.emplace_back(SetChangeInfo(Display, curStage % 10, Stage.size() * ONEBOX / 2, Dir_Vector(ONEBOX * 5, 0), 0.06));
	}
	for (int i = 0; i < (int)Stage.size(); i++) {
		int digit = curStage % 10;
		if (i != (int)Stage.size() - 1)
			digit = curStage / static_cast<int>(std::pow(10, Stage.size() - (i + 1)));
		Stage[i]->SetAnimRange(digit, digit);
	}
	// SetTime
	double time = Game::getTime();
	int min = static_cast<int>(time / 60), sec = static_cast<int>(time) % 60, mil = static_cast<int>(time * 100) % 100;
	int timeInfo[6] = { 0 };
	if (min >= 100) {
		for (int i = 0; i < 6; i++) {
			timeInfo[i] = 9;
		}
	}
	else {
		timeInfo[0] = min / 10;
		timeInfo[1] = min % 10;
		timeInfo[2] = sec / 10;
		timeInfo[3] = sec % 10;
		timeInfo[4] = mil / 10;
		timeInfo[5] = mil % 10;
	}
	for (int i = 0; i < 6; i++) {
		Time[i]->SetAnimRange(timeInfo[i], timeInfo[i]);
	}
	string ammo = to_string(mPlayer->GetGun()->GetAmmo());
	string maxAmmo = to_string(mPlayer->GetGun()->GetMaxAmmo());
	int currentSize = Bullet.size();
	int afterSize = ammo.length();
	for (int i = 0; i < afterSize; i++) {
		if (i >= currentSize) {
			Actor* Display = new Actor(GetGame());
			Bullet_Actor.emplace_back(Display);
			Bullet.emplace_back(SetMoveInfo(Display, ammo[i] - '0', i * ONEBOX, Dir_Vector(ONEBOX * 3.0 - afterSize * ONEBOX / 2, Game::getHeight() - ONEBOX * 2.5), 0.12));
		}
		else {
			Bullet_Actor[i]->SetPosition(Dir_Vector(ONEBOX * 3.0 + i * ONEBOX - afterSize * ONEBOX / 2, Game::getHeight() - ONEBOX * 2.5));
			Bullet[i]->ChangeTexture(ammo[i] - '0');
		}
	}
	while (afterSize < currentSize) {
		currentSize--;
		Bullet_Actor.back()->SetState(State::Dead);
		Bullet_Actor.pop_back();
		Bullet.pop_back();
	}

	currentSize = Max_Bullet.size();
	afterSize = maxAmmo.length();
	double maxPlacementCorrector = afterSize / 2.0 + 2.0;//2.0ÇÕÉXÉâÉbÉVÉÖÇÃÇΩÇﬂÇÃãÛä‘
	BulletSlash->SetPosition(Dir_Vector(ONEBOX * 2.0 + ONEBOX * maxPlacementCorrector, Game::getHeight() - ONEBOX * 2.5));
	maxPlacementCorrector += 1.5;//ç≈ëÂíeêîÇï\é¶Ç∑ÇÈÇ‹Ç≈ÇÃÉXÉâÉbÉVÉÖÇ∆ÇÃãóó£
	for (int i = 0; i < afterSize; i++) {
		if (i >= currentSize) {
			Actor* Display = new Actor(GetGame());
			Max_Bullet_Actor.emplace_back(Display);
			Max_Bullet.emplace_back(SetMoveInfo(Display, maxAmmo[i] - '0', maxPlacementCorrector * ONEBOX + i  * ONEBOX / 2, Dir_Vector(ONEBOX * 2.0, Game::getHeight() - ONEBOX * 2.0), 0.06));
		}
		else {
			Max_Bullet_Actor[i]->SetPosition(Dir_Vector(ONEBOX * 2.0 + maxPlacementCorrector * ONEBOX + i * ONEBOX / 2, Game::getHeight() - ONEBOX * 2.0));
			Max_Bullet[i]->ChangeTexture(maxAmmo[i] - '0');
		}
	}
	int health = mPlayer->GetHealth();
	double oneHalfHeart = MaxLife / 10;
	health = 10 - static_cast<int>(floor(((double)MaxLife - health) / oneHalfHeart));
	bool noHealthFlag = false;
	if (health == 0) {
		Life[0]->SetAnimRange(20, 20);
	}
	for (int i = 0; i < 5; i++) {
		if (i < (health + 1) / 2) {
			if ((health + 1) / 2 <= 1) {
				Life[i]->SetAnimRange(FULL_DANGER_HEART.first, FULL_DANGER_HEART.second);
			}
			else {
				Life[i]->SetAnimRange(FULL_NOR_HEART, FULL_NOR_HEART);
			}
		}
		else {
			Life[i]->SetAnimRange(NULL_PIC, NULL_PIC);
		}
	}
	int last = (health + 1) / 2 - 1;
	if (health % 2 != 0) {
		if (last == 0) {
			Life[last]->SetAnimRange(HALF_DANGER_HEART.first, HALF_DANGER_HEART.second);
		}
		else {
			Life[last]->SetAnimRange(HALF_NOR_HEART, HALF_NOR_HEART);
		}
	}
	int kill = mPlayer->GetKill();
	if (kill > 999) {
		kill = 999;
	}
	if (kill >= pow(10, (int)Kill.size())) {
		Actor* Display = NULL;
		Kill.emplace_back(SetChangeInfo(Display, kill % 10, (5.0+(double)Kill.size()) * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX), 0.06));
	}
	for (int i = 0; i < (int)Kill.size(); i++) {
		int digit = (kill / static_cast<int>(pow(10, (int)Kill.size() - (i + 1)))) % 10;
		Kill[i]->SetAnimRange(digit, digit);
	}
	int left = (int)EntityList[1].size();
	if (left >= pow(10, (int)Left.size())) {
		Actor* Display = NULL;
		Left.emplace_back(SetChangeInfo(Display, left % 10, (5.0 + (double)Left.size()) * ONEBOX / 2, Dir_Vector(ONEBOX * 2, ONEBOX / 2 * 3), 0.06));
	}
	for (int i = 0; i < (int)Left.size(); i++) {
		int digit = (left / static_cast<int>(pow(10, (int)Left.size() - (i + 1)))) % 10;
		Left[i]->SetAnimRange(digit, digit);
	}
	//Heal
	string numOfHeal = to_string(GetGame()->getMenu()->getUpgradeLevel(ITEM_HEAL));
	currentSize = Heal.size();
	afterSize = numOfHeal.length();
	for (int i = 0; i < afterSize; i++) {
		if (i >= currentSize) {
			Actor* Display = new Actor(GetGame());
			Heal_Actor.emplace_back(Display);
			Heal.emplace_back(SetMoveInfo(Display, numOfHeal[i] - '0', i * ONEBOX, Dir_Vector(ONEBOX * 2.5  - afterSize * ONEBOX / 2+ Game::getWidth() - ONEBOX * 4, Game::getHeight() - ONEBOX * 2.5), 0.12));
		}
		else {
			Heal_Actor[i]->SetPosition(Dir_Vector(ONEBOX * 2.5 - afterSize * ONEBOX / 2 + Game::getWidth() - ONEBOX * 4 + i * ONEBOX, Game::getHeight() - ONEBOX * 2.5));
			Heal[i]->ChangeTexture(numOfHeal[i] - '0');
		}
	}
	while (afterSize < currentSize) {
		currentSize--;
		Heal_Actor.back()->SetState(State::Dead);
		Heal_Actor.pop_back();
		Heal.pop_back();
	}
	if (atoi(numOfHeal.c_str()) == 0) {
		Heal_Pic->ChangeTexture(HEAL_COMP_TEX);
	}
	else {
		Heal_Pic->ChangeTexture(HEAL_TEX);
	}
}

SpriteComponent* Camera::SetMoveInfo(Actor* Display, int place, double gap, Dir_Vector first, double scale) {
	Display->SetScale(scale);
	Display->SetPosition(first);
	SpriteComponent* nDisp = new SpriteComponent(Display, GetGame()->GetShader(), 1180);
	Display->SetPosition(Display->GetPosition() + Dir_Vector(gap, 0));
	nDisp->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, place);
	return nDisp;
}

void Camera::StopAllEnemies() {
	while ((int)EntityList[1].size() > 0) {
		Entity* del = EntityList[1].back();
		del->SetState(State::Paused);
		EntityList[1].pop_back();
	}
}

//CreateResultDisplay
void Camera::CreateGameOver() {
	StopAllEnemies();
	SunOrMoon->SetState(State::Paused);
	//Calculate SumScore
	int revTime = mPlayer->GetNoReverse();
	int damageTime = mPlayer->getNoDamage();
	int sumOfKills = mPlayer->GetKill();
	double score = ((double)sumOfKills - lapKill) * 100.0;
	//É_ÉÅÅ[ÉWÇéÛÇØÇΩâÒêîÇ™10âÒÇ…Ç»ÇÈÇ…Ç¬ÇÍÇƒî{ó¶Ç™2->1î{Ç…ÇªÇÃå„ÇÕ100âÒÇ…Ç»ÇÈÇ…Ç¬ÇÍÇƒî{ó¶Ç™1->0.6î{Ç…Ç»ÇÈ
	if (damageTime < 10) {
		score *= 2.0 - damageTime / 10;
	}
	else {
		score *= 1.0 - (damageTime - 10) / 225;
	}
	//îΩì]âÒêîÇ™20âÒÇ…Ç»ÇÈÇ…Ç¬ÇÍÇƒî{ó¶Ç™1.5->1î{Ç…ÇªÇÃå„ÇÕ100âÒÇ…Ç»ÇÈÇ…Ç¬ÇÍÇƒî{ó¶Ç™1->0.7î{Ç…Ç»ÇÈ
	if (revTime < 20) {
		score *= 1.5 - revTime / 40;
	}
	else {
		score *= 1.0 - (revTime - 20) / 266;
	}
	//ÉXÉeÅ[ÉWÉNÉäÉAéûä‘Ç™100ïbÇ…ãﬂÇ√Ç≠Ç…Ç¬ÇÍÇƒî{ó¶Ç™1.5->1î{Ç…ÇªÇÃå„ÇÕ1î{ÇÃÇ‹Ç‹
	if (stageSumTime < 100) {
		score *= 1.5 - stageSumTime / 200;
	}
	if (GetEntity(1).size() == 0) {
		score *= 1.5;
	}
	sumScore += static_cast<long long>(score);
	double scale = (Game::getWidth() + ONEBOX) / RESULTSIZE_CORRECTOR;
	Actor* white = new Actor(GetGame());
	white->SetScale(scale);
	white->SetPosition(Dir_Vector((Game::getWidth() - scale * RESULTSIZE_CORRECTOR) / 2 ,-ONEBOX * 10));
	SpriteComponent* wAnim = new SpriteComponent(white, GetGame()->GetShader(), 1500);
	wAnim->SetTexture(mapID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, 11);
	Actor* Display = NULL;
	int length = 8;
	//GAMEOVER
	SetLastInfo(Display, ALPHA_G, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetLastInfo(Display, ALPHA_A, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetLastInfo(Display, ALPHA_M, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetLastInfo(Display, ALPHA_E, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetLastInfo(Display, ALPHA_O, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetLastInfo(Display, ALPHA_V, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetLastInfo(Display, ALPHA_E, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	SetLastInfo(Display, ALPHA_R, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX));
	//STAGE
	length = 5;
	SetLastInfo(Display, ALPHA_S, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetLastInfo(Display, ALPHA_T, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetLastInfo(Display, ALPHA_A, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetLastInfo(Display, ALPHA_G, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	SetLastInfo(Display, ALPHA_E, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 3));
	//STAGE_NUM
	if (curStage > 9999) {
		curStage = 9999;
	}
	length = to_string(curStage).size();
	for (int i = 0; i < length; i++) {
		int digit = (curStage / static_cast<int>(pow(10, length - (i + 1)))) % 10;
		SetLastInfo(Display, digit, i * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 4.5));
	}
	//KILL
	length = 4;
	SetLastInfo(Display, ALPHA_K, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 6));
	SetLastInfo(Display, ALPHA_I, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 6));
	SetLastInfo(Display, ALPHA_L, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 6));
	SetLastInfo(Display, ALPHA_L, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 6));
	//KILL_NUM
	int kill = mPlayer->GetKill();
	if (kill > 9999) {
		kill = 9999;
	}
	length = to_string(kill).size();
	for (int i = 0; i < length; i++) {
		int digit = (kill / static_cast<int>(pow(10, length - (i + 1)))) % 10;
		SetLastInfo(Display, digit, i * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 7.5));
	}
	//TIME
	length = 4;
	SetLastInfo(Display, ALPHA_T, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetLastInfo(Display, ALPHA_I, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetLastInfo(Display, ALPHA_M, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	SetLastInfo(Display, ALPHA_E, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 9));
	//TIME_NUM
	double time = Game::getTime();
	int hour = static_cast<int>(time / 3600), min = static_cast<int>(time / 60) % 60, sec = static_cast<int>(time) % 60, mil = static_cast<int>(time * 100) % 100;
	if (hour >= 100) {
		hour = 99, min = 99, sec = 99, mil = 99;
	}
	int Atime = hour;
	Atime = Atime * 100 + min;
	Atime = Atime * 100 + sec;
	Atime = Atime * 100 + mil;
	length = to_string(Atime).size();
	if (length % 2 != 0) {
		length++;
	}
	int sumsize = length + length / 2 - 1;
	for (int i = 0; i < length; i++) {
		int digit = (Atime / static_cast<int>(pow(10, length - (i + 1)))) % 10;
		SetLastInfo(Display, digit, i * ONEBOX + (i / 2) * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * sumsize / 2, ONEBOX * 10.5));
		if (i % 2 == 0 && i > 0) {
			SetLastInfo(Display, COLON, ((double)i + (i / 2.0 - 1)) * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * sumsize / 2, ONEBOX * 10.5));
		}
	}
	//SCORE
	length = 5;
	SetLastInfo(Display, ALPHA_S, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetLastInfo(Display, ALPHA_C, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetLastInfo(Display, ALPHA_O, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetLastInfo(Display, ALPHA_R, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	SetLastInfo(Display, ALPHA_E, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 12));
	//SCORE_NUM
	string scoreStr = to_string(sumScore);
	length = scoreStr.length();
	for (int i = 0; i < length; i++) {
		SetLastInfo(Display, scoreStr[i] - '0', i* ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 13.5));
	}

	//PRESS_ESC_TO_END
	length = 16;
	SetLastInfo(Display, ALPHA_P, 0 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_R, 1 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_E, 2 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_S, 3 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_S, 4 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, NULL_PIC, 5 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_E, 6 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_S, 7 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_C, 8 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, NULL_PIC, 9 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_T, 10 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_O, 11 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, NULL_PIC, 12 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_E, 13 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_N, 14 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
	SetLastInfo(Display, ALPHA_D, 15 * ONEBOX, Dir_Vector(Game::getWidth() / 2 - ONEBOX * length / 2, ONEBOX * 15));
}

void Camera::SetLastInfo(Actor* Display, int place, double gap, Dir_Vector first) {
	Display = new Actor(GetGame());
	Display->SetScale(0.12);
	Display->SetPosition(first);
	SpriteComponent* nDisp = new SpriteComponent(Display, GetGame()->GetShader(), 1700);
	Display->SetPosition(Display->GetPosition() + Dir_Vector(gap, 0));
	nDisp->SetTexture(litsID, TEXWIDTH, TEXHEIGHT, SIZEX, SIZEY, place);
}

int Camera::GetStageScore() {
	//1ÉXÉeÅ[ÉWÉNÉäÉAÇ≤Ç∆Ç…äÓëbÉXÉRÉAÇ™200ÉAÉbÉvÇ∑ÇÈ(äÓñ{ÇÕ100)
	double score = BASIC_SCORE + (double)INCREASE_SCORE * curStage;
	int revTime = mPlayer->GetNoReverse();
	int damageTime = mPlayer->getNoDamage(); 
	int sumOfKills = mPlayer->GetKill(); 
	score += ((double)sumOfKills - lapKill) * 100.0;
	//É_ÉÅÅ[ÉWÇéÛÇØÇΩâÒêîÇ™10âÒÇ…Ç»ÇÈÇ…Ç¬ÇÍÇƒî{ó¶Ç™2->1î{Ç…ÇªÇÃå„ÇÕ100âÒÇ…Ç»ÇÈÇ…Ç¬ÇÍÇƒî{ó¶Ç™1->0.6î{Ç…Ç»ÇÈ
	if (damageTime < 10) {
		score *= 2.0 - damageTime / 10;
	}
	else {
		score *= 1.0 - (damageTime - 10) / 225;
	}
	//îΩì]âÒêîÇ™20âÒÇ…Ç»ÇÈÇ…Ç¬ÇÍÇƒî{ó¶Ç™1.5->1î{Ç…ÇªÇÃå„ÇÕ100âÒÇ…Ç»ÇÈÇ…Ç¬ÇÍÇƒî{ó¶Ç™1->0.7î{Ç…Ç»ÇÈ
	if (revTime< 20) {
		score *= 1.5 - revTime / 40;
	}
	else {
		score *= 1.0 - (revTime - 20) / 266;
	}
	//ÉXÉeÅ[ÉWÉNÉäÉAéûä‘Ç™100ïbÇ…ãﬂÇ√Ç≠Ç…Ç¬ÇÍÇƒî{ó¶Ç™1.5->1î{Ç…ÇªÇÃå„ÇÕ1î{ÇÃÇ‹Ç‹
	if (stageSumTime < 100) {
		score *= 1.5 - stageSumTime / 200;
	}
	if (GetEntity(1).size() == 0) {
		score *= 1.5;
	}
	mPlayer->addScore(static_cast<int>(score));
	sumScore += static_cast<long long>(score);
	return static_cast<int>(score);
}