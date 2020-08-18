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
	//追加される個数が0でなかったら、処理を実行。
	if (addnum != 0) {
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
				//m_deleteItemDataMap.emplace(*it);
				m_isDeleteItem = true;
				m_isAddNum = false;
			}
		}
	}
}

int Inventory::FindItem(const wchar_t * name)
{
	//アイテムデータマップのイテレータ。
	auto itr = m_itemDataMap.begin();
	while (1) {
		//イテレータが最後まで到達したら。
		if (itr == m_itemDataMap.end()) {
			return 0;
		}
		//同じ名前がすでに存在していたら。
		if (wcscmp(name, itr->first.c_str()) == 0) {
			return itr->second;
		}
		itr++;
	}
}