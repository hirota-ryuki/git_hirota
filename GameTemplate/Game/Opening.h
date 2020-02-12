#pragma once
#include "IGameObject.h"

#define EVENT_SCENE 2;

class Opening :
	public IGameObject
{
public:
	Opening();
	~Opening();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
private:
	SpriteRender*	m_sprite[2];			//画像のポインタ。
	Game*			m_game = nullptr;		//Gameのポインタ。
	bool			m_isButton = false;		//ボタンを押したかどうか。
	int				m_poseTimer = 0;		//一時停止させるまでのタイマー。
	CSoundSource	m_se;
};