#pragma once
#include "Physics/ICollider.h"

//�Z���N���X
struct Cell {
	CVector3 vertexPos[3];	//�Z���̒��_�̃|�W�V����
	Cell* linkCell[3];		//�Z���ɗאڂ��Ă���Z��
	CVector3 centerPos;		//�Z���̐^�񒆂̃|�W�V����
};

class Navimesh :
	public IGameObject
{
public:
	Navimesh();
	~Navimesh();
	void Create(SkinModel& model);
private:
	//typedef�͊����̌^���ɐV�������O��t����
	typedef std::vector<CVector3>					VertexBuffer;			//���_�o�b�t�@�B
	typedef std::vector<unsigned int>				IndexBuffer;	//�C���f�b�N�X�o�b�t�@�B
	//unique_ptr�̓R�s�[�s�\�ȃN���X
	typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
	typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
	std::vector<VertexBufferPtr>					m_vertexBufferArray;	//���_�o�b�t�@�̔z��B
	std::vector<IndexBufferPtr>						m_indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B
};