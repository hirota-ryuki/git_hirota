#pragma once
#include "IGameObject.h"
#include <unordered_map>
const float			ITEM_SPRITE_W = 270.0f;
const float			ITEM_SPRITE_H = 130.0f;
const CVector2		FRAME_IN_POS =  { -(FRAME_BUFFER_W / 2 - ITEM_SPRITE_W / 2),-200.0f };
const CVector2		FRAME_OUT_POS = { -(FRAME_BUFFER_W / 2 + ITEM_SPRITE_W / 2),-200.0f };
const float			AMOUNT_OF_CHANGE = 5.0f;					//�摜���ړ�����ʁA�ω��ʁB
const float			B_BUTTON_SIZE    = 25.0f;					//B�{�^���̃T�C�Y�B
const float			ENEMY_AND_PLAYER_DISTANCE_BUTTON = 500.0f;	//�{�^����\������A�G�ƃv���C���[�̋����͈̔́B
const float			ENEMY_AND_PLAYER_DISTANCE_MOVE   = 100.0f;	//�摜�𓮂����A�G�ƃv���C���[�̋����͈̔́B

class Player;
class Game;

struct deleter_SpriteRender  {
	void operator()(SpriteRender* ptr) {
		DeleteGO(ptr);
	}
};

class IItem :
	public IGameObject
{
public:
	IItem();
	virtual ~IItem();

	/// <summary>
	/// �摜�����[�h����֐��B
	/// </summary>
	/// <param name="filePath">�摜�̃t�@�C���p�X�B</param>
	/// <param name="w">�����B</param>
	/// <param name="h">�c���B</param>
	/// <returns>�X�v���C�g�����_�[�B</returns>
	SpriteRender* SpriteLoad(const wchar_t* filePath, float w, float h);
	
	/// <summary>
	/// 
	/// </summary>
	void IItemInit();

	/// <summary>
	/// IItem�N���X�̋��ʏ����B
	/// �s��Ȃ���΂Ȃ�Ȃ��������܂Ƃ߂����́B
	/// </summary>
	/// <param name="sprite"></param>
	/// <param name="pos"></param>
	void ItemCommonProcessing(SpriteRender* sprite, CVector3 pos);
	
	/// <summary>
	/// �A�C�e�����Q�b�g�����Ƃ��̏��������R�ɏ�����֐��B
	/// </summary>
	virtual void OnGet() {}

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


	/// <summary>
	/// ����B
	/// static�֐���incrude���Ă�����
	/// �ǂ��ł��g����B
	/// </summary>
	static void Release();
private:
	/// <summary>
	/// �A�C�e�����Q�b�g���ꂽ���ǂ�����Ԃ��֐��B
	/// </summary>
	/// <param name="diff">�v���C���[�ƃA�C�e���Ƃ̋���</param>
	/// <returns>�v���C���[�ɃQ�b�g���ꂽ���ǂ����B</returns>
	bool IsGetItem(CVector3 diff);
	
	/// <summary>
	/// �Q�b�g���ꂽ���̋��ʏ����B
	/// </summary>
	void GettingItem(bool isGetItem);

	/// <summary>
	/// �摜�𓮂����֐��B
	/// </summary>
	/// <param name="sprite">�X�v���C�g�����_�[�B</param>
	/// <param name="diff">�v���C���[�ƃA�C�e���Ƃ̋����B</param>
	void SpriteMove(SpriteRender* sprite, CVector3 diff);

	/// <summary>
	/// 
	/// </summary>
	void ButtonSpriteLoad();
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="diff"></param>
	/// <param name="position"></param>
	void ButtonSpriteMove(CVector3 diff, CVector3 position);

private:
	wchar_t*	m_name = nullptr;							//�A�C�e���̖��O�B
	bool		m_isGetItem = false;						//B�{�^���������ꂽ���ǂ����B
	
	//�摜�̈ړ��֌W�B
	CVector2	m_movedPos = CVector2::Zero();				//1�t���[���œ�������̍��W�B
	bool		m_isNearPlayer = false;						//�v���C���[���߂��ɂ��邩�ǂ����B
	bool		m_isFinishedMove = false;					//�����I��������ǂ����B
	//�X�e�[�g�B
	enum State {
		enState_nearPlayer,
		enState_stopPlayer,
		enState_farPlayer
	};
	State		m_state = enState_nearPlayer;
	
	//unordered_map�͏��Ԃ��߂��Ⴍ����ɂȂ邪map��葬���B
	static std::unordered_map<
		std::wstring,
		std::unique_ptr<SpriteRender>
	>	m_itemSpriteMap;
	
	//B�{�^���֌W�B
	SpriteRender*	m_buttonSprite = nullptr;
	CVector4		m_model2Dpos = CVector4::Black();		//3D���f����2D�̍��W�ɕϊ��������W�B

	//OnGet�֐��֌W�B
	bool			m_isOnce = false;		//������̏������s�����߂̔���B

	//�C���X�^���X�֌W�B
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	Player*				m_player = nullptr;
};