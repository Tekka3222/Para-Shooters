#include "Game.h"
#include "SpriteComponent.h"
#include "Actor.h"
#include "Entity.h"
#include "Camera.h"
#include "Result.h"
#include "Menu.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cassert>
#include <Windows.h>

using namespace std;

Game::Game()
	:mWindow(nullptr)
	, mUpdatingActors(false)
	, mCurTime(glfwGetTime())
	, mCurFPS(0.0)
	, mShader(0)
{
	
}

bool Game::mIsFullScreen = false;
bool Game::mIsRunning = true;
int Game::mWIDTH = 1920;
int Game::mHEIGHT =1080;
double Game::sumTime = 0.0;
Player* Game::mPlayer = nullptr;
Result* Game::mResult = nullptr;
Menu* Game::mMenu = nullptr;
Game::GameState Game::mGameState = Game::GameState::MainGame;

//Input process function
void Game::keyFuncCB(GLFWwindow* mWindow, int key, int scancode, int action, int mods) {
	if (GetGameState() == GameState::MainGame) {
		mPlayer->keyInput(mWindow, key, scancode, action, mods);
	}
	if (GetGameState() == GameState::Menu) {
		mMenu->keyInput(mWindow, key, scancode, action, mods);
	}
	if (action != GLFW_PRESS) {
		return;
	}
	switch(key){
		case GLFW_KEY_ESCAPE:
			if (MessageBox(NULL, TEXT("終了しますか？"), TEXT("終了確認"), MB_YESNO) == IDYES) {
				mIsRunning = false;
				glfwTerminate();
			}
			break;
		case GLFW_KEY_F11:
			if (mIsFullScreen) {
				mIsFullScreen = false;
				glfwSetWindowMonitor(mWindow, NULL, 0, 32, mWIDTH, mHEIGHT, GLFW_DONT_CARE);
				break;
			}
			else {
				mIsFullScreen = true;
				glfwSetWindowMonitor(mWindow, glfwGetPrimaryMonitor(), 0, 0, mWIDTH, mHEIGHT, GLFW_DONT_CARE);
				break;
			}
		case GLFW_KEY_T:
			if (GetGameState() == GameState::MainGame) {
				SetGameState(GameState::Menu);
				mMenu->ResetPrice();
			}
			else if (GetGameState() == GameState::Menu) {
				SetGameState(GameState::MainGame);
			}
			break;
		case GLFW_KEY_ENTER:
			if (GetGameState() == GameState::Result) {
				SetGameState(GameState::MainGame);
				mPlayer->ResetWorld();
			}
	}
	//cout << key << scancode << action << mods << endl;
}

void Game::mouseCB(GLFWwindow* mWindow, double xpos, double ypos) {
	//cout << "Mouse Input" << xpos << " " <<  ypos << endl;
}

void Game::closeCB(GLFWwindow* mWindow) {
	if (MessageBox(NULL, TEXT("終了しますか？"), TEXT("終了確認") , MB_YESNO) == IDYES) {
		mIsRunning = false;
		glfwTerminate();
	}
}

GLint Game::makeShader() {
	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec2 vuv;\n"
		"out vec2 Flag_uv;\n"
		"void main(void) {\n"
		"Flag_uv = vuv;\n"
		"gl_Position =vec4(position, 1.0f);\n"
		"}\n";


	const char* fragment_shader =
		"#version 400\n"
		"in vec2 Flag_uv;"
		"uniform sampler2D Texture;"
		"out vec4 outFragmentColor; \n"
		"void main(void) {\n"
		"outFragmentColor = texture2D(Texture, Flag_uv); \n"
		"}\n";


	GLuint vs, fs;
	GLuint shader_programme;

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	return shader_programme;
}

bool Game::Initialize() {
	if (glfwInit() == GL_FALSE) {
		cerr << "GLFW Initialization failed" << endl;
		return false;
	}

	atexit(glfwTerminate);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	mWindow = glfwCreateWindow(mWIDTH,mHEIGHT,"Para-Shooters", NULL,NULL);
	if (mWindow == NULL) {
		cerr << "GLFW window creation failed" << endl;
		glfwTerminate();
		return false;
	}

	//Input process
	{
		glfwSetKeyCallback(mWindow, Game::keyFuncCB);
		glfwSetWindowCloseCallback(mWindow, Game::closeCB);
		glfwSetCursorPosCallback(mWindow, Game::mouseCB);
	}

	glfwMakeContextCurrent(mWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "GLEW Initialization failed" << endl;
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSwapInterval(1);
	glLoadIdentity();
	glOrtho(0.0f, mWIDTH, 0.0f, mHEIGHT, -1.0f, 1.0f);

	mShader = makeShader();
	LoadData();

	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	glfwPollEvents();
}

void Game::UpdateGame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(mShader);

	double cur = glfwGetTime();
	double dif = cur - mCurTime;
	FrameTimes.insert(FrameTimes.begin(), dif);
	while (FrameTimes.size() > 10) {
		FrameTimes.pop_back();
	}
	mCurTime = cur;
	if(mPlayer->GetState() != Actor::State::Paused && GetGameState() == GameState::MainGame)
		sumTime += dif;
	double delta = 0;
	for (auto fTime : FrameTimes) {
		delta += fTime;
	}
	delta /= FrameTimes.size();
	if (delta > 0.05) {
		delta = 0.05;
	}
	mCurFPS = 1 / delta;
	//cout << "FPS" << 1 / delta << endl;

	mUpdatingActors = true;
	for (auto actor : mActors) {
		if (actor->GetGameState() == GetGameState()) {
			actor->Update(delta);
		}
	}
	mUpdatingActors = false;
	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::State::Dead) {
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors) {
		delete actor;
	}

}

void Game::GenerateOutput() {
	//Back Ground color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (auto sprite : mSprites) {
		if (sprite->GetGameState() == GetGameState()) {
			sprite->Draw();
		}
	}

	glfwSwapBuffers(mWindow);
}

void Game::SetupTexture(GLuint *textureID, string fname, int width, int height) {
	ifstream fstr(fname, ios::binary);
	assert(fstr);

	const size_t fSize = static_cast<size_t>(fstr.seekg(0, fstr.end).tellg());
	fstr.seekg(0, fstr.beg);
	char* textureBuffer = new char[fSize];
	fstr.read(textureBuffer, fSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, *textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	delete[] textureBuffer;
	glBindTexture(GL_TEXTURE_2D, 1);
}

void Game::LoadData() {	
	mPlayer = new Player(this);
	mPlayer->SetPosition(PLAYER_FIRST_POSITION);
	mPlayer->SetScale(0.24);
	SetGameState(GameState::Result);
	mResult = new Result(this, mPlayer);
	SetGameState(GameState::Menu);
	mMenu = new Menu(this, mPlayer);
	SetGameState(GameState::MainGame);
}

void Game::UnloadData() {
	while (!mActors.empty()) {
		delete mActors.back();
	}

	for (auto i : mTextures) {
		glDeleteTextures(1,&i);
	}
	mTextures.clear();
}

void Game::AddActor(class Actor* actor) {
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(class Actor* actor) {
	auto iter = find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sc) {
	int mOrder = sc->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); iter++) {
		if (mOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

	mSprites.insert(iter, sc);
}

void Game::RemoveSprite(SpriteComponent* sc) {
	auto iter = find(mSprites.begin(), mSprites.end(), sc);
	if (iter != mSprites.end()) {
		mSprites.erase(iter);
	}
}