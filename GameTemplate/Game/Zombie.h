#pragma once
#include "character/CharacterController.h"
#include "AStar.h"
#include "Navimesh.h"

class Player;
class Floor;

class Zombie :
	public IGameObject
{
public:
	Zombie();
	~Zombie();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �X�e�[�g��ύX����֐��B
	/// ����ȏꍇ�������B
	/// </summary>
	void ChangeState();
	/// <summary>
	/// �ړ��B
	/// </summary>
	void Move();
	/// <summary>
	/// A*���s���֐��B
	/// </summary>
	void Astar();
	/// <summary>
	/// ��]����֐��B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �U������֐��B
	/// </summary>
	void Attack();
	/// <summary>
	/// �_���[�W���󂯂�֐��B
	/// </summary>
	void Damage();
	/// ���ʊ֐��B
	/// </summary>
	void Death();
	/// <summary>
	/// �]���r�̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// �]���r�̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model = nullptr;						//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	CharacterController m_charaCon;								//�L�����N�^�[�R���g���[���[�B

	//A*�֌W�B
	Game*			m_game = nullptr;			//�Q�[���N���X�̃|�C���^�B
	Player*			m_player = nullptr;			//�v���C���[�N���X�̃|�C���^�B
	Floor*			m_floor = nullptr;			//���N���X�̃|�C���^�B
	AStar			m_aStar;					//A*�N���X�̃C���X�^���X�B
	Navimesh*		m_nav = nullptr;			//�i�r���b�V���N���X�̃|�C���^�B
	std::vector<CVector3> m_moveList;			//A*��ɎZ�o���ꂽ�S�[���܂ł̃��[�g���X�g�B
	std::vector<CVector3>::iterator m_itr;		//m_moveList�̃C�e���[�^�B
	bool			m_isAstar = false;			//�S�[�����Ă��邩�ǂ����B
	bool			m_isPoint = false;			//�e*itr�ɓ��B�������ǂ����B
	bool			m_isHit   = false;			//�R���W�����Ƀq�b�g�������ǂ����B

	//�A�j���[�V�����֌W�B
	enum {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_attack,
		enAnimationClip_death,
		enAnimationClip_num,
	};
	AnimationClip	m_animationClip[enAnimationClip_num];		//�A�j���[�V�����N���b�v�B
	Animation		m_animation;

	//�X�e�[�g�֌W�B
	enum State {
		enState_idle,
		enState_walk,
		enState_attack,
		enState_death,
		enState_num,
	};
	State			m_state = enState_idle;
	int				m_coolTimer = 0;						//�U����̃N�[���^�C���B

	//�ړ��֌W�B
	CVector3		m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	float			m_speed = 200.f;						//�L�����������X�s�[�h�B
	BoxCollider		m_collider;								//�Z���p�̃{�b�N�X�R���C�_�[
	CVector3		m_boxSize = CVector3::One() * 70.0f;	//�R���C�_�[�̃T�C�Y�B
	bool			m_isFind = false;						//�v���C���[�������������ǂ����B

	//�U���֌W�B
	int				m_atkTimer = 0;

	//�X�e�[�^�X�֌W�B
	int				m_hp = 3;		//HP�B
};