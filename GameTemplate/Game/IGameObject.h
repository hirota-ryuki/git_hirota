#pragma once
class IGameObject
{
public:
	/// <summary>
	/// 更新関数。
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画関数。
	/// </summary>
	//virtual void Render() = 0;
	/// <summary>/// 
	/// /// 描画関数。
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// 削除リクエストを行う。
	/// </summary>
	void RequestDelete()
	{
		isRequestDelete = true;
	}
	/// <summary>
	/// 削除リクエストを受けているか調べる。
	/// </summary>
	/// <returns>
	/// trueが返ってきたら、リクエストを受けている。
	/// </returns>
	bool IsRequestDelete()
	{
		return isRequestDelete;
	}
private:
	bool isRequestDelete = false;		//削除フラグ
};

