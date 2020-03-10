#pragma once
class Player;
class UI :
	public IGameObject
{
public:
	UI();
	~UI();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// フォントの表示、非表示を管理する関数。
	/// </summary>
	void CangeActives(bool active) {
		m_fontCapacity->ActiveMode(active);
		m_fontStack->ActiveMode(active);
		m_fontLine->ActiveMode(active);
	}
private:
	//残弾数関係。
	Game*			m_game = nullptr;				//Gameクラスのインスタンス。
	Player*			m_player = nullptr;				//Playerクラスのポインタ。
	FontRender*		m_fontCapacity = nullptr;		//残弾数のフォントレンダー。
	FontRender*		m_fontStack = nullptr;			//装弾数のフォントレンダー。
	FontRender*		m_fontLine = nullptr;			//区切り線のフォントレンダー。
};

