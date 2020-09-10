#include "stdafx.h"
#include "IItem.h"
#include "Game.h"
#include "Player.h"

//static�Œ�`�����Ȃ�cpp�Ŏ��̂������K�v������B
std::unordered_map<
	std::wstring,
	SpriteRender*
> IItem::m_itemSpriteMap;
bool	IItem::m_isMove = false;

IItem::IItem()
{
}

IItem::~IItem()
{
	
}

bool IItem::Start()
{
	ItemStart();
	IItemInit();
	return true;
}

void IItem::Update()
{
	ItemUpdate();
}

void IItem::OnDestroy()
{
	DeleteGO(m_buttonSprite);
	ItemDestroy();
}


void IItem::ItemCommonProcessing(SpriteRender * sprite, CVector3 pos, SkinModelRender* model)
{
	CVector3 diff = m_player->GetPos() - pos;
	IItem::GettingItem(IItem::IsGetItem(diff), sprite, diff, model);
	IItem::ButtonSpriteMove(diff, pos);
}

bool IItem::IsGetItem(CVector3 diff)
{
	//�������߂�������B
	if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_MOVE) {
		//B�{�^������������B
		if (g_pad[0].IsTrigger(enButtonB)) {
			//�����V���b�g�Đ���SE�B
			Sound(L"sound/story/decision.wav", false);
			m_isGetItem = true;
		}
	}
	return m_isGetItem;
}

void IItem::GettingItem(bool isGetItem, SpriteRender* sprite, CVector3 diff, SkinModelRender* model)
{
	//�A�C�e�����Q�b�g���Ă�����B
	if (isGetItem) {
		if (!m_isOnce) {
			model->ActiveMode(false);
			model->SetShadowCaster(false);
			model->SetShadowReciever(false);
			m_buttonSprite->ActiveMode(false);
			OnGet();
			m_isOnce = true;
		}
		IItem::SpriteMove(sprite, diff);
		//�摜�̓������I��������B
		if (m_isFinishedMove) {
			//���̃C���X�^���X�������B
			DeleteGO(this);
		}
	}
}

SpriteRender* IItem::SpriteLoad(const wchar_t* filePath, float w, float h)
{
	SpriteRender* sprite = nullptr;
	auto it = m_itemSpriteMap.find(filePath);
	if (it == m_itemSpriteMap.end()) 
	{
	//if (m_itemSpriteMap.count(filePath) == 1) {
		//���o�^�Ȃ̂ŁA�V�K�Ń��[�h����B
		SpriteRender* spriteData = NewGO<SpriteRender>(GOPrio_Sprite);
		spriteData->Init(filePath, w, h);
		spriteData->SetPos(FRAME_OUT_POS);
		sprite = spriteData;
		m_itemSpriteMap.emplace(filePath, spriteData);
	}
	else {
		sprite = it->second;
	}
	return sprite;
}

void IItem::IItemInit()
{
	ButtonSpriteLoad();
	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
}

void IItem::ButtonSpriteLoad()
{
	m_buttonSprite = NewGO<SpriteRender>(GOPrio_Sprite,"buttonsprite");
	m_buttonSprite->Init(L"sprite/item/button.dds", B_BUTTON_SIZE, B_BUTTON_SIZE);
}

void IItem::ButtonSpriteMove(CVector3 diff, CVector3 pos) 
{
	if (!m_isGetItem) {
		if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_BUTTON) { 
			//3D���W����2D���W�ւ̕ϊ��B
			m_model2Dpos = { pos.x, pos.y, pos.z, 1.0f };
			g_camera3D.GetViewMatrix().Mul(m_model2Dpos);
			g_camera3D.GetProjectionMatrix().Mul(m_model2Dpos);
			m_model2Dpos.x /= m_model2Dpos.w;
			m_model2Dpos.y /= m_model2Dpos.w;
			m_buttonSprite->SetPos({ m_model2Dpos.x*FRAME_BUFFER_W / 2 * -1,m_model2Dpos.y*FRAME_BUFFER_H / 2 });
			//�I�u�W�F�N�g���v���C���[�̌�둤�ɗ�����\�����Ȃ��B
			if (m_model2Dpos.w >= 0.0f) {
				m_buttonSprite->ActiveMode(true);
			}
			else {
				m_buttonSprite->ActiveMode(false);
			}
		}
		else {
			m_buttonSprite->ActiveMode(false);
		}
	}
}

//static���������ʂ�̋L�q�ł����̂��`���Ă��邱�ƂɂȂ�B
//cpp�ɏ�����Ă����ok
void IItem::Release()
{
	for (auto itr = m_itemSpriteMap.begin(); itr != m_itemSpriteMap.end(); itr++) 
	{
		DeleteGO(itr->second);
	}
	//map����ɂ���B
	m_itemSpriteMap.clear();
}

void IItem::SpriteMove(SpriteRender* sprite, CVector3 diff)
{
	switch (m_state) {
	//�摜�𓮂������ԂɂȂ�̂�҂��Ă����ԁB
	case enState_waitMove:
		//�摜�𓮂����Ă���C���X�^���X���Ȃ�������B
		if (!m_isMove) {
			//�摜�������Ă����Ԃɂ���B
			m_isMove = true;
			//���̃X�e�b�v�Ɉڍs�B
			m_state = enState_startMove;
		}
		break;
	//�摜�������n�߂���ԁB
	case enState_startMove:
			//�摜�����ɓ������B
			m_movedPos.x = sprite->GetPos().x + AMOUNT_OF_CHANGE;
			m_movedPos.y = FRAME_IN_POS.y;
			sprite->SetPos(m_movedPos);
			//�ŏI�n�_�ɒB������B
			if (sprite->GetPos().x >= FRAME_IN_POS.x) {
				//���̃X�e�b�v�Ɉڍs�B
				m_state = enState_stopMove;
			}
		break;
	//�摜���~�܂��Ă����ԁB
	case enState_stopMove:
		m_stopCount++;
		//���Ԃ���������B
		if (m_stopCount > TIME_TO_STOP) {
			//���̃X�e�b�v�Ɉڍs�B
			m_state = enState_endMove;
		}
		break;
	//�摜���߂��Ă�����ԁB
	case enState_endMove:
		//�摜���E�ɓ������B
		m_movedPos.x = sprite->GetPos().x - AMOUNT_OF_CHANGE;
		m_movedPos.y = FRAME_OUT_POS.y;
		sprite->SetPos(m_movedPos);
		//�ŏI�n�_�ɒB������B
		if (sprite->GetPos().x <= FRAME_OUT_POS.x) {
			//�摜�̈ړ��͊����B
			m_isFinishedMove = true;
			//�摜�������Ă��Ȃ���Ԃɂ���B
			m_isMove = false;
		}
		break;
	default:
		break;
	}
}
