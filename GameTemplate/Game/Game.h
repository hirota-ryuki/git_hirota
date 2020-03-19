#pragma once
#include "level/Level.h"

class GameCamera;
class Floor;
class Ceiling;
class Player;
class Goal;
class Zombie;
class Ball;
class RockDoor;
class BulletStack;
class Result;
class Pose;
class Opening;
class UI;
class Map;

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
	/// ポーズメニューのインスタンスを取得。
	/// </summary>
	/// <returns>ポーズメニューのインスタンス</returns>
	Pose* GetPose()
	{
		return m_pose;
	}
	/// <summary>
	/// UIのインスタンスを取得。
	/// </summary>
	/// <returns>UIのインスタンス</returns>
	UI* GetUI()
	{
		return m_ui;
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
	/// <summary>
	/// 一時停止したいときに呼び出す関数。
	/// </summary>
	void ChangePose(){
		m_isPose = !m_isPose;
	}
	/// <summary>
	/// 一時停止したいときに呼び出す関数。
	/// trueなら一時停止、falseなら一時停止解除。
	/// </summary>
	void IsPose(bool flag){
		m_isPose = flag;
	}
	/// <summary>
	/// 一時停止しているかどうかを取得。
	/// </summary>
	/// <returns>一時停止しているかどうか。</returns>
	bool GetIsPose() {
		return m_isPose;
	}
private:
	//インスタンス関係。
	static Game*	m_game;					//Gameクラスのインスタンス。
	GameCamera*		m_gamecamera = nullptr;	//GameCameraクラスのポインタ。
	Level			m_level;				//レベル。
	Floor*			m_floor = nullptr;		//Floorクラスのポインタ。
	Ceiling*		m_ceiling = nullptr;	//Ceilingクラスのポインタ。
	Player*			m_player = nullptr;		//Playerクラスのポインタ。
	Goal*			m_goal = nullptr;		//Goalクラスのポインタ。
	Zombie*			m_zombie = nullptr;		//Zombieクラスのポインタ。
	Ball*			m_ball = nullptr;		//Ballクラスのポインタ。
	RockDoor*		m_rockdoor = nullptr;	//RockDoorクラスのポインタ。
	BulletStack*	m_bs = nullptr;			//BulletStackクラスのポインタ。
	Pose*			m_pose = nullptr;		//Poseクラスのポインタ。
	Opening*		m_op = nullptr;			//Openingクラスのポインタ。
	Result*			m_result = nullptr;		//Resultクラスのポインタ。
	UI*				m_ui = nullptr;			//UIクラスのポインタ。
	Map*			m_map = nullptr;		//Mapクラスのポインタ。
	//状態関係。
	bool m_isClear = false;				//クリアしたかどうか。
	bool m_isGameOver = false;			//ゲームオーバーしたかどうか。
	bool m_isPose = false;				//一時停止したかどうか。
};

static inline Game* GetGame()
{
	return Game::GetGame();
}