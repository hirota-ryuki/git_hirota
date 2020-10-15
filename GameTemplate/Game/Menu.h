#pragma once

const CVector2 NAME_TOP_POS = { 200.0f,250.0f };	//フォントが並ぶ一番上の名前の座標。
const CVector2 NUM_TOP_POS = { 450.0f,250.0f };	//フォントが並ぶ一番上の個数の座標。
const CVector2 ADD_BELOW_POS = { 0.0f,77.0f };	//項目の座標の追加量。この定数分座標が下がっていく。
const CVector3 ADD2_BELOW_POS = { 0.0f,5.0f,0.0f };	//項目の座標の追加量。この定数分座標が下がっていく。

//アイテムのフォントデータ。
struct ItemFontData {
	FontRender* nameFR = nullptr;
	FontRender* numFR = nullptr;
};

struct FontPos {
	CVector2	namepos = CVector2::Zero();
	CVector2	countpos = CVector2::Zero();
};
class Menu :
	public IGameObject
{
public:
	Menu();
	~Menu();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// フォントレンダーリストの更新関数。
	/// フォントレンダーの追加や配置の整理を行う。
	/// </summary>
	void FontRenderUpdate();
	/// <summary>
	/// 文字列の配置を整理する関数。
	/// </summary>
	void SortingFontRnderPos();
	/// <summary>
	/// ResetIsAddData関数を呼ぶかどうかを判定している関数。
	/// </summary>
	void ResetIsAdd();
private:
	/// <summary>
	/// 一個のアイテム説明のためのフォントレンダラーを作成。
	/// </summary>
	void CreateItemListFontRendersFirst();
	/// <summary>
	/// アイテムの説明のためのフォントレンダラーを作成。
	/// </summary>
	/// <param name="itemDataIt">作成するアイテムのデータ</param>
	void CreateItemListFontRenders(
		std::unordered_map<
			std::wstring,
			int
		>::iterator itemDataIt
	);
	/// <summary>
	/// 所持数0のアイテムのフォントレンダラーを削除する。
	/// </summary>
	void DeleteFontRendererOnZeroItem();
	/// <summary>
	/// アイテムリストの表示をアクティブにする。
	/// </summary>
	void ActivateDispItemList();
private:
	bool			m_isStart = false;			//スタート画面が表示されているかどうか。
	FontRender*		m_itemNameFont = nullptr;	//アイテム名のフォント。
	FontRender*		m_itemCountfont = nullptr;	//アイテムの個数のフォント。
	SpriteRender*	m_sprite = nullptr;
	std::list<ItemFontData>	m_fontList;
	CVector2	m_position = CVector2::Zero();
	CVector3	m_position2 = CVector3::Zero();
};