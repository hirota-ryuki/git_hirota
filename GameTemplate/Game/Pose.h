#pragma once
#include "IGameObject.h"
class Pose :
	public IGameObject
{
public:
	Pose();
	~Pose();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void AddItem(const wchar_t* textureFIlePath) {
		if (m_item < m_maxItem) {
			m_itemTexList.emplace_back(textureFIlePath);
			m_item++;
		}
	}
private:
	SpriteRender*	m_sprite = nullptr;					//画像のポインタ。
	SpriteRender*	m_itemSprite = nullptr;				//アイテムの画像のポインタ。
	std::vector< CVector3* >		m_itemPosList;		//アイテムの座標配列。
	std::vector< wchar_t* >			m_itemTexList;		//アイテムのテクスチャの配列。
	std::vector< SpriteRender* >	m_itemSpriteList;	//アイテムのテクスチャの配列。
	int				m_item = 0;							//アイテムの手持ちの総数。			
	const int		m_maxItem = 12;						//持てるアイテムの最大数。
	bool			m_isStart = false;					//スタート画面が表示されているかどうか。
};

