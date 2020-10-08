#pragma once
#include "Physics/ICollider.h"
#include "physics/PhysicsStaticObject.h"
#include "physics/BoxCollider.h"

//������`����ƃi�r���b�V���̃f�[�^���č쐬�����B
//#define REBUILD_NAVIMESH_DATA		

//������`����ƁA�i�r���b�V���̃f�o�b�O�\�����L���ɂȂ�B
#ifdef _DEBUG
//#define USE_NAVIMESH_DEBUG	//Debug�B
#else
//#define USE_NAVIMESH_DEBUG	//Release.
#endif
class Game;
struct CellBinary {

	CVector3 vertexPos[3];	//�Z���̒��_�̃|�W�V����
	int linkCellNo[3];		//�Z���ɗאڂ��Ă���Z��
	CVector3 centerPos;		//�Z���̐^�񒆂̃|�W�V����
};
//�Z���N���X
struct Cell {
	enum State {
		State_NotResearch,
		State_Opened,
		State_Closed,
	};
	CVector3 vertexPos[3];	//�Z���̒��_�̃|�W�V����
	Cell* linkCells[3];		//�Z���ɗאڂ��Ă���Z��
	CVector3 centerPos;		//�Z���̐^�񒆂̃|�W�V����
	int linkMax = 0;		//�אڂ��Ă���Z���̌�
	float costFromStart;
	float costToGoal;
	Cell* parentCell;
	State state = State_NotResearch; 
};

class Floor;
class Navimesh :
	public IGameObject
{
public:
	Navimesh();
	~Navimesh();
	bool Start() override { return true; };
	void Update() override;
	void Init(Floor* floor);
	void Create(const SkinModel& model);
	const std::vector<Cell*>& GetCell() const
	{
		return m_cells;
	}
private:
	/// <summary>
	/// �Z���̗אڏ����\�z����B
	/// </summary>
	/// <param name="startCellNo">�J�n�Z���̔ԍ��B</param>
	/// <param name="endCellNo">�I���Z���̔ԍ��B</param>
	void BuildLinkCellInfo(int startCellNo, int endCellNo);
private:
	//typedef�͊����̌^���ɐV�������O��t����B
	typedef std::vector<CVector3>					VertexBuffer;	//���_�o�b�t�@�B
	typedef std::vector<unsigned int>				IndexBuffer;	//�C���f�b�N�X�o�b�t�@�B
	//unique_ptr�̓R�s�[�s�\�ȃN���X�B
	typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
	typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
	std::vector<VertexBufferPtr>					m_vertexBufferArray;	//���_�o�b�t�@�̔z��B
	std::vector<IndexBufferPtr>						m_indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B
	
	BoxCollider			m_collider;						//�Z���p�̃J�v�Z���R���C�_�[�B
	const float			COLLIDER_EXPANSION = 2.45f;		//�R���C�_�[�̊g��{���B���X��2.45f�B
	std::vector<Cell*>	m_cells;						//�Z���̔z��B
	
	//�f�o�b�O�֌W�B
	SkinModelRender*				m_debugModel = nullptr;			//�f�o�b�O���f���B
	std::vector<SkinModelRender*>	m_debugModelList;				//�f�o�b�O���f���̔z��B
	bool							m_isDebug = false;				//�f�o�b�O���[�h�̃I���I�t�B
	bool							m_isChangeActiveModel = false;	//�f�o�b�O���[�h�̃I���I�t�B
	bool							m_isCreateModel = false;		//�f�o�b�O���f������������ǂ����B
};