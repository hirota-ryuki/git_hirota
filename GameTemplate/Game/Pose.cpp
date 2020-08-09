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
	DeleteGOs("item");
}

bool Pose::Start()
{
	AddItem(L"�e��", 25);
	AddItem(L"�񕜖�", 25);
	AddItem(L"������", 25);
	return true;
}

void Pose::Update()
{
	FontRenderUpdate();
	//�X�^�[�g�{�^������������B
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		//�t�H���g��`�悷��B
		for (auto& itemdata : m_fontList) {
			itemdata.nameFR->ChangeActive();
			itemdata.numFR->ChangeActive();
		}
	}

	//ResetIsAdd();
}
void Pose::FontRenderUpdate()
{
	//�ǉ��������͒l�̕ϓ����N���Ă�����B
	if (GetIsAddData() || GetIsAddNum()) {
		//���������Ă��Ȃ�������B
		if (m_fontList.begin() == m_fontList.end()) {
			auto& IDMap = GetItemDataMap();
			auto itr = IDMap.begin();
			//�A�C�e���t�H���g�f�[�^�̍\�z�B
			ItemFontData ifd;
			//���O�̃t�H���g�����_�[�쐬�B
			ifd.nameFR = NewGO<FontRender>(GOPrio_Sprite, "item");
			ifd.nameFR->SetText(itr->first.c_str());
			//���̃t�H���g�����_�[�쐬�B
			ifd.numFR = NewGO<FontRender>(GOPrio_Sprite, "item");
			std::wstring num = std::to_wstring(itr->second);
			ifd.numFR->SetText(num.c_str());
			//�o�^�B
			m_fontList.emplace_back(ifd);
		}
		else {
			//�A�C�e���f�[�^�̎擾�B
			auto itemMap = GetItemDataMap();
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
						//���̃t�H���g�����_�[�쐬�B
						ifd.numFR = NewGO<FontRender>(GOPrio_Sprite, "item");
						std::wstring num = std::to_wstring(IMitr->second);
						ifd.numFR->SetText(num.c_str());
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
void Pose::SortingFontRnderPos()
{
	auto FRitr = m_fontList.begin();
	//�����l���牺�ɍ��W�����炵�Ă����B
	for (int i = 0; i < GetItemDataMap().size(); i++) {
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
void Pose::ResetIsAdd()
{
	//�ǉ��������͒l�̕ϓ����N���Ă�����B
	if (GetIsAddData() || GetIsAddNum()) {
		//�t�H���g���X�g�ƃA�C�e���f�[�^�̃T�C�Y���������Ȃ�����B
		if (m_fontList.size() == GetItemDataMap().size()) {
			//������v���Ă��邩�m�F�B
			//�A�C�e���f�[�^�̎擾�B
			auto itemMap = GetItemDataMap();
			//�A�C�e���f�[�^�}�b�v�̃C�e���[�^�B
			auto IMitr = itemMap.begin();
			//�����l�̓��Z�b�g�����ԁB
			bool isReset = true;
			while (1) {
				//�C�e���[�^���Ō�܂œ��B������B
				//GetItemDataMap()��m_fontList�̃f�[�^�̈Ⴂ�͂Ȃ������Ƃ������ƁB
				if (IMitr == itemMap.end()) {
					//�f�[�^�ǉ��̗L�����������B
					ResetIsAddData();
					break;
				}
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
						//�����ς���Ă���Ȃ�B
						if (FLitr->numFR->GetText().compare(num) != 0) {
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