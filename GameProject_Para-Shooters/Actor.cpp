#include "Actor.h"
#include <iostream>

Actor::Actor(class Game* game)
	:mState(State::Active)
	, mVector(Dir_Vector::Zero)
	, mFront(Dir_Vector::Front)
	, mScale(1.0)
	, mGame(game)
	, mRot(0.0)
	, mGameState(game->GetGameState())
{
	mGame->AddActor(this);
}

Actor::~Actor() {
	mGame->RemoveActor(this);
	while (!mComponents.empty()) {
		delete mComponents.back();
	}
}

void Actor::Update(double delta) {
	if (mState == State::Active) {
		UpdateComponents(delta);
		UpdateActor(delta);
	}
}

void Actor::UpdateComponents(double delta) {
	for (auto cs : mComponents) {
		cs->Update(delta);
	}
}

//各UpdateActorは、StateがActiveならば毎フレーム実行される
void Actor::UpdateActor(double delta) {

}

//各コンポーネントは昇順でソートされる。(挿入ソート)
void Actor::AddComponent(class Component* component) {
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++) {
		if (myOrder < (*iter)->GetUpdateOrder()){
			break;
		}
	}
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(class Component* component) {
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end()) {
		mComponents.erase(iter);
	}
}