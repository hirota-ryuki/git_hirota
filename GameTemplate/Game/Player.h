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
	/// ���Ԃɂ���ĉ񕜂���֐��B
	/// </summary>
	void Heal();
	/// <summary>
	/// �_���[�W��^����֐��B
	/// </summary>
	void Damage();
	/// ���ʊ֐��B
	/// </summary>
	void Death();
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
	/// <param name="rot">��]</param>
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
private:
	SkinModelRender*	m_model = nullptr;						//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CharacterController m_charaCon;								//�L�����N�^�[�R���g���[���[�B
	CVector3			m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	float m_speed = 300.0f;			//�L�����������X�s�[�h�B
	float m_runSpeed = 500.0f;		//�L����������X�s�[�h�B
	
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
		enState_death,
		enState_num,
	};
	State			m_state = enState_idle;
	bool			m_isRun = false;		//�����Ă��邩�ǂ����B
	bool			m_isShot = false;		//�����Ă��邩�ǂ����B
	//Shot�֌W�B
	Bullet*			m_bullet = nullptr;		//Bullet�N���X�̃|�C���^�B
	int				m_timer = 0;			//�^�C�}�[�B
	bool			m_isBullet = false;		//���������ǂ����B
	//�_���[�W�摜�֌W�B
	SpriteRender*	m_sprite = nullptr;
	CQuaternion		m_rot = CQuaternion::SpriteRot();
	float			m_alpha = 0.0f;
	//�񕜊֌W�B
	bool			m_isDamage = false;			//�_���[�W���󂯂����ǂ����B
	int 			m_healTimer = 0;			//�񕜂���܂ł̃^�C�}�[�B
	int				m_healStartTime = 130;		//�񕜂���܂ł̎��ԁB
	int 			m_heaIntervalTimer = 0;		//�񕜂Ɖ񕜂̊Ԃ̃^�C�}�[�B���X�ɉ񕜂���悤�ɂ���B
	int				m_healIntervalTime = 30;	//�񕜂Ɖ񕜂̊Ԃ̎��ԁB���X�ɉ񕜂���悤�ɂ���B
	//�X�e�[�^�X�֌W�B
	float			m_hp = 5.0f;		//�̗́B
	const float		m_maxhp = 5.0f;		//�ő�̗́B
};