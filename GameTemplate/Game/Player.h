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
	/// 待機ステート関数。
	/// </summary>
	void En_Idle();
	/// <summary>
	/// 歩きステート関数。
	/// </summary>
	void En_Walk();
	/// <summary>
	/// 走りステート関数。
	/// </summary>
	void En_Run();
	/// <summary>
	/// エイムステート関数。
	/// </summary>
	void En_Aim();
	/// <summary>
	/// 撃つステート関数。
	/// </summary>
	void En_Shot();
	/// <summary>
	/// リロードするステート関数。
	/// </summary>
	void En_Reload();
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
	/// 時間によって回復する関数。
	/// </summary>
	void Heal();
	/// <summary>
	/// ダメージを与える関数。
	/// </summary>
	void Damage();
	/// 死ぬ関数。
	/// </summary>
	void Death();
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
	/// <param name="rot">回転</param>
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
private:
	SkinModelRender*	m_model = nullptr;						//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CharacterController m_charaCon;								//キャラクターコントローラー。
	CVector3			m_moveSpeed = CVector3::Zero();			//移動速度。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	Game*				m_game = nullptr;						//Gameのポインタ。
	float m_speed = 300.0f;			//キャラが歩くスピード。
	float m_runSpeed = 500.0f;		//キャラが走るスピード。
	
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
		enAnimationClip_reload,
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
		enState_reload,
		enState_death,
		enState_num,
	};
	State			m_state = enState_idle;
	bool			m_isRun = false;		//走っているかどうか。
	bool			m_isShot = false;		//撃っているかどうか。
	//Shot関係。
	Bullet*			m_bullet = nullptr;		//Bulletクラスのポインタ。
	int				m_timer = 0;			//タイマー。
	bool			m_isBullet = false;		//撃ったかどうか。
	int				m_capacity = 30;		//残弾数。
	const int		m_maxCapacity = 30;		//装弾数。
	//ダメージ画像関係。
	SpriteRender*	m_sprite = nullptr;
	CQuaternion		m_rot = CQuaternion::SpriteRot();
	float			m_alpha = 0.0f;
	//回復関係。
	bool			m_isDamage = false;			//ダメージを受けたかどうか。
	int 			m_healTimer = 0;			//回復するまでのタイマー。
	int				m_healStartTime = 200;		//回復するまでの時間。
	int 			m_heaIntervalTimer = 0;		//回復と回復の間のタイマー。徐々に回復するようにする。
	int				m_healIntervalTime = 30;	//回復と回復の間の時間。徐々に回復するようにする。
	//ステータス関係。
	float			m_hp = 5.0f;		//体力。
	const float		m_maxhp = 5.0f;		//最大体力。
	//サウンド関係。
	CSoundSource	m_se;

};