#include "stdafx.h"
#include "ZombieDeath.h"
#include "Zombie.h"

void ZombieDeath::Update()
{
	//HPが0以下になったら
	if (m_zombie->m_hp <= 0) {
		//死ぬ。
		m_zombie->m_state = m_zombie->enState_death;
	}
}
