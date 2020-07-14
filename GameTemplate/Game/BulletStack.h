#pragma once
class Player;
class Pose;
class BulletStack :
	public IItem
{
public:
	BulletStack();
	~BulletStack();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void GettingItem(bool isGetItem) override;
	static void sp();
	/// <summary>
	/// ポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ポジションを取得。
	/// </summary>
	CVector3 GetPos()
	{
		return m_position;
	}
	/// <summary>
	/// 回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
	
private:
	//モデル関係。
	SkinModelRender*	m_model = nullptr;
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	//インスタンス関係。
	Game*				m_game = nullptr;						//Gameのポインタ。
	Player*				m_player = nullptr;						//プレイヤークラスのポインタ。
	//Pose*				m_pose = nullptr;
	//ライト関係。
	CVector4			m_directionLightDirection = { 1.0f, -1.0f, 0.0f, 0.0f };
	CVector4			m_directionLightColor = { 0.5f, 0.5f, 0.5f, 10.2f };
	float				m_spec = 5.0f;
	CVector3			m_ambientLight = CVector3::Zero();

	//ポップアップ画像関係。
	const int			ADD_BULLET_STACK = 10;					//落ちている弾薬の数。		
	SpriteRender*		m_sprite = nullptr;
	CVector2			m_spritePos = FRAME_OUT_POS;

	SpriteRender*		m_spriteA = nullptr;
	CVector4			m_model2Dpos = CVector4::Black();		//3Dモデルを2Dの座標に変換した座標。
};

