#pragma once
#include "IGameObject.h"
class UI :
	public IGameObject
{
public:
	UI();
	~UI();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
private:
	//残弾数関係。
	Game*			m_game = nullptr;				//Gameクラスのインスタンス。
	Player*			m_player = nullptr;				//Playerクラスのポインタ。
	FontRender*		m_fontCapacity = nullptr;		//残弾数のフォントレンダー。
	FontRender*		m_fontMaxCapacity = nullptr;	//装弾数のフォントレンダー。
	FontRender*		m_fontLine = nullptr;			//区切り線のフォントレンダー。
};

