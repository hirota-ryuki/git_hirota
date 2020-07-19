#pragma once
#include "IGameObject.h"
//NewGOする必要があるのか検討
class Inventory :
	public IGameObject
{
private:
	Inventory();
	~Inventory();
public:
	//シングルトン。
	static Inventory& GetInstance() {
		static Inventory inctance;
		return inctance;
	}
	bool Start() override {};
	void Update() override {};

	/// <summary>
	/// アイテムの個数を追加する関数。
	/// </summary>
	/// <param name="name">アイテムの名前。</param>
	/// <param name="addnum">追加する関数。</param>
	void AddItem(const wchar_t* name, int addnum);
	/// <summary>
	/// アイテムデータを返してくる関数。
	/// </summary>
	/// <returns>アイテムデータ。</returns>
	std::unordered_map<
		std::wstring,			//アイテムの名前。
		int						//アイテムの個数。
	>* GetItemDataMap() {
		return &m_itemDataMap;
	}
private:
	//アイテムデータ。
	std::unordered_map<
		std::wstring,			//アイテムの名前。
		int						//アイテムの個数。
	>	m_itemDataMap;
};

static inline 	void AddItem(const wchar_t* name, int addnum) {
	Inventory::GetInstance().AddItem(name, addnum);
}

static inline std::unordered_map<
	std::wstring,			//アイテムの名前。
	int						//アイテムの個数。
>* GetItemDataMap() {
	return Inventory::GetInstance().GetItemDataMap();
}

