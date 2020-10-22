#pragma once
#include "character/CharacterController.h"

class Bullet;
class UI;
class SpotLight;
class SkinModel;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void Update_NotPause() override;
	
	/// <summary>
	/// プレイヤーのポジションをセット。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// プレイヤーのポジションを取得。
	/// </summary>
	const CVector3& GetPos() const
	{
		return m_position;
	}
	/// <summary>
	/// 引数で渡された座標とプレイヤーの距離の2乗を計算します。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <returns>距離の2乗</returns>
	float CalcDistanceSQFrom(const CVector3& pos) const
	{
		CVector3 diff = m_position - pos;
		return diff.LengthSq();
	}
	/// <summary>
	/// 引数で渡された座標からプレイヤーに向かって伸びる
	/// XZ平面での向きベクトル(大きさ１)を計算。
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	CVector3 CalcDirectionXZFrom(const CVector3& pos) const
	{
		CVector3 moveDirection = m_position - pos;
		moveDirection.y = 0.0f;
		moveDirection.Normalize();
		return moveDirection;
	}
	/// <summary>
	/// プレイヤーのムーブスピードを取得。
	/// </summary>
	const CVector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	/// <summary>
	/// プレイヤーの回転をセット。
	/// </summary>
	/// <param name="rot">回転。</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// プレイヤーの回転を取得。
	/// </summary>
	/// <returns>回転。</returns>
	const CQuaternion& GetRot() const
	{
		return m_rotation;
	}
	/// <summary>
	/// プレイヤーの残弾数を取得。
	/// </summary>
	const int GetCapacity() const
	{
		return m_capacity;
	}
	/// <summary>
	/// プレイヤーの装弾数を取得。
	/// </summary>
	const int GetMaxCapacity() const
	{
		return m_maxCapacity;
	}
	/// <summary>
	/// プレイヤーの手持ちの総弾数を取得。
	/// </summary>
	const int GetStack() const
	{
		return m_stack;
	}
	/// <summary>
	/// プレイヤーの手持ちの総弾数を足す。
	/// </summary>
	void AddStack(const int bullet)
	{
		m_stack += bullet;
	}
	/// <summary>
	/// プレイヤーの手持ちの総弾数を取得。
	/// </summary>
	const SkinModelRender* GetSkinModelRender() const
	{
		return m_model;
	}
	/// <summary>
	/// 噛まれたかどうかをセット。
	/// </summary>
	/// <param name="flag">噛まれたかどうか。</param>
	void SetIsBiteconst (bool flag) {
		m_isBite = flag;
	}
	/// <summary>
	/// キャラコンを返す関数。
	/// </summary>
	/// <returns>キャラコン。</returns>
	const CharacterController* GetCharaCon()  const{
		return &m_charaCon;
	}
	const CharacterController& GetCharaCon_Ghorst()  const{
		return m_charaCon;
	}
	/// <summary>
	/// モデルの前方向。
	/// </summary>
	/// <returns>モデルの前方向。</returns>
	const CVector3& GetForward() const{
		return m_forward;
	}
	/// <summary>
	/// 影用の視点。
	/// </summary>
	/// <returns>影用の視点。</returns>
	const CVector3& GetShadowPos() const{
		return m_shadowPos;
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
	/// ダメージ表現の2Dを初期化
	/// </summary>
	void InitDamageSprite();
	/// <summary>
	/// 照準2Dを初期化。
	/// </summary>
	void InitAimSprite();
	/// <summary>
	/// GameCameraのインスタンスを取得する関数。
	/// </summary>
	void GetGameCameraInst() {
		if (m_game->GetCamera() != nullptr
			&& !m_isGameCamera) {
			m_gamecamera = m_game->GetCamera();
			m_isGameCamera = true;
		}
	}
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
	/// 噛みつかれるステート関数。
	/// </summary>
	void En_Lie();
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
	/// 女優ライト関数。
	/// </summary>
	void ActressLight();
	/// <summary>
	/// 懐中電灯。
	/// </summary>
	void LightUpdate();
private:
	SkinModelRender*	m_model = nullptr;						//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CharacterController m_charaCon;								//キャラクターコントローラー。
	CVector3			m_moveSpeed = CVector3::Zero();			//移動速度。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	Game*				m_game = nullptr;						//Gameのポインタ。
	UI*					m_ui = nullptr;							//UIのポインタ。
	SpotLight*			m_sl = nullptr;							//UIのポインタ。
	float				m_speed = 210.0f;			//キャラが歩くスピード。
	float				m_runSpeed = 280.0f;		//キャラが走るスピード。
	
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
		enAnimationClip_lie,
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
		enState_lie,
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
	bool			m_isEmpty = false;		//空砲を撃ったかどうか。
	int				m_capacity = 12;		//残弾数。
	const int		m_maxCapacity = 12;		//装弾数。
	int				m_stack = 24;			//手持ちの総弾数。
	//ダメージ関係。
	bool			m_isBite = false;			//噛まれたかどうか。
	bool			m_isOldCameraInfo = false;	//カメラの情報を記録したかどうか。
	bool			m_isGameCamera = false;		//GameCameraクラスのポインタを取得したかどうか。
	GameCamera*		m_gamecamera = nullptr;		//GameCameraクラスのポインタ。
	//カメラ情報のバックアップ。
	float			m_oldplayer = 0.0f;			//倒れる前のカメラの視点。
	float			m_oldpos = 0.0f;			//倒れる前のカメラの注視点。
	float			m_oldAddY = 0.0f;			//倒れる前の点を地面から上昇させる量。
	//ダメージ画像関係。
	SpriteRender*	m_sprite = nullptr;
	float			m_alpha = 0.0f;
	//照準画像関係。
	SpriteRender*	m_aimSprite = nullptr;
	//回復関係。
	bool			m_isDamage = false;			//ダメージを受けたかどうか。
	int 			m_healTimer = 0;			//回復するまでのタイマー。
	const int		m_healStartTime = 600;		//回復するまでの時間。
	int 			m_heaIntervalTimer = 0;		//回復と回復の間のタイマー。徐々に回復するようにする。
	int				m_healIntervalTime = 30;	//回復と回復の間の時間。徐々に回復するようにする。
	//ステータス関係。
	const float		m_maxhp = 5.0f;		//最大体力。
	float			m_hp = m_maxhp;		//体力。
	//ライト。
	bool			m_isLight = true;			//ライトをつけているかどうか。
	CVector3		m_shadowPos = CVector3::One();

	//モデルの前方向。
	CVector3		m_forward = CVector3::AxisX();
};