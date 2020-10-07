#pragma once
#include "physics/PhysicsStaticObject.h"
class Player;
//拡張予定だった名残の基底クラス。
class IDoor :
	public IGameObject
{
public:
	IDoor();
	virtual ~IDoor();
	/// <summary>
	/// ドアを動かす関数。
	/// </summary>
	/// <param name="diff">プレイヤーとドアとの距離。</param>
	/// <param name="model"></param>
	/// <param name="PSOmodel"></param>
	/// <param name="pso"></param>
	/// <param name="rot"></param>
	void MoveDoor(const CVector3& diff, SkinModelRender* model, SkinModelRender* PSOmodel, const PhysicsStaticObject& pso, const CQuaternion& rot);
	/// <summary>
	/// 部屋の名前を設定。
	/// </summary>
	/// <param name="name">名前</param>
	void SetName(const std::wstring& name) {
		m_name = name;
	}
private:
	PhysicsStaticObject		m_physicsStaticObject;			//床の当たり判定。
	const float				ADD_ROTATE = -2.0f;				//1フレームに回転する量。
	const float				ACTION_DISTANCE = 150.0f;		//プレイヤーがドアにアクションできる距離。
	float					m_maxRotate = 90.0f;			//回転の最大量。
	bool					m_isRotate = false;				//回転したかどうか。
	bool					m_isOpenDoor = false;			//ドアが開いたかどうか。
	std::wstring			m_name;							//部屋の名前。どこのドアかわかるように。
	Game*					m_game = nullptr;				//Gameクラスのポインタ。
};