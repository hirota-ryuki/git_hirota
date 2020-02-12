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
	/// クリアしたときに呼び出す関数。
	/// </summary>
	void SetIsClear() {
		m_isClear = true;
	}
private:
	SpriteRender* m_sprite = nullptr;	//画像のポインタ。
	bool m_isClear = false;				//クリアしたかどうか。
};

