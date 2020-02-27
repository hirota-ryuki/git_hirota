#include "stdafx.h"
#include "Pose.h"

Pose::Pose()
{
}

Pose::~Pose()
{
}

void Pose::OnDestroy()
{
	DeleteGO(m_sprite);
	DeleteGOs("item");
}

bool Pose::Start()
{
	//�摜�B
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/item_menu.dds", 640.f, 360.f);
	//�摜�������Ă����B
	m_sprite->ChangeActive();
	int bagEmpty = 2;
	for (int i = 0; i < bagEmpty; i++) {
		m_posList[i].namepos = { 0.0f,0.0f - 50.f*i };
		m_posList[i].countpos = { 10.0f,10.0f - 50.f*i };
	}

	return true;
}

void Pose::Update()
{
	//�X�^�[�g�{�^������������B
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		//m_sprite->ChangeActive();
		m_isStart = !m_isStart;
	}
	//���j���[��ʂ��\������Ă�����B
	if (m_isStart) {
		//�A�C�e�����������Ȃ珈�����s��Ȃ��B
		if (m_allItemCount != 0) {
			//���肵�Ă����ސ��������t�H���g��\������B
			for (int i = 0; i < m_allItemCount; i++) {
				//�܂��t�H���g��NewGO����Ă��Ȃ�������B
				if (!m_itemList[i].m_isNewGO) {
					wchar_t text[50];
					m_itemList[i].m_itemNameFont = NewGO<FontRender>(GOPrio_Sprite, "item");
					//m_itemList[i].m_itemNameFont->SetText(m_itemList[i].m_itemName);
					m_itemList[i].m_itemNameFont->SetText(L"ball");
					m_itemList[i].m_itemNameFont->SetPosition(m_posList[i].namepos);

					m_itemList[i].m_itemCountfont = NewGO<FontRender>(GOPrio_Sprite, "item");
					swprintf_s(text, L"%d", m_itemList[i].m_itemCount);
					m_itemList[i].m_itemCountfont->SetText(text);
					m_itemList[i].m_itemCountfont->SetPosition(m_posList[i].countpos);
					
					m_itemList[i].m_isNewGO = true;
				}
				//�t�H���g��NewGO����Ă�����B
				else {

				}
			}
		}
	}


	////���j���[��ʂ��\������Ă�����B
	//if (m_isStart) {
	//	//�摜���ł��ĂȂ�������
	//	if (m_isNewSprite) {
	//		for (int i = 0; i < m_item; i++) {
	//			//�摜�B
	//			m_itemSprite = NewGO<SpriteRender>(GOPrio_Sprite,"sprite");
	//			m_itemSprite->Init(m_itemTexList[i], 1280.f, 720.f);
	//			m_itemSprite->SetPos(*m_itemPosList[i]);
	//			m_itemSpriteList.emplace_back(m_itemSprite);
	//			if (m_item - 1 == i) {
	//				m_isNewSprite = true;
	//			}
	//		}
	//	}
	//}
	//else {
	//	m_isNewSprite = false;
	//	DeleteGOs("sprite");
	//}
}

void Pose::AddItem(const wchar_t * itemname)
{
	ItemInfo_Font	itemInfo;
	//�������O�̃A�C�e�������邩�ǂ�������B
	bool isDoneName = false;
	int  doneNumber = 0;
	//�A�C�e�����������Ȃ珈�����s��Ȃ��B
	if (m_allItemCount != 0) {
		for (int i = 0; i < m_allItemCount; i++) {
			if (m_itemList[i].m_itemName == itemname) {
				doneNumber = i;
				isDoneName = true;
				break;
			}
		}
	}
	//�����������O�����݂��Ȃ�������B
	if (!isDoneName) {
		//�A�C�e���̖��O�̐ݒ�B
		itemInfo.m_itemName = itemname;
		//�A�C�e���̌���1�ɐݒ�B
		itemInfo.m_itemCount = 1;
		//�o�b�O�̉��Ԗڂɒu�����ݒ�B
		itemInfo.m_bagNumber = m_allItemCount;
		//���X�g�ɒǉ��B
		m_itemList[m_allItemCount] = itemInfo;
		//�A�C�e���̎�ނ��J�E���g�B
		m_allItemCount++;
	}
	//�����������O�����݂�����B
	else {
		//�A�C�e���̌��𑝂₷�B
		m_itemList[doneNumber].m_itemCount++;
		//�����ύX���ꂽ�̂Ńt���O�𗧂ĂĂ����B
		m_itemList[doneNumber].m_isChange = true;
	}
		
}
