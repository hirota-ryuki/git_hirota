#include "stdafx.h"
#include "AStar.h"


AStar::AStar()
{
}


AStar::~AStar()
{
}

void AStar::AstarSearch(Cell* startcell, Cell* endcell)
{
	//リストを初期化
	m_openList.clear();
	m_closeList.clear();

	//スタートから現在地までのコストの初期化
	startcell->costFromStart = 0;
	//現在地からゴールまでのコストの計算
	startcell->costToGoal = Length(startcell, endcell);
	//親のセルの初期化
	startcell->parentCell = NULL;
	//オープンリストに登録
	m_openList.push_back(startcell);
	//オープンリストが空になるまで
	while (m_openList.empty() !=true)
	{
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
		//このセルがゴールだったら
		if (cell == endcell) {
			//終了
			break;
		}
		//除外
		for (auto it = m_openList.begin(); it != m_openList.end(); it++) {
			if (*it==cell) {
				//オープンリストから削除
				m_openList.erase(it);
				break;
			}
		}
		//クローズリストに登録
		m_closeList.push_back(cell);
		//このセルにリンクしているセルのコストを計算して、オープンリストに積む。
		for (int i = 0; i < 3; i++)
		{
			//セルが隣接していたら
			if (cell->linkCells[i] != NULL)
			{
				Cell* link = cell->linkCells[i];
				//クローズリストの中に隣接セルが無いか最初から最後まで探す
				auto itFind = std::find(m_closeList.begin(), m_closeList.end(), link);
				//リストの最後まで行ったら
				//endのセルがlinkだったら？
				if (itFind == m_closeList.end()) {
					//スタートから調査していたセルの距離と
					//調査していたセルと隣接セルの距離を足す
					link->costFromStart = cell->costFromStart + Length(cell, link);
					//隣接セルとゴールまでの距離
					link->costToGoal = Length(link, endcell);
					//調査していたセルを親として登録
					link->parentCell = cell;
					//オープンリストに隣接セルを登録
					m_openList.push_back(link);
				}
				
			}
		}
	}
}

float AStar::Length(Cell * startcell, Cell * endcell)
{
	CVector3 length;
	length = endcell->centerPos - startcell->centerPos;
	return length.Length();
}
