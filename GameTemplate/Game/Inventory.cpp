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
	}
	else {
		//���łɒǉ�����Ă�������̂ݕϓ�������B
		it->second += addnum;
		m_isAddNum=true;
	}
}

int Inventory::FindItem(const wchar_t * name)
{
	//�A�C�e���f�[�^�}�b�v�̃C�e���[�^�B
	auto itr = m_itemDataMap.begin();
	while (1) {
		//�C�e���[�^���Ō�܂œ��B������B
		if (itr == m_itemDataMap.end()) {
			return 0;
		}
		//�������O�����łɑ��݂��Ă�����B
		if (wcscmp(name, itr->first.c_str()) == 0) {
			return itr->second;
		}
		itr++;
	}
}