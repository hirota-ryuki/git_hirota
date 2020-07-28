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
	swprintf_s(bullet, L"玉x");
	m_itemNameFont->SetText(bullet);
	m_itemNameFont->SetPosition({ 0.0f, 0.0f });
	m_itemNameFont->ChangeActive();

	m_itemCountfont = NewGO<FontRender>(GOPrio_Sprite,"item");
	m_itemCountfont->SetPosition({ 100.0f, 0.0f });
	m_itemCountfont->ChangeActive();

#ifdef BAG_MODE
	//画像。
	m_spriteMenu = NewGO<SpriteRender>(GOPrio_Sprite);
	m_spriteMenu->Init(L"sprite/item_menu.dds", 640.f, 360.f);
	//画像を消しておく。
	m_spriteMenu->ActiveMode(false);
#endif // BAG_MODE

	return true;
}

void Pose::Update()
{
	FontRenderUpdate();
	//スタートボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		/*m_isStart = !m_isStart;
		wchar_t allbullet[50];
		swprintf_s(allbullet, L"%d", m_itemCount);
		m_itemCountfont->SetText(allbullet);
		m_itemNameFont->ChangeActive();
		m_itemCountfont->ChangeActive();*/

		//フォントを描画する。
		for (auto& itemdata : m_fontList) {
			itemdata.nameFR->ChangeActive();
			itemdata.numFR->ChangeActive();
		}
#ifdef BAG_MODE
		m_spriteMenu->ChangeActive();
#endif // BAG_MODE

		//データ追加の有無を初期化。
	}
	//ResetIsAddData();

#ifdef BAG_MODE
	//メニュー画面が表示されていたら。
	if (m_isStart) {
		
	}
#endif // BAG_MODE
}
void Pose::FontRenderUpdate()
{
	//追加もしくは値の変動が起きていたら。
	if (GetIsAddData() || GetIsAddNum()) {
		//何も入っていなかったら。
		if (m_fontList.begin() == m_fontList.end()) {
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
		else {
			//アイテムデータの取得。
			auto& itemMap = GetItemDataMap();
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
						//intからstd::wstringに変換。
						std::wstring num = std::to_wstring(IMitr->second);
						//まず同じ名前を見つける。
						if (FLitr->nameFR->GetText().compare(IMitr->first) == 0) {
							//文字列の比較。
							if (FLitr->numFR->GetText().compare(num) != 0) {
								FLitr->numFR->SetText(num.c_str());
							}
						}
					}
				}
			}
		}
	//追加もしくは値の変動が起きていたら。
	//if (GetIsAddData() || GetIsAddNum()) {
	//	//アイテムデータの取得。
	//	auto& itemMap = GetItemDataMap();
	//	//アイテムデータマップのイテレータ。
	//	for (auto itr = itemMap.begin(); itr != itemMap.end(); itr++) {
	//		//フォントレンダーリストのイテレータ。
	//		for (auto &itemdata : m_fontList) {
	//			//アイテムが追加されていたら。
	//			if (GetIsAddData()) {
	//				//フォントレンダーのテキストとアイテムデータの名前の比較。
	//				//フォントレンダーリストに登録されていなかったら。
	//				if (itemdata.nameFR->GetText().compare(itr->first) != 0) {
	//					//名前のフォントレンダー作成。
	//					FontRender* namefr = NewGO<FontRender>(GOPrio_Sprite, "item");
	//					namefr->SetText(itr->first.c_str());
	//					//個数のフォントレンダー作成。
	//					FontRender* numfr = NewGO<FontRender>(GOPrio_Sprite, "item");
	//					std::wstring num = std::to_wstring(itr->second);
	//					numfr->SetText(num.c_str());
	//					//アイテムフォントデータの構築。
	//					ItemFontData ifd;
	//					ifd.nameFR = namefr;
	//					ifd.numFR = numfr;
	//					//登録。
	//					m_fontList.emplace_back(ifd);
	//				}
	//			}
	//			//アイテムの個数が変動されていたら。
	//			if (GetIsAddNum()) {
	//				//intからstd::wstringに変換。
	//				std::wstring num = std::to_wstring(itr->second);
	//				//まず同じ名前を見つける。
	//				if (itemdata.nameFR->GetText().compare(itr->first) == 0) {
	//					//文字列の比較。
	//					if (itemdata.numFR->GetText().compare(num) != 0) {
	//						itemdata.numFR->SetText(num.c_str());
	//					}
	//				}
	//			}
	//		}
	//	}

		auto& FRitr = m_fontList.begin();
		//初期値から下に座標をずらしていく。
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