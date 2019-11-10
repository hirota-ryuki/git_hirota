#include "stdafx.h"
#include "Navimesh.h"

Navimesh::Navimesh()
{
	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/battlefield/floor.cmo");
	m_model->SetData(m_position, m_rotation, m_scale);
	m_physicsStaticObject.CreateMeshObject(m_model->GetModel(), m_position, m_rotation);
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
	//インデックスの数だけ回す
	for (int i = 0; i < m_indexBufferArray.size(); i++) {
		//インデックスバッファの配列
		//頂点の場所
		auto& indexBuffer = m_indexBufferArray[i];
		//頂点バッファの配列
		//一つの頂点バッファにつき1パーツ
		auto& vertexBuffer = m_vertexBufferArray[i];
		//セルの生成
		Cell* cell = new Cell;

		//ポリゴンの数
		//ポリゴンは頂点3つで形成されるので3で割っている
		int numPoly = indexBuffer->size() / 3;
		//頂点の数だけ回す
		for(int polyNo = 0; polyNo < numPoly; polyNo++){
			//ポリゴン一個に対する3か所の頂点の場所を割り出す
			int vertNo_0 = indexBuffer->at(polyNo * 3);
			int vertNo_1 = indexBuffer->at(polyNo * 3 + 1);
			int vertNo_2 = indexBuffer->at(polyNo * 3 + 2);
			//セルに頂点の場所を格納
			cell->vertexPos[0] = vertexBuffer->at(vertNo_0);
			cell->vertexPos[1] = vertexBuffer->at(vertNo_1);
			cell->vertexPos[2] = vertexBuffer->at(vertNo_2);
		
		}
	}
}
