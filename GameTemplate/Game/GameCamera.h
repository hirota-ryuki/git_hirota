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
	CVector3 GetPos()
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
	CVector3 GetTarget()
	{
		return m_target;
	}
	/// <summary>
	/// �����_���Z�b�g�B
	/// </summary>
	void SetTargetFromPos(const float& targetFromPos)
	{
		m_targetFromPos = targetFromPos;
	}
	/// <summary>
	/// �����_���擾�B
	/// </summary>
	/// <returns>�����_�B</returns>
	float GetTargetFromPos()
	{
		return m_targetFromPos;
	}
	/// <summary>
	/// �����_���Z�b�g�B
	/// </summary>
	void SetTargetFromPlayer(const float& targetFromPlayer)
	{
		m_targetFromPlayer = targetFromPlayer;
	}
	/// <summary>
	/// �����_���擾�B
	/// </summary>
	/// <returns>�����_�B</returns>
	float GetTargetFromPlayer()
	{
		return m_targetFromPlayer;
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
	float			toCameraPosRotAngle = 0.f;				//���_�̊p�x�B
	float			toCameraTargetRotAngle = 0.f;			//�����_�̊p�x�B
	//float			m_targetFromPos = -1100.0f;			//�����_�̊p�x�B
	//float			m_targetFromPlayer = 1000.0f;			//�����_�̊p�x�B
	float			m_targetFromPos = -1100.0f;			//�����_�̊p�x�B
	float			m_targetFromPlayer = 1000.0f;			//�����_�̊p�x�B
	bool			m_isMapMode = false;
	//�΂˃J�����B
	SpringCamera m_springCamera; //�΂˃J�����B
};
