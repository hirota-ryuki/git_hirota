#pragma once
#include "Navimesh.h"

class AStar
{
public:
	AStar();
	~AStar();
	/// <summary>
	/// A*
	/// </summary>
	/// <param name="startcell">�G�̂���n�_</param>
	/// <param name="endcell">�S�[���n�_</param>
	void AstarSearch(Cell* startcell,Cell* endcell);
	/// <summary>
	/// 2�_�Ԃ̋��������߂�֐�
	/// </summary>
	/// <param name="startcell">�X�^�[�g�n�_</param>
	/// <param name="endcell">�G���h�n�_</param>
	/// <returns>2�_�Ԃ̋���</returns>
	float Length(Cell* startcell, Cell* endcell);
private:
	std::vector<Cell*> m_openList;		//�������̃I�[�v�����X�g
	std::vector<Cell*> m_closeList;		//�����ς݂̃I�[�v�����X�g
};

