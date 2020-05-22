#pragma once
#define BAG_MODE

#ifdef BAG_MODE
struct ItemInfo {
	SpriteRender*	m_sprite = nullptr;		//画像のポインタ。
	const wchar_t*  m_texFIlePath;
	int				m_nomber = 0;			//バッグの何番目にあるか。
};
#endif // BAG_MODE


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
#ifdef BAG_MODE
	
#endif // BAG_MODE


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

#ifdef BAG_MODE
	SpriteRender*	m_spriteMenu = nullptr;		//メニュー画面の画像。

#endif // BAG_MODE
};

