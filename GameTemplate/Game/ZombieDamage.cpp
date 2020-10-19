#include "stdafx.h"
#include "ZombieDamage.h"
#include "Bullet.h"
#include "Zombie.h"

void ZombieDamage::Update()
{
	//頭との衝突判定を行う。
	QueryGOs<Bullet>("bullet", [&](Bullet * bullet)->bool {
		//頭の骨の読み込み。
		auto& model = m_zombie->m_model->GetModel();
		auto bone = model.FindBone(L"Head");
		bone->CalcWorldTRS(
			m_bonePos, 
			m_boneRot, 
			m_boneScale
		);
		//頭の判定。
		CVector3 diff = bullet->GetPos() - m_bonePos;
		if (diff.Length() < RENGE_OF_HEAD_COLLISION) {
			m_zombie->m_hp = m_zombie->m_hp - HEAD_SHOT_DAMAGE;
			m_zombie->m_state = m_zombie->enState_knockback;
			DeleteGO(bullet);
		}
		//体の判定。
		else {
			CVector3 pos = m_zombie->m_position;
			pos.y += RAISE_CENTER_BODY;
			CVector3 diff2 = bullet->GetPos() - pos;
			if (diff2.Length() < RENGE_OF_BODY_COLLISION) {
				m_zombie->m_hp--;
				DeleteGO(bullet);
			}
		}
		return true;
	});
}