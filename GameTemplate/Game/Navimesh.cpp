#include "stdafx.h"
#include "Navimesh.h"
#include <thread>
#include "Floor.h"

/*!
	*@brief	�X�g�b�v�E�H�b�`�N���X�B
	*@details
	* C#��Stopwatch�̂悤�Ɏg����B</br>
	*@code
	�T���v���R�[�h
	Stopwatch sw;
	sw.Start();		//�v���J�n�B
	for(int i = 0; i < 100; i++ ){
	}
	sw.Stop();		//�v���I��
	printf("�o�ߎ��� = %lf(�P�ʁF�b)\n", sw.GetElapsed());
	printf("�o�ߎ��� = %lf(�P�ʁF�~��)\n", sw.GetElapsedMillisecond());
	*@endcode
	*
	*/
class CStopwatch {
public:
	/*!
		*@brief	�R���X�g���N�^�B
		*/
	CStopwatch()
	{
		freq = 0;
		end = 0;
		begin = 0;
		elapsed = 0.0;
		elapsedMill = 0.0;
		elapsedMicro = 0.0;
	}
	/*!
		*@brief	�f�X�g���N�^�B
		*/
	~CStopwatch()
	{
	}
	/*!
		*@brief	�v���J�n�B
		*/
	void Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	}
	/*!
		*@brief	�v���I��
		*/
	void Stop()
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		::QueryPerformanceCounter((LARGE_INTEGER*)&end);
		elapsed = double(end - begin) / freq;
		elapsedMill = elapsed * 1000.0;
		elapsedMicro = elapsedMill * 1000.0;
	}
	/*!
		*@brief	�o�ߎ��Ԃ��擾(�P��:�b)
		*/
	double GetElapsed() const
	{
		return elapsed;
	}
	/*!
		*@brief	�o�ߎ��Ԃ��擾(�P��:�~���b)
		*/
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
	/*!
		*@brief	�o�ߎ��Ԃ��擾(�P��:�}�C�N���b)
		*/
	double GetElapsedMicrosecond() const
	{
		return elapsedMicro;
	}
private:
	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//�o�ߎ���(�P�ʁF�b)
	double elapsedMill;		//�o�ߎ���(�P�ʁF�~���b)
	double elapsedMicro;	//�o�ߎ���(�P�ʁF�}�C�N���b)

};

//#define REBUILD_NAVIMESH_DATA		//������`����ƃi�r���b�V���̃f�[�^���č쐬�����B

#ifdef _DEBUG
	//#define USE_NAVIMESH_DEBUG	//������`����ƁA�i�r���b�V���̃f�o�b�O�\�����L���ɂȂ�B
#else
	//#define USE_NAVIMESH_DEBUG	//������`����ƁA�i�r���b�V���̃f�o�b�O�\�����L���ɂȂ�B
#endif
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
}

Navimesh::~Navimesh()
{
}

void Navimesh::Init(Floor* floor)
{
	//�Z���̓����蔻��p�̃J�v�Z���R���C�_�[
	//������
	//���a��1�ł���̂͌�X�ɎO�p�`�̈�Ԓ����d�S���_�ԂɊg�傷�邽��
//	m_collider.Create(1.f, 30.f);
#ifdef REBUILD_NAVIMESH_DATA
	Create(floor->GetSkinModelRender()->GetModel());
#else
	//�A�Z�b�g�t�H���_����i�r���b�V���̃f�[�^�����[�h����B
	FILE* fp = fopen("stage.hnv", "rb");
	if (fp == nullptr) {
		//�t�@�C���I�[�v���Ɏ��s
		MessageBox(nullptr, "�G���[", "stage.hnv���J���܂���ł����B", MB_OK);
	}
	int numCell;
	fread(&numCell, sizeof(int), 1, fp);
	//CellBinary�̔z��𓮓I�Ɋm��
	CellBinary* cellBinarys = new CellBinary[numCell];
	fread(cellBinarys, sizeof(CellBinary) * numCell, 1, fp);

	//CellBinary����Cell�̏����\�z����B
	for (int i = 0; i < numCell; i++) {
		//�Z���̐���
		Cell* cell = new Cell;

		for (int j = 0; j < 3; j++) {
			cell->vertexPos[j] = cellBinarys[i].vertexPos[j];
		}
		cell->centerPos = cellBinarys[i].centerPos;
		m_cells.push_back(cell);
	}

	for (int i = 0; i < numCell; i++) {
		for (int j = 0; j < 3; j++) {
			if (cellBinarys[i].linkCellNo[j] != -1) {
				m_cells[i]->linkCells[j] = m_cells[(cellBinarys[i].linkCellNo[j])];
			}
			else {
				m_cells[i]->linkCells[j] = NULL;
			}
		}
	}
	delete[] cellBinarys;
	fclose(fp);
#endif
#ifdef USE_NAVIMESH_DEBUG
	for (auto &all : m_cells)
	{
		m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
		m_model->Init(L"modelData/debug/debugbox.cmo");
		m_model->SetPos(all->centerPos);
		int No = 0;
		if (all->linkCells[No] != NULL)
		{
			//float dot = all->centerPos.Dot(all->linkCells[2]->centerPos);
			float dot = all->centerPos.x*all->linkCells[No]->centerPos.x +
				all->centerPos.y*all->linkCells[No]->centerPos.y +
				all->centerPos.z*all->linkCells[No]->centerPos.z;
			float k = atan(dot);
			CQuaternion g;
			//g.SetRotationDeg(CVector3::AxisX(), 45.0f);
			g.SetRotation(CVector3::AxisY(), k);
			m_model->SetRot(g);
		}
	}
#endif
}

void Navimesh::BuildLinkCellInfo(int startCellNo, int endCellNo)
{
	//�o�^���ꂽ�Z���̐�������
	for (int cellNo = startCellNo; cellNo < endCellNo && cellNo < m_cells.size(); cellNo++ )
	{
		auto all = m_cells[cellNo];

		//�����Z���Ɣ�r���Ȃ��悤�ɂ��邽�߂̃��[�v
		for (auto &currentCell : m_cells)
		{

			//�����Z���Ȃ珈�������O����
			if (all != currentCell)
			{
				//���W����v�������_���J�E���g
				int countSameVertex = 0;
				//all�̒��_�̗v�f��i�߂�
				for (int i = 0; i < 3; i++)
				{
					//all�̒��_���i�[
					CVector3 allVPos;
					allVPos = all->vertexPos[i];

					//currentCell�̒��_�̗v�f��i�߂�
					for (int j = 0; j < 3; j++)
					{
						//allVPos��currentVPos�̋���
						CVector3 diff;
						diff = allVPos - currentCell->vertexPos[j];
						//�������_�ǂ������قړ��������ɂ�����
						if (diff.Length() <= 0.01f)
						{
							//��v�������_�����J�E���g
							countSameVertex++;
						}
					}
				}
				//�Q���_���d�����Ă�����A�אڂ��Ă���Ƃ���B
				if (countSameVertex == 2) {
					all->linkCells[all->linkMax] = currentCell;
					all->linkMax++;

				}

			}
		}
	}
}

void Navimesh::Create(SkinModel& model)
{
	CMatrix mBias = model.GetWorldMatrix();
	
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
			CVector3 halfSize;
			halfSize.x = maxLength.Length() * 2.0f;
			halfSize.z = maxLength.Length() * 2.0f;
			halfSize.y = 30.0f;
			m_collider.Create(halfSize);

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

	for (auto &all : m_cells)
	{
		for (int i = 0; i < 3; i++)
		{
			all->linkCells[i] = nullptr;
			all->costFromStart = 0.0f;
			all->costToGoal = 0.0f;
			all->parentCell = nullptr;
		}
	}
	//��̃X���b�h�Œ��ׂ�Z���̐����v�Z�B
	CStopwatch sw;
	sw.Start();
	int numCellOneThread = m_cells.size() / 3;
#if 1
	//4�X���b�h�ɕ��S���ėאڃZ�������\�z����B
	//����B
	std::thread buildLinkCellThread00([&] {
		BuildLinkCellInfo(0, numCellOneThread);
	});

	std::thread buildLinkCellThread01([&] {
		BuildLinkCellInfo(numCellOneThread, numCellOneThread*2);
	});

	std::thread buildLinkCellThread02([&] {
		BuildLinkCellInfo(numCellOneThread*2, numCellOneThread*3);
	});
	std::thread buildLinkCellThread03([&] {
		BuildLinkCellInfo(numCellOneThread*3, numCellOneThread*4);
	});

	buildLinkCellThread00.join();
	buildLinkCellThread01.join();
	buildLinkCellThread02.join();
	buildLinkCellThread03.join();
#else
	//����B
	BuildLinkCellInfo(0, numCellOneThread);
	BuildLinkCellInfo(numCellOneThread, numCellOneThread * 2);
	BuildLinkCellInfo(numCellOneThread * 2, numCellOneThread * 3);
	BuildLinkCellInfo(numCellOneThread * 3, numCellOneThread * 4);
#endif
#ifdef _DEBUG
	//���Ԍv���B
	sw.Stop();
	char text[256];
	sprintf(text, "build time = %f\n", sw.GetElapsed());
	OutputDebugString(text);
#endif
#ifdef USE_NAVIMESH_DEBUG

	for (auto &all : m_cells)
	{
		m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
		m_model->Init(L"modelData/debug/debugbox.cmo");
		m_model->SetPos(all->centerPos);
		int No = 0;
		if (all->linkCells[No] != NULL)
		{
			//float dot = all->centerPos.Dot(all->linkCells[2]->centerPos);
			float dot = all->centerPos.x*all->linkCells[No]->centerPos.x +
				all->centerPos.y*all->linkCells[No]->centerPos.y +
				all->centerPos.z*all->linkCells[No]->centerPos.z;
			float k = atan(dot);
			CQuaternion g;
			//g.SetRotationDeg(CVector3::AxisX(), 45.f);
			g.SetRotation(CVector3::AxisY(), k);
			m_model->SetRot(g);
		}
		/*for (int i = 0; i < 3; i++) {
			if (all->linkCells[i] != nullptr) {
				auto dir = all->linkCells[i]->centerPos - all->centerPos;
				dir.Normalize();
				auto hoge = NewGO<SkinModelRender>(GOPrio_Defalut);
				m_model->Init(L"modelData/zombie/zombie.cmo");
				hoge->SetPos(all->centerPos);
				CQuaternion rot;
				rot.SetRotation({ 0.0f, 1.0f, 0.0f }, dir);
				hoge->SetRot(rot);
			}
		}*/
	}
#endif //

	FILE* fp = fopen( "stage.hnv", "wb");
	if (fp == nullptr) {
		//�t�@�C���I�[�v���Ɏ��s
		MessageBox(nullptr, "�G���[", "stage.hnv���J���܂���ł����B", MB_OK);
	}
	int cellNum = m_cells.size();
	fwrite(&cellNum, sizeof(int), 1, fp);
	for (auto &cell : m_cells)
	{
		CellBinary cellBinary;
		//for (int i = 0; i < 3; i++) {
		//	auto itr = std::find(m_cells.begin(), m_cells.end(), cell->linkCells[i]);
		//	//size_t index = std::distance(m_cells.begin(), itr);
		//	/*if (index != m_cells.size()) {
		//		cellBinary.linkCellNo[i] = index;
		//	}
		//	else {
		//		cellBinary.linkCellNo[i] = -1;
		//	}*/
		//}

		//���_�B
		for (int i = 0; i < 3; i++) {
			cellBinary.vertexPos[i] = cell->vertexPos[i];
		}
		//�אڃZ���B
		for (int i = 0; i < 3; i++) {
			int count = 0;
			if (cell->linkCells[i] == nullptr) {
				cellBinary.linkCellNo[i] = -1;
			}
			else {
				for (auto itr = m_cells.begin(); itr != m_cells.end(); ++itr) {
					if (cell->linkCells[i] == *itr) {
						cellBinary.linkCellNo[i] = count;
						break;
					}
					else if (itr == m_cells.end()) {
						cellBinary.linkCellNo[i] = -1;
					}
					count++;
				}
			}
		}

		//���S
		cellBinary.centerPos = cell->centerPos;

		fwrite(&cellBinary, sizeof(cellBinary), 1, fp);
	}
	fclose(fp);
}

bool Navimesh::Start()
{
	return true;
}