#pragma once

class Game;
class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	void Update() override;
	void Draw() override;
	
private:
	Game* m_game = nullptr;							//�Q�[���̃|�C���^�B
	Player*	m_player = nullptr;						//�v���C���[�̃|�C���^�B
	CVector3 m_toCameraTarget = CVector3::Zero();	
	CVector3 m_toCameraPos = CVector3::Zero();		
	CVector3 m_target = CVector3::Zero();			//�����_�B
	CVector3 m_pos = CVector3::Zero();				//���_�B
	float toCameraPosRotAngle = 0.f;				//���_�̊p�x
	float toCameraTargetRotAngle = 0.f;				//�����_�̊p�x
};
