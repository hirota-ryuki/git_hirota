#include "stdafx.h"
#include "Navimesh.h"

struct CallBack : public btCollisionWorld::ContactResultCallback
{
	//CallBackクラスのコンストラクタ
	CallBack()
		//初期化リスト
		//{}の前に初期化を行う
		//namespace　btCollisionWorld
		//構造体　ClosestConvexResultCallback
		//ClosestConvexResultCallbackはコンストラクタに引数があるので
		//初期化しなくてはならない
	{}
	//障害物があるかないか判定
	bool isHit = false;
	//衝突したら勝手に呼んでくれる
	//virtual  btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
	//{
	//	//当たった
	//	isHit = true;
	//	return 0;
	//}
	virtual  btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{
		//btManifoldPoint& cp　接触情報
		//btCollisionObjectWrapper　コリジョンオブジェクトの情報

		btScalar dist = cp.getDistance();
		// dist > 0の場合接触していない　最短距離
		// dist < 0なら重なっている　侵入距離

		//接触情報使った判定処理や情報収集

		return 0;//戻り値に意味なし　呼ぶ側が使っていない
	}
};

Navimesh::Navimesh()
{
	//セルの当たり判定用のカプセルコライダー
	//初期化
	//半径が1であるのは後々に三角形の一番長い重心頂点間に拡大するため
	m_collider.Create(1.f, 30.f);
	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/battlefield/floor.cmo");
	m_model->NotModelMode();
	Create(m_model->GetModel());
}


Navimesh::~Navimesh()
{
}

void Navimesh::Create(SkinModel& model)
{
	CMatrix mBias;
	mBias.MakeRotationX(CMath::PI * -0.5f);
	//コールバック関数
	model.FindMesh([&](const auto& mesh) {
		ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//頂点バッファを作成。
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			HRESULT hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->vertexBuffer->GetDesc(&bufferDesc);
			int vertexCount = bufferDesc.ByteWidth / mesh->vertexStride;
			char* pData = reinterpret_cast<char*>(subresource.pData);
			VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
			CVector3 pos;
			for (int i = 0; i < vertexCount; i++) {
				pos = *reinterpret_cast<CVector3*>(pData);
				//バイアスをかける。
				mBias.Mul(pos);
				vertexBuffer->push_back(pos);
				//次の頂点へ。
				pData += mesh->vertexStride;
			}
			//頂点バッファをアンロック
			deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
			m_vertexBufferArray.push_back(std::move(vertexBuffer));
		}
		//インデックスバッファを作成。
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			//インデックスバッファをロック。
			HRESULT hr = deviceContext->Map(mesh->indexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->indexBuffer->GetDesc(&bufferDesc);
			//@todo cmoファイルはインデックスバッファのサイズは2byte固定。
			IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
			int stride = 2;
			int indexCount = bufferDesc.ByteWidth / stride;
			unsigned short* pIndex = reinterpret_cast<unsigned short*>(subresource.pData);
			for (int i = 0; i < indexCount; i++) {
				indexBuffer->push_back(pIndex[i]);
			}
			//インデックスバッファをアンロック。
			deviceContext->Unmap(mesh->indexBuffer.Get(), 0);
			m_indexBufferArray.push_back(std::move(indexBuffer));
		}
		}
	);

	//セルに頂点3つと重心を設定し、セルに登録
	//m_vertexBufferArrayとm_indexBufferArrayの要素数は同じ
	for (int i = 0; i < m_indexBufferArray.size(); i++) {
		//インデックスバッファの配列
		//頂点の場所
		auto& indexBuffer = m_indexBufferArray[i];
		//頂点バッファの配列
		//一つの頂点バッファにつき1パーツ
		auto& vertexBuffer = m_vertexBufferArray[i];
		
		//ポリゴンの数
		//ポリゴンは頂点3つで形成されるので3で割っている
		int numPoly = indexBuffer->size() / 3;
		//ポリゴンの数だけ回す
		for (int polyNo = 0; polyNo < numPoly; polyNo++) {
			//セルの生成
			Cell* cell = new Cell;

			//頂点の格納
			{
				//ポリゴン一個に対する3か所の頂点の場所を割り出す
				int vertNo_0 = indexBuffer->at(polyNo * 3);
				int vertNo_1 = indexBuffer->at(polyNo * 3 + 1);
				int vertNo_2 = indexBuffer->at(polyNo * 3 + 2);
				//セルに頂点の場所を格納
				cell->vertexPos[0] = vertexBuffer->at(vertNo_0);
				cell->vertexPos[1] = vertexBuffer->at(vertNo_1);
				cell->vertexPos[2] = vertexBuffer->at(vertNo_2);
			}

			//centerPosの登録
			//三角形の重心
			//合計なので初期化
			cell->centerPos.Zero();
			cell->centerPos = cell->vertexPos[0] + cell->vertexPos[1] + cell->vertexPos[2];
			cell->centerPos /= 3;

			//カプセルコライダーの半径を
			//三角形の一番長い重心頂点間の長さに拡大
			{
				//重心から頂点までの長さを求める
				//これがカプセルコライダーの半径になる
				CVector3 radius[3];
				for (int i = 0; i < 3; i++)
				{
					radius[i] = cell->centerPos - cell->vertexPos[i];
				}
				//1番長い距離を比較で求める
				CVector3 maxLength;
				maxLength = radius[0];
				for (int i = 1; i < 3; i++)
				{
					if (maxLength.Length() < radius[i].Length())
					{
						maxLength = radius[i];
					}
				}
				//カプセルコライダーの半径を拡大
				m_collider.GetBody()->setLocalScaling(btVector3(maxLength.Length(), 1.0f, maxLength.Length()));
			}
			{
				//コリジョンの移動の始点と終点の設定
				//btTransformは座標、回転の情報を持っている
				//情報の初期化を行わなければならない
				btTransform start, end;
				{
					//回転の設定
					start.setIdentity();
					end.setIdentity();
					//座標の設定
					//Origin 意味：原点
					start.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y, cell->centerPos.z));
					end.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y + 10.f, cell->centerPos.z));
				}
				CallBack callback;
				//startからendまでコリジョンを移動させて当たり判定を取る
				g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
			}
			//if (callback.isHit == false) {
			//オブジェクトが上にあったら消えているかどうかの確認用。
			//ポリゴンの上に出現
			/*m_skin = NewGO<SkinModelRender>(GOPrio_Defalut);
			m_skin->Init(L"modelData/enemy/enemy.cmo");
			m_skin->SetPos(cell->centerPos);
*/
			//セルに登録
			m_cells.push_back(cell);
			//}
		}
	}

	//登録されたセルの数だけ回す
	for (auto &all : m_cells) 
	{		
		//オブジェクトが上にあったら消えているかどうかの確認用。
		//ポリゴンの上に出現
		m_skin = NewGO<SkinModelRender>(GOPrio_Defalut);
		m_skin->Init(L"modelData/enemy/enemy.cmo");
		m_skin->SetPos(all->centerPos);
	}
}
