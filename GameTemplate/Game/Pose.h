#pragma once
//struct ItemInfo {
//	SpriteRender*	m_sprite = nullptr;		//画像のポインタ。
//	const wchar_t*  m_texFIlePath;
//	int				m_nomber = 0;			//バッグの何番目にあるか。
//};
struct ItemInfo_Font {
	//アイテムの基本情報。
	const wchar_t*  m_itemName;			//アイテムの名前。
	int				m_itemCount = 0;	//このアイテムの個数。
	int				m_bagNumber = 0;	//バッグの何番目にあるか。
	//フォントを表示するための情報。
	FontRender*		m_itemNameFont = nullptr;	//アイテム名のフォント。
	FontRender*		m_itemCountfont = nullptr;	//アイテムの個数のフォント。
	bool			m_isNewGO = false;			//フォントをNewGOしたかどうか。
	bool			m_isChange = false;			//アイテムの個数が変更されたかどうか。
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
	//void AddItem(const wchar_t* textureFIlePath) {
	//	/*if (m_item < m_maxItem) {
	//		m_itemTexList.emplace_back(textureFIlePath);
	//		m_item++;
	//	}*/
	//	m_itemInfo.m_texFIlePath = textureFIlePath;
	//}
	//void AddItem(const wchar_t* itemname);
	int GetItemCount(){
		return m_itemCount;
	}
	void AddItemCount(){
		m_itemCount++;
	}
private:
	SpriteRender*	m_sprite = nullptr;			//画像のポインタ。
	bool			m_isStart = false;			//スタート画面が表示されているかどうか。
	FontRender*		m_itemNameFont = nullptr;	//アイテム名のフォント。
	FontRender*		m_itemCountfont = nullptr;	//アイテムの個数のフォント。
	int				m_itemCount = 0;	//このアイテムの個数。


	//ItemInfo_Font	m_itemList[12];
	//FontPos			m_posList[12];
	//bool			m_isStart = false;			//スタート画面が表示されているかどうか。
	//int				m_allItemCount = 0;			//アイテムの種類の数。

	//SpriteRender*	m_itemSprite = nullptr;				//アイテムの画像のポインタ。
	//std::vector< CVector3* >		m_itemPosList;		//アイテムの座標配列。
	//std::vector< CVector3* >		m_itemPosList;		//アイテムの座標配列。
	//std::vector< ItemInfo* >		m_itemList;			//アイテムの座標配列。
	//ItemInfo m_itemInfo;
	////std::vector< const wchar_t* >	m_itemTexList;		//アイテムのテクスチャの配列。
	////std::vector< SpriteRender* >	m_itemSpriteList;	//アイテムのテクスチャの配列。
	//int				m_item = 0;							//アイテムの手持ちの総数。			
	//const int		m_maxItem = 12;						//持てるアイテムの最大数。
	//bool			m_isStart = false;					//スタート画面が表示されているかどうか。
	//bool			m_isNewSprite = false;				//スタート画面が表示されているかどうか。
};

