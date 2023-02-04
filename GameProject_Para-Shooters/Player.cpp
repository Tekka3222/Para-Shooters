#include "Entity.h"

Player::Player(Game* game)
	: Entity(game)
	, checkG(false)
	, finFlag(false)
	, resultFlag(false)
	, numOfHeal(HEAL_HEALTH)
	, playerScore(0)
	, healInterval(0.0)
{
	entityType = 0;
	health = MAX_HEALTH;
	mAnim = new AnimSpriteComponent(this, game->GetShader(), 150);
	mCam = new Camera(game,this);
	mGun = new Gun(game, this);
	GLuint texID;
	std::string ImURL = "images/";
	int texwidth = BASIC_TEX_SIZE, texheight = BASIC_TEX_SIZE, sizex = TEX_SMALL, sizey = TEX_BIG, start = PLAYER_STAND_RIGHT.first, end = PLAYER_STAND_RIGHT.second;
	glGenTextures(1, &texID);
	game->SetupTexture(&texID, ImURL + "character.raw", texwidth, texheight);
	bool cont = true;
	mAnim->SetAnimTextures(texID,texwidth, texheight, sizex, sizey, start,end,cont,20);
}

void Player::UpdateActor(double delta) {
	if (health <= 0) {
		SetState(State::Paused);
		mCam->CreateGameOver();
		return;
	}
	if (finFlag) {
		finFlag = false;
			health = 0;
	}
	//pos�͂��ꂼ��̍����\���x�N�g��
	if (checkG) {
		checkG = false;
		rev_count += 0.2;
		if (checkGoal()) {
			GetGame()->SetGameState(Game::GameState::Result);
			resultFlag = true;
			return;
		}
	}
	if (healInterval > 0) {
		if (healInterval <= delta) {
			healInterval = 0.0;
		}
		else {
			healInterval -= delta;
		}
	}
	Entity::UpdateActor(delta);
}

void Player::ResetWorld() {
	this->SetState(State::Paused);
	mCam->SetState(State::Paused);
	mGun->SetState(State::Paused);
	ReflectUpgrade();
	revFlag = false;
	isReverse = false;
	isMoving = false;
	isFalling = false;
	mRightSpeed = 0.0;
	mRightAcc = 0.0;
	mFallSpeed = 0.0;
	mFallAcc = FALL_ACC;
	numOfReverse = 0;
	numOfDamage = 0;
	mCam->ResetWorld();
	mGun->ResetWorld();
	rev_count = 0.0;
	ePos.x = ONEBOX * 1.0;
	ePos.y = ONEBOX * 8.0;
	SetPosition(PLAYER_FIRST_POSITION);
	this->SetState(State::Active);
	mCam->SetState(State::Active);
	mGun->SetState(State::Active);
}

void Player::ReverseWorld() {
	whileRevFlag = true;
	Entity::ReverseWorld();
	mCam->ReverseWorld();
	mGun->ReverseWorld();
	whileRevFlag = false;
}	

void Player::SetCharacterTex() {
	if (!isReverse) {
		if (GetFront() == Dir_Vector::Front) {
			if (isMoving && !isFalling)
				mAnim->SetAnimRange(PLAYER_MOVE_RIGHT.first, PLAYER_MOVE_RIGHT.second);
			else
				mAnim->SetAnimRange(PLAYER_STAND_RIGHT.first, PLAYER_STAND_RIGHT.second);
		}
		else {
			if (isMoving && !isFalling)
				mAnim->SetAnimRange(PLAYER_MOVE_LEFT.first, PLAYER_MOVE_LEFT.second);
			else
				mAnim->SetAnimRange(PLAYER_STAND_LEFT.first, PLAYER_STAND_LEFT.second);
		}
	}
	else {
		if (GetFront() == Dir_Vector::Front) {
			if (isMoving && !isFalling)
				mAnim->SetAnimRange(PLAYER_MOVE_RIGHT_REV.first, PLAYER_MOVE_RIGHT_REV.second);
			else
				mAnim->SetAnimRange(PLAYER_STAND_RIGHT_REV.first, PLAYER_STAND_RIGHT_REV.second);
		}
		else {
			if (isMoving && !isFalling)
				mAnim->SetAnimRange(PLAYER_MOVE_LEFT_REV.first, PLAYER_MOVE_LEFT_REV.second);
			else
				mAnim->SetAnimRange(PLAYER_STAND_LEFT_REV.first, PLAYER_STAND_LEFT_REV.second);
		}
	}
	//�ړ����x�ɉ����ăA�j���[�V������FPS��ύX
	if (isMoving && !isFalling) {
		mAnim->SetAnimFPS(abs(mRightSpeed) / ANIMFPS_CORRECTOR);
	}
}

bool Player::checkGoal() {
	Dir_Vector pos = ePos;
	if (mCam->GetMap(static_cast<int>(floor((pos.x + 1) / ONEBOX)), static_cast<int>(floor((pos.y + ONEBOX) / ONEBOX))) == BLOCK_DOOR_GROUND || 
		mCam->GetMap(static_cast<int>(floor((pos.x + ONEBOX - 2) / ONEBOX)), static_cast<int>(floor((pos.y + ONEBOX) / ONEBOX))) == BLOCK_DOOR_GROUND) {
		return true;
	}
	else if (mCam->GetMap(static_cast<int>(floor((pos.x + 1)/ ONEBOX)), static_cast<int>(floor((pos.y) / ONEBOX))) == BLOCK_DOOR_REVERSE ||
		mCam->GetMap(static_cast<int>(floor((pos.x + ONEBOX - 2) / ONEBOX)), static_cast<int>(floor((pos.y) / ONEBOX))) == BLOCK_DOOR_REVERSE) {
		return true;
	}
	else return false;
}

//�A�b�v�O���[�h�̔��f
void Player::ReflectUpgrade() {
	shield = 1.0 * pow(SHIELD_PAR_UPGRADE, GetGame()->getMenu()->getUpgradeLevel(UPGRADE_SHIELD));
	return;
}

void Player::keyInput(GLFWwindow *mWindow, int key, int scancode, int action, int mods) {
	mRightAcc = 0.0;
	mRotSpeed = 0.0;
		int sRight = glfwGetKey(mWindow, GLFW_KEY_RIGHT);
		int sD = glfwGetKey(mWindow, GLFW_KEY_D);
		if (sRight == GLFW_PRESS || sRight == GLFW_REPEAT || sD == GLFW_PRESS || sD == GLFW_REPEAT) {
			mRightAcc += PLAYER_ACC;
		}
		int sLeft = glfwGetKey(mWindow, GLFW_KEY_LEFT);
		int sA = glfwGetKey(mWindow, GLFW_KEY_A);
		if (sLeft == GLFW_PRESS || sLeft == GLFW_REPEAT || sA == GLFW_PRESS || sA == GLFW_REPEAT) {
			mRightAcc -= PLAYER_ACC;
		}
		int sDown = glfwGetKey(mWindow, GLFW_KEY_DOWN);
		int sS = glfwGetKey(mWindow, GLFW_KEY_S);
		if (sDown == GLFW_PRESS || sDown == GLFW_REPEAT || sS == GLFW_PRESS || sS == GLFW_REPEAT) {
			mRotSpeed += PLAYER_ACC_ROTATE;
		}
		int sUp = glfwGetKey(mWindow, GLFW_KEY_UP);
		int sW = glfwGetKey(mWindow, GLFW_KEY_W);
		if (sUp == GLFW_PRESS || sUp == GLFW_REPEAT || sW == GLFW_PRESS || sW == GLFW_REPEAT) {
			mRotSpeed -= PLAYER_ACC_ROTATE;
		}
		int Rev = glfwGetKey(mWindow, GLFW_KEY_E);
		if (Rev == GLFW_PRESS || Rev == GLFW_REPEAT) {
			if (!isFalling && rev_count == 0) {
				checkG = true;
				revFlag = true;
			}
		}
		int Shot = glfwGetKey(mWindow, GLFW_KEY_SPACE);
		if (Shot == GLFW_PRESS || Shot == GLFW_REPEAT) {
			shotflag = true;
		}
		if (Shot == GLFW_RELEASE) {
			shotflag = false;
		}
		int Heal = glfwGetKey(mWindow, GLFW_KEY_Q);
		if (Heal == GLFW_PRESS || Heal == GLFW_REPEAT) {
			if (healInterval == 0.0 && health != MAX_HEALTH) {
				if (GetGame()->getMenu()->healPlayer()) {
					health += HEAL_HEALTH;
					if (health > MAX_HEALTH) {
						health = MAX_HEALTH;
					}
				}
			}
		}
		int Shift = glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT);
		if (Shift == GLFW_PRESS || Shift == GLFW_REPEAT) {
			mRotSpeed /= 2;
		}
		int Reload = glfwGetKey(mWindow, GLFW_KEY_R);
		if (Reload == GLFW_PRESS || Reload == GLFW_REPEAT) {
			reloadflag = true;
		}
		if (key == GLFW_KEY_K && action == GLFW_PRESS) {
			if (MessageBox(NULL, TEXT("�r���I�����܂����H\n�̗͂�0�ɂȂ�܂�"), TEXT("�I���m�F"), MB_YESNO) == IDYES) {
				finFlag = true;
			}
		}
}
