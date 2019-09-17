#pragma once

class Player;
class GameCamera;
class GameObjectManager;

class Game :
	public IGameObject
{
private:
	
	
public:
	Game();
	~Game();
	void Update() override;
	void Draw() override;
	/// <summary>
	/// Game�̃C���X�^���X�����B
	/// </summary>
	/*static void Create()
	{
		if (!m_game){
			m_game = g_goMgr.NewGO<Game>();
		}
	}
	/// <summary>
	/// Game�̃C���X�^���X�������B
	/// </summary>
	static void Destroy()
	{
		g_goMgr.DeleteGO(m_game);
		m_game = nullptr;
	}*/
	/// <summary>
	/// Game�̃C���X�^���X���擾����B
	/// </summary>
	/// <returns></returns>
	static Game& GetInstance()
	{
		/*static Game gameinst;
		return gameinst;
		if (m_game == nullptr) {
			m_game = new Game;
		}
		return m_game;*/
	}
	Player* GetPlayer()
	{
		return m_player;
	}
private:
	Player* m_player;	//�v���C���[�N���X�̃|�C���^
	GameCamera* m_gamecamera;	//�Q�[���J�����N���X�̃|�C���^
	Game* m_game = nullptr;
};
//Game* game = new Game;

class GameData {
public:
	GameData();
	~GameData();
	Game* GetGame() {
		if (m_game == nullptr) {
			m_game = new Game;
		}
		return m_game;
	}
private:
	Game* m_game = nullptr;
};
GameData gamedata;
Game* m_game = gamedata.GetGame();
Player* player = m_game->GetPlayer();
