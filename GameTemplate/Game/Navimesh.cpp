#include "stdafx.h"
#include "Navimesh.h"

struct CallBack : public btCollisionWorld::ConvexResultCallback
{
	/// <summary>
	/// ���l
		//CallBack�N���X�̃R���X�g���N�^
		//CallBack()
		//���������X�g
		//{}�̑O�ɏ��������s��
		//namespace�@btCollisionWorld
		//�\���́@ClosestConvexResultCallback
		//ClosestConvexResultCallback�̓R���X�g���N�^�Ɉ���������̂�
		//���������Ȃ��Ă͂Ȃ�Ȃ�
		//{}
	/// </summary>
	
	//��Q�������邩�Ȃ�������
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//��������
		isHit = true;
		return 0;
	}
};



Navimesh::Navimesh()
{
	//�Z���̓����蔻��p�̃J�v�Z���R���C�_�[
	//������
	//���a��1�ł���̂͌�X�ɎO�p�`�̈�Ԓ����d�S���_�ԂɊg�傷�邽��
	m_collider.Create(1.f, 30.f);
	//cmo�t�@�C���̓ǂݍ��݁B
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

	//�Z���ɒ��_3�Əd�S��ݒ肵�A�Z���ɓo�^
	//m_vertexBufferArray��m_indexBufferArray�̗v�f���͓���
	for (int i = 0; i < m_indexBufferArray.size(); i++) {
		//�C���f�b�N�X�o�b�t�@�̔z��
		//���_�̏ꏊ
		auto& indexBuffer = m_indexBufferArray[i];
		//���_�o�b�t�@�̔z��
		//��̒��_�o�b�t�@�ɂ�1�p�[�c
		auto& vertexBuffer = m_vertexBufferArray[i];
		
		//�|���S���̐�
		//�|���S���͒��_3�Ō`�������̂�3�Ŋ����Ă���
		int numPoly = indexBuffer->size() / 3;
		//�|���S���̐�������
		for (int polyNo = 0; polyNo < numPoly; polyNo++) {
			//�Z���̐���
			Cell* cell = new Cell;

			//���_�̊i�[
			{
				//�|���S����ɑ΂���3�����̒��_�̏ꏊ������o��
				int vertNo_0 = indexBuffer->at(polyNo * 3);
				int vertNo_1 = indexBuffer->at(polyNo * 3 + 1);
				int vertNo_2 = indexBuffer->at(polyNo * 3 + 2);
				//�Z���ɒ��_�̏ꏊ���i�[
				cell->vertexPos[0] = vertexBuffer->at(vertNo_0);
				cell->vertexPos[1] = vertexBuffer->at(vertNo_1);
				cell->vertexPos[2] = vertexBuffer->at(vertNo_2);
			}

			//centerPos�̓o�^
			//�O�p�`�̏d�S
			//���v�Ȃ̂ŏ�����
			cell->centerPos.Zero();
			cell->centerPos = cell->vertexPos[0] + cell->vertexPos[1] + cell->vertexPos[2];
			cell->centerPos /= 3;

			//�J�v�Z���R���C�_�[�̔��a��
			//�O�p�`�̈�Ԓ����d�S���_�Ԃ̒����Ɋg��
			
			//�d�S���璸�_�܂ł̒��������߂�
			//���ꂪ�J�v�Z���R���C�_�[�̔��a�ɂȂ�
			CVector3 radius[3];
			for (int i = 0; i < 3; i++)
			{
				radius[i] = cell->centerPos - cell->vertexPos[i];
			}
			//1�Ԓ����������r�ŋ��߂�
			CVector3 maxLength;
			maxLength = radius[0];
			for (int i = 1; i < 3; i++)
			{
				if (maxLength.Length() < radius[i].Length())
				{
					maxLength = radius[i];
				}
			}
			//�J�v�Z���R���C�_�[�̔��a���g��
			m_collider.Create(maxLength.Length(), 30.f);

			//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�
			//btTransform�͍��W�A��]�̏��������Ă���
			//���̏��������s��Ȃ���΂Ȃ�Ȃ�
			btTransform start, end;
			{
				//��]�̐ݒ�
				start.setIdentity();
				end.setIdentity();
				//���W�̐ݒ�
				//Origin �Ӗ��F���_
				start.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y + 20.f, cell->centerPos.z));
				end.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y + 30.f, cell->centerPos.z));
			}

			CallBack callback;
			//start����end�܂ŃR���W�������ړ������ē����蔻������
			g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
			
			if (callback.isHit == false) {
				
				//�Z���ɓo�^
				m_cells.push_back(cell);
			}
		}
	}

	//�o�^���ꂽ�Z���̐�������
	for (auto &all : m_cells) 
	{		
		//�����Z���Ɣ�r���Ȃ��悤�ɂ��邽�߂̃��[�v
		for (auto &currentCell : m_cells)
		{
			//�����Z���Ȃ珈�������O����
			if (all != currentCell)
			{
				//all�̒��_�̗v�f��i�߂�
				for (int i = 0; i < 3; i++)
				{
					//all�̒��_���i�[
					CVector3 allVPos;
					allVPos = all->vertexPos[i];

					//���W����v�������_���J�E���g
					int countSameVertex = 0;

					//��ӂɑ��݂��钸�_���o�^���邽�߂̔z��
					int sameVertexNo[2] = { 0,0 };

					//currentCell�̒��_�̗v�f��i�߂�
					for (int j = 0; j < 3; j++)
					{
						//allVPos��currentVPos�̋���
						float diff;
						diff = allVPos.Length() - currentCell->vertexPos[j].Length();
						//�������_�ǂ������قړ��������ɂ�����
						if (diff <= 0.01f)
						{
							//��v�������_��o�^
							sameVertexNo[countSameVertex] = i;
							//��v�������_�����J�E���g
							countSameVertex++;
							//��v�������_��2��������
							if (countSameVertex == 2) {
								//�Z�����אڂ��Ă���̂�
								//��v�������_�̏ꏊ�ɂ���ēo�^����ԍ���ς���
								if (sameVertexNo[0] == 0 && sameVertexNo[1] == 1
									|| sameVertexNo[0] == 1 && sameVertexNo[1] == 0) 
								{
									all->linkCells[0] = currentCell;
									all->linkMax++;
								}
								else if (sameVertexNo[0] == 1 && sameVertexNo[1] == 2 
									|| sameVertexNo[0] == 2 && sameVertexNo[1] == 1) 
								{
									all->linkCells[1] = currentCell;
									all->linkMax++;
								}
								else if (sameVertexNo[0] == 2 && sameVertexNo[1] == 0 
									|| sameVertexNo[0] == 0 && sameVertexNo[1] == 2) 
								{
									all->linkCells[2] = currentCell;
									all->linkMax++;
								}
								break;
							}
						}
					}
				}
				
			}
		}
	}

	for (auto &all : m_cells)
	{
		m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
		m_model->Init(L"modelData/enemy/enemy.cmo");
		m_model->SetPos(all->centerPos);
		
	}
}
