#include "stdafx.h"
#include "Pose.h"
#include <iostream>
#include <string>

Pose::Pose()
{
}

Pose::~Pose()
{
}

void Pose::OnDestroy()
{
#ifdef BAG_MODE
	DeleteGO(m_spriteMenu);
#endif // BAG_MODE
	DeleteGOs("item");
}

bool Pose::Start()
{
	m_itemNameFont = NewGO<FontRender>(GOPrio_Sprite,"item");
	wchar_t bullet[50];
	swprintf_s(bullet, L"��x");
	m_itemNameFont->SetText(bullet);
	m_itemNameFont->SetPosition({ 0.0f, 0.0f });
	m_itemNameFont->ChangeActive();

	m_itemCountfont = NewGO<FontRender>(GOPrio_Sprite,"item");
	m_itemCountfont->SetPosition({ 100.0f, 0.0f });
	m_itemCountfont->ChangeActive();

#ifdef BAG_MODE
	//�摜�B
	m_spriteMenu = NewGO<SpriteRender>(GOPrio_Sprite);
	m_spriteMenu->Init(L"sprite/item_menu.dds", 640.f, 360.f);
	//�摜�������Ă����B
	m_spriteMenu->ActiveMode(false);
#endif // BAG_MODE

	return true;
}

void Pose::Update()
{
	FontRenderUpdate();
	//�X�^�[�g�{�^������������B
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		/*m_isStart = !m_isStart;
		wchar_t allbullet[50];
		swprintf_s(allbullet, L"%d", m_itemCount);
		m_itemCountfont->SetText(allbullet);
		m_itemNameFont->ChangeActive();
		m_itemCountfont->ChangeActive();*/

		//�t�H���g��`�悷��B
		for (auto& itemdata : m_fontList) {
			itemdata.nameFR->ChangeActive();
			itemdata.numFR->ChangeActive();
		}
#ifdef BAG_MODE
		m_spriteMenu->ChangeActive();
#endif // BAG_MODE

		//�f�[�^�ǉ��̗L�����������B
	}
	//ResetIsAddData();

#ifdef BAG_MODE
	//���j���[��ʂ��\������Ă�����B
	if (m_isStart) {
		
	}
#endif // BAG_MODE
}
void Pose::FontRenderUpdate()
{
	//�ǉ��������͒l�̕ϓ����N���Ă�����B
	if (GetIsAddData() || GetIsAddNum()) {
		//���������Ă��Ȃ�������B
		if (m_fontList.begin() == m_fontList.end()) {
			//���O�̃t�H���g�����_�[�쐬�B
			FontRender* namefr = NewGO<FontRender>(GOPrio_Sprite, "item");
			namefr->SetText(IMitr->first.c_str());
			//���̃t�H���g�����_�[�쐬�B
			FontRender* numfr = NewGO<FontRender>(GOPrio_Sprite, "item");
			std::wstring num = std::to_wstring(IMitr->second);
			numfr->SetText(num.c_str());
			//�A�C�e���t�H���g�f�[�^�̍\�z�B
			ItemFontData ifd;
			ifd.nameFR = namefr;
			ifd.numFR = numfr;
			//�o�^�B
			m_fontList.emplace_back(ifd);
		}
		else {
			//�A�C�e���f�[�^�̎擾�B
			auto& itemMap = GetItemDataMap();
			//�A�C�e���f�[�^�}�b�v�̃C�e���[�^�B
			for (auto IMitr = itemMap.begin(); IMitr != itemMap.end(); IMitr++) {

				//�t�H���g�����_�[���X�g�̃C�e���[�^�B
				for (auto FLitr = m_fontList.begin(); FLitr != m_fontList.end(); FLitr++) {
					//�A�C�e�����ǉ�����Ă�����B
					if (GetIsAddData()) {
						//�t�H���g�����_�[�̃e�L�X�g�ƃA�C�e���f�[�^�̖��O�̔�r�B
						//�t�H���g�����_�[���X�g�ɓo�^����Ă��Ȃ�������B
						if (FLitr->nameFR->GetText().compare(IMitr->first) != 0) {
							//���O�̃t�H���g�����_�[�쐬�B
							FontRender* namefr = NewGO<FontRender>(GOPrio_Sprite, "item");
							namefr->SetText(IMitr->first.c_str());
							//���̃t�H���g�����_�[�쐬�B
							FontRender* numfr = NewGO<FontRender>(GOPrio_Sprite, "item");
							std::wstring num = std::to_wstring(IMitr->second);
							numfr->SetText(num.c_str());
							//�A�C�e���t�H���g�f�[�^�̍\�z�B
							ItemFontData ifd;
							ifd.nameFR = namefr;
							ifd.numFR = numfr;
							//�o�^�B
							m_fontList.emplace_back(ifd);
						}
					}
					//�A�C�e���̌����ϓ�����Ă�����B
					if (GetIsAddNum()) {
						//int����std::wstring�ɕϊ��B
						std::wstring num = std::to_wstring(IMitr->second);
						//�܂��������O��������B
						if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
							//������̔�r�B
							if (FLitr->numFR->GetText().compare(num) != 0) {
								FLitr->numFR->SetText(num.c_str());
							}
						}
					}
				}
			}
		}
	//�ǉ��������͒l�̕ϓ����N���Ă�����B
	//if (GetIsAddData() || GetIsAddNum()) {
	//	//�A�C�e���f�[�^�̎擾�B
	//	auto& itemMap = GetItemDataMap();
	//	//�A�C�e���f�[�^�}�b�v�̃C�e���[�^�B
	//	for (auto itr = itemMap.begin(); itr != itemMap.end(); itr++) {
	//		//�t�H���g�����_�[���X�g�̃C�e���[�^�B
	//		for (auto &itemdata : m_fontList) {
	//			//�A�C�e�����ǉ�����Ă�����B
	//			if (GetIsAddData()) {
	//				//�t�H���g�����_�[�̃e�L�X�g�ƃA�C�e���f�[�^�̖��O�̔�r�B
	//				//�t�H���g�����_�[���X�g�ɓo�^����Ă��Ȃ�������B
	//				if (itemdata.nameFR->GetText().compare(itr->first) != 0) {
	//					//���O�̃t�H���g�����_�[�쐬�B
	//					FontRender* namefr = NewGO<FontRender>(GOPrio_Sprite, "item");
	//					namefr->SetText(itr->first.c_str());
	//					//���̃t�H���g�����_�[�쐬�B
	//					FontRender* numfr = NewGO<FontRender>(GOPrio_Sprite, "item");
	//					std::wstring num = std::to_wstring(itr->second);
	//					numfr->SetText(num.c_str());
	//					//�A�C�e���t�H���g�f�[�^�̍\�z�B
	//					ItemFontData ifd;
	//					ifd.nameFR = namefr;
	//					ifd.numFR = numfr;
	//					//�o�^�B
	//					m_fontList.emplace_back(ifd);
	//				}
	//			}
	//			//�A�C�e���̌����ϓ�����Ă�����B
	//			if (GetIsAddNum()) {
	//				//int����std::wstring�ɕϊ��B
	//				std::wstring num = std::to_wstring(itr->second);
	//				//�܂��������O��������B
	//				if (itemdata.nameFR->GetText().compare(itr->first) == 0) {
	//					//������̔�r�B
	//					if (itemdata.numFR->GetText().compare(num) != 0) {
	//						itemdata.numFR->SetText(num.c_str());
	//					}
	//				}
	//			}
	//		}
	//	}

		auto& FRitr = m_fontList.begin();
		//�����l���牺�ɍ��W�����炵�Ă����B
		for (int i = 0; i < GetItemCount(); i++) {
			m_position = NAME_TOP_POS + ADD_BELOW_POS * i;
			FRitr->nameFR->SetPosition(m_position);
			m_position = NUM_TOP_POS + ADD_BELOW_POS * i;
			FRitr->numFR->SetPosition(m_position);
			FRitr++;
		}

	}	


	
}
#ifdef BAG_MODE
void Pose::AddItem(const wchar_t * name, const wchar_t * textureFIlePath)
{
	ItemInfo itemData;
	itemData.name = name;
	itemData.sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	itemData.sprite->Init(textureFIlePath, 640.0f, 360.0f);
	m_itemList.push_back(itemData);
	m_totalItemCount++;
}
#endif // BAG_MODE