#include "stdafx.h"
#include "ZombieDeath.h"
#include "Zombie.h"

void ZombieDeath::Update()
{
	//HP��0�ȉ��ɂȂ�����
	if (m_zombie->m_hp <= 0) {
		//���ʁB
		m_zombie->m_state = enState_death;
	}
}
