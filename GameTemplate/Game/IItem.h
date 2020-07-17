#pragma once
#include "IGameObject.h"
#include <unordered_map>
const float			ITEM_SPRITE_W = 270.0f;
const float			ITEM_SPRITE_H = 130.0f;
const CVector2		FRAME_IN_POS =  { -(FRAME_BUFFER_W / 2 - ITEM_SPRITE_W / 2),-200.0f };
const CVector2		FRAME_OUT_POS = { -(FRAME_BUFFER_W / 2 + ITEM_SPRITE_W / 2),-200.0f };
const float			AMOUNT_OF_CHANGE = 5.0f;					//画像が移動する量、変化量。
const float			B_BUTTON_SIZE    = 25.0f;					//Bボタンのサイズ。
const float			ENEMY_AND_PLAYER_DISTANCE_BUTTON = 500.0f;	//ボタンを表示する、敵とプレイヤーの距離の範囲。
const float			ENEMY_AND_PLAYER_DISTANCE_MOVE   = 100.0f;	//画像を動かす、敵とプレイヤーの距離の範囲。

class Player;
class Game;

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
	/// 画像をロードする関数。
	/// </summary>
	/// <param name="filePath">画像のファイルパス。</param>
	/// <param name="w">横幅。</param>
	/// <param name="h">縦幅。</param>
	/// <returns>スプライトレンダー。</returns>
	SpriteRender* SpriteLoad(const wchar_t* filePath, float w, float h);
	
	/// <summary>
	/// 
	/// </summary>
	void IItemInit();

	/// <summary>
	/// IItemクラスの共通処理。
	/// 行わなければならない処理をまとめたもの。
	/// </summary>
	/// <param name="sprite"></param>
	/// <param name="pos"></param>
	void ItemCommonProcessing(SpriteRender* sprite, CVector3 pos);
	
	/// <summary>
	/// アイテムをゲットしたときの処理を自由に書ける関数。
	/// </summary>
	virtual void OnGet() {}

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
	/// <summary>
	/// アイテムがゲットされたかどうかを返す関数。
	/// </summary>
	/// <param name="diff">プレイヤーとアイテムとの距離</param>
	/// <returns>プレイヤーにゲットされたかどうか。</returns>
	bool IsGetItem(CVector3 diff);
	
	/// <summary>
	/// ゲットされた時の共通処理。
	/// </summary>
	void GettingItem(bool isGetItem);

	/// <summary>
	/// 画像を動かす関数。
	/// </summary>
	/// <param name="sprite">スプライトレンダー。</param>
	/// <param name="diff">プレイヤーとアイテムとの距離。</param>
	void SpriteMove(SpriteRender* sprite, CVector3 diff);

	/// <summary>
	/// 
	/// </summary>
	void ButtonSpriteLoad();
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="diff"></param>
	/// <param name="position"></param>
	void ButtonSpriteMove(CVector3 diff, CVector3 position);

private:
	wchar_t*	m_name = nullptr;							//アイテムの名前。
	bool		m_isGetItem = false;						//Bボタンが押されたかどうか。
	
	//画像の移動関係。
	CVector2	m_movedPos = CVector2::Zero();				//1フレームで動いた後の座標。
	bool		m_isNearPlayer = false;						//プレイヤーが近くにいるかどうか。
	bool		m_isFinishedMove = false;					//動き終わったかどうか。
	//ステート。
	enum State {
		enState_nearPlayer,
		enState_stopPlayer,
		enState_farPlayer
	};
	State		m_state = enState_nearPlayer;
	
	//unordered_mapは順番がめちゃくちゃになるがmapより速い。
	static std::unordered_map<
		std::wstring,
		std::unique_ptr<SpriteRender>
	>	m_itemSpriteMap;
	
	//Bボタン関係。
	SpriteRender*	m_buttonSprite = nullptr;
	CVector4		m_model2Dpos = CVector4::Black();		//3Dモデルを2Dの座標に変換した座標。

	//OnGet関数関係。
	bool			m_isOnce = false;		//一回限りの処理を行うための判定。

	//インスタンス関係。
	Game*				m_game = nullptr;						//Gameのポインタ。
	Player*				m_player = nullptr;
};