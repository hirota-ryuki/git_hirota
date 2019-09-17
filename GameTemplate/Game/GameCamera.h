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
	Game* m_game;
	Player*	m_player;								//�v���C���[�̃|�C���^�B
	Camera m_camera;								//�J�����B
	CVector3 m_toCameraTarget = CVector3::Zero();	
	CVector3 m_toCameraPos = CVector3::Zero();		
	CVector3 m_target = CVector3::Zero();			//�����_�B
	CVector3 m_pos = CVector3::Zero();				//���_�B
	float toCameraPosRotAngle = 0.f;
	float toCameraTargetRotAngle = 0.f;
};
