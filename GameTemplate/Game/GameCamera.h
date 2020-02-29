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
	/// ���_���擾�B
	/// </summary>
	/// <returns>���_�B</returns>
	CVector3 GetPos()
	{
		return m_pos;
	}
	/// <summary>
	/// �����_���擾�B
	/// </summary>
	/// <returns>�����_�B</returns>
	CVector3 GetTarget()
	{
		return m_target;
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
	bool			m_isMapMode = false;
	//�Ə��摜�֌W�B
	SpriteRender*	m_sprite = nullptr;
	CQuaternion		m_rot = CQuaternion::SpriteRot();
	SpringCamera m_springCamera; //�΂˃J�����B

};
