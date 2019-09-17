#pragma once

#include "IGameObject.h"

class GameObjectManager
{
public:
	/// <summary>
	/// 更新。
	/// </summary>
	void Update();
	/// <summary>
	/// ゲームオブジェクトを追加。
	/// </summary>
	template <class T>
	T* NewGO()
	{
		T* newObj = new T;
		m_goList.push_back(newObj);
		return newObj;
	}

	/// <summary>
	/// ゲームオブジェクトをリストから削除する。
	/// </summary>
	/// <param name="go">削除するゲームオブジェクト</param>
	void DeleteGO(IGameObject* go)
	{
		//リストから検索して、見つかったら削除する。
		for (auto it = m_goList.begin();
			it != m_goList.end();
			it++
			) {
			if ((*it) == go) {
				//見つかった。
				//削除リクエストを送る。
				go->RequestDelete();
				//削除できたので終わり。
				return;
			}
		}
	}
private:
	std::vector< IGameObject* > m_goList;		//ゲームオブジェクトのリスト。
};

//外部からアクセスするので、extern宣言も必要。
extern GameObjectManager g_goMgr;

