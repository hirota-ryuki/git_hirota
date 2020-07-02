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
	void SpriteMove(SpriteRender* sprite, CVector3 diff);
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
	wchar_t*	m_name = nullptr;							//アイテムの名前。
	bool		m_isGetItem = false;						//Bボタンが押されたかどうか。
	bool		m_isNearPlayer = false;						//プレイヤーが近くにいるかどうか。
	CVector3	m_initialPos = { 650.0f,-200.0f,0.0f };		//画像の初期座標。
	CVector3	m_addPos = { -5.0f,0.0f,0.0f };				//画像が動く量。
	int			m_addCount = 0;								//画像を動かした回数。
	const int	ADD_COUNT = 20;
};

