#pragma once
#include "level/Level.h"

class BackGround;
class Floor;
class Player;
class Zombie;

class LoadLevel :
	public IGameObject
{
public:
	LoadLevel();
	~LoadLevel();
	void Update() override;
	void Draw() override {};
private:
	Level m_level;						//レベル。
	BackGround* m_background = nullptr;	//BackGroundクラスのポインタ。
	Floor* m_floor = nullptr;			//Floorクラスのポインタ。
	Player* m_player = nullptr;			//Playerクラスのポインタ。
	Zombie* m_zombie = nullptr;			//Zombieクラスのポインタ。
};

