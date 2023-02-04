#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner,GLint shader, int draworder) 
	:SpriteComponent(owner,shader,draworder)
	,animRange(0,0)
	,mAnimFPS(10.0)
	,mCurrFrame(0.0)
{

}

void AnimSpriteComponent::Update(double delta) {
	SpriteComponent::Update(delta);
	if (animRange.first < animRange.second) {
		mCurrFrame += mAnimFPS * delta;
		while (mCurrFrame >= (double)animRange.second - (double)animRange.first + 1.0) {
			if (contflag) {
				mCurrFrame -= (double)animRange.second - (double)animRange.first + 1.0;
			}
			else {
				mCurrFrame = (double)animRange.second - (double)animRange.first;
			}
		}
		int cur = static_cast<int>(mCurrFrame) + animRange.first;
		SetTexture(mTextureID, mTextureWidth, mTextureHeight, mTexSizeX, mTexSizeY, cur);
	}
	else SetTexture(mTextureID, mTextureWidth, mTextureHeight, mTexSizeX, mTexSizeY, animRange.first);
}

void AnimSpriteComponent::SetAnimTextures(GLuint texture,int width, int height, int size_x, int size_y, int start, int end, bool is_Roop, double animFPS) {
	SetTexture(texture,width,height,size_x,size_y,start);
	mTextureID = texture;
	mTextureWidth;
	mTextureHeight;
	mTexSizeX = size_x;
	mTexSizeY = size_y;
	animRange.first = start;
	animRange.second = end;
	contflag = is_Roop;
	mAnimFPS = animFPS;
}
