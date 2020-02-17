#pragma once
#include "graphics/font/Font.h"

class Title :
	public IGameObject
{
public:
	Title();
	~Title();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
private:
	SpriteRender*	m_sprite = nullptr;		//画像のポインタ。
	CSoundSource   m_se;
	std::unique_ptr<Font>	m_font;
	float					m_fps = 30.0f;
};