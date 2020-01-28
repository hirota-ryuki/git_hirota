#pragma once
#include "character/CharacterController.h"

class Bullet;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ステートを変更する関数。
	/// 特殊な場合を除く。
	/// </summary>
	void ChangeState();
	/// <summary>
	/// プレイヤーの動き。
	/// </summary>
	void Move();
	/// <summary>
	/// プレイヤーの回転。
	/// </summary>
	void Rotation();
	/// <summary>
	/// プレイヤーのポジションをセット。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// プレイヤーのポジションを取得。
	/// </summary>
	CVector3 GetPos()
	{
		return m_position;
	}
	/// <summary>
	/// プレイヤーの回転をセット。
	/// </summary>
	/// <param name="pos">回転</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// プレイヤーの回転を取得。
	/// </summary>
	CQuaternion GetRot()
	{
		return m_rotation;
	}
	/// <summary>
	/// ダメージを与える関数。
	/// </summary>
	void Damage()
	{
		m_hp--;
	}
private:
	SkinModelRender* m_model;							//スキンモデル。
	CVector3 m_position = CVector3::Zero();				//座標。
	CharacterController m_charaCon;						//キャラクターコントローラー。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	Game* m_game = nullptr;								//Gameのインスタンス。
	//bool isRun;										//走っているかどうか。
	//float RunParam = 2.0f;							//走るときの倍率 走ると2倍になる。
	float m_speed = 1000.0f;							//キャラが歩くスピード。
	//アニメーション関係。
	enum {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_walk_left,
		enAnimationClip_walk_right,
		enAnimationClip_back,
		enAnimationClip_run,
		enAnimationClip_aim,
		enAnimationClip_shot,
		enAnimationClip_num,
	};
	AnimationClip m_animationClip[enAnimationClip_num];		//アニメーションクリップ。
	Animation m_animation;//アニメーションのインスタンス。
	//ステート関係。
	enum State {
		enState_idle,
		enState_walk,
		enState_run,
		enState_aim,
		enState_shot,
		enState_num,
	};
	State m_state = enState_idle;
	bool m_isRun = false;			//走っているかどうか
	bool m_isShot = false;			//撃っているかどうか
	//Shot関係。
	Bullet* m_bullet;//Bulletクラスのポインタ。
	int m_timer = 0;//タイマー。
	bool m_isBullet = false;//撃ったかどうか。
	//ステータス関係。
	int m_hp = 10;
};