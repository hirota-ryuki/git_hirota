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
	/// <summary>
	/// 背景のインスタンスを取得。
	/// </summary>
	/// <returns>背景のインスタンス</returns>
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
	Player* m_player = nullptr;			//Playerクラスのポインタ
	GameCamera* m_gamecamera = nullptr;	//GameCameraクラスのポインタ
	BackGround* m_background = nullptr;	//BackGroundクラスのポインタ
	Navimesh* m_navimesh = nullptr;		//Navimeshクラスのポインタ
};

static inline Game* GetGame()
{
	return Game::GetGame();
}