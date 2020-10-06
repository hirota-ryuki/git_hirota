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
		static Inventory instance;
		return instance;
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
	/// アイテムの個数を返してくる関数。
	/// </summary>
	/// <param name="name">アイテムの名前。</param>
	/// <returns>個数。</returns>
	int FindItem(const wchar_t* name);
	/// <summary>
	/// アイテムデータを返してくる関数。
	/// </summary>
	/// <returns>アイテムデータ。</returns>
	const std::unordered_map<
		std::wstring,		
		int					
	>& GetItemDataMap() const{
		return m_itemDataMap;
	}
	/// <summary>
	/// アイテムデータが追加されたどうかを返す関数。
	/// </summary>
	/// <returns>m_isAddData</returns>
	bool GetIsAddData() const{
		return m_isAddData;
	}
	/// <summary>
	/// アイテムの個数が追加されたどうかを返す関数。
	/// </summary>
	/// <returns>m_isAddNum</returns>
	bool GetIsAddNum() const{
		return m_isAddNum;
	}
	/// <summary>
	/// アイテムの総数を返す関数。
	/// </summary>
	/// <returns>m_itemCount</returns>
	int GetItemCount() const {
		return m_itemDataMap.size();
	}
	/// <summary>
	/// 追加の有無の判定をリセットする関数。
	/// </summary>
	void ResetIsAddData() {
		m_isAddData = false;
		m_isAddNum = false;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetIsDeleteItem() {
		return m_isDeleteItem;
	}
	/// <summary>
	/// 削除の有無の判定をリセットする関数。
	/// </summary>
	void ResetIsDeleteItem() {
		m_isDeleteItem = false;
	}
private:
	//アイテムデータ。
	std::unordered_map<
		std::wstring,			//アイテムの名前。
		int						//アイテムの個数。
	>	m_itemDataMap;
	std::unordered_map<
		std::wstring,			//アイテムの名前。
		int						//アイテムの個数。
	>	m_deleteItemDataMap;

	bool	m_isAddData = false;		//アイテムデータが追加されたどうか。
	bool	m_isAddNum = false;			//アイテムの個数が追加されたどうか。
	bool	m_isDeleteItem = false;		//アイテムが削除されたどうか。
};

static inline void Inv_AddItem(const wchar_t* name, int addnum) {
	Inventory::GetInstance().AddItem(name, addnum);
}

static inline std::unordered_map<
	std::wstring,			
	int						
> Inv_GetItemDataMap() {
	return Inventory::GetInstance().GetItemDataMap();
}

static inline int Inv_FindItem(const wchar_t* name) {
	return Inventory::GetInstance().FindItem(name);
}

static inline void Inv_ResetIsAddData() {
	Inventory::GetInstance().ResetIsAddData();
}

static inline bool Inv_GetIsAddData() {
	return Inventory::GetInstance().GetIsAddData();
}

static inline bool Inv_GetIsAddNum() {
	return Inventory::GetInstance().GetIsAddNum();
}

static inline bool Inv_GetIsDeleteItem() {
	return Inventory::GetInstance().GetIsDeleteItem();
}

static inline int Inv_GetItemCount() {
	return Inventory::GetInstance().GetItemCount();
}