#pragma once
#include "IGameObject.h"
#include <unordered_map>
const float			ITEM_SPRITE_W = 270.0f;
const float			ITEM_SPRITE_H = 130.0f;
const CVector2		FRAME_IN_POS =  { -(FRAME_BUFFER_W / 2 - ITEM_SPRITE_W / 2),-200.0f };
const CVector2		FRAME_OUT_POS = { -(FRAME_BUFFER_W / 2 + ITEM_SPRITE_W / 2),-200.0f };
const float			AMOUNT_OF_CHANGE = 5.0f;				//画像が移動する量、変化量。

struct deleter_SpriteRender  {
	void operator()(SpriteRender* ptr) {
		DeleteGO(ptr);
	}
};

class IItem :
	public IGameObject
{
public:
	IItem();
	virtual ~IItem();
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
	/// 画像をロードする関数。
	/// </summary>
	/// <param name="filePath">画像のファイルパス。</param>
	/// <param name="w">横幅。</param>
	/// <param name="h">縦幅。</param>
	/// <returns>スプライトレンダー。</returns>
	SpriteRender* SpriteLoad(const wchar_t* filePath, float w, float h);
	
	/// /// <summary>
	/// 画像を動かす関数。
	/// </summary>
	/// <param name="sprite">スプライトレンダー。</param>
	/// <param name="diff">プレイヤーとアイテムとの距離。</param>
	void SpriteMove(SpriteRender* sprite, CVector3 diff);
	void ButtonSpriteLoad();
	void ButtonSpriteMove(CVector3 diff, CVector3 position);
	
	/// <summary>
	/// m_isFinishedMoveを取得。
	/// </summary>
	/// <returns>m_isFinishedMove</returns>
	bool GetIsFinishedMove() {
		return m_isFinishedMove;
	}
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
	/// <summary>
	/// 解放。
	/// static関数はincrudeしていたら
	/// どこでも使える。
	/// </summary>
	static void Release();
private:
	wchar_t*	m_name = nullptr;							//アイテムの名前。
	bool		m_isGetItem = false;						//Bボタンが押されたかどうか。
	bool		m_isNearPlayer = false;						//プレイヤーが近くにいるかどうか。
	bool		m_isFinishedMove = false;					//動き終わったかどうか。
	//ステート関係。
	enum State {
		enState_nearPlayer,
		enState_stopPlayer,
		enState_farPlayer
	};
	State		m_state = enState_nearPlayer;
	CVector2	m_movedPos = CVector2::Zero();
	//unordered_mapは順番がめちゃくちゃになるがmapより速い。
	static std::unordered_map<
		std::wstring,
		std::unique_ptr<SpriteRender>
	>	m_itemSpriteMap;
	static Sprite	m_sprite;
	static bool		m_isCreateSprite;

	//Bボタン関係。
	SpriteRender*		m_buttonSprite = nullptr;
	CVector4		m_model2Dpos = CVector4::Black();		//3Dモデルを2Dの座標に変換した座標。

};