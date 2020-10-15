#include "stdafx.h"
#include "ZombieStateMachine.h"
#include "Zombie.h"

void ZombieStateMachine::Update()
{
	switch (m_zombie->m_state) {
	case enState_idle:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_idle, 0.2f);

		//�U����ƃm�b�N�o�b�N��̃N�[���^�C���B
		if (m_zombie->m_coolTimer > 0) {
			m_zombie->m_coolTimer++;
			if (m_zombie->m_coolTimer > 50) {
				//�^�C�}�[�̃��Z�b�g�B
				m_zombie->m_coolTimer = 0;
			}
		}
		else {
			m_zombie->ChangeState();
		}
		break;
	case enState_walk:
		//�A�j���[�V�����̍Đ��B
		if (m_zombie->m_aStarCount > 100) {
			m_zombie->m_animation.Play(m_zombie->enAnimationClip_idle, 0.2f);
		}
		else {
			m_zombie->m_animation.Play(m_zombie->enAnimationClip_walk, 0.2f);
		}
		m_zombie->Move();
		m_zombie->ChangeState();
		break;
	case enState_attack:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_attack, 0.2f);
		m_zombie->m_atkTimer++;
		if (m_zombie->m_atkTimer >= m_zombie->ATK_INTERVAL && !m_zombie->m_isAttack) {
			//�U���B
			m_zombie->Attack();
			m_zombie->m_isAttack = true;
		}
		//�A�j���[�V�����̍Đ�������Ȃ�������B
		if (!m_zombie->m_animation.IsPlaying()) {
			//�ҋ@��ԂɑJ�ځB
			m_zombie->m_state = m_zombie->enState_idle;
			m_zombie->m_coolTimer++;
			m_zombie->m_atkTimer = 0;
			m_zombie->m_isAttack = false;
		}
		break;
	case enState_bite:
		m_zombie->En_Bite();
		break;
	case enState_knockback:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_knockback, 0.1f);
		//�A�j���[�V�����̍Đ�������Ȃ�������B
		if (!m_zombie->m_animation.IsPlaying()) {
			//�ҋ@��ԂɑJ�ځB
			m_zombie->m_state = m_zombie->enState_idle;
			m_zombie->m_coolTimer++;
		}
		break;
	case enState_death:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_death, 0.2f);
		//�A�j���[�V�����̍Đ�������Ȃ�������B
		if (!m_zombie->m_animation.IsPlaying())
		{
			//DeleteGO(this);
			m_zombie->m_charaCon.RemoveRigidBoby();
		}
		break;
	default:
		break;
	}
}
