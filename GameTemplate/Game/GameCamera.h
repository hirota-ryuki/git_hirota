#pragma once
#include "SpringCamera.h"

class Game;
class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// 視点をセット。
	/// </summary>
	void SetPos(const CVector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// 視点を取得。
	/// </summary>
	/// <returns>視点。</returns>
	CVector3 GetPos()
	{
		return m_pos;
	}
	/// <summary>
	/// 注視点をセット。
	/// </summary>
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	/// <summary>
	/// 注視点を取得。
	/// </summary>
	/// <returns>注視点。</returns>
	CVector3 GetTarget()
	{
		return m_target;
	}
	/// <summary>
	/// 注視点をセット。
	/// </summary>
	void SetTargetFromPos(const float& targetFromPos)
	{
		m_targetFromPos = targetFromPos;
	}
	/// <summary>
	/// 注視点を取得。
	/// </summary>
	/// <returns>注視点。</returns>
	float GetTargetFromPos()
	{
		return m_targetFromPos;
	}
	/// <summary>
	/// 注視点をセット。
	/// </summary>
	void SetTargetFromPlayer(const float& targetFromPlayer)
	{
		m_targetFromPlayer = targetFromPlayer;
	}
	/// <summary>
	/// 注視点を取得。
	/// </summary>
	/// <returns>注視点。</returns>
	float GetTargetFromPlayer()
	{
		return m_targetFromPlayer;
	}
private:
	//インスタンス関係。
	Game*			m_game = nullptr;						//ゲームのポインタ。
	Player*			m_player = nullptr;						//プレイヤーのポインタ。
	//カメラ関係。
	CVector3		m_toCameraTarget = CVector3::Zero();	
	CVector3		m_toCameraPos = CVector3::Zero();		
	CVector3		m_target = CVector3::Zero();			//注視点。
	CVector3		m_pos = CVector3::Zero();				//視点。
	float			toCameraPosRotAngle = 0.f;				//視点の角度。
	float			toCameraTargetRotAngle = 0.f;			//注視点の角度。
	//float			m_targetFromPos = -1100.0f;			//注視点の角度。
	//float			m_targetFromPlayer = 1000.0f;			//注視点の角度。
	float			m_targetFromPos = -1100.0f;			//注視点の角度。
	float			m_targetFromPlayer = 1000.0f;			//注視点の角度。
	bool			m_isMapMode = false;
	//ばねカメラ。
	SpringCamera m_springCamera; //ばねカメラ。
};
