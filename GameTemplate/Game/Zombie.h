#pragma once
#include "character/CharacterController.h"
#include "AStar.h"
#include "Navimesh.h"
//#define DEBUG_MODE
const float NOT_ASTAR_DISTANCE = 200.0f;
const float ATTACK_DISTANCE = 100.0f;

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
	void Update_NotPause() override;
	/// <summary>
	/// ���݂��U���B
	/// </summary>
	void En_Bite();
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
	/// �X���[�W���O���ꂽ�o�H���ǂ���ɓ����֐��B
	/// Move�֐����ōs���B
	/// </summary>
	void Move_AStar();
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
	/// <summary>
	/// ���ʊ֐��B
	/// </summary>
	void Death();
	/// <summary>
	/// ����p���v�Z����֐��B
	/// </summary>
	float CalcViewingAngleDeg(CVector3 v1, CVector3 v2);
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
	bool			m_isMove = false;
	CVector3		m_endPos = CVector3::Zero();//�ŏI�n�_�B
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
	AnimationClip	m_animationClip[enAnimationClip_num];		//�A�j���[�V�����N���b�v�B
	Animation		m_animation;

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
	State			m_state = enState_idle;
	int				m_coolTimer = 0;						//�U����̃N�[���^�C���B

	//�ړ��֌W�B
	CVector3		m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	const float		m_speed = 150.f;						//�L�����������X�s�[�h�B
	BoxCollider		m_collider;								//�Z���p�̃{�b�N�X�R���C�_�[
	CVector3		m_boxSize = CVector3::One() * 30.0f;	//�R���C�_�[�̃T�C�Y�B
	bool			m_isFind = false;						//�v���C���[�������������ǂ����B
	int				m_aStarCount = 0;						//A*�̖����v�Z���[�v�̉��p�̃J�E���^�B

	//�U���֌W�B
	int				m_atkTimer = 0;
	bool			m_isAttack = false;//�U���������ǂ����B

	//���f�[�^�B
	//�w�b�h�V���b�g�̔���Ɗ��݂��U���̔���Ɏg�p����B	
	CVector3		m_bonePos = CVector3::Zero();			//���̍��W�B
	CQuaternion		m_boneRot = CQuaternion::Identity();	//���̉�]�B
	CVector3		m_boneScale = CVector3::Zero();			//���̊g�嗦�B

	//���݂��֌W�B
	bool			m_isBite = false;

	//�X�e�[�^�X�֌W�B
	int				m_hp = 10;		//HP�B

#ifdef DEBUG_MODE
	SkinModelRender*	m_debugModel = nullptr;						//�X�L�����f���B
	CQuaternion			m_debugrotation = CQuaternion::Identity();	//��]�B
#endif //DEBUG_MODE
};