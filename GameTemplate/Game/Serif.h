#pragma once
#include "IGameObject.h"
const CVector2 FONT_POS = { -600.0f,-300.0f };
const CVector2 FONT_PIVOT = { 2.0f,2.0f };
class Game;
class Serif :
	public IGameObject
{
public:
	Serif();
	~Serif();
	//シングルトン。
	static Serif* GetSerif() {
		return m_serif;
	}
	bool Start() override;
	void Update() override;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="serif"></param>
	void Message(const wchar_t* serif);
	void Sound(wchar_t* filePath, bool isLoop) {
		//ワンショット再生のSE
		CSoundSource* m_se = new CSoundSource;
		m_se->Init(filePath);
		m_se->Play(isLoop);
	}
private:
	static Serif*	m_serif;					//Gameクラスのインスタンス。
	SpriteRender*	m_sprite = nullptr;
	CVector2		m_spriteSize = { 1280.0f,780.0f };	//表示される画像の大きさ。
	FontRender*		m_font = nullptr;
	bool			m_isSerif = false;
	int m_displayCount = 0;
	Game*			m_game = nullptr;
};

static inline void Message(const wchar_t* serif) {
	Serif::GetSerif()->Message(serif);
}

static inline void Sound(wchar_t* filePath, bool isLoop) {
	Serif::GetSerif()->Sound(filePath,isLoop);
}