#pragma once
#include "physics/PhysicsStaticObject.h"

class Door :
	public IGameObject
{
public:
	Door(){}
	~Door();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ドアを動かす関数。
	/// </summary>
	/// <param name="diff">プレイヤーとドアとの距離。</param>
	/// <param name="model">ドアのスキンモデルレンダー。</param>
	/// <param name="PSOmodel">ドアの当たり判定のモデルレンダー。</param>
	/// <param name="pso">ドアのスタティックオブジェクト。</param>
	/// <param name="rot">モデルの回転情報。</param>
	void MoveDoor(const float diff, SkinModelRender* model, SkinModelRender* PSOmodel, PhysicsStaticObject& pso, CQuaternion& rot);
	/// <summary>
	/// 部屋の名前を設定。
	/// </summary>
	/// <param name="name">名前。</param>
	void SetName(const std::wstring& name) {
		m_name = name;
	}
	/// <summary>
	/// ドアのポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ドアのセンターポジションをセット。
	/// </summary>
	/// <param name="pos">中心座標。</param>
	void SetCenterPos(const CVector3& pos)
	{
		m_centerPos = pos;
	}
	/// <summary>
	/// ドアの回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*		m_model = nullptr;						//スキンモデル。
	CVector3				m_position = CVector3::Zero();			//座標。
	CQuaternion				m_rotation = CQuaternion::Identity();	//回転。
	SkinModelRender*		m_PSOmodel = nullptr;					//当たり判定のスキンモデル。
	PhysicsStaticObject		m_beforeMovingPSO;					//床の当たり判定。
	Game*					m_game = nullptr;						//Gameのポインタ。
	Player*					m_player = nullptr;						//プレイヤークラスのポインタ。
	CVector3				m_centerPos = CVector3::Zero();			//ドアの中心座標。

	PhysicsStaticObject		m_afterMovingPSO;					//床の当たり判定。
	const float				ADD_ROTATE = -2.0f;						//1フレームに回転する量。
	const float				ACTION_DISTANCE = 150.0f;				//プレイヤーがドアにアクションできる距離。
	const float				ACTION_DISTANCE_SQ = ACTION_DISTANCE * ACTION_DISTANCE;;		//プレイヤーがドアにアクションできる距離の2乗。
	float					m_maxRotate = 90.0f;					//回転の最大量。
	bool					m_isRotate = false;						//回転したかどうか。
	bool					m_isOpenDoor = false;					//ドアが開いたかどうか。
	std::wstring			m_name;									//部屋の名前。どこのドアかわかるように。
};