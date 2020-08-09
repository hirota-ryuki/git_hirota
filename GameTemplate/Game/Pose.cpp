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
	AddItem(L"弾薬", 25);
	AddItem(L"回復薬", 25);
	AddItem(L"強化薬", 25);
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

	//ResetIsAdd();
}
void Pose::FontRenderUpdate()
{
	//追加もしくは値の変動が起きていたら。
	if (GetIsAddData() || GetIsAddNum()) {
		//何も入っていなかったら。
		if (m_fontList.begin() == m_fontList.end()) {
			auto& IDMap = GetItemDataMap();
			auto itr = IDMap.begin();
			//アイテムフォントデータの構築。
			ItemFontData ifd;
			//名前のフォントレンダー作成。
			ifd.nameFR = NewGO<FontRender>(GOPrio_Sprite, "item");
			ifd.nameFR->SetText(itr->first.c_str());
			//個数のフォントレンダー作成。
			ifd.numFR = NewGO<FontRender>(GOPrio_Sprite, "item");
			std::wstring num = std::to_wstring(itr->second);
			ifd.numFR->SetText(num.c_str());
			//登録。
			m_fontList.emplace_back(ifd);
		}
		else {
			//アイテムデータの取得。
			auto itemMap = GetItemDataMap();
			//アイテムデータマップのイテレータ。
			for (auto IMitr = itemMap.begin(); IMitr != itemMap.end(); IMitr++) {
				auto FLitr = m_fontList.begin();
				while (1) {
					//イテレータが最後まで到達したら。
					if (FLitr == m_fontList.end()) {
						//アイテムフォントデータの構築。
						ItemFontData ifd;
						//名前のフォントレンダー作成。
						ifd.nameFR = NewGO<FontRender>(GOPrio_Sprite, "item");
						ifd.nameFR->SetText(IMitr->first.c_str());
						//個数のフォントレンダー作成。
						ifd.numFR = NewGO<FontRender>(GOPrio_Sprite, "item");
						std::wstring num = std::to_wstring(IMitr->second);
						ifd.numFR->SetText(num.c_str());
						//登録。
						m_fontList.emplace_back(ifd);
						break;
					}
					//同じ名前がすでに存在していたら。
					if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
						//文字列の比較を行い、個数の変動が起きていないか確認。
						//intからstd::wstringに変換。
						std::wstring num = std::to_wstring(IMitr->second);
						//個数が変わっているなら。
						if (FLitr->numFR->GetText().compare(num) != 0) {
							//値を変更。
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
	//初期値から下に座標をずらしていく。
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
	//追加もしくは値の変動が起きていたら。
	if (GetIsAddData() || GetIsAddNum()) {
		//フォントリストとアイテムデータのサイズが等しくなったら。
		if (m_fontList.size() == GetItemDataMap().size()) {
			//個数が一致しているか確認。
			//アイテムデータの取得。
			auto itemMap = GetItemDataMap();
			//アイテムデータマップのイテレータ。
			auto IMitr = itemMap.begin();
			//初期値はリセットする状態。
			bool isReset = true;
			while (1) {
				//イテレータが最後まで到達したら。
				//GetItemDataMap()とm_fontListのデータの違いはなかったということ。
				if (IMitr == itemMap.end()) {
					//データ追加の有無を初期化。
					ResetIsAddData();
					break;
				}
				auto FLitr = m_fontList.begin();
				while (1) {
					//イテレータが最後まで到達したら。
					//同じ名前が存在していなかったということ。
					if (FLitr == m_fontList.end()) {
						isReset = false;
						break;
					}
					//同じ名前がすでに存在していたら。
					if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
						//文字列の比較を行い、個数の変動が起きていないか確認。
						//intからstd::wstringに変換。
						std::wstring num = std::to_wstring(IMitr->second);
						//個数が変わっているなら。
						if (FLitr->numFR->GetText().compare(num) != 0) {
							isReset = false;
							break;
						}
					}
					FLitr++;
				}
				//リセットしないならbreakする。
				if (!isReset) {
					break;
				}
				IMitr++;
			}
		}
	}
}