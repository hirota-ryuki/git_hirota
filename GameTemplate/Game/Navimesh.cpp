#include "stdafx.h"
#include "Navimesh.h"
#include "Physics/MeshCollider.h"

Navimesh::Navimesh()
{
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
		Cell* cell = new Cell;
		//cell->vertexPos[0] = m_indexBufferArray.at(i)->at(i);
		cell->vertexPos[1] = m_vertexBufferArray.at(i + 1)->at(i + 1);
		cell->vertexPos[2] = m_vertexBufferArray.at(i + 2)->at(i + 2);
	}
}
