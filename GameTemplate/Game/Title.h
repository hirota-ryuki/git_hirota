#pragma once
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
	SpriteRender*	m_sprite;		//�摜�B
	CSoundSource	m_se;			//���͎���SE�B
};

