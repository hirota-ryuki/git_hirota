：：：#pragma once
#include "character/CharacterController.h"
#include "AStar.h"
#include "Navimesh.h"

//#define DEBUG_MODE
const float NOT_ASTAR_DISTANCE = 200.0f;
const float NOT_ASTAR_DISTANCE_SQ = NOT_ASTAR_DISTANCE * NOT_ASTAR_DISTANCE;

class Player;
class Floor;
class IZombieComponent;
class ZombieDamage;
class ZombieDeath;
class ZombieStateMachine;

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
	/// プレイヤーに対してレイキャスト
	/// </summary>
	/// <returns>trueが返ってきたらレイが何かのコライダーと衝突しています。</returns>
	template<class TCallback> 
	bool RaycastToPlayer() const;
	/// <summary>
	/// A*を強制終了させる必要があるか判定。
	/// </summary>
	/// <remarks>
	/// 最終地点よりプレイヤーが離れていたらA*強制終了になります。
	/// </remarks>
	/// <returns>trueが返ってきたら強制終了。</returns>
	bool IsEndAStarForce() const;
	/// <summary>
	/// コンポーネントの初期化。
	/// </summary>
	void InitComponents();
private:
	std::list< IZombieComponent* > m_component;			//ゾンビのコンポーネントのリスト。

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
	const float		END_ASTSR_OF_NEER_PLAYER_SQ = 200.0f * 200.0f;//プレイヤーの近くに来たらA*を終了させる時の距離の2乗。
	const float		END_ASTSR_OF_LEAVE_FINALPOINT_SQ = 300.0f * 300.0f;	//経路の最終地点からプレイヤーが離れていたらA*を終了させる時の距離の2乗。

	//攻撃関係。
	int				m_atkTimer = 0;
	bool			m_isAttack = false;						//攻撃したかどうか。
	const int		ATK_INTERVAL = 60;						//攻撃を行う間隔。
	const float		ATTACK_DISTANCE = 100.0f;
	const float		ATTACK_DISTANCE_SQ = ATTACK_DISTANCE * ATTACK_DISTANCE;	//ATTACK_DISTANCEの2乗。
	const float		FIND_DISTANCE = 1000.0f;				//プレイヤーに攻撃を行いだす距離。
	const float		FIND_DISTANCE_SQ = FIND_DISTANCE * FIND_DISTANCE;	//FIND_DISTANCEの2乗。
	const float		ATTACK_VIEWING_ANGLE = 45.0f;			//攻撃の視野角。
	  
	//ダメージ関係。
	
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