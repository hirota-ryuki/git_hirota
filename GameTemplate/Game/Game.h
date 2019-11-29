#pragma once

class Player;
class GameCamera;
class BackGround;
class Navimesh;

class Game :
	public IGameObject
{
public:
	Game();
	~Game();
	void Update() override {};
	void Draw() override {};
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
	/// �w�i�̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�w�i�̃C���X�^���X</returns>
	BackGround* GetBackGround()
	{
		return m_background;
	}
	/*Navimesh* GetNavi()
	{
		return m_navimesh;
	}*/
private:
	static Game* m_game;
	Player* m_player = nullptr;			//Player�N���X�̃|�C���^
	GameCamera* m_gamecamera = nullptr;	//GameCamera�N���X�̃|�C���^
	BackGround* m_background = nullptr;	//BackGround�N���X�̃|�C���^
	Navimesh* m_navimesh = nullptr;		//Navimesh�N���X�̃|�C���^
};

static inline Game* GetGame()
{
	return Game::GetGame();
}