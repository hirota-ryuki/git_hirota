#pragma once
#include "IGameObject.h"
class Result :
	public IGameObject
{
public:
	Result();
	~Result();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �N���A�����Ƃ��ɌĂяo���֐��B
	/// </summary>
	void SetIsClear() {
		m_isClear = true;
	}
private:
	SpriteRender* m_sprite;		//�摜
	bool m_isClear = false;
};

