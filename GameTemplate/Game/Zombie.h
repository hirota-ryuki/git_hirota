#pragma once
#include "character/CharacterController.h"
#include "AStar.h"
#include "Navimesh.h"

class Player;
class Floor;

class Zombie :
	public IGameObject
{
public:
	Zombie();
	~Zombie();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ステートを変更する関数。
	/// 特殊な場合を除く。
	/// </summary>
	void ChangeState();
	/// <summary>
	/// 移動。
	/// </summary>
	void Move();
	/// <summary>
	/// A*を行う関数。
	/// </summary>
	void Astar();
	/// <summary>
	/// 回転する関数。
	/// </summary>
	void Rotation();
	/// <summary>
	/// 攻撃する関数。
	/// </summary>
	void Attack();
	/// <summary>
	/// ダメージを受ける関数。
	/// </summary>
	void Damage();
	/// 死ぬ関数。
	/// </summary>
	void Death();
	/// <summary>
	/// ゾンビのポジションをセット。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// ゾンビの回転をセット。
	/// </summary>
	/// <param name="pos">回転</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model = nullptr;						//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	CharacterController m_charaCon;								//キャラクターコントローラー。

	//A*関係。
	Game*			m_game = nullptr;			//ゲームクラスのポインタ。
	Player*			m_player = nullptr;			//プレイヤークラスのポインタ。
	Floor*			m_floor = nullptr;			//床クラスのポインタ。
	AStar			m_aStar;					//A*クラスのインスタンス。
	Navimesh*		m_nav = nullptr;			//ナビメッシュクラスのポインタ。
	std::vector<CVector3> m_moveList;			//A*後に算出されたゴールまでのルートリスト。
	std::vector<CVector3>::iterator m_itr;		//m_moveListのイテレータ。
	bool			m_isAstar = false;			//ゴールしているかどうか。
	bool			m_isPoint = false;			//各*itrに到達したかどうか。
	bool			m_isHit   = false;			//コリジョンにヒットしたかどうか。

	//アニメーション関係。
	enum {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_attack,
		enAnimationClip_death,
		enAnimationClip_num,
	};
	AnimationClip	m_animationClip[enAnimationClip_num];		//アニメーションクリップ。
	Animation		m_animation;

	//ステート関係。
	enum State {
		enState_idle,
		enState_walk,
		enState_attack,
		enState_death,
		enState_num,
	};
	State			m_state = enState_idle;
	int				m_coolTimer = 0;						//攻撃後のクールタイム。

	//移動関係。
	CVector3		m_moveSpeed = CVector3::Zero();			//移動速度。
	float			m_speed = 200.f;						//キャラが歩くスピード。
	BoxCollider		m_collider;								//セル用のボックスコライダー
	CVector3		m_boxSize = CVector3::One() * 70.0f;	//コライダーのサイズ。
	bool			m_isFind = false;						//プレイヤーが見つかったかどうか。

	//攻撃関係。
	int				m_atkTimer = 0;

	//ステータス関係。
	int				m_hp = 3;		//HP。
};