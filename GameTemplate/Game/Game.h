#pragma once

class Player;
class GameCamera;
class GameObjectManager;

class Game :
	public IGameObject
{
public:
	Game();
	~Game();
	void Update() override;
	void Draw() override;
	
	static Game* GetGame()
	{
		return m_game;
	}
	Player* GetPlayer()
	{
		return m_player;
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