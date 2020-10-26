#pragma once
#include "IZombieComponent.h"
#include "AStar.h"
#include "Navimesh.h"

class Player;
class Floor;

class ZombieStateMachine :
	public IZombieComponent
{
public:
	void Start()override;
	void Update()override;
private:
	/// <summary>
	/// インスタンスを初期化。
	/// </summary>
	void InitInstance();
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
	/// A*を強制終了させる必要があるか判定。
	/// </summary>
	/// <remarks>
	/// 最終地点よりプレイヤーが離れていたらA*強制終了になります。
	/// </remarks>
	/// <returns>trueが返ってきたら強制終了。</returns>
	bool IsEndAStarForce() const;
	/// <summary>
	/// プレイヤーに対してレイキャスト。
	/// </summary>
	/// <returns>trueが返ってきたらレイが何かのコライダーと衝突しています。</returns>
	template<class TCallback>
	bool RaycastToPlayer() const;
	/// <summary>
	/// 噛みつき攻撃。
	/// 使用していない関数。
	/// </summary>
	void En_Bite();
	/// <summary>
	/// ゾンビに合わせてプレイヤーの角度を修正する関数。
	/// 使用していない関数。
	/// </summary>
	void AngleCorrection();
private:
	//アニメーション関係。
	const float		m_comprementTime[6] = { 0.2f,0.2f,0.2f,0.2f,0.1f,0.2f };			//アニメーションの補完時間。アニメーションクリップの順番。

	//移動関係。
	CVector3		m_moveSpeed = CVector3::Zero();										//移動速度。
	const float		WALK_SPEED = 150.f;													//キャラが歩くスピード。
	bool			m_isFind = false;													//プレイヤーが見つかったかどうか。
	int				m_aStarCount = 0;													//A*の無限計算ループの回避用のカウンタ。
	const float		NOT_ASTAR_DISTANCE = 200.0f;										//A*を行わない距離。	
	const float		NOT_ASTAR_DISTANCE_SQ = NOT_ASTAR_DISTANCE * NOT_ASTAR_DISTANCE;	//NOT_ASTAR_DISTANCEの2乗。
	const float		ARRIVAL_DISTANCE = 50.0f;											//到着したかどうか判定するための距離。
	const float		END_ASTSR_OF_NEER_PLAYER_SQ = 200.0f * 200.0f;						//プレイヤーの近くに来たらA*を終了させる時の距離の2乗。
	const float		END_ASTSR_OF_LEAVE_FINALPOINT_SQ = 300.0f * 300.0f;					//経路の最終地点からプレイヤーが離れていたらA*を終了させる時の距離の2乗。
	CVector3		m_endPos = CVector3::Zero();										//最終地点。

	//A*関係。
	Game*			m_game = nullptr;				//ゲームクラスのポインタ。
	Player*			m_player = nullptr;				//プレイヤークラスのポインタ。
	Floor*			m_floor = nullptr;				//床クラスのポインタ。
	AStar			m_aStar;						//A*クラスのインスタンス。
	Navimesh*		m_nav = nullptr;				//ナビメッシュクラスのポインタ。
	std::vector<CVector3>			m_moveList;		//A*後に算出されたゴールまでのルートリスト。
	std::vector<CVector3>::iterator m_itr;			//m_moveListのイテレータ。
	bool			m_isAstar = false;				//ゴールしているかどうか。
	bool			m_isPoint = false;				//各*itrに到達したかどうか。
	bool			m_isHit = false;				//コリジョンにヒットしたかどうか。
	bool			m_isMove = false;

	//攻撃関係。
	int				m_atkTimer = 0;
	bool			m_isAttack = false;										//攻撃したかどうか。
	const int		ATK_INTERVAL = 60;										//攻撃を行う間隔。
	const float		ATTACK_DISTANCE = 100.0f;								//攻撃を行う距離。
	const float		ATTACK_DISTANCE_SQ = ATTACK_DISTANCE * ATTACK_DISTANCE;	//ATTACK_DISTANCEの2乗。
	const float		FIND_DISTANCE = 1000.0f;								//プレイヤーに攻撃を行いだす距離。
	const float		FIND_DISTANCE_SQ = FIND_DISTANCE * FIND_DISTANCE;		//FIND_DISTANCEの2乗。
	const float		ATTACK_VIEWING_ANGLE = 45.0f;							//攻撃の視野角。

	//コライダー関係。
	BoxCollider		m_collider;												//セル用のボックスコライダー。
	const float		BOX_SIZE_DOUBLE = 30.0f;								//コライダーを拡大するための倍率。
	CVector3		m_boxSize = CVector3::One() * BOX_SIZE_DOUBLE;			//コライダーのサイズ。

	//噛みつき関係。
	bool			m_isBite = false;
};

