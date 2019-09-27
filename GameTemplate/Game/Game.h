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
	/// ゲームのインスタンスを取得。
	/// </summary>
	/// <returns>ゲームのインスタンス</returns>
	static Game* GetGame()
	{
		return m_game;
	}
	/// <summary>
	/// プレイヤーのインスタンスを取得。
	/// </summary>
	/// <returns>プレイヤーのインスタンス</returns>
	Player* GetPlayer()
	{
		return m_player;
	}
	/// <summary>
	/// カメラのインスタンスを取得。
	/// </summary>
	/// <returns>カメラのインスタンス</returns>
	GameCamera* GetCamera()
	{
		return m_gamecamera;
	}
private:
	static Game* m_game;
	Player* m_player = nullptr;			//プレイヤークラスのポインタ
	GameCamera* m_gamecamera = nullptr;	//ゲームカメラクラスのポインタ
};

static inline Game* GetGame()
{
	return Game::GetGame();
}