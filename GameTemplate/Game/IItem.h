#pragma once
#include "IGameObject.h"
#include <unordered_map>
const float			ITEM_SPRITE_W = 270.0f;
const float			ITEM_SPRITE_H = 130.0f;
const CVector2		FRAME_IN_POS =  { -(FRAME_BUFFER_W / 2 - ITEM_SPRITE_W / 2),-200.0f };
const CVector2		FRAME_OUT_POS = { -(FRAME_BUFFER_W / 2 + ITEM_SPRITE_W / 2),-200.0f };
const float			AMOUNT_OF_CHANGE = 5.0f;				//�摜���ړ�����ʁA�ω��ʁB

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
	/// �摜�����[�h����֐��B
	/// </summary>
	/// <param name="filePath">�摜�̃t�@�C���p�X�B</param>
	void SpriteLoad(const wchar_t* filePath);
	/// <summary>
	/// �摜�𓮂����֐��B
	/// </summary>
	/// <param name="sprite">�X�v���C�g�����_�[�B</param>
	/// <param name="diff">�v���C���[�ƃA�C�e���Ƃ̋����B</param>
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
	bool		m_isFinishedMove = false;
	//�X�e�[�g�֌W�B
	enum State {
		enState_nearPlayer,
		enState_stopPlayer,
		enState_farPlayer
	};
	State		m_state = enState_nearPlayer;
	CVector2	m_movedPos = CVector2::Zero();
	//unordered_map�͏��Ԃ��߂��Ⴍ����ɂȂ邪map��葬���B
	static std::unordered_map<
		std::wstring,
		std::unique_ptr<SpriteRender*>
	>	m_itemSpriteMap;
};