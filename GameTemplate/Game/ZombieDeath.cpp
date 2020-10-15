#include "stdafx.h"
#include "ZombieDeath.h"
#include "Zombie.h"

void ZombieDeath::Update()
{
	//HP‚ª0ˆÈ‰º‚É‚È‚Á‚½‚ç
	if (m_zombie->m_hp <= 0) {
		//Ž€‚ÊB
		m_zombie->m_state = m_zombie->enState_death;
	}
}
