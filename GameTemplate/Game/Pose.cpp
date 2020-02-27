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
	//画像。
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/item_menu.dds", 640.f, 360.f);
	//画像を消しておく。
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
	//スタートボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		//m_sprite->ChangeActive();
		m_isStart = !m_isStart;
	}
	//メニュー画面が表示されていたら。
	if (m_isStart) {
		//アイテムが未所持なら処理を行わない。
		if (m_allItemCount != 0) {
			//入手している種類数分だけフォントを表示する。
			for (int i = 0; i < m_allItemCount; i++) {
				//まだフォントがNewGOされていなかったら。
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
				//フォントがNewGOされていたら。
				else {

				}
			}
		}
	}


	////メニュー画面が表示されていたら。
	//if (m_isStart) {
	//	//画像ができてなかったら
	//	if (m_isNewSprite) {
	//		for (int i = 0; i < m_item; i++) {
	//			//画像。
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
	//同じ名前のアイテムがあるかどうか判定。
	bool isDoneName = false;
	int  doneNumber = 0;
	//アイテムが未所持なら処理を行わない。
	if (m_allItemCount != 0) {
		for (int i = 0; i < m_allItemCount; i++) {
			if (m_itemList[i].m_itemName == itemname) {
				doneNumber = i;
				isDoneName = true;
				break;
			}
		}
	}
	//もし同じ名前が存在しなかったら。
	if (!isDoneName) {
		//アイテムの名前の設定。
		itemInfo.m_itemName = itemname;
		//アイテムの個数を1に設定。
		itemInfo.m_itemCount = 1;
		//バッグの何番目に置くか設定。
		itemInfo.m_bagNumber = m_allItemCount;
		//リストに追加。
		m_itemList[m_allItemCount] = itemInfo;
		//アイテムの種類をカウント。
		m_allItemCount++;
	}
	//もし同じ名前が存在したら。
	else {
		//アイテムの個数を増やす。
		m_itemList[doneNumber].m_itemCount++;
		//個数が変更されたのでフラグを立てておく。
		m_itemList[doneNumber].m_isChange = true;
	}
		
}
