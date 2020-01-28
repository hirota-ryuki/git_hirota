#pragma once
#include "physics/PhysicsStaticObject.h"
class Navimesh;

class Floor :
	public IGameObject
{
public:
	Floor();
	~Floor(); 
	void OnDestroy() override;
	bool Start() override;
	void Update() override ;
	/// <summary>
	/// 床のポジションをセット。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 床の回転をセット。
	/// </summary>
	/// <param name="pos">回転</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// ナビメッシュのインスタンスを取得。
	/// </summary>
	/// <returns>ナビメッシュのインスタンス。</returns>
	Navimesh* GetNavimesh()
	{
		return m_navimesh;
	}
	SkinModelRender* GetSkinModelRender()
	{
		return m_model;
	}
private:
	SkinModelRender* m_model = nullptr;					//床のスキンモデル。
	PhysicsStaticObject m_physicsStaticObject;			//床の当たり判定。
	CVector3 m_position = CVector3::Zero();				//座標。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	Navimesh* m_navimesh;								//Navimeshクラス。
	bool m_isPhysics = false;							//あたり判定を付けたかどうか。
};