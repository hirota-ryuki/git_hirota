#pragma once
class Player;
class Map :
	public IGameObject
{
public:
	Map();
	~Map();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
private:
	Game*			m_game = nullptr;						//Gameクラスのインスタンス。
	Player*			m_player = nullptr;						//Playerクラスのポインタ。
	SpriteRender*	m_mapSprite = nullptr;
	//CVector2		m_mapSpriteSize = { 850.0f,500.0f };	//表示される画像の大きさ。
	CVector2		m_mapSpriteSize = { 20.0f*37.0f,20.0f*27.0f };	//表示される画像の大きさ。
	CVector2		m_mapSpritePos = { 0.0f,0.0f };			//表示される画像の座標。
	SpriteRender*	m_playerSprite = nullptr;
	//CVector2		m_mapSize = { 17000.0f,10000.0f };		//実際のフィールドの大きさ。
	CVector2		m_mapSize = { 3700.0f,2700.0f };		//実際のフィールドの大きさ。
	CVector2		m_ratio = CVector2::Zero();				//フィールドから画像への縮小率。画像/フィールド。
	bool			m_isMap = false;						//マップ画面が表示されているかどうか。
};