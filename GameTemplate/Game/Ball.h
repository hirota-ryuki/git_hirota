#pragma once
#include "IGameObject.h"
class Player;
class Pose;

class Ball :
	public IItem
{
public:
	Ball();
	~Ball();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void OnGet() override;	
	/// <summary>
	/// ボールのポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ボールの回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// ボールの識別番号をセット。
	/// </summary>
	/// <param name="pos">識別番号。</param>
	void SetNomber(int no)
	{
		m_nomber = no;
	}
private:
	SkinModelRender*	m_model = nullptr;								//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	int					m_nomber = 0;							//識別番号。		
	Game*				m_game = nullptr;						//Gameのポインタ。
	Player*				m_player = nullptr;						//プレイヤークラスのポインタ。
	Pose*				m_pose = nullptr;

	//ポップアップ画像関係。
	SpriteRender*		m_sprite = nullptr;
	CVector2			m_spritePos = FRAME_OUT_POS;
};

