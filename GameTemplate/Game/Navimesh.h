#pragma once
#include "Physics/ICollider.h"
#include "physics/PhysicsStaticObject.h"

//セルクラス
struct Cell {
	CVector3 vertexPos[3];	//セルの頂点のポジション
	Cell* linkCell[3];		//セルに隣接しているセル
	CVector3 centerPos;		//セルの真ん中のポジション
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
	CVector3 m_position = CVector3::Zero();				//座標。
	CQuaternion m_rotation = CQuaternion::SpriteRot();	//回転。
	CVector3 m_scale = CVector3::One();					//大きさ。
	PhysicsStaticObject m_physicsStaticObject;
};