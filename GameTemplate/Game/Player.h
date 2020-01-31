#pragma once
#include "character/CharacterController.h"

class Bullet;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �X�e�[�g��ύX����֐��B
	/// ����ȏꍇ�������B
	/// </summary>
	void ChangeState();
	/// <summary>
	/// �v���C���[�̓����B
	/// </summary>
	void Move();
	/// <summary>
	/// �v���C���[�̉�]�B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �v���C���[�̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// �v���C���[�̃|�W�V�������擾�B
	/// </summary>
	CVector3 GetPos()
	{
		return m_position;
	}
	/// <summary>
	/// �v���C���[�̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �v���C���[�̉�]���擾�B
	/// </summary>
	CQuaternion GetRot()
	{
		return m_rotation;
	}
	/// <summary>
	/// �_���[�W��^����֐��B
	/// </summary>
	void Damage()
	{
		m_hp--;
	}
private:
	SkinModelRender* m_model;							//�X�L�����f���B
	CVector3 m_position = CVector3::Zero();				//���W�B
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	Game* m_game = nullptr;								//Game�̃|�C���^�B
	//bool isRun;										//�����Ă��邩�ǂ����B
	//float RunParam = 2.0f;							//����Ƃ��̔{�� �����2�{�ɂȂ�B
	float m_speed = 300.0f;								//�L�����������X�s�[�h�B
	float m_runSpeed = 500.0f;							//�L����������X�s�[�h�B
	
	//�A�j���[�V�����֌W�B
	enum {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_walk_left,
		enAnimationClip_walk_right,
		enAnimationClip_back,
		enAnimationClip_run,
		enAnimationClip_aim,
		enAnimationClip_shot,
		enAnimationClip_num,
	};
	AnimationClip m_animationClip[enAnimationClip_num];		//�A�j���[�V�����N���b�v�B
	Animation m_animation;//�A�j���[�V�����̃C���X�^���X�B
	//�X�e�[�g�֌W�B
	enum State {
		enState_idle,
		enState_walk,
		enState_run,
		enState_aim,
		enState_shot,
		enState_num,
	};
	State m_state = enState_idle;
	bool m_isRun = false;			//�����Ă��邩�ǂ����B
	bool m_isShot = false;			//�����Ă��邩�ǂ����B
	//Shot�֌W�B
	Bullet* m_bullet;//Bullet�N���X�̃|�C���^�B
	int m_timer = 0;//�^�C�}�[�B
	bool m_isBullet = false;//���������ǂ����B
	//�X�e�[�^�X�֌W�B
	int m_hp = 10;
};