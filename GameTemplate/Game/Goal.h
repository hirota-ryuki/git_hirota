#pragma once
#include "IGameObject.h"
class Player;
class Goal :
	public IGameObject
{
public:
	Goal();
	~Goal();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ゴールのポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ゴールの回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model = nullptr;						//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	Game*				m_game = nullptr;						//Gameのポインタ。
	Player*				m_player = nullptr;						//プレイヤークラスのポインタ。
	const float			CLEAR_DISTANCE = 100.0f;				//近くに来たらクリアとする。
};

