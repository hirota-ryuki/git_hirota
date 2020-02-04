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
	SpriteRender*	m_sprite = nullptr;		//�摜�̃|�C���^�B
	CSoundSource	m_se;					//���͎���SE�B
};