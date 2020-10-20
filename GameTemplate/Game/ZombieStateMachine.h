#pragma once
#include "IZombieComponent.h"
class ZombieStateMachine :
	public IZombieComponent
{
public:
	void Update()override;
private:
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
private:
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

	//�ړ��֌W�B
	CVector3		m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	const float		WALK_SPEED = 150.f;						//�L�����������X�s�[�h�B
	bool			m_isFind = false;						//�v���C���[�������������ǂ����B
	int				m_aStarCount = 0;						//A*�̖����v�Z���[�v�̉��p�̃J�E���^�B
	const float		NOT_ASTAR_DISTANCE = 200.0f;			//A*���s��Ȃ������B	
	const float		NOT_ASTAR_DISTANCE_SQ = NOT_ASTAR_DISTANCE * NOT_ASTAR_DISTANCE;	//A*���s��Ȃ�������2��B
	const float		ARRIVAL_DISTANCE = 50.0f;							//�����������ǂ������肷�邽�߂̋����B
	const float		END_ASTSR_OF_NEER_PLAYER_SQ = 200.0f * 200.0f;		//�v���C���[�̋߂��ɗ�����A*���I�������鎞�̋�����2��B
	const float		END_ASTSR_OF_LEAVE_FINALPOINT_SQ = 300.0f * 300.0f;	//�o�H�̍ŏI�n�_����v���C���[������Ă�����A*���I�������鎞�̋�����2��B

	//�U���֌W�B
	int				m_atkTimer = 0;
	bool			m_isAttack = false;						//�U���������ǂ����B
	const int		ATK_INTERVAL = 60;						//�U�����s���Ԋu�B
	const float		ATTACK_DISTANCE = 100.0f;
	const float		ATTACK_DISTANCE_SQ = ATTACK_DISTANCE * ATTACK_DISTANCE;	//ATTACK_DISTANCE��2��B
	const float		FIND_DISTANCE = 1000.0f;				//�v���C���[�ɍU�����s�����������B
	const float		FIND_DISTANCE_SQ = FIND_DISTANCE * FIND_DISTANCE;	//FIND_DISTANCE��2��B
	const float		ATTACK_VIEWING_ANGLE = 45.0f;			//�U���̎���p�B

};

