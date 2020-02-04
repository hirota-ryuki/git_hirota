#pragma once
#include "IGameObject.h"
class Pose :
	public IGameObject
{
public:
	Pose();
	~Pose();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
private:
	SpriteRender*	m_sprite = nullptr;		//画像のポインタ。
};

