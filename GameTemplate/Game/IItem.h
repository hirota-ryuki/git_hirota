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
	void SpriteMove(SpriteRender* sprite, CVector3 diff);
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
	wchar_t*	m_name = nullptr;							//�A�C�e���̖��O�B
	bool		m_isGetItem = false;						//B�{�^���������ꂽ���ǂ����B
	bool		m_isNearPlayer = false;						//�v���C���[���߂��ɂ��邩�ǂ����B
	CVector3	m_initialPos = { 650.0f,-200.0f,0.0f };		//�摜�̏������W�B
	CVector3	m_addPos = { -5.0f,0.0f,0.0f };				//�摜�������ʁB
	int			m_addCount = 0;								//�摜�𓮂������񐔁B
	const int	ADD_COUNT = 20;
};

