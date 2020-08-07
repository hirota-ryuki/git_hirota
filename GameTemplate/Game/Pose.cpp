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

	ResetIsAdd();
}
void Pose::FontRenderUpdate()
{
	//�ǉ��������͒l�̕ϓ����N���Ă�����B
	if (GetIsAddData() || GetIsAddNum()) {
		//���������Ă��Ȃ�������B
		if (m_fontList.begin() == m_fontList.end()) {
			auto& IDMap = GetItemDataMap();
			auto itr = IDMap.begin();
			//���O�̃t�H���g�����_�[�쐬�B
			FontRender* namefr = NewGO<FontRender>(GOPrio_Sprite, "item");
			int d = 0;
			namefr->SetText(itr->first.c_str());
			//���̃t�H���g�����_�[�쐬�B
			FontRender* numfr = NewGO<FontRender>(GOPrio_Sprite, "item");
			std::wstring num = std::to_wstring(itr->second);
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
			auto itemMap = GetItemDataMap();
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
						//�܂��������O��������B
						if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
							//int����std::wstring�ɕϊ��B
							std::wstring num = std::to_wstring(IMitr->second);
							//������̔�r�B
							if (FLitr->numFR->GetText().compare(num) != 0) {
								FLitr->numFR->SetText(num.c_str());
							}
						}
					}
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
		m_position = NAME_TOP_POS + ADD_BELOW_POS * i;
		FRitr->nameFR->SetPosition(m_position);
		m_position = NUM_TOP_POS + ADD_BELOW_POS * i;
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
		//�t�H���g���X�g�ƃA�C�e���f�[�^�̃T�C�Y���������Ȃ�������B
		if (m_fontList.size() == GetItemDataMap().size()) {
			//�f�[�^�ǉ��̗L�����������B
			ResetIsAddData();
		}
		else {
			//�A�C�e���f�[�^�̎擾�B
			auto itemMap = GetItemDataMap();
			//�A�C�e���f�[�^�}�b�v�̃C�e���[�^�B
			for (auto IMitr = itemMap.begin(); IMitr != itemMap.end(); IMitr++) {
				//�t�H���g�����_�[���X�g�̃C�e���[�^�B
				for (auto FLitr = m_fontList.begin(); FLitr != m_fontList.end(); FLitr++) {
					//�܂��������O��������B
					if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
						//int����std::wstring�ɕϊ��B
						std::wstring num = std::to_wstring(IMitr->second);
						//������̔�r�B
						if (FLitr->numFR->GetText().compare(num) != 0) {
							//�f�[�^�ǉ��̗L�����������B
							ResetIsAddData();
						}
					}
				}
			}
		}
	}
}