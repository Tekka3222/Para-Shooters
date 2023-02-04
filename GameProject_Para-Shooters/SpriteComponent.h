#pragma once
#include "Actor.h"
#include "Component.h"
#include "GLFW/glfw3.h"

class SpriteComponent : public Component {
public:
	SpriteComponent(class Actor* owner,GLint shader, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw();
	virtual void SetTexture(GLuint texture, int width, int height, int tex_x, int tex_y, int placement);

	void ChangeTexture(int placement) { mTexPlacement = placement; }
	int GetDrawOrder() const { return mDrawOrder; }
	Game::GameState GetGameState() { return mGameState; }
	int GetTexWidth() const { return mTextureWidth; }
	int GetTexHeight() const { return mTextureHeight; }
protected:
	Game::GameState mGameState;
	GLuint mTextureID;
	GLint mShader;
	int mDrawOrder;
	int mTextureWidth;
	int mTextureHeight;
	int mTexSizeX;
	int mTexSizeY;
	int mTexPlacement;
};