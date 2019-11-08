#include "stdafx.h"
#include "GameObjectManager.h"

//GameObjectManagerクラスのインスタンス。
GameObjectManager g_goMgr;

void GameObjectManager::Update()
{
	//登録されているゲームオブジェクトの更新処理を呼ぶ。
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			go->Update();
		}
		for (auto go : m_goList[i]) {
			go->Draw();
		}
	}

	//全てのゲームオブジェクトの1フレーム分の処理が終わってから、削除する。
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto it = m_goList[i].begin(); it != m_goList[i].end();) {
			if ((*it)->IsRequestDelete()) {
				//削除リクエストを受けているので削除。
				delete* it;
				it = m_goList[i].erase(it);
			}
			else {
				//リクエストを受けていないので。
				it++;
			}
		}
	}
}