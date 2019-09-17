#include "stdafx.h"
#include "GameObjectManager.h"

//GameObjectManagerクラスのインスタンス。
GameObjectManager g_goMgr;

void GameObjectManager::Update()
{
	//登録されているゲームオブジェクトの更新処理を呼ぶ。
	for (auto go : m_goList) {
		go->Update();
	}
	/*for (auto go : m_goList) {
		go->Render();
	}*/
	for (auto go : m_goList) {
		go->Draw();
	}
	//全てのゲームオブジェクトの1フレーム分の処理が終わってから、削除する。
	for (auto it = m_goList.begin(); it != m_goList.end();) {
		if ((*it)->IsRequestDelete()) {
			//削除リクエストを受けているので削除。
			delete* it;
			it = m_goList.erase(it);
		}
		else {
			//リクエストを受けていないので。
			it++;
		}
	}
}