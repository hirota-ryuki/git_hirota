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
	//nameをmapから見つける。
	auto it = m_itemDataMap.find(name);
	if (it == m_itemDataMap.end())
	{
		//新規追加。
		m_itemDataMap.emplace(name, addnum);
		m_isAddData = true;
		m_itemCount++;
	}
	else {
		//すでに追加されていたら個数のみ変動させる。
		it->second += addnum;
	}
}
