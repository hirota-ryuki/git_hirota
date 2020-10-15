#pragma once
#include "IZombieComponent.h"
class ZombieStateMachine :
	public IZombieComponent
{
public:
	void Update()override;
private:
	//ステート関係。
	enum State {
		enState_idle,
		enState_walk,
		enState_attack,
		enState_bite,
		enState_knockback,
		enState_death,
		enState_num,
	};
};

