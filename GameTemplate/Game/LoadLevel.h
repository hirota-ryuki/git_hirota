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
	Level m_level;						//���x���B
	BackGround* m_background = nullptr;	//BackGround�N���X�̃|�C���^�B
	Floor* m_floor = nullptr;			//Floor�N���X�̃|�C���^�B
	Player* m_player = nullptr;			//Player�N���X�̃|�C���^�B
	Zombie* m_zombie = nullptr;			//Zombie�N���X�̃|�C���^�B
};

