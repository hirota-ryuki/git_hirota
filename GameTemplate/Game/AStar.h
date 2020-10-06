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
	/// <summary>
	/// �X���[�W���O�B
	/// </summary>
	void Smoothing();
	/// <summary>
	/// �Z���̃��X�g��Ԃ��֐��B
	/// </summary>
	/// <returns>m_moveCellList</returns>
	const std::vector<CVector3>& GetMoveList() const
	{
		return m_moveCellList;
	}
private:
	std::vector<Cell*> m_openList;					//�������̃I�[�v�����X�g�B
	std::vector<Cell*> m_closeList;					//�����ς݂̃I�[�v�����X�g�B
	std::vector<CVector3> m_moveCellListTmp;		//�X���[�W���O�O�̎Z�o���ꂽ�Z���̃��X�g�B
	std::vector<CVector3> m_moveCellSmoothListTmp;	//�X���[�W���O��̎Z�o���ꂽ�Z���̃��X�g�B
	std::vector<CVector3>::iterator m_itr;			//m_moveCellListTmp�̃C�e���[�^�B
	std::vector<CVector3> m_moveCellList;			//�ŏI�I�Ɉړ�����Z���̃��X�g�B
	BoxCollider m_collider;							//�Z���p�̃{�b�N�X�R���C�_�[�B
	CVector3	m_boxSize = CVector3::One()*20.0f;	//�R���C�_�[�̃T�C�Y�B
	float		m_boxHeight = 40.0f;				//����ݒu���鍂���B		
};