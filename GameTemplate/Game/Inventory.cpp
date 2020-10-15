#include "stdafx.h"
#include "Inventory.h"


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

void Inventory::AddItem(const wchar_t * name, int addnum)
{
	if (addnum == 0) {
		//追加される数が0なら処理を中断。
		return;
	}
	//nameをmapから見つける。
	auto it = m_itemDataMap.find(name);
	if (it == m_itemDataMap.end())
	{
		//新規追加。
		m_itemDataMap.emplace(name, addnum);
		m_isAddData = true;
	}
	else {
		//すでに追加されていたら個数のみ変動させる。
		it->second += addnum;
		m_isAddNum = true;
		//個数が0になったら。
		if (it->second <= 0) {
			//アイテムデータマップから削除。
			it = m_itemDataMap.erase(it);
			m_isDeleteItem = true;
			m_isAddNum = false;
		}
	}
}

int Inventory::FindItem(const wchar_t * name)
{
	//アイテムデータマップのイテレータ。
	auto itr = m_itemDataMap.find(name);
	if (itr == m_itemDataMap.end()) {
		//見つからなかった。
		return 0;
	}
	return itr->second;
}