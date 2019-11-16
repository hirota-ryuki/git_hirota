#include "stdafx.h"
#include "Navimesh.h"

struct CallBack : public btCollisionWorld::ContactResultCallback
{
	//CallBack�N���X�̃R���X�g���N�^
	CallBack()
		//���������X�g
		//{}�̑O�ɏ��������s��
		//namespace�@btCollisionWorld
		//�\���́@ClosestConvexResultCallback
		//ClosestConvexResultCallback�̓R���X�g���N�^�Ɉ���������̂�
		//���������Ȃ��Ă͂Ȃ�Ȃ�
	{}
	//��Q�������邩�Ȃ�������
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����
	//virtual  btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
	//{
	//	//��������
	//	isHit = true;
	//	return 0;
	//}
	virtual  btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{
		//btManifoldPoint& cp�@�ڐG���
		//btCollisionObjectWrapper�@�R���W�����I�u�W�F�N�g�̏��

		btScalar dist = cp.getDistance();
		// dist > 0�̏ꍇ�ڐG���Ă��Ȃ��@�ŒZ����
		// dist < 0�Ȃ�d�Ȃ��Ă���@�N������

		//�ڐG���g�������菈��������W

		return 0;//�߂�l�ɈӖ��Ȃ��@�Ăԑ����g���Ă��Ȃ�
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
			{
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
				m_collider.GetBody()->setLocalScaling(btVector3(maxLength.Length(), 1.0f, maxLength.Length()));
			}
			{
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
					start.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y, cell->centerPos.z));
					end.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y + 10.f, cell->centerPos.z));
				}
				CallBack callback;
				//start����end�܂ŃR���W�������ړ������ē����蔻������
				g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
			}
			//if (callback.isHit == false) {
			//�I�u�W�F�N�g����ɂ�����������Ă��邩�ǂ����̊m�F�p�B
			//�|���S���̏�ɏo��
			/*m_skin = NewGO<SkinModelRender>(GOPrio_Defalut);
			m_skin->Init(L"modelData/enemy/enemy.cmo");
			m_skin->SetPos(cell->centerPos);
*/
			//�Z���ɓo�^
			m_cells.push_back(cell);
			//}
		}
	}

	//�o�^���ꂽ�Z���̐�������
	for (auto &all : m_cells) 
	{		
		//�I�u�W�F�N�g����ɂ�����������Ă��邩�ǂ����̊m�F�p�B
		//�|���S���̏�ɏo��
		m_skin = NewGO<SkinModelRender>(GOPrio_Defalut);
		m_skin->Init(L"modelData/enemy/enemy.cmo");
		m_skin->SetPos(all->centerPos);
	}
}
