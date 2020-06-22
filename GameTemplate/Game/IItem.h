#pragma once
#include "IGameObject.h"
class IItem :
	public IGameObject
{
public:
	IItem();
	~IItem();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="texFilePath"></param>
	void InitModel(const wchar_t* texFilePath);
	/// <summary>
	/// ポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model;								//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。

};

