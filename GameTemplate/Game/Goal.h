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
	/// プレイヤーのポジションをセット。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// プレイヤーの回転をセット。
	/// </summary>
	/// <param name="pos">回転</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender* m_model;							//スキンモデル。
	CVector3 m_position = CVector3::Zero();				//座標。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	Game* m_game = nullptr;								//Gameのポインタ。
	Player* m_player = nullptr;							//プレイヤークラスのポインタ。
};

