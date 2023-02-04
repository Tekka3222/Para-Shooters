#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateorder) 
	:mOwner(owner)
	,mUpdateOrder(updateorder)
{
	mOwner->AddComponent(this);
}

Component::~Component() {
	mOwner->RemoveComponent(this);
}

void Component::Update(double delta) {
}
