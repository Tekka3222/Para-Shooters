#pragma once
//コンポーネント全体の親クラス
class Component {
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update(double delta);
	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner;
	int mUpdateOrder;
};