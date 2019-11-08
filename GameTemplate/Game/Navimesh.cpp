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
	//�R�[���o�b�N�֐�
	model.FindMesh([&](const auto& mesh) {
		ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//���_�o�b�t�@���쐬�B
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
				//�o�C�A�X��������B
				mBias.Mul(pos);
				vertexBuffer->push_back(pos);
				//���̒��_�ցB
				pData += mesh->vertexStride;
			}
			//���_�o�b�t�@���A�����b�N
			deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
			m_vertexBufferArray.push_back(std::move(vertexBuffer));
		}
		//�C���f�b�N�X�o�b�t�@���쐬�B
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			//�C���f�b�N�X�o�b�t�@�����b�N�B
			HRESULT hr = deviceContext->Map(mesh->indexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->indexBuffer->GetDesc(&bufferDesc);
			//@todo cmo�t�@�C���̓C���f�b�N�X�o�b�t�@�̃T�C�Y��2byte�Œ�B
			IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
			int stride = 2;
			int indexCount = bufferDesc.ByteWidth / stride;
			unsigned short* pIndex = reinterpret_cast<unsigned short*>(subresource.pData);
			for (int i = 0; i < indexCount; i++) {
				indexBuffer->push_back(pIndex[i]);
			}
			//�C���f�b�N�X�o�b�t�@���A�����b�N�B
			deviceContext->Unmap(mesh->indexBuffer.Get(), 0);
			m_indexBufferArray.push_back(std::move(indexBuffer));
		}
		}
	);
	//�C���f�b�N�X�̐�������
	for (int i = 0; i < m_indexBufferArray.size(); i++) {
		Cell* cell = new Cell;
		//cell->vertexPos[0] = m_indexBufferArray.at(i)->at(i);
		cell->vertexPos[1] = m_vertexBufferArray.at(i + 1)->at(i + 1);
		cell->vertexPos[2] = m_vertexBufferArray.at(i + 2)->at(i + 2);
	}
}
