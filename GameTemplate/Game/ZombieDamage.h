#pragma once
#include "IZombieComponent.h"

/// <summary>
/// �]���r�̃_���[�W�����B
/// </summary>
class ZombieDamage : public IZombieComponent
{
public:
	void Update()override;
private:
	const int		HEAD_SHOT_DAMAGE = 5;					//�w�b�h�V���b�g�̃_���[�W�ʁB
	const int		NORMAL_DAMAGE = 1;						//�ʏ�̃_���[�W�ʁB
	const float		RENGE_OF_HEAD_COLLISION = 20.0f;		//���̓����蔻��̑傫���B
	const float		RENGE_OF_BODY_COLLISION = 60.0f;		//�̂̓����蔻��̑傫���B
	const float		RAISE_CENTER_BODY = 70.0f;				//�����蔻��͋��̂Ȃ̂ő�������̂̒��S�ɏグ�邽�߂̏㏸�ʁB

	//���f�[�^�B
	//�w�b�h�V���b�g�̔���Ɗ��݂��U���̔���Ɏg�p����B	
	CVector3		m_bonePos = CVector3::Zero();			//���̍��W�B
	CQuaternion		m_boneRot = CQuaternion::Identity();	//���̉�]�B
	CVector3		m_boneScale = CVector3::Zero();			//���̊g�嗦�B

};

