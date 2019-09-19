#pragma once
//#include "character/CharacterController.h"

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Update() override;
	/// <summary>
	/// プレイヤーの動き。
	/// </summary>
	void Move();
	/// <summary>
	/// プレイヤーの回転。
	/// </summary>
	void Rotation();
	//void Render() override;
	void Draw() override;
	/// <summary>
	/// プレイヤーのポジションを取得。
	/// </summary>
	CVector3 GetPos()
	{
		return m_position;
	}
	/// <summary>
	/// プレイヤーの回転を取得。
	/// </summary>
	CQuaternion GetRot()
	{
		return m_rotation;
	}
private:
	SkinModel m_model;									//スキンモデル。
	Sprite m_sprite;
	CVector3 m_position = CVector3::Zero();				//座標。
	//CharacterController m_charaCon;					//キャラクターコントローラー。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	CVector3 m_scale= CVector3::One();					//大きさ。
	bool isRun;											//走っているかどうか
	float RunParam = 2.f;								//走るときの倍率 走ると2倍になる。
	float m_speed = 10.f;								//キャラが歩くスピード。
	
};