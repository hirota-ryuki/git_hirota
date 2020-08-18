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
	//�ǉ���������0�łȂ�������A���������s�B
	if (addnum != 0) {
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
			m_isAddNum = true;
			//����0�ɂȂ�����B
			if (it->second <= 0) {
				//�A�C�e���f�[�^�}�b�v����폜�B
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