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
	SpriteRender*	m_sprite;		//‰æ‘œB
	CSoundSource	m_se;			//“ü—Í‚ÌSEB
};

