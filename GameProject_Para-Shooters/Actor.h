#pragma once
#include "vector.h"
#include "Game.h"
#include "Component.h"
#include <vector>


//Classifying Actor's type from Component Class
//Making Actors from Extending this Class
class Actor {
public:
	enum class State {
		Active,
		Paused,
		Dead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(double delta);
	void UpdateComponents(double delta);
	virtual void UpdateActor(double delta);

	const Dir_Vector& GetPosition() const { return mVector; }
	void SetPosition(const Dir_Vector& vec) { mVector = vec; }
	double GetScale() const { return mScale; }
	void SetScale(double scale) { mScale = scale; }
	const Dir_Vector& GetFront() const { return mFront; }
	void SetFront(const Dir_Vector& vec) { mFront = vec; }
	double GetRotate() const { return mRot; }
	void SetRotate(double rotate) { mRot = rotate; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }
	Game::GameState GetGameState() const { return mGameState; }

	class Game* GetGame() const { return mGame; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	State mState;
	Game::GameState mGameState;

	Dir_Vector mVector,mFront;
	double mScale;
	double mRot;
	std::vector<class Component*> mComponents;
	class Game* mGame;
};