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
	//���X�g��������
	m_openList.clear();
	m_closeList.clear();

	//�X�^�[�g���猻�ݒn�܂ł̃R�X�g�̏�����
	startcell->costFromStart = 0;
	//���ݒn����S�[���܂ł̃R�X�g�̌v�Z
	startcell->costToGoal = Length(startcell, endcell);
	//�e�̃Z���̏�����
	startcell->parentCell = NULL;
	//�I�[�v�����X�g�ɓo�^
	m_openList.push_back(startcell);
	bool isSuccess = false;
	//�I�[�v�����X�g����ɂȂ�܂�
	while (m_openList.empty() != true)
	{
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
		//���̃Z�����S�[����������
		if (cell == endcell) {
			//�I��
			isSuccess = true;
			break;
		}
		//���O
		for (auto it = m_openList.begin(); it != m_openList.end(); it++) {
			if (*it == cell) {
				//�I�[�v�����X�g����폜
				m_openList.erase(it);
				break;
			}
		}
		//�N���[�Y���X�g�ɓo�^
		cell->state = Cell::State_Closed; //����B
		m_closeList.push_back(cell);
		//���̃Z���Ƀ����N���Ă���Z���̃R�X�g���v�Z���āA�I�[�v�����X�g�ɐςށB
		for (int i = 0; i < 3; i++)
		{
			//�Z�����אڂ��Ă�����
			if (cell->linkCells[i] != NULL)
			{
				Cell* link = cell->linkCells[i];
			
				//���X�g�̍Ō�܂ōs������
				if (link->state ==Cell::State_NotResearch) { //�������Ȃ�
					//�X�^�[�g���璲�����Ă����Z���̋�����
					//�������Ă����Z���ƗאڃZ���̋����𑫂�
					link->costFromStart = cell->costFromStart + Length(cell, link);
					//�אڃZ���ƃS�[���܂ł̋���
					link->costToGoal = Length(link, endcell);
					//�������Ă����Z����e�Ƃ��ēo�^
					link->parentCell = cell;
					//�I�[�v�����X�g�ɗאڃZ����o�^
					link->state = Cell::State_Opened; //�I�[�v�����X�g�ɐς܂ꂽ�B
					m_openList.push_back(link);
				}

			}
		}
	}
	m_moveCellListTmp.clear();
	if (isSuccess) {
		//�o�H���i�[
		Cell* cell = endcell;
		while (true)
		{
			//�Z���̏d�S���i�[
			m_moveCellListTmp.push_back(cell->centerPos);

			//�������Ă���Z�����X�^�[�g�n�_��������I���
			if (cell == startcell)
			{
				break;
			}
			//�Z���̐e���i�[
			cell = cell->parentCell;
		}
		//�v�f�̓S�[���n�_����X�^�[�g�n�_�܂ł̏��ԂŊi�[����Ă���̂Ŕ��]������B
		std::reverse(m_moveCellListTmp.begin(), m_moveCellListTmp.end());
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
	//��Q�������邩�Ȃ�������
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Map) {
			//��������
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
	m_moveCellList.clear();
	//�X�^�[�g�ʒu���i�[�B
	m_moveCellList.push_back(startPos);
	//�C�e���[�^��i�߂Ă����B
	m_itr++;
	//�R���C�_�[�̐ݒ�B
	m_collider.Create(m_boxSize);
	//�ŏ����������̃L�����R���ɓ�����̂Ŕ͈͂�L�΂����߂̔���B
	bool isFirst = false;
	//�ړ��\�ȍ��W���L�����Ă������߁B
	CVector3 oldPos = startPos;
	while (1) {
		//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�
		btTransform start, end;
		{
			//��]�̐ݒ�
			start.setIdentity();
			end.setIdentity();
			//���W�̐ݒ�
			CVector3 endPos = *m_itr;
			start.setOrigin(btVector3(startPos.x, startPos.y + 20.f, startPos.z));
			end.setOrigin(btVector3(endPos.x, startPos.y + 20.f, endPos.z));
		}
		CallBack callback;
		//start����end�܂ŃR���W�������ړ������ē����蔻������
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
			m_moveCellList.push_back(startPos);
		}
		//�I���ɒB������B
		if (m_itr == m_moveCellListTmp.end()) {
			//���ݒn��ݒ�B
			startPos = oldPos;
			//�ړ�����i�[�B
			m_moveCellList.push_back(startPos);
			break;
		}
	}
}