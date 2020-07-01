#pragma once
#include "IGameObject.h"
class IItem :
	public IGameObject
{
public:
	IItem();
	virtual ~IItem() {}	
	/// <summary>
	/// アイテムがゲットされたかどうかを返す関数。
	/// </summary>
	/// <param name="diff">プレイヤーとアイテムとの距離</param>
	/// <returns>プレイヤーにゲットされたかどうか。</returns>
	bool IsGetItem(CVector3 diff);
	/// <summary>
	/// ゲットされた時の処理。
	/// </summary>
	virtual void GettingItem(bool isGetItem) {}
	/// <summary>
	/// 名前を設定。
	/// </summary>
	void SetName(wchar_t* name)
	{
		m_name = name;
	}
	/// <summary>
	/// 名前を取得。
	/// </summary>
	wchar_t* GetName()
	{
		return m_name;
	}
private:
	//モデル関係。
	SkinModelRender*	m_model = nullptr;				//スキンモデル。
	CVector3			m_pos = CVector3::Zero();
	wchar_t*			m_name = nullptr;
	bool				m_isGetItem = false;				//Bボタンが押されたかどうか。
};

