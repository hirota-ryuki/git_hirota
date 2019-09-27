#pragma once

class Player;
class GameCamera;

class Game :
	public IGameObject
{
public:
	Game();
	~Game();
	void Update() override;
	void Draw() override;
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
private:
	static Game* m_game;
	Player* m_player = nullptr;			//�v���C���[�N���X�̃|�C���^
	GameCamera* m_gamecamera = nullptr;	//�Q�[���J�����N���X�̃|�C���^
};

static inline Game* GetGame()
{
	return Game::GetGame();
}