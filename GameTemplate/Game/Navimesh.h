#pragma once
#include "Physics/ICollider.h"
#include "physics/PhysicsStaticObject.h"
#include "physics/CapsuleCollider.h"

//セルクラス
struct Cell {
	CVector3 vertexPos[3];	//セルの頂点のポジション
	Cell* linkCells[3];		//セルに隣接しているセル
	CVector3 centerPos;		//セルの真ん中のポジション
	int linkMax = 0;		//隣接しているセルの個数
	float costFromStart;
	float costToGoal;
	Cell* parentCell;
};

class Navimesh :
	public IGameObject
{
public:
	Navimesh();
	~Navimesh();
	void Create(SkinModel& model);
	void Update() override {};
	void Draw() override {};
	std::vector<Cell*> GetCell()
	{
		return m_cells;
	}
private:
	//typedefは既存の型名に新しい名前を付ける
	typedef std::vector<CVector3>					VertexBuffer;			//頂点バッファ。
	typedef std::vector<unsigned int>				IndexBuffer;	//インデックスバッファ。
	//unique_ptrはコピー不可能なクラス
	typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
	typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
	std::vector<VertexBufferPtr>					m_vertexBufferArray;	//頂点バッファの配列。
	std::vector<IndexBufferPtr>						m_indexBufferArray;		//インデックスバッファの配列。
	
	SkinModelRender* m_model = nullptr;					//スキンモデル。
	SkinModelRender* m_skin = nullptr;					//スキンモデル。
	CapsuleCollider m_collider;							//セル用のカプセルコライダー
	std::vector<Cell*>					m_cells;		//セルの配列。
};