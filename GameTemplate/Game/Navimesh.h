#pragma once
#include "Physics/ICollider.h"
#include "physics/PhysicsStaticObject.h"
#include "physics/CapsuleCollider.h"

//�Z���N���X
struct Cell {
	CVector3 vertexPos[3];	//�Z���̒��_�̃|�W�V����
	Cell* linkCells[3];		//�Z���ɗאڂ��Ă���Z��
	CVector3 centerPos;		//�Z���̐^�񒆂̃|�W�V����
	int linkMax = 0;		//�אڂ��Ă���Z���̌�
	float costFromStart;
	float costToGoal;
	Cell* parentCell;
};

class Navimesh :
	public IGameObject
{
public:
	Navimesh();
	~Navimesh();
	void Create(SkinModel& model);
	void Update() override {};
	void Draw() override {};
	std::vector<Cell*> GetCell()
	{
		return m_cells;
	}
private:
	//typedef�͊����̌^���ɐV�������O��t����
	typedef std::vector<CVector3>					VertexBuffer;			//���_�o�b�t�@�B
	typedef std::vector<unsigned int>				IndexBuffer;	//�C���f�b�N�X�o�b�t�@�B
	//unique_ptr�̓R�s�[�s�\�ȃN���X
	typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
	typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
	std::vector<VertexBufferPtr>					m_vertexBufferArray;	//���_�o�b�t�@�̔z��B
	std::vector<IndexBufferPtr>						m_indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B
	
	SkinModelRender* m_model = nullptr;					//�X�L�����f���B
	SkinModelRender* m_skin = nullptr;					//�X�L�����f���B
	CapsuleCollider m_collider;							//�Z���p�̃J�v�Z���R���C�_�[
	std::vector<Cell*>					m_cells;		//�Z���̔z��B
};