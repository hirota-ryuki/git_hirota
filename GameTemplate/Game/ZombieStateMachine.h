#pragma once
#include "IZombieComponent.h"
class ZombieStateMachine :
	public IZombieComponent
{
public:
	void Update()override;
private:
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
private:
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

	//移動関係。
	CVector3		m_moveSpeed = CVector3::Zero();			//移動速度。
	const float		WALK_SPEED = 150.f;						//キャラが歩くスピード。
	bool			m_isFind = false;						//プレイヤーが見つかったかどうか。
	int				m_aStarCount = 0;						//A*の無限計算ループの回避用のカウンタ。
	const float		NOT_ASTAR_DISTANCE = 200.0f;			//A*を行わない距離。	
	const float		NOT_ASTAR_DISTANCE_SQ = NOT_ASTAR_DISTANCE * NOT_ASTAR_DISTANCE;	//A*を行わない距離の2乗。
	const float		ARRIVAL_DISTANCE = 50.0f;							//到着したかどうか判定するための距離。
	const float		END_ASTSR_OF_NEER_PLAYER_SQ = 200.0f * 200.0f;		//プレイヤーの近くに来たらA*を終了させる時の距離の2乗。
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

};

