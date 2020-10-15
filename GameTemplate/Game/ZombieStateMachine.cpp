#include "stdafx.h"
#include "ZombieStateMachine.h"
#include "Zombie.h"

void ZombieStateMachine::Update()
{
	switch (m_zombie->m_state) {
	case enState_idle:
		//アニメーションの再生。
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_idle, 0.2f);

		//攻撃後とノックバック後のクールタイム。
		if (m_zombie->m_coolTimer > 0) {
			m_zombie->m_coolTimer++;
			if (m_zombie->m_coolTimer > 50) {
				//タイマーのリセット。
				m_zombie->m_coolTimer = 0;
			}
		}
		else {
			m_zombie->ChangeState();
		}
		break;
	case enState_walk:
		//アニメーションの再生。
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
		//アニメーションの再生。
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_attack, 0.2f);
		m_zombie->m_atkTimer++;
		if (m_zombie->m_atkTimer >= m_zombie->ATK_INTERVAL && !m_zombie->m_isAttack) {
			//攻撃。
			m_zombie->Attack();
			m_zombie->m_isAttack = true;
		}
		//アニメーションの再生中じゃなかったら。
		if (!m_zombie->m_animation.IsPlaying()) {
			//待機状態に遷移。
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
		//アニメーションの再生。
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_knockback, 0.1f);
		//アニメーションの再生中じゃなかったら。
		if (!m_zombie->m_animation.IsPlaying()) {
			//待機状態に遷移。
			m_zombie->m_state = m_zombie->enState_idle;
			m_zombie->m_coolTimer++;
		}
		break;
	case enState_death:
		//アニメーションの再生。
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_death, 0.2f);
		//アニメーションの再生中じゃなかったら。
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
