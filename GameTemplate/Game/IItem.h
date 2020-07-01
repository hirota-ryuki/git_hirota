#pragma once
#include "IGameObject.h"
class IItem :
	public IGameObject
{
public:
	IItem();
	virtual ~IItem() {}	
	/// <summary>
	/// �A�C�e�����Q�b�g���ꂽ���ǂ�����Ԃ��֐��B
	/// </summary>
	/// <param name="diff">�v���C���[�ƃA�C�e���Ƃ̋���</param>
	/// <returns>�v���C���[�ɃQ�b�g���ꂽ���ǂ����B</returns>
	bool IsGetItem(CVector3 diff);
	/// <summary>
	/// �Q�b�g���ꂽ���̏����B
	/// </summary>
	virtual void GettingItem(bool isGetItem) {}
	/// <summary>
	/// ���O��ݒ�B
	/// </summary>
	void SetName(wchar_t* name)
	{
		m_name = name;
	}
	/// <summary>
	/// ���O���擾�B
	/// </summary>
	wchar_t* GetName()
	{
		return m_name;
	}
private:
	//���f���֌W�B
	SkinModelRender*	m_model = nullptr;				//�X�L�����f���B
	CVector3			m_pos = CVector3::Zero();
	wchar_t*			m_name = nullptr;
	bool				m_isGetItem = false;				//B�{�^���������ꂽ���ǂ����B
};

