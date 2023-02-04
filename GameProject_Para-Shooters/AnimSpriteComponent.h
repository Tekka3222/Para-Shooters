#pragma once
#include <iostream>
#include <algorithm>
#include "SpriteComponent.h"

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner,GLint shader, int draworder = 100);
	void Update(double delta) override;
	//Setting Textures(1 textures, separating texture with offset, size and the number of animation)
	void SetAnimTextures(GLuint texture,int width, int height, int start, int end, int size_x, int size_y ,bool is_Roop, double animFps = 10.0);
	void SetAnimRange(int s, int e) { animRange = { s, e }; }
	double GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(double fps) { mAnimFPS = fps; }
private:
	std::pair<int, int> animRange;
	double mAnimFPS;
	double mCurrFrame;
	bool contflag = true;

};