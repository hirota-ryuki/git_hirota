#include "stdafx.h"
#include "Menu.h"
#include <iostream>
#include <string>

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::OnDestroy()
{
	DeleteGOs("item");
}

bool Menu::Start()
{
	//Inv_AddItem(L"�e��", 25);
	Inv_AddItem(L"�񕜖�", 3);
	Inv_AddItem(L"������", 4);
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/menu.dds", 360.0f, 480.0f);
	CVector2 pos = { -350.0f,50.0f };
	m_sprite->SetPos(pos);
	m_sprite->ActiveMode(false);
	return true;
}

void Menu::Update()
{
	FontRenderUpdate();
	ResetIsAdd();
	//�ǉ��ς݂̃A�C�e����0�ɂȂ��Ă��ăA�C�e���f�[�^����폜����Ă��Ȃ����m�F�B
	if (!Inv_GetIsAddData() && !Inv_GetIsAddNum()) {
		if (Inv_GetIsDeleteItem()) {
			//�A�C�e���f�[�^�̎擾�B
			auto itemMap = Inv_GetItemDataMap();
			//�t�H���g���X�g�̃C�e���[�^�B
			for (auto fitr = m_fontList.begin(); fitr != m_fontList.end();) {
				auto iitr = itemMap.begin();
				bool isContinue = false;
				while (1) {
					//�C�e���[�^���Ō�܂œ��B������B
					if (iitr == itemMap.end()) {
						fitr = m_fontList.erase(fitr);
						isContinue = true;
						break;
					}
					//�������O�����łɑ��݂��Ă�����B
					if (fitr->nameFR->GetText().compare(iitr->first) == 0) {
						break;
					}
					iitr++;
				}
				if (!isContinue) {
					fitr++;
				}
			}
			SortingFontRnderPos();
		}
	}
	
	//�X�^�[�g�{�^������������B
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		m_sprite->ChangeActive();
		//�t�H���g��`�悷��B
		for (auto& itemdata : m_fontList) {
			itemdata.nameFR->ChangeActive();
			itemdata.numFR->ChangeActive();
		}
	}

	
}
void Menu::FontRenderUpdate()
{
	//�ǉ��������͒l�̕ϓ����N���Ă�����B
	if (Inv_GetIsAddData() || Inv_GetIsAddNum()) {
		//���������Ă��Ȃ�������B
		if (m_fontList.begin() == m_fontList.end()) {
			auto IDMap = Inv_GetItemDataMap();
			auto itr = IDMap.begin();
			//�A�C�e���t�H���g�f�[�^�̍\�z�B
			ItemFontData ifd;
			//���O�̃t�H���g�����_�[�쐬�B
			ifd.nameFR = NewGO<FontRender>(GOPrio_Sprite, "item");
			ifd.nameFR->SetText(itr->first.c_str());
			ifd.nameFR->ActiveMode(false);
			//���̃t�H���g�����_�[�쐬�B
			ifd.numFR = NewGO<FontRender>(GOPrio_Sprite, "item");
			std::wstring num = std::to_wstring(itr->second);
			ifd.numFR->SetText(num.c_str());
			ifd.numFR->ActiveMode(false);
			//�o�^�B
			m_fontList.emplace_back(ifd);
		}
		else {
			//�A�C�e���f�[�^�̎擾�B
			auto itemMap = Inv_GetItemDataMap();
			//�A�C�e���f�[�^�}�b�v�̃C�e���[�^�B
			for (auto IMitr = itemMap.begin(); IMitr != itemMap.end(); IMitr++) {
				auto FLitr = m_fontList.begin();
				while (1) {
					//�C�e���[�^���Ō�܂œ��B������B
					if (FLitr == m_fontList.end()) {
						//�A�C�e���t�H���g�f�[�^�̍\�z�B
						ItemFontData ifd;
						//���O�̃t�H���g�����_�[�쐬�B
						ifd.nameFR = NewGO<FontRender>(GOPrio_Sprite, "item");
						ifd.nameFR->SetText(IMitr->first.c_str());
						ifd.nameFR->ActiveMode(false);
						//���̃t�H���g�����_�[�쐬�B
						ifd.numFR = NewGO<FontRender>(GOPrio_Sprite, "item");
						std::wstring num = std::to_wstring(IMitr->second);
						ifd.numFR->SetText(num.c_str());
						ifd.numFR->ActiveMode(false);
						//�o�^�B
						m_fontList.emplace_back(ifd);
						break;
					}
					//�������O�����łɑ��݂��Ă�����B
					if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
						//������̔�r���s���A���̕ϓ����N���Ă��Ȃ����m�F�B
						//int����std::wstring�ɕϊ��B
						std::wstring num = std::to_wstring(IMitr->second);
						//�����ς���Ă���Ȃ�B
						if (FLitr->numFR->GetText().compare(num) != 0) {
							//�l��ύX�B
							FLitr->numFR->SetText(num.c_str());
						}
						break;
					}
					FLitr++;
				}
			}
		}
		SortingFontRnderPos();
	}
	
}
void Menu::SortingFontRnderPos()
{
	auto FRitr = m_fontList.begin();
	//�����l���牺�ɍ��W�����炵�Ă����B
	for (int i = 0; i < Inv_GetItemDataMap().size(); i++) {
		m_position = NAME_TOP_POS - ADD_BELOW_POS * i;
		FRitr->nameFR->SetPosition(m_position);
		m_position = NUM_TOP_POS - ADD_BELOW_POS * i;
		FRitr->numFR->SetPosition(m_position);
		FRitr++;
		if (FRitr == m_fontList.end()) {
			break;
		}
	}
}
void Menu::ResetIsAdd()
{
	//�ǉ��������͒l�̕ϓ����N���Ă�����B
	if (Inv_GetIsAddData() || Inv_GetIsAddNum()) {
		//�t�H���g���X�g�ƃA�C�e���f�[�^�̃T�C�Y���������Ȃ�����B
		if (m_fontList.size() >= Inv_GetItemDataMap().size()) {
			//������v���Ă��邩�m�F�B
			//�A�C�e���f�[�^�̎擾�B
			auto itemMap = Inv_GetItemDataMap();
			//�A�C�e���f�[�^�}�b�v�̃C�e���[�^�B
			auto IMitr = itemMap.begin();
			while (1) {
				//�����l�̓��Z�b�g�����ԁB
				bool isReset = true;
				//�C�e���[�^���Ō�܂œ��B������B
				//GetItemDataMap()��m_fontList�̃f�[�^�̈Ⴂ�͂Ȃ������Ƃ������ƁB
				if (IMitr == itemMap.end()) {
					//�f�[�^�ǉ��̗L�����������B
					Inv_ResetIsAddData();
					break;
				}
				//�t�H���g���X�g�̖��O���m�F�B
				auto FLitr = m_fontList.begin();
				while (1) {
					//�C�e���[�^���Ō�܂œ��B������B
					//�������O�����݂��Ă��Ȃ������Ƃ������ƁB
					if (FLitr == m_fontList.end()) {
						isReset = false;
						break;
					}
					//�������O�����łɑ��݂��Ă�����B
					if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
						//������̔�r���s���A���̕ϓ����N���Ă��Ȃ����m�F�B
						//int����std::wstring�ɕϊ��B
						std::wstring num = std::to_wstring(IMitr->second);
						//�����ς���Ă��Ȃ��Ȃ�B
						if (FLitr->numFR->GetText().compare(num) == 0) {
							break;
						}
						else {
							isReset = false;
							break;
						}
					}
					FLitr++;
				}
				//���Z�b�g���Ȃ��Ȃ�break����B
				if (!isReset) {
					break;
				}
				IMitr++;
			}
		}
	}
}