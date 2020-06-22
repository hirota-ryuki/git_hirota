#pragma once
//#define BAG_MODE

#ifdef BAG_MODE
struct ItemInfo {
	const wchar_t*		name;				//アイテムの名前。
	SpriteRender*		sprite = nullptr;	//画像のポインタ。
	int					nomber = 0;			//バッグの何番目にあるか。
};
#endif // BAG_MODE

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
	/// <summary>
	/// アイテムを追加する関数。
	/// すでに持っているアイテムなら、個数だけ追加する
	/// </summary>
	/// <param name="name"></param>
	/// <param name="textureFIlePath"></param>
	void AddItem(const wchar_t* name, const wchar_t* textureFIlePath);
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
	SpriteRender*			m_spriteMenu = nullptr;		//メニュー画面の画像。
	std::vector<ItemInfo>	m_itemList;					//アイテムのリスト。
	int						m_totalItemCount = 0;		//総アイテム数。
#endif // BAG_MODE
};

