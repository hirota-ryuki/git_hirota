#include "stdafx.h"
#include "AStar.h"
#include "physics/CollisionAttr.h"

AStar::AStar()
{
}


AStar::~AStar()
{
}

void AStar::AstarSearch(Cell* startcell, Cell* endcell)
{
	//リストを初期化。
	m_openList.clear();
	m_closeList.clear();

	//スタートから現在地までのコストの初期化。
	startcell->costFromStart = 0;
	//現在地からゴールまでのコストの計算。
	startcell->costToGoal = Length(startcell, endcell);
	//親のセルの初期化。
	startcell->parentCell = NULL;
	//オープンリストに登録。
	m_openList.push_back(startcell);
	bool isSuccess = false;
	//クラッシュを防ぐため。
	int crashCount = 0;
	//オープンリストが空になるまで。
	while (m_openList.empty() != true)
	{
		//カウント。
		crashCount++;
		//オープンリストの最初のセル
		Cell* cell = m_openList[0];
		//最終的なコストの計算
		float cost = cell->costFromStart + cell->costToGoal;
		//一番コストが安いセルを探す。
		for (int i = 1; i < m_openList.size(); i++)
		{
			if (cost > m_openList[i]->costFromStart + m_openList[i]->costToGoal)
			{
				cost = m_openList[i]->costFromStart + m_openList[i]->costToGoal;
				cell = m_openList[i];
			}
		}
		//このセルがゴールだったら。
		if (cell == endcell) {
			//終了。
			isSuccess = true;
			break;
		}
		//除外。
		for (auto it = m_openList.begin(); it != m_openList.end(); it++) {
			if (*it == cell) {
				//オープンリストから削除。
				m_openList.erase(it);
				break;
			}
		}
		//クローズリストに登録。
		cell->state = Cell::State_Closed; //閉じる。
		m_closeList.push_back(cell);
		//このセルにリンクしているセルのコストを計算して、オープンリストに積む。
		for (int i = 0; i < 3; i++)
		{
			//セルが隣接していたら。
			if (cell->linkCells[i] != NULL)
			{
				Cell* link = cell->linkCells[i];
			
				//リストの最後まで行ったら。
				if (link->state ==Cell::State_NotResearch) { //未調査なら
					//スタートから調査していたセルの距離と
					//調査していたセルと隣接セルの距離を足す。
					link->costFromStart = cell->costFromStart + Length(cell, link);
					//隣接セルとゴールまでの距離。
					link->costToGoal = Length(link, endcell);
					//調査していたセルを親として登録。
					link->parentCell = cell;
					//オープンリストに隣接セルを登録。
					link->state = Cell::State_Opened; //オープンリストに積まれた。
					m_openList.push_back(link);
				}

			}
		}
		//600回ループしたら強制終了。
		if (crashCount == 600) {
			//isSuccesshはfalseのまま。
			break;
		}
	}
	m_moveCellListTmp.clear();
	if (isSuccess) {
		//経路を格納。
		Cell* cell = endcell;
		while (true)
		{
			//セルの重心を格納。
			m_moveCellListTmp.push_back(cell->centerPos);

			//調査しているセルがスタート地点だったら終わり。
			if (cell == startcell)
			{
				break;
			}
			//セルの親を格納。
			cell = cell->parentCell;
		}
		//要素はゴール地点からスタート地点までの順番で格納されているので反転させる。
		std::reverse(m_moveCellListTmp.begin(), m_moveCellListTmp.end());
		//m_moveCellList = m_moveCellListTmp;
		//スムージング。
		Smoothing();
	}
	
	//調査状態をリセット。
	for (auto cell : m_openList) {
		cell->state = Cell::State_NotResearch;
	}
	for (auto cell : m_closeList) {
		cell->state = Cell::State_NotResearch;
	}
}

float AStar::Length(Cell * startcell, Cell * endcell)
{
	CVector3 length;
	length = endcell->centerPos - startcell->centerPos;
	return length.Length();
}

struct CallBack : public btCollisionWorld::ConvexResultCallback
{
	//障害物があるかないか判定。
	bool isHit = false;
	//衝突したら勝手に呼んでくれる。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Map
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_RigidBody) {
			//当たった。
			isHit = true;
		}
		return 0;
	}
};

void AStar::Smoothing()
{
	////初期化。////
	//イテレータの初期化。
	m_itr = m_moveCellListTmp.begin();
	//最初の座標の設定。
	CVector3 startPos = *m_itr;
	//リストの初期化。
	m_moveCellSmoothListTmp.clear();
	m_moveCellList.clear();
	//スタート位置を格納。
	m_moveCellSmoothListTmp.push_back(startPos);
	//イテレータを進めておく。
	m_itr++;
	//コライダーの設定。
	m_collider.Create(m_boxSize);
	//移動可能な座標を記憶しておくため。
	//CVector3 oldPos = startPos;
	CVector3 oldPos;
	//クラッシュを防ぐため。
	int crashCount = 0;
	bool isSuccess = false;
	while (1) {
		//カウント。
		crashCount++;
		//コリジョンの移動の始点と終点の設定。
		btTransform start, end;
		{
			//回転の設定。
			start.setIdentity();
			end.setIdentity();
			//座標の設定。
			CVector3 endPos = *m_itr;
			start.setOrigin(btVector3(startPos.x, startPos.y + m_boxHeight, startPos.z));
			end.setOrigin(btVector3(endPos.x, startPos.y + m_boxHeight, endPos.z));
		}
		CallBack callback;
		//startからendまでコリジョンを移動させて当たり判定を取る。
		g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
		//コリジョンにヒットしなかったら。
		if (callback.isHit == false) {
			oldPos = *m_itr;
			//イテレータを一つ進める。
			m_itr++;
		}
		else {
			//現在地を設定。
			startPos = oldPos;
			//移動先を格納。
			m_moveCellSmoothListTmp.push_back(startPos);
		}
		//終わりに達したら。
		if (m_itr == m_moveCellListTmp.end()) {
			//現在地を設定。
			startPos = oldPos;
			//移動先を格納。
			m_moveCellSmoothListTmp.push_back(startPos);
			isSuccess = true;
			break;
		}
		//100回ループしたら強制終了。
		if (crashCount >= 100) {
			break;
		}
	}
	if (isSuccess) {
		m_moveCellList = m_moveCellSmoothListTmp;
	}
}