#pragma once
#include "level/Level.h"

class GameCamera;
class Floor;
class Player;
class Goal;
class Zombie;

class Game :
	public IGameObject
{
public:
	Game();
	~Game();	
	void OnDestroy() override;
	bool Start() override;
	void Update() override ;
	/// <summary>
	/// �Q�[���̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�Q�[���̃C���X�^���X</returns>
	static Game* GetGame()
	{
		return m_game;
	}
	/// <summary>
	/// �v���C���[�̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�v���C���[�̃C���X�^���X</returns>
	Player* GetPlayer()
	{
		return m_player;
	}
	/// <summary>
	/// �J�����̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�J�����̃C���X�^���X</returns>
	GameCamera* GetCamera()
	{
		return m_gamecamera;
	}
	/// <summary>
	/// ���̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>���̃C���X�^���X</returns>
	Floor* GetFloor()
	{
		return m_floor;
	}
	/// <summary>
	/// �N���A�����Ƃ��ɌĂяo���֐��B
	/// </summary>
	void Clear(){
		m_isClear = true;
	}
	/// <summary>
	/// �Q�[���I�[�o�[�����Ƃ��ɌĂяo���֐��B
	/// </summary>
	void GameOver(){
		m_isGameOver = true;
	}
private:
	static Game* m_game;
	GameCamera* m_gamecamera = nullptr;	//GameCamera�N���X�̃|�C���^�B
	Level m_level;						//���x���B
	Floor* m_floor = nullptr;			//Floor�N���X�̃|�C���^�B
	Player* m_player = nullptr;			//Player�N���X�̃|�C���^�B
	Goal* m_goal = nullptr;				//Goal�N���X�̃|�C���^�B
	Zombie* m_zombie = nullptr;			//Zombie�N���X�̃|�C���^�B
	bool m_isClear = false;				//�N���A�������ǂ����B
	bool m_isGameOver = false;			//�Q�[���I�[�o�[�������ǂ����B
};

static inline Game* GetGame()
{
	return Game::GetGame();
}