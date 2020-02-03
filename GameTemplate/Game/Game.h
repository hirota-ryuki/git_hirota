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
	/// 床のインスタンスを取得。
	/// </summary>
	/// <returns>床のインスタンス</returns>
	Floor* GetFloor()
	{
		return m_floor;
	}
	/// <summary>
	/// クリアしたときに呼び出す関数。
	/// </summary>
	void Clear(){
		m_isClear = true;
	}
	/// <summary>
	/// ゲームオーバーしたときに呼び出す関数。
	/// </summary>
	void GameOver(){
		m_isGameOver = true;
	}
private:
	static Game* m_game;
	GameCamera* m_gamecamera = nullptr;	//GameCameraクラスのポインタ。
	Level m_level;						//レベル。
	Floor* m_floor = nullptr;			//Floorクラスのポインタ。
	Player* m_player = nullptr;			//Playerクラスのポインタ。
	Goal* m_goal = nullptr;				//Goalクラスのポインタ。
	Zombie* m_zombie = nullptr;			//Zombieクラスのポインタ。
	bool m_isClear = false;				//クリアしたかどうか。
	bool m_isGameOver = false;			//ゲームオーバーしたかどうか。
};

static inline Game* GetGame()
{
	return Game::GetGame();
}