#pragma once
#include "character/CharacterController.h"

//#define DEBUG_MODE

class IZombieComponent;
class ZombieDamage;
class ZombieDeath;
class ZombieStateMachine;

//�X�e�[�g�֌W�B
enum State {
	enState_idle,
	enState_walk,
	enState_attack,
	enState_bite,
	enState_knockback,
	enState_death,
	enState_num,
};

class Zombie :
	public IGameObject
{
	friend class ZombieDamage;
	friend class ZombieDeath;
	friend class ZombieStateMachine;
public:
	Zombie();
	~Zombie();
	void OnDestroy() override;
	bool Start() override;
	void Update() override {};
	void Update_NotPause() override;
	/// <summary>
	/// ����p���v�Z����֐��B
	/// </summary>
	float CalcViewingAngleDeg(CVector3 v1, CVector3 v2);
	/// <summary>
	/// �]���r�̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// �]���r�̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	/// <summary>
	/// �A�j���[�V�����N���b�v���������B
	/// </summary>
	void InitAnimationClips();
	/// <summary>
	/// ���f�����������B
	/// </summary>
	void InitModel();
	/// <summary>
	/// �R���|�[�l���g�̏������B
	/// </summary>
	void InitComponents();
	
private:
	std::list< IZombieComponent* > m_component;					//�]���r�̃R���|�[�l���g�̃��X�g�B

	SkinModelRender*	m_model = nullptr;						//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	CharacterController m_charaCon;								//�L�����N�^�[�R���g���[���[�B
	
	State			m_state = enState_idle;

	int				m_coolTimer = 0;							//�U����̃N�[���^�C���B

	//�A�j���[�V�����֌W�B
	enum {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_attack,
		enAnimationClip_bite,
		enAnimationClip_knockback,
		enAnimationClip_death,
		enAnimationClip_num,
	};
	AnimationClip	m_animationClip[enAnimationClip_num];			//�A�j���[�V�����N���b�v�B
	Animation		m_animation;

	//�R���C�_�[�֌W�B
	BoxCollider		m_collider;										//�Z���p�̃{�b�N�X�R���C�_�[�B
	const float		BOX_SIZE_DOUBLE = 30.0f;						//�R���C�_�[���g�傷�邽�߂̔{���B
	CVector3		m_boxSize = CVector3::One() * BOX_SIZE_DOUBLE;	//�R���C�_�[�̃T�C�Y�B
	
	
	//���݂��֌W�B
	bool			m_isBite = false;

	//�X�e�[�^�X�֌W�B
	int				m_hp = 10;		//HP�B

#ifdef DEBUG_MODE
	SkinModelRender*	m_debugModel = nullptr;						//�X�L�����f���B
	CQuaternion			m_debugrotation = CQuaternion::Identity();	//��]�B
#endif //DEBUG_MODE
};