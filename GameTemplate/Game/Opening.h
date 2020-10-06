#pragma once
#include "IGameObject.h"

class Opening :
	public IGameObject
{
public:
	Opening();
	~Opening();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void Update_NotPause() override;
private:
	enum ESprite {
		eSprite_Comment_1,		//一言目のセリフ用のスプライト
		eSprite_Comment_2,		//二言目のセリフ用のスプライト
		eSprite_Comment_Num,	//セリフ用のスプライトの枚数
	};

	SpriteRender*	m_commentSprite[eSprite_Comment_Num];	//セリフ用のスプレイトのポインタ。
	Game*			m_game = nullptr;		//Gameのポインタ。
	bool			m_isButton = false;		//ボタンを押したかどうか。
	int				m_poseTimer = 0;		//一時停止させるまでのタイマー。
	const int		POSE_TIME_LIMIT = 10;	//10秒経ったら一時停止させる。
	CSoundSource	m_se;
};