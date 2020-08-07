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
	//スタートボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		//フォントを描画する。
		for (auto& itemdata : m_fontList) {
			itemdata.nameFR->ChangeActive();
			itemdata.numFR->ChangeActive();
		}
	}

	ResetIsAdd();
}
void Pose::FontRenderUpdate()
{
	//追加もしくは値の変動が起きていたら。
	if (GetIsAddData() || GetIsAddNum()) {
		//何も入っていなかったら。
		if (m_fontList.begin() == m_fontList.end()) {
			auto& IDMap = GetItemDataMap();
			auto itr = IDMap.begin();
			//名前のフォントレンダー作成。
			FontRender* namefr = NewGO<FontRender>(GOPrio_Sprite, "item");
			int d = 0;
			namefr->SetText(itr->first.c_str());
			//個数のフォントレンダー作成。
			FontRender* numfr = NewGO<FontRender>(GOPrio_Sprite, "item");
			std::wstring num = std::to_wstring(itr->second);
			numfr->SetText(num.c_str());
			//アイテムフォントデータの構築。
			ItemFontData ifd;
			ifd.nameFR = namefr;
			ifd.numFR = numfr;
			//登録。
			m_fontList.emplace_back(ifd);
		}
		else {
			//アイテムデータの取得。
			auto itemMap = GetItemDataMap();
			//アイテムデータマップのイテレータ。
			for (auto IMitr = itemMap.begin(); IMitr != itemMap.end(); IMitr++) {
				//フォントレンダーリストのイテレータ。
				for (auto FLitr = m_fontList.begin(); FLitr != m_fontList.end(); FLitr++) {
					//アイテムが追加されていたら。
					if (GetIsAddData()) {
						//フォントレンダーのテキストとアイテムデータの名前の比較。
						//フォントレンダーリストに登録されていなかったら。
						if (FLitr->nameFR->GetText().compare(IMitr->first) != 0) {
							//名前のフォントレンダー作成。
							FontRender* namefr = NewGO<FontRender>(GOPrio_Sprite, "item");
							namefr->SetText(IMitr->first.c_str());
							//個数のフォントレンダー作成。
							FontRender* numfr = NewGO<FontRender>(GOPrio_Sprite, "item");
							std::wstring num = std::to_wstring(IMitr->second);
							numfr->SetText(num.c_str());
							//アイテムフォントデータの構築。
							ItemFontData ifd;
							ifd.nameFR = namefr;
							ifd.numFR = numfr;
							//登録。
							m_fontList.emplace_back(ifd);
						}
					}
					//アイテムの個数が変動されていたら。
					if (GetIsAddNum()) {						
						//まず同じ名前を見つける。
						if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
							//intからstd::wstringに変換。
							std::wstring num = std::to_wstring(IMitr->second);
							//文字列の比較。
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
	//初期値から下に座標をずらしていく。
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
	//追加もしくは値の変動が起きていたら。
	if (GetIsAddData() || GetIsAddNum()) {
		//フォントリストとアイテムデータのサイズが等しくなかったら。
		if (m_fontList.size() == GetItemDataMap().size()) {
			//データ追加の有無を初期化。
			ResetIsAddData();
		}
		else {
			//アイテムデータの取得。
			auto itemMap = GetItemDataMap();
			//アイテムデータマップのイテレータ。
			for (auto IMitr = itemMap.begin(); IMitr != itemMap.end(); IMitr++) {
				//フォントレンダーリストのイテレータ。
				for (auto FLitr = m_fontList.begin(); FLitr != m_fontList.end(); FLitr++) {
					//まず同じ名前を見つける。
					if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
						//intからstd::wstringに変換。
						std::wstring num = std::to_wstring(IMitr->second);
						//文字列の比較。
						if (FLitr->numFR->GetText().compare(num) != 0) {
							//データ追加の有無を初期化。
							ResetIsAddData();
						}
					}
				}
			}
		}
	}
}