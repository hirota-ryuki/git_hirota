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
	/// <summary>
	/// スムージング。
	/// </summary>
	void Smoothing();
	/// <summary>
	/// セルのリストを返す関数。
	/// </summary>
	/// <returns>m_moveCellList</returns>
	const std::vector<CVector3>& GetMoveList() const
	{
		return m_moveCellList;
	}
private:
	std::vector<Cell*> m_openList;					//未調査のオープンリスト。
	std::vector<Cell*> m_closeList;					//調査済みのオープンリスト。
	std::vector<CVector3> m_moveCellListTmp;		//スムージング前の算出されたセルのリスト。
	std::vector<CVector3> m_moveCellSmoothListTmp;	//スムージング後の算出されたセルのリスト。
	std::vector<CVector3>::iterator m_itr;			//m_moveCellListTmpのイテレータ。
	std::vector<CVector3> m_moveCellList;			//最終的に移動するセルのリスト。
	BoxCollider m_collider;							//セル用のボックスコライダー。
	CVector3	m_boxSize = CVector3::One()*20.0f;	//コライダーのサイズ。
	float		m_boxHeight = 40.0f;				//箱を設置する高さ。		
};