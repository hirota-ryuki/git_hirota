#pragma once
#include "SpringCamera.h"

class Game;
class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void Update_NotPause() override;
	/// <summary>
	/// ���_���Z�b�g�B
	/// </summary>
	void SetPos(const CVector3& pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// ���_���擾�B
	/// </summary>
	/// <returns>���_�B</returns>
	const CVector3& GetPos() const
	{
		return m_pos;
	}
	/// <summary>
	/// �����_���Z�b�g�B
	/// </summary>
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	/// <summary>
	/// �����_���擾�B
	/// </summary>
	/// <returns>�����_�B</returns>
	const CVector3& GetTarget() const
	{
		return m_target;
	}
	/// <summary>
	/// �v���C���[���璍���_���Z�b�g�B
	/// </summary>
	void SetTargetFromPlayer(const float& targetFromPlayer)
	{
		m_targetFromPlayer = targetFromPlayer;
	}
	/// <summary>
	/// �v���C���[���璍���_���擾�B
	/// </summary>
	/// <returns>�v���C���[���璍���_�B</returns>
	const float& GetTargetFromPlayer() const
	{
		return m_targetFromPlayer;
	}
	/// <summary>
	/// �����_���王�_���Z�b�g�B
	/// </summary>
	void SetPosFromTarget(const float& posFromTarget)
	{
		m_posFromTarget = posFromTarget;
	}
	/// <summary>
	/// �����_���王�_���擾�B
	/// </summary>
	/// <returns>�����_���王�_�B</returns>
	const float& GetPosFromTarget() const
	{
		return m_posFromTarget;
	}
	/// <summary>
	/// ���_��n�ʂ���㏸������ʂ��Z�b�g�B
	/// </summary>
	void SetAddY(const float& playerPosAddY)
	{
		m_playerPosAddY = playerPosAddY;
	}
	/// <summary>
	/// ���_��n�ʂ���㏸������ʂ��擾�B
	/// </summary>
	/// <returns>���_��n�ʂ���㏸������ʁB</returns>
	const float& GetAddY() const
	{
		return m_playerPosAddY;
	}
private:
	//�C���X�^���X�֌W�B
	Game*			m_game = nullptr;						//�Q�[���̃|�C���^�B
	Player*			m_player = nullptr;						//�v���C���[�̃|�C���^�B
	//�J�����֌W�B
	CVector3		m_toCameraTarget = CVector3::Zero();	
	CVector3		m_toCameraPos = CVector3::Zero();		
	CVector3		m_target = CVector3::Zero();			//�����_�B
	CVector3		m_pos = CVector3::Zero();				//���_�B
	float			m_toCameraPosRotAngle = 0.f;			//���_�̊p�x�B
	float			m_toCameraTargetRotAngle = 0.f;			//�����_�̊p�x�B
	const float		POS_ANGLE_UP_LIMIT = 60.0f;				//���_�̏�̌��E�l�B
	const float		POS_ANGLE_DOWN_LIMIT = -70.0f;			//���_�̉��̌��E�l�B
	float			m_targetFromPlayer = 100.0f;			//�v���C���[���璍���_�B
	float			m_posFromTarget = -200.0f;				//�����_���王�_�B
	float			m_playerPosAddY = 150.0f;				//���_��n�ʂ���㏸������ʁB
	SpringCamera	m_springCamera;							//�΂˃J�����B
};
