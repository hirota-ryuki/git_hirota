#pragma once
#include "Navimesh.h"

class AStar
{
public:
	AStar();
	~AStar();
	/// <summary>
	/// A*
	/// </summary>
	/// <param name="startcell">敵のいる地点</param>
	/// <param name="endcell">ゴール地点</param>
	void AstarSearch(Cell* startcell,Cell* endcell);
	/// <summary>
	/// 2点間の距離を求める関数
	/// </summary>
	/// <param name="startcell">スタート地点</param>
	/// <param name="endcell">エンド地点</param>
	/// <returns>2点間の距離</returns>
	float Length(Cell* startcell, Cell* endcell);
private:
	std::vector<Cell*> m_openList;		//未調査のオープンリスト
	std::vector<Cell*> m_closeList;		//調査済みのオープンリスト
};

