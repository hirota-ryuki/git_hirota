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
	//Inv_AddItem(L"弾薬", 25);
	Inv_AddItem(L"回復薬", 3);
	Inv_AddItem(L"強化薬", 4);
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
	//追加済みのアイテムが0個になっていてアイテムデータから削除されていないか確認。
	if (!Inv_GetIsAddData() && !Inv_GetIsAddNum()) {
		if (Inv_GetIsDeleteItem()) {
			//アイテムデータの取得。
			auto itemMap = Inv_GetItemDataMap();
			//フォントリストのイテレータ。
			for (auto fitr = m_fontList.begin(); fitr != m_fontList.end();) {
				auto iitr = itemMap.begin();
				bool isContinue = false;
				while (1) {
					//イテレータが最後まで到達したら。
					if (iitr == itemMap.end()) {
						fitr = m_fontList.erase(fitr);
						isContinue = true;
						break;
					}
					//同じ名前がすでに存在していたら。
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
	
	//スタートボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		m_sprite->ChangeActive();
		//フォントを描画する。
		for (auto& itemdata : m_fontList) {
			itemdata.nameFR->ChangeActive();
			itemdata.numFR->ChangeActive();
		}
	}

	
}
void Menu::FontRenderUpdate()
{
	//追加もしくは値の変動が起きていたら。
	if (Inv_GetIsAddData() || Inv_GetIsAddNum()) {
		//何も入っていなかったら。
		if (m_fontList.begin() == m_fontList.end()) {
			auto IDMap = Inv_GetItemDataMap();
			auto itr = IDMap.begin();
			//アイテムフォントデータの構築。
			ItemFontData ifd;
			//名前のフォントレンダー作成。
			ifd.nameFR = NewGO<FontRender>(GOPrio_Sprite, "item");
			ifd.nameFR->SetText(itr->first.c_str());
			ifd.nameFR->ActiveMode(false);
			//個数のフォントレンダー作成。
			ifd.numFR = NewGO<FontRender>(GOPrio_Sprite, "item");
			std::wstring num = std::to_wstring(itr->second);
			ifd.numFR->SetText(num.c_str());
			ifd.numFR->ActiveMode(false);
			//登録。
			m_fontList.emplace_back(ifd);
		}
		else {
			//アイテムデータの取得。
			auto itemMap = Inv_GetItemDataMap();
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
						ifd.nameFR->ActiveMode(false);
						//個数のフォントレンダー作成。
						ifd.numFR = NewGO<FontRender>(GOPrio_Sprite, "item");
						std::wstring num = std::to_wstring(IMitr->second);
						ifd.numFR->SetText(num.c_str());
						ifd.numFR->ActiveMode(false);
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
void Menu::SortingFontRnderPos()
{
	auto FRitr = m_fontList.begin();
	//初期値から下に座標をずらしていく。
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
	//追加もしくは値の変動が起きていたら。
	if (Inv_GetIsAddData() || Inv_GetIsAddNum()) {
		//フォントリストとアイテムデータのサイズが等しくなったら。
		if (m_fontList.size() >= Inv_GetItemDataMap().size()) {
			//個数が一致しているか確認。
			//アイテムデータの取得。
			auto itemMap = Inv_GetItemDataMap();
			//アイテムデータマップのイテレータ。
			auto IMitr = itemMap.begin();
			while (1) {
				//初期値はリセットする状態。
				bool isReset = true;
				//イテレータが最後まで到達したら。
				//GetItemDataMap()とm_fontListのデータの違いはなかったということ。
				if (IMitr == itemMap.end()) {
					//データ追加の有無を初期化。
					Inv_ResetIsAddData();
					break;
				}
				//フォントリストの名前を確認。
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
						//個数が変わっていないなら。
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
				//リセットしないならbreakする。
				if (!isReset) {
					break;
				}
				IMitr++;
			}
		}
	}
}