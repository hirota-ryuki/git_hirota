#pragma once
class Title :
	public IGameObject
{
public:
	Title();
	~Title();
	void Update() override;
	void Draw() override {};
private:
	SpriteRender* m_sprite;		//‰æ‘œ
};

