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
	//name��map���猩����B
	auto it = m_itemDataMap.find(name);
	if (it == m_itemDataMap.end())
	{
		//�V�K�ǉ��B
		m_itemDataMap.emplace(name, addnum);
		m_isAddData = true;
		m_itemCount++;
	}
	else {
		//���łɒǉ�����Ă�������̂ݕϓ�������B
		it->second += addnum;
	}
}
