#include "stdafx.h"
#include "AStar.h"


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
	//�I�[�v�����X�g����ɂȂ�܂�
	while (m_openList.empty() !=true)
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
			break;
		}
		//���O
		for (auto it = m_openList.begin(); it != m_openList.end(); it++) {
			if (*it==cell) {
				//�I�[�v�����X�g����폜
				m_openList.erase(it);
				break;
			}
		}
		//�N���[�Y���X�g�ɓo�^
		m_closeList.push_back(cell);
		//���̃Z���Ƀ����N���Ă���Z���̃R�X�g���v�Z���āA�I�[�v�����X�g�ɐςށB
		for (int i = 0; i < 3; i++)
		{
			//�Z�����אڂ��Ă�����
			if (cell->linkCells[i] != NULL)
			{
				Cell* link = cell->linkCells[i];
				//�N���[�Y���X�g�̒��ɗאڃZ�����������ŏ�����Ō�܂ŒT��
				auto itFind = std::find(m_closeList.begin(), m_closeList.end(), link);
				//���X�g�̍Ō�܂ōs������
				//end�̃Z����link��������H
				if (itFind == m_closeList.end()) {
					//�X�^�[�g���璲�����Ă����Z���̋�����
					//�������Ă����Z���ƗאڃZ���̋����𑫂�
					link->costFromStart = cell->costFromStart + Length(cell, link);
					//�אڃZ���ƃS�[���܂ł̋���
					link->costToGoal = Length(link, endcell);
					//�������Ă����Z����e�Ƃ��ēo�^
					link->parentCell = cell;
					//�I�[�v�����X�g�ɗאڃZ����o�^
					m_openList.push_back(link);
				}
				
			}
		}
	}
}

float AStar::Length(Cell * startcell, Cell * endcell)
{
	CVector3 length;
	length = endcell->centerPos - startcell->centerPos;
	return length.Length();
}
