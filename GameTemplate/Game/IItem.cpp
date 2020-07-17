#include "stdafx.h"
#include "IItem.h"
#include "Game.h"
#include "Player.h"

//static�Œ�`�����Ȃ�cpp�Ŏ��̂������K�v������B
std::unordered_map<
	std::wstring,
	std::unique_ptr<SpriteRender>
> IItem::m_itemSpriteMap;

IItem::IItem()
{
}

IItem::~IItem()
{
	DeleteGO(m_buttonSprite);
}

void IItem::ItemCommonProcessing(SpriteRender * sprite, CVector3 pos)
{
	CVector3 diff = m_player->GetPos() - pos;
	IItem::SpriteMove(sprite, diff);
	IItem::GettingItem(IItem::IsGetItem(diff));
	IItem::ButtonSpriteMove(diff, pos);
}

bool IItem::IsGetItem(CVector3 diff)
{
	//�������߂�������B
	if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_MOVE) {
		//B�{�^������������B
		if (g_pad[0].IsTrigger(enButtonB)) {
			//�����V���b�g�Đ���SE�B
			CSoundSource* m_se = new CSoundSource;
			m_se->Init(L"sound/story/decision.wav");
			m_se->Play(false);
			m_isGetItem = true;
		}
	}
	return m_isGetItem;
}

void IItem::GettingItem(bool isGetItem)
{
	//�A�C�e�����Q�b�g���Ă�����B
	if (isGetItem) {
		if (!m_isOnce) {
			OnGet();
			m_isOnce = true;
		}
		
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
		auto mapData = it->second.get();
		sprite = mapData;
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

void IItem::ButtonSpriteMove(CVector3 diff, CVector3 position) 
{
	if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_BUTTON) { //������500�ȉ��ɂȂ�����B
		//3D���W����2D���W�ւ̕ϊ��B
		m_model2Dpos = { position.x, position.y, position.z, 1.0f };
		g_camera3D.GetViewMatrix().Mul(m_model2Dpos);
		g_camera3D.GetProjectionMatrix().Mul(m_model2Dpos);
		m_model2Dpos.x /= m_model2Dpos.w;
		m_model2Dpos.y /= m_model2Dpos.w;
		m_buttonSprite->SetPos({ m_model2Dpos.x*FRAME_BUFFER_W / 2 * -1,m_model2Dpos.y*FRAME_BUFFER_H / 2 });
		m_buttonSprite->ActiveMode(true);
	}
	else {
		m_buttonSprite->ActiveMode(false);
	}
}

//static���������ʂ�̋L�q�ł����̂��`���Ă��邱�ƂɂȂ�B
//cpp�ɏ�����Ă����ok
void IItem::Release()
{
	//map����ɂ���B
	m_itemSpriteMap.clear();
}

void IItem::SpriteMove(SpriteRender* sprite, CVector3 diff)
{
	switch (m_state) {
	//�v���C���[���߂��ɗ������B
	case enState_nearPlayer:
		//�v���C���[�Ƃ̋������߂��Ȃ�����B
		if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_MOVE) {
			//�t���O�𗧂Ă�B
			m_isNearPlayer = true;
			m_isFinishedMove = false;
		}
		//�t���O�������Ă�����B
		if (m_isNearPlayer) {
			//�摜�����ɓ������B
			m_movedPos.x = sprite->GetPos().x + AMOUNT_OF_CHANGE;
			m_movedPos.y = FRAME_IN_POS.y;
			sprite->SetPos(m_movedPos);
			//�ŏI�n�_�ɒB������B
			if (sprite->GetPos().x >= FRAME_IN_POS.x) {
				//���̃X�e�b�v�Ɉڍs�B
				m_state = enState_stopPlayer;
				m_isNearPlayer = false;
			}
		}
		break;
	//�v���C���[���A�C�e���̋߂��ɂ��鎞�B
	case enState_stopPlayer:
		//�v���C���[���A�C�e�����痣�ꂽ��B
		if (diff.Length() >= ENEMY_AND_PLAYER_DISTANCE_MOVE || m_isGetItem) {
			//���̃X�e�b�v�Ɉڍs�B
			m_state = enState_farPlayer;
		}
		break;
	//�v���C���[���A�C�e�����痣�ꂽ���B
	case enState_farPlayer:
		//�摜���E�ɓ������B
		m_movedPos.x = sprite->GetPos().x - AMOUNT_OF_CHANGE;
		m_movedPos.y = FRAME_OUT_POS.y;
		sprite->SetPos(m_movedPos);
		//�ŏI�n�_�ɒB������B
		if (sprite->GetPos().x <= FRAME_OUT_POS.x) {
			//�ŏ��̃X�e�b�v�ɖ߂�B
			m_state = enState_nearPlayer;
			m_isFinishedMove = true;
		}
		break;
	default:
		break;
	}
}
