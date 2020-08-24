#include "stdafx.h"
#include "AStar.h"
#include "physics/CollisionAttr.h"

AStar::AStar()
{
}


AStar::~AStar()
{
}

void AStar::AstarSearch(Cell* startcell, Cell* endcell)
{
	//���X�g���������B
	m_openList.clear();
	m_closeList.clear();

	//�X�^�[�g���猻�ݒn�܂ł̃R�X�g�̏������B
	startcell->costFromStart = 0;
	//���ݒn����S�[���܂ł̃R�X�g�̌v�Z�B
	startcell->costToGoal = Length(startcell, endcell);
	//�e�̃Z���̏������B
	startcell->parentCell = NULL;
	//�I�[�v�����X�g�ɓo�^�B
	m_openList.push_back(startcell);
	bool isSuccess = false;
	//�N���b�V����h�����߁B
	int crashCount = 0;
	//�I�[�v�����X�g����ɂȂ�܂ŁB
	while (m_openList.empty() != true)
	{
		//�J�E���g�B
		crashCount++;
		//�I�[�v�����X�g�̍ŏ��̃Z��
		Cell* cell = m_openList[0];
		//�ŏI�I�ȃR�X�g�̌v�Z
		float cost = cell->costFromStart + cell->costToGoal;
		//��ԃR�X�g�������Z����T���B
		for (int i = 1; i < m_openList.size(); i++)
		{
			if (cost > m_openList[i]->costFromStart + m_openList[i]->costToGoal)
			{
				cost = m_openList[i]->costFromStart + m_openList[i]->costToGoal;
				cell = m_openList[i];
			}
		}
		//���̃Z�����S�[����������B
		if (cell == endcell) {
			//�I���B
			isSuccess = true;
			break;
		}
		//���O�B
		for (auto it = m_openList.begin(); it != m_openList.end(); it++) {
			if (*it == cell) {
				//�I�[�v�����X�g����폜�B
				m_openList.erase(it);
				break;
			}
		}
		//�N���[�Y���X�g�ɓo�^�B
		cell->state = Cell::State_Closed; //����B
		m_closeList.push_back(cell);
		//���̃Z���Ƀ����N���Ă���Z���̃R�X�g���v�Z���āA�I�[�v�����X�g�ɐςށB
		for (int i = 0; i < 3; i++)
		{
			//�Z�����אڂ��Ă�����B
			if (cell->linkCells[i] != NULL)
			{
				Cell* link = cell->linkCells[i];
			
				//���X�g�̍Ō�܂ōs������B
				if (link->state ==Cell::State_NotResearch) { //�������Ȃ�
					//�X�^�[�g���璲�����Ă����Z���̋�����
					//�������Ă����Z���ƗאڃZ���̋����𑫂��B
					link->costFromStart = cell->costFromStart + Length(cell, link);
					//�אڃZ���ƃS�[���܂ł̋����B
					link->costToGoal = Length(link, endcell);
					//�������Ă����Z����e�Ƃ��ēo�^�B
					link->parentCell = cell;
					//�I�[�v�����X�g�ɗאڃZ����o�^�B
					link->state = Cell::State_Opened; //�I�[�v�����X�g�ɐς܂ꂽ�B
					m_openList.push_back(link);
				}

			}
		}
		//600�񃋁[�v�����狭���I���B
		if (crashCount == 600) {
			//isSuccessh��false�̂܂܁B
			break;
		}
	}
	m_moveCellListTmp.clear();
	if (isSuccess) {
		//�o�H���i�[�B
		Cell* cell = endcell;
		while (true)
		{
			//�Z���̏d�S���i�[�B
			m_moveCellListTmp.push_back(cell->centerPos);

			//�������Ă���Z�����X�^�[�g�n�_��������I���B
			if (cell == startcell)
			{
				break;
			}
			//�Z���̐e���i�[�B
			cell = cell->parentCell;
		}
		//�v�f�̓S�[���n�_����X�^�[�g�n�_�܂ł̏��ԂŊi�[����Ă���̂Ŕ��]������B
		std::reverse(m_moveCellListTmp.begin(), m_moveCellListTmp.end());
		//m_moveCellList = m_moveCellListTmp;
		//�X���[�W���O�B
		Smoothing();
	}
	
	//������Ԃ����Z�b�g�B
	for (auto cell : m_openList) {
		cell->state = Cell::State_NotResearch;
	}
	for (auto cell : m_closeList) {
		cell->state = Cell::State_NotResearch;
	}
}

float AStar::Length(Cell * startcell, Cell * endcell)
{
	CVector3 length;
	length = endcell->centerPos - startcell->centerPos;
	return length.Length();
}

struct CallBack : public btCollisionWorld::ConvexResultCallback
{
	//��Q�������邩�Ȃ�������B
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Map
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_RigidBody) {
			//���������B
			isHit = true;
		}
		return 0;
	}
};

void AStar::Smoothing()
{
	////�������B////
	//�C�e���[�^�̏������B
	m_itr = m_moveCellListTmp.begin();
	//�ŏ��̍��W�̐ݒ�B
	CVector3 startPos = *m_itr;
	//���X�g�̏������B
	m_moveCellSmoothListTmp.clear();
	m_moveCellList.clear();
	//�X�^�[�g�ʒu���i�[�B
	m_moveCellSmoothListTmp.push_back(startPos);
	//�C�e���[�^��i�߂Ă����B
	m_itr++;
	//�R���C�_�[�̐ݒ�B
	m_collider.Create(m_boxSize);
	//�ړ��\�ȍ��W���L�����Ă������߁B
	//CVector3 oldPos = startPos;
	CVector3 oldPos;
	//�N���b�V����h�����߁B
	int crashCount = 0;
	bool isSuccess = false;
	while (1) {
		//�J�E���g�B
		crashCount++;
		//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�B
		btTransform start, end;
		{
			//��]�̐ݒ�B
			start.setIdentity();
			end.setIdentity();
			//���W�̐ݒ�B
			CVector3 endPos = *m_itr;
			start.setOrigin(btVector3(startPos.x, startPos.y + m_boxHeight, startPos.z));
			end.setOrigin(btVector3(endPos.x, startPos.y + m_boxHeight, endPos.z));
		}
		CallBack callback;
		//start����end�܂ŃR���W�������ړ������ē����蔻������B
		g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
		//�R���W�����Ƀq�b�g���Ȃ�������B
		if (callback.isHit == false) {
			oldPos = *m_itr;
			//�C�e���[�^����i�߂�B
			m_itr++;
		}
		else {
			//���ݒn��ݒ�B
			startPos = oldPos;
			//�ړ�����i�[�B
			m_moveCellSmoothListTmp.push_back(startPos);
		}
		//�I���ɒB������B
		if (m_itr == m_moveCellListTmp.end()) {
			//���ݒn��ݒ�B
			startPos = oldPos;
			//�ړ�����i�[�B
			m_moveCellSmoothListTmp.push_back(startPos);
			isSuccess = true;
			break;
		}
		//100�񃋁[�v�����狭���I���B
		if (crashCount >= 100) {
			break;
		}
	}
	if (isSuccess) {
		m_moveCellList = m_moveCellSmoothListTmp;
	}
}