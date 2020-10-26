#pragma once
#include "IZombieComponent.h"
#include "AStar.h"
#include "Navimesh.h"

class Player;
class Floor;

class ZombieStateMachine :
	public IZombieComponent
{
public:
	void Start()override;
	void Update()override;
private:
	/// <summary>
	/// �C���X�^���X���������B
	/// </summary>
	void InitInstance();
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
	/// A*�������I��������K�v�����邩����B
	/// </summary>
	/// <remarks>
	/// �ŏI�n�_���v���C���[������Ă�����A*�����I���ɂȂ�܂��B
	/// </remarks>
	/// <returns>true���Ԃ��Ă����狭���I���B</returns>
	bool IsEndAStarForce() const;
	/// <summary>
	/// �v���C���[�ɑ΂��ă��C�L���X�g�B
	/// </summary>
	/// <returns>true���Ԃ��Ă����烌�C�������̃R���C�_�[�ƏՓ˂��Ă��܂��B</returns>
	template<class TCallback>
	bool RaycastToPlayer() const;
	/// <summary>
	/// ���݂��U���B
	/// �g�p���Ă��Ȃ��֐��B
	/// </summary>
	void En_Bite();
	/// <summary>
	/// �]���r�ɍ��킹�ăv���C���[�̊p�x���C������֐��B
	/// �g�p���Ă��Ȃ��֐��B
	/// </summary>
	void AngleCorrection();
private:
	//�A�j���[�V�����֌W�B
	const float		m_comprementTime[6] = { 0.2f,0.2f,0.2f,0.2f,0.1f,0.2f };			//�A�j���[�V�����̕⊮���ԁB�A�j���[�V�����N���b�v�̏��ԁB

	//�ړ��֌W�B
	CVector3		m_moveSpeed = CVector3::Zero();										//�ړ����x�B
	const float		WALK_SPEED = 150.f;													//�L�����������X�s�[�h�B
	bool			m_isFind = false;													//�v���C���[�������������ǂ����B
	int				m_aStarCount = 0;													//A*�̖����v�Z���[�v�̉��p�̃J�E���^�B
	const float		NOT_ASTAR_DISTANCE = 200.0f;										//A*���s��Ȃ������B	
	const float		NOT_ASTAR_DISTANCE_SQ = NOT_ASTAR_DISTANCE * NOT_ASTAR_DISTANCE;	//NOT_ASTAR_DISTANCE��2��B
	const float		ARRIVAL_DISTANCE = 50.0f;											//�����������ǂ������肷�邽�߂̋����B
	const float		END_ASTSR_OF_NEER_PLAYER_SQ = 200.0f * 200.0f;						//�v���C���[�̋߂��ɗ�����A*���I�������鎞�̋�����2��B
	const float		END_ASTSR_OF_LEAVE_FINALPOINT_SQ = 300.0f * 300.0f;					//�o�H�̍ŏI�n�_����v���C���[������Ă�����A*���I�������鎞�̋�����2��B
	CVector3		m_endPos = CVector3::Zero();										//�ŏI�n�_�B

	//A*�֌W�B
	Game*			m_game = nullptr;				//�Q�[���N���X�̃|�C���^�B
	Player*			m_player = nullptr;				//�v���C���[�N���X�̃|�C���^�B
	Floor*			m_floor = nullptr;				//���N���X�̃|�C���^�B
	AStar			m_aStar;						//A*�N���X�̃C���X�^���X�B
	Navimesh*		m_nav = nullptr;				//�i�r���b�V���N���X�̃|�C���^�B
	std::vector<CVector3>			m_moveList;		//A*��ɎZ�o���ꂽ�S�[���܂ł̃��[�g���X�g�B
	std::vector<CVector3>::iterator m_itr;			//m_moveList�̃C�e���[�^�B
	bool			m_isAstar = false;				//�S�[�����Ă��邩�ǂ����B
	bool			m_isPoint = false;				//�e*itr�ɓ��B�������ǂ����B
	bool			m_isHit = false;				//�R���W�����Ƀq�b�g�������ǂ����B
	bool			m_isMove = false;

	//�U���֌W�B
	int				m_atkTimer = 0;
	bool			m_isAttack = false;										//�U���������ǂ����B
	const int		ATK_INTERVAL = 60;										//�U�����s���Ԋu�B
	const float		ATTACK_DISTANCE = 100.0f;								//�U�����s�������B
	const float		ATTACK_DISTANCE_SQ = ATTACK_DISTANCE * ATTACK_DISTANCE;	//ATTACK_DISTANCE��2��B
	const float		FIND_DISTANCE = 1000.0f;								//�v���C���[�ɍU�����s�����������B
	const float		FIND_DISTANCE_SQ = FIND_DISTANCE * FIND_DISTANCE;		//FIND_DISTANCE��2��B
	const float		ATTACK_VIEWING_ANGLE = 45.0f;							//�U���̎���p�B

	//�R���C�_�[�֌W�B
	BoxCollider		m_collider;												//�Z���p�̃{�b�N�X�R���C�_�[�B
	const float		BOX_SIZE_DOUBLE = 30.0f;								//�R���C�_�[���g�傷�邽�߂̔{���B
	CVector3		m_boxSize = CVector3::One() * BOX_SIZE_DOUBLE;			//�R���C�_�[�̃T�C�Y�B

	//���݂��֌W�B
	bool			m_isBite = false;
};

