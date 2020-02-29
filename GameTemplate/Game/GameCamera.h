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
	/// 視点を取得。
	/// </summary>
	/// <returns>視点。</returns>
	CVector3 GetPos()
	{
		return m_pos;
	}
	/// <summary>
	/// 注視点を取得。
	/// </summary>
	/// <returns>注視点。</returns>
	CVector3 GetTarget()
	{
		return m_target;
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
	bool			m_isMapMode = false;
	//照準画像関係。
	SpriteRender*	m_sprite = nullptr;
	CQuaternion		m_rot = CQuaternion::SpriteRot();
	SpringCamera m_springCamera; //ばねカメラ。

};
