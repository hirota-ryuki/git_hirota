#pragma once
#include "character/CharacterController.h"
#include "AStar.h"
#include "Navimesh.h"
//#define DEBUG_MODE
const float NOT_ASTAR_DISTANCE = 200.0f;
const float ATTACK_DISTANCE = 100.0f;

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
	void Update_NotPause() override;
	/// <summary>
	/// 噛みつき攻撃。
	/// </summary>
	void En_Bite();
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
	/// スムージングされた経路情報どおりに動く関数。
	/// Move関数内で行う。
	/// </summary>
	void Move_AStar();
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
	/// <summary>
	/// 死ぬ関数。
	/// </summary>
	void Death();
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
	std::vector<CVector3>			m_moveList;	//A*後に算出されたゴールまでのルートリスト。
	std::vector<CVector3>::iterator m_itr;		//m_moveListのイテレータ。
	bool			m_isAstar = false;			//ゴールしているかどうか。
	bool			m_isPoint = false;			//各*itrに到達したかどうか。
	bool			m_isHit   = false;			//コリジョンにヒットしたかどうか。
	bool			m_isMove = false;
	CVector3		m_endPos = CVector3::Zero();//最終地点。
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
	AnimationClip	m_animationClip[enAnimationClip_num];		//アニメーションクリップ。
	Animation		m_animation;

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
	State			m_state = enState_idle;
	int				m_coolTimer = 0;						//攻撃後のクールタイム。

	//移動関係。
	CVector3		m_moveSpeed = CVector3::Zero();			//移動速度。
	const float		WALK_SPEED = 150.f;						//キャラが歩くスピード。
	BoxCollider		m_collider;								//セル用のボックスコライダー
	CVector3		m_boxSize = CVector3::One() * 30.0f;	//コライダーのサイズ。
	bool			m_isFind = false;						//プレイヤーが見つかったかどうか。
	int				m_aStarCount = 0;						//A*の無限計算ループの回避用のカウンタ。
	const float		ARRIVAL_DISTANCE = 50.0f;				//到着したかどうか判定するための距離。
	const float		END_ASTSR_OF_NEER_PLAYER = 200.0f;		//プレイヤーの近くに来たらA*を終了させる時の距離。
	//攻撃関係。
	int				m_atkTimer = 0;
	bool			m_isAttack = false;//攻撃したかどうか。
	const int		ATK_INTERVAL = 60;
	  
	//骨データ。
	//ヘッドショットの判定と噛みつき攻撃の判定に使用する。	
	CVector3		m_bonePos = CVector3::Zero();			//骨の座標。
	CQuaternion		m_boneRot = CQuaternion::Identity();	//骨の回転。
	CVector3		m_boneScale = CVector3::Zero();			//骨の拡大率。

	//噛みつき関係。
	bool			m_isBite = false;

	//ステータス関係。
	int				m_hp = 10;		//HP。

#ifdef DEBUG_MODE
	SkinModelRender*	m_debugModel = nullptr;						//スキンモデル。
	CQuaternion			m_debugrotation = CQuaternion::Identity();	//回転。
#endif //DEBUG_MODE
};