#pragma once

#include "IGameObject.h"

//NewGOの優先順位
enum prio {
	GOPrio_Defalut = 2,
	GOPrio_Sprite,
	GOPrio_num = 5,
};

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
	T* NewGO(int prio)
	{
		T* newObj = new T;
		m_goList[prio].push_back(newObj);
		return newObj;
	}

	/// <summary>
	/// ゲームオブジェクトをリストから削除する。
	/// </summary>
	/// <param name="go">削除するゲームオブジェクト</param>
	void DeleteGO(IGameObject* go)
	{
		for (int i = 0; i < GOPrio_num; i++) {
			//リストから検索して、見つかったら削除する。
			for (auto it = m_goList[i].begin();
				it != m_goList[i].end();
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
	}
private:
	std::vector< IGameObject* > m_goList[5];		//ゲームオブジェクトのリスト。
};

//外部からアクセスするので、extern宣言も必要。
extern GameObjectManager g_goMgr;

template <class T>
static inline T* NewGO(int prio)
{
	return g_goMgr.NewGO<T>(prio);
}
static inline void DeleteGO(IGameObject* go)
{
	return g_goMgr.DeleteGO(go);
}