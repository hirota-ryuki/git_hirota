#pragma once

const CVector2 NAME_TOP_POS  = { 0.0f,0.0f };	//フォントが並ぶ一番上の名前の座標。
const CVector2 NUM_TOP_POS   = { 250.0f,0.0f };	//フォントが並ぶ一番上の個数の座標。
const CVector2 ADD_BELOW_POS = { 0.0f,50.0f };	//項目の座標の追加量。この定数分座標が下がっていく。
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
class Pose :
	public IGameObject
{
public:
	Pose();
	~Pose();
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

	int GetItemCount(){
		return m_itemCount;
	}
	void AddItemCount(){
		m_itemCount++;
	}
private:
	bool			m_isStart = false;			//スタート画面が表示されているかどうか。
	FontRender*		m_itemNameFont = nullptr;	//アイテム名のフォント。
	FontRender*		m_itemCountfont = nullptr;	//アイテムの個数のフォント。
	int				m_itemCount = 0;			//このアイテムの個数。

	std::list<ItemFontData>	m_fontList;
	CVector2	m_position = CVector2::Zero();
	CVector3	m_position2 = CVector3::Zero();
};

