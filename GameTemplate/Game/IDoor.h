#pragma once
#include "physics/PhysicsStaticObject.h"
class Player;
//�g���\�肾�������c�̊��N���X�B
class IDoor :
	public IGameObject
{
public:
	IDoor();
	virtual ~IDoor();
	/// <summary>
	/// �h�A�𓮂����֐��B
	/// </summary>
	/// <param name="diff">�v���C���[�ƃh�A�Ƃ̋����B</param>
	/// <param name="model"></param>
	/// <param name="PSOmodel"></param>
	/// <param name="pso"></param>
	/// <param name="rot"></param>
	void MoveDoor(const CVector3& diff, SkinModelRender* model, SkinModelRender* PSOmodel, const PhysicsStaticObject& pso, const CQuaternion& rot);
	/// <summary>
	/// �����̖��O��ݒ�B
	/// </summary>
	/// <param name="name">���O</param>
	void SetName(const std::wstring& name) {
		m_name = name;
	}
private:
	PhysicsStaticObject		m_physicsStaticObject;			//���̓����蔻��B
	const float				ADD_ROTATE = -2.0f;				//1�t���[���ɉ�]����ʁB
	const float				ACTION_DISTANCE = 150.0f;		//�v���C���[���h�A�ɃA�N�V�����ł��鋗���B
	float					m_maxRotate = 90.0f;			//��]�̍ő�ʁB
	bool					m_isRotate = false;				//��]�������ǂ����B
	bool					m_isOpenDoor = false;			//�h�A���J�������ǂ����B
	std::wstring			m_name;							//�����̖��O�B�ǂ��̃h�A���킩��悤�ɁB
	Game*					m_game = nullptr;				//Game�N���X�̃|�C���^�B
};