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
	SpriteRender*	m_sprite = nullptr;		//画像のポインタ。
	CSoundSource	m_se;					//入力時のSE。
};