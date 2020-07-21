#pragma once
#include "IGameObject.h"
#include <unordered_map>

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
	bool Start() override { return true; };
	void Update() override {};

	/// <summary>
	/// アイテムを追加したり、
	/// 個数の変動を行う関数。
	/// </summary>
	/// <param name="name">アイテムの名前。</param>
	/// <param name="addnum">追加する関数。</param>
	void AddItem(const wchar_t* name, int addnum);
	/// <summary>
	/// アイテムデータを返してくる関数。
	/// </summary>
	/// <returns>アイテムデータ。</returns>
	std::unordered_map<
		std::wstring,		
		int					
	> GetItemDataMap() {
		return m_itemDataMap;
	}
	/// <summary>
	/// アイテムデータが追加されたどうかを返す関数。
	/// </summary>
	/// <returns>m_isAddData</returns>
	bool GetIsAddData() {
		return m_isAddData;
	}
	/// <summary>
	/// アイテムの個数が追加されたどうかを返す関数。
	/// </summary>
	/// <returns>m_isAddNum</returns>
	bool GetIsAddNum() {
		return m_isAddNum;
	}
	/// <summary>
	/// アイテムの総数を返す関数。
	/// </summary>
	/// <returns>m_itemCount</returns>
	int GetItemCount() {
		return m_itemCount;
	}
	/// <summary>
	/// 追加の有無の判定をリセットする関数。
	/// </summary>
	void ResetIsAddData() {
		m_isAddData = false;
		m_isAddNum = false;
	}
private:
	//アイテムデータ。
	std::unordered_map<
		std::wstring,			//アイテムの名前。
		int						//アイテムの個数。
	>	m_itemDataMap;

	bool	m_isAddData = false;	//アイテムデータが追加されたどうか。
	bool	m_isAddNum = false;		//アイテムの個数が追加されたどうか。
	int		m_itemCount = 0;		//アイテムの総数。
};

static inline 	void AddItem(const wchar_t* name, int addnum) {
	Inventory::GetInstance().AddItem(name, addnum);
}

static inline std::unordered_map<
	std::wstring,			
	int						
> GetItemDataMap() {
	return Inventory::GetInstance().GetItemDataMap();
}

static inline void ResetIsAddData() {
	Inventory::GetInstance().ResetIsAddData();
}

static inline bool GetIsAddData() {
	return Inventory::GetInstance().GetIsAddData();
}

static inline bool GetIsAddNum() {
	return Inventory::GetInstance().GetIsAddNum();
}

static inline int GetItemCount() {
	return Inventory::GetInstance().GetItemCount();
}