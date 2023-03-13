#pragma once
#include <iostream>
#include <algorithm>
#include "SpriteComponent.h"
//アニメーションを含む描画内容の管理
class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner,GLint shader, int draworder = 100);
	void Update(double delta) override;
	//Setting Textures(1 textures, separating texture with offset, size and the number of animation)
	void SetAnimTextures(GLuint texture,int width, int height, int start, int end, int size_x, int size_y ,bool is_Roop, double animFps = 10.0);
	void SetAnimRange(int s, int e) {
		if (animRange.first == s && animRange.second == e) return; else { animRange = { s, e }; mCurrFrame = 0.0; }
	}
	double GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(double fps) { mAnimFPS = fps; }
private:
	std::pair<int, int> animRange;
	double mAnimFPS;
	double mCurrFrame;
	bool contflag = true;

};