#pragma once
#include "character/CharacterController.h"

//#define DEBUG_MODE

class IZombieComponent;
class ZombieDamage;
class ZombieDeath;
class ZombieStateMachine;

//ステート関係。
enum State {
	enState_idle,
	enState_walk,
	enState_attack,
	enState_bite,
	enState_knockback,
	enState_death,
	enState_num,
};

class Zombie :
	public IGameObject
{
	friend class ZombieDamage;
	friend class ZombieDeath;
	friend class ZombieStateMachine;
public:
	Zombie();
	~Zombie();
	void OnDestroy() override;
	bool Start() override;
	void Update() override {};
	void Update_NotPause() override;
	/// <summary>
	/// 視野角を計算する関数。
	/// </summary>
	float CalcViewingAngleDeg(CVector3 v1, CVector3 v2);
	/// <summary>
	/// ゾンビのポジションをセット。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// ゾンビの回転をセット。
	/// </summary>
	/// <param name="pos">回転</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	/// <summary>
	/// アニメーションクリップを初期化。
	/// </summary>
	void InitAnimationClips();
	/// <summary>
	/// モデルを初期化。
	/// </summary>
	void InitModel();
	/// <summary>
	/// コンポーネントの初期化。
	/// </summary>
	void InitComponents();
	
private:
	std::list< IZombieComponent* > m_component;					//ゾンビのコンポーネントのリスト。

	SkinModelRender*	m_model = nullptr;						//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	CharacterController m_charaCon;								//キャラクターコントローラー。
	
	State			m_state = enState_idle;

	int				m_coolTimer = 0;							//攻撃後のクールタイム。

	//アニメーション関係。
	enum {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_attack,
		enAnimationClip_bite,
		enAnimationClip_knockback,
		enAnimationClip_death,
		enAnimationClip_num,
	};
	AnimationClip	m_animationClip[enAnimationClip_num];			//アニメーションクリップ。
	Animation		m_animation;

	//コライダー関係。
	BoxCollider		m_collider;										//セル用のボックスコライダー。
	const float		BOX_SIZE_DOUBLE = 30.0f;						//コライダーを拡大するための倍率。
	CVector3		m_boxSize = CVector3::One() * BOX_SIZE_DOUBLE;	//コライダーのサイズ。
	
	
	//噛みつき関係。
	bool			m_isBite = false;

	//ステータス関係。
	int				m_hp = 10;		//HP。

#ifdef DEBUG_MODE
	SkinModelRender*	m_debugModel = nullptr;						//スキンモデル。
	CQuaternion			m_debugrotation = CQuaternion::Identity();	//回転。
#endif //DEBUG_MODE
};