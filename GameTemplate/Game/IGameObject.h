#pragma once
//NewGOの優先順位
enum Prio {
	GOPrio_Defalut = 2,
	GOPrio_Sprite,
	GOPrio_DrawAfter,
	GOPrio_num,
};

class IGameObject
{
public:
	//基底クラスには仮想デストラクタが必要
	//忘れないように
	virtual ~IGameObject() {}

	/// <summary>
	/// 開始関数。
	/// </summary>
	virtual bool Start() = 0;

	/// <summary>
	/// 更新関数。
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// ポーズされていない時だけ呼ばれる更新関数
	/// </summary>
	/// <remarks>
	/// この関数はゲームがポーズ状態になると呼ばれなくなります。
	/// ポーズ中に止めたい更新処理などは、この関数をオーバーライドして実装してください。
	/// </remarks>
	virtual void Update_NotPause() {}
	/// <summary>
	/// 削除の直前に呼ばれる関数。
	/// </summary>
	virtual void OnDestroy() {}

	/// <summary>
	///  描画関数。
	/// </summary>
	virtual void Draw() {}

	/// <summary>
	/// HUD描画時に呼ばれる処理。
	/// </summary>
	virtual void DrawHUD() {}
	
	/// /// <summary>
	/// HUD描画時に呼ばれる処理。
	/// </summary>
	virtual void DrawFont() {}

	/// <summary>
	/// 削除リクエストを行う。
	/// </summary>
	void RequestDelete()
	{
		m_isRequestDelete = true;
	}

	/// <summary>
	/// 削除リクエストを受けているか調べる。
	/// </summary>
	/// <returns>
	/// trueが返ってきたら、リクエストを受けている。
	/// </returns>
	bool IsRequestDelete()
	{
		return m_isRequestDelete;
	}
	/// <summary>
	/// 開始関数を呼んだか調べる。
	/// </summary>
	/// <returns>
	/// trueが返ってきたら、開始関数を呼んでいる。
	/// </returns>
	bool IsStart()
	{
		return m_isStart;
	}
	/// <summary>
	/// 開始関数を処理したら呼ぶ関数。
	/// </summary>
	void StartEnd()
	{
		m_isStart = true;
	}
	/// <summary>
	/// ネームキーをセットする関数。
	/// </summary>
	void SetNameKey(unsigned int namekey)
	{
		m_nameKey = namekey;
	}
	/// <summary>
	/// ネームキーを返す関数。
	/// </summary>
	/// <returns>ネームキー。</returns>
	unsigned int GetNameKey()
	{
		return m_nameKey;
	}
	/// <summary>
	/// 消された状態にする関数。
	/// </summary>
	void SetDelete()
	{
		m_isDelete = true;
	}
	/// <summary>
	/// 消されたかどうか。
	/// </summary>
	/// <returns>消されたフラグ。</returns>
	bool IsDelete()
	{
		return m_isDelete;
	}
	//friend宣言、フレンドクラスは指定されたクラスのprivateやprotectedメンバにアクセスすることができる。
	friend class GameObjectManager;
private:
	bool			m_isRequestDelete = false;	//削除フラグ。
	bool			m_isStart = false;			//開始関数が呼ばれたかどうか。
	bool			m_isDelete = false;			//消された状態かどうか。
	unsigned int	m_nameKey = 0;				//名前キー。
	int				prio;						//優先順位。
protected:
};