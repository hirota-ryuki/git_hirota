#pragma once
class Title :
	public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override;
	void Update() override;
private:
	SpriteRender* m_sprite;		//‰æ‘œ
};

