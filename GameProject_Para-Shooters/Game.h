#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "vector.h"

#define PLAYER_FIRST_POSITION Dir_Vector(60.0 * 1.0 ,60.0 * 8.0)
class Game
{
public:
	enum class GameState {
		MainGame,
		Menu,
		Result
	};
	Game();
	bool Initialize();
	void RunLoop();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	static int getWidth() { return mWIDTH; }
	static int getHeight() { return mHEIGHT; }
	static double getTime() { return sumTime; }
	static class Menu* getMenu() { return mMenu; }
	GLint GetShader() const { return mShader; }
	static void StopGame() { mIsRunning = false; }
	static GameState GetGameState() { return mGameState; }
	static void SetGameState(GameState state) { mGameState = state; }
	//width-height must be expressed as 2^m
	void SetupTexture(GLuint* textureID, std::string fname, int width, int height);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	GLint makeShader();


	//Input process function
	static void keyFuncCB(GLFWwindow * mWindow, int key, int scancode, int action, int mods);
	static void mouseCB(GLFWwindow* mWindow, double xpos, double ypos);
	static void closeCB(GLFWwindow * mWindow);

	std::vector<GLuint> mTextures;
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<double> FrameTimes;
	GLFWwindow* mWindow;

	GLint mShader;
	static class Player* mPlayer;
	static class Result* mResult;
	static class Menu* mMenu;
	static bool mIsFullScreen;
	static int mWIDTH, mHEIGHT;
	static bool mIsRunning;
	static double sumTime;
	static GameState mGameState;
	double mCurTime,mCurFPS;
	bool mUpdatingActors;

}; 

