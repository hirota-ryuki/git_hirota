#pragma once
#include "physics/PhysicsStaticObject.h"

class Door :
	public IGameObject
{
public:
	Door(){}
	~Door();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �h�A�𓮂����֐��B
	/// </summary>
	/// <param name="diff">�v���C���[�ƃh�A�Ƃ̋����B</param>
	/// <param name="model">�h�A�̃X�L�����f�������_�[�B</param>
	/// <param name="PSOmodel">�h�A�̓����蔻��̃��f�������_�[�B</param>
	/// <param name="pso">�h�A�̃X�^�e�B�b�N�I�u�W�F�N�g�B</param>
	/// <param name="rot">���f���̉�]���B</param>
	void MoveDoor(const float diff, SkinModelRender* model, SkinModelRender* PSOmodel, PhysicsStaticObject& pso, CQuaternion& rot);
	/// <summary>
	/// �����̖��O��ݒ�B
	/// </summary>
	/// <param name="name">���O�B</param>
	void SetName(const std::wstring& name) {
		m_name = name;
	}
	/// <summary>
	/// �h�A�̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �h�A�̃Z���^�[�|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���S���W�B</param>
	void SetCenterPos(const CVector3& pos)
	{
		m_centerPos = pos;
	}
	/// <summary>
	/// �h�A�̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*		m_model = nullptr;						//�X�L�����f���B
	CVector3				m_position = CVector3::Zero();			//���W�B
	CQuaternion				m_rotation = CQuaternion::Identity();	//��]�B
	SkinModelRender*		m_PSOmodel = nullptr;					//�����蔻��̃X�L�����f���B
	PhysicsStaticObject		m_beforeMovingPSO;					//���̓����蔻��B
	Game*					m_game = nullptr;						//Game�̃|�C���^�B
	Player*					m_player = nullptr;						//�v���C���[�N���X�̃|�C���^�B
	CVector3				m_centerPos = CVector3::Zero();			//�h�A�̒��S���W�B

	PhysicsStaticObject		m_afterMovingPSO;					//���̓����蔻��B
	const float				ADD_ROTATE = -2.0f;						//1�t���[���ɉ�]����ʁB
	const float				ACTION_DISTANCE = 150.0f;				//�v���C���[���h�A�ɃA�N�V�����ł��鋗���B
	const float				ACTION_DISTANCE_SQ = ACTION_DISTANCE * ACTION_DISTANCE;;		//�v���C���[���h�A�ɃA�N�V�����ł��鋗����2��B
	float					m_maxRotate = 90.0f;					//��]�̍ő�ʁB
	bool					m_isRotate = false;						//��]�������ǂ����B
	bool					m_isOpenDoor = false;					//�h�A���J�������ǂ����B
	std::wstring			m_name;									//�����̖��O�B�ǂ��̃h�A���킩��悤�ɁB
};