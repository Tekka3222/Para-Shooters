#include "SpriteComponent.h"
#include <iostream>
#include <algorithm>

SpriteComponent::SpriteComponent(class Actor* owner,GLint shader, int drawOrder)
	:Component(owner)
    ,mGameState(owner->GetGameState())
	,mTextureID(0)
    ,mShader(shader)
	,mDrawOrder(drawOrder)
	,mTextureWidth(512)
	,mTextureHeight(512)
	,mTexSizeX(32)
	,mTexSizeY(32)
	,mTexPlacement(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->RemoveSprite(this);
}


void SpriteComponent::Draw() {
    double gx = Game::getWidth();
    double gy = Game::getHeight();
    double scale = mOwner->GetScale();
    Dir_Vector pos = mOwner->GetPosition();
    double longer = std::max(mTexSizeX, mTexSizeY);
    double magX = mTexSizeX / longer;
    double magY = mTexSizeY / longer;
    double psx = (pos.x);
    double psy = (pos.y);
    double pex = (pos.x + mTextureWidth * magX* scale - 1.0);
    double pey = (pos.y + mTextureHeight * magY * scale - 1.0);
    Dir_Vector rec[5];
    rec[0] = Dir_Vector(pex, psy);
    rec[1] = Dir_Vector(psx, psy);
    rec[2] = Dir_Vector(psx, pey);
    rec[3] = Dir_Vector(pex, pey);
    rec[4] = Dir_Vector((pex + psx) / 2, (pey + psy) / 2);
    double rot = mOwner->GetRotate();
    for (int i = 0; i < 4; i++) {
        Dir_Vector tmp = rec[i] - rec[4];
        tmp.rotate(rot); 
        rec[i] = Dir_Vector((tmp.x + rec[4].x) / gx * 2 - 1.0, ((tmp.y + rec[4].y) / gy * 2 - 1.0) * (-1));
    }
    GLdouble points[12] = {
        rec[0].x, rec[0].y, 0.0,
        rec[1].x, rec[1].y, 0.0,
        rec[2].x, rec[2].y, 0.0,
        rec[3].x, rec[3].y, 0.0,
    };
    int numOfRow = mTextureWidth / mTexSizeX;
    double tsx = ((double)mTexSizeX * (mTexPlacement % numOfRow) + 1.0) / (double)mTextureWidth;
    double tsy = ((double)mTexSizeY * (mTexPlacement / numOfRow) + 1.0) / (double)mTextureHeight;
    double tex = ((double)mTexSizeX * ((double)(mTexPlacement % numOfRow) + 1.0) - 1.0) / (double)mTextureWidth;
    double tey = ((double)mTexSizeY * ((double)(mTexPlacement / numOfRow) + 1.0) - 1.0) / (double)mTextureHeight;

    GLdouble vertex_uv[8] = {
        tex, tsy,
        tsx, tsy,
        tsx, tey,
        tex, tey
    };

    GLuint vao, vertex_vbo, uv_vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    //VBO for texture
    glGenBuffers(1, &uv_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_uv), vertex_uv, GL_STATIC_DRAW);

    int textureLocation = glGetUniformLocation(mShader, "texture");
    glUseProgram(mShader);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
    glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 0, (GLvoid*)0);
    glUniform1i(textureLocation, 0);
    glDepthMask(GL_FALSE);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDepthMask(GL_TRUE);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vertex_vbo);
    glDeleteBuffers(1, &uv_vbo);
}

void SpriteComponent::SetTexture(GLuint texture,int width, int height, int tex_x, int tex_y, int placement) {
    mTextureID = texture;
	mTextureWidth = width;
	mTextureHeight = height;
	mTexSizeX = tex_x;
	mTexSizeY = tex_y;
	mTexPlacement = placement;
}