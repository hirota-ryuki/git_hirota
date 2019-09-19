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
	Game* m_game = nullptr;							//ゲームのポインタ。
	Player*	m_player = nullptr;						//プレイヤーのポインタ。
	CVector3 m_toCameraTarget = CVector3::Zero();	
	CVector3 m_toCameraPos = CVector3::Zero();		
	CVector3 m_target = CVector3::Zero();			//注視点。
	CVector3 m_pos = CVector3::Zero();				//視点。
	float toCameraPosRotAngle = 0.f;				//視点の角度
	float toCameraTargetRotAngle = 0.f;				//注視点の角度
};
