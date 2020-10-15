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
		//�ǉ�����鐔��0�Ȃ珈���𒆒f�B
		return;
	}
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
			m_isDeleteItem = true;
			m_isAddNum = false;
		}
	}
}

int Inventory::FindItem(const wchar_t * name)
{
	//�A�C�e���f�[�^�}�b�v�̃C�e���[�^�B
	auto itr = m_itemDataMap.find(name);
	if (itr == m_itemDataMap.end()) {
		//������Ȃ������B
		return 0;
	}
	return itr->second;
}