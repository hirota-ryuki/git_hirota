#pragma once
//#include "character/CharacterController.h"

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Update() override;
	/// <summary>
	/// �v���C���[�̓����B
	/// </summary>
	void Move();
	/// <summary>
	/// �v���C���[�̉�]�B
	/// </summary>
	void Rotation();
	//void Render() override;
	void Draw() override;
	/// <summary>
	/// �v���C���[�̃|�W�V�������擾�B
	/// </summary>
	CVector3 GetPos()
	{
		return m_position;
	}
	/// <summary>
	/// �v���C���[�̉�]���擾�B
	/// </summary>
	CQuaternion GetRot()
	{
		return m_rotation;
	}
private:
	SkinModel m_model;									//�X�L�����f���B
	Sprite m_sprite;
	CVector3 m_position = CVector3::Zero();				//���W�B
	//CharacterController m_charaCon;					//�L�����N�^�[�R���g���[���[�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	CVector3 m_scale= CVector3::One();					//�傫���B
	bool isRun;											//�����Ă��邩�ǂ���
	float RunParam = 2.f;								//����Ƃ��̔{�� �����2�{�ɂȂ�B
	float m_speed = 10.f;								//�L�����������X�s�[�h�B
	
};