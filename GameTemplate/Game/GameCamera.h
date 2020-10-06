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
	void Update_NotPause() override;
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
	const CVector3& GetPos() const
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
	const CVector3& GetTarget() const
	{
		return m_target;
	}
	/// <summary>
	/// プレイヤーから注視点をセット。
	/// </summary>
	void SetTargetFromPlayer(const float& targetFromPlayer)
	{
		m_targetFromPlayer = targetFromPlayer;
	}
	/// <summary>
	/// プレイヤーから注視点を取得。
	/// </summary>
	/// <returns>プレイヤーから注視点。</returns>
	const float& GetTargetFromPlayer() const
	{
		return m_targetFromPlayer;
	}
	/// <summary>
	/// 注視点から視点をセット。
	/// </summary>
	void SetPosFromTarget(const float& posFromTarget)
	{
		m_posFromTarget = posFromTarget;
	}
	/// <summary>
	/// 注視点から視点を取得。
	/// </summary>
	/// <returns>注視点から視点。</returns>
	const float& GetPosFromTarget() const
	{
		return m_posFromTarget;
	}
	/// <summary>
	/// 視点を地面から上昇させる量をセット。
	/// </summary>
	void SetAddY(const float& playerPosAddY)
	{
		m_playerPosAddY = playerPosAddY;
	}
	/// <summary>
	/// 視点を地面から上昇させる量を取得。
	/// </summary>
	/// <returns>視点を地面から上昇させる量。</returns>
	const float& GetAddY() const
	{
		return m_playerPosAddY;
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
	float			m_toCameraPosRotAngle = 0.f;			//視点の角度。
	float			m_toCameraTargetRotAngle = 0.f;			//注視点の角度。
	const float		POS_ANGLE_UP_LIMIT = 60.0f;				//視点の上の限界値。
	const float		POS_ANGLE_DOWN_LIMIT = -70.0f;			//視点の下の限界値。
	float			m_targetFromPlayer = 100.0f;			//プレイヤーから注視点。
	float			m_posFromTarget = -200.0f;				//注視点から視点。
	float			m_playerPosAddY = 150.0f;				//視点を地面から上昇させる量。
	SpringCamera	m_springCamera;							//ばねカメラ。
};
