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
	SpriteRender*	m_sprite = nullptr;					//�摜�̃|�C���^�B
	SpriteRender*	m_itemSprite = nullptr;				//�A�C�e���̉摜�̃|�C���^�B
	std::vector< CVector3* >		m_itemPosList;		//�A�C�e���̍��W�z��B
	std::vector< wchar_t* >			m_itemTexList;		//�A�C�e���̃e�N�X�`���̔z��B
	std::vector< SpriteRender* >	m_itemSpriteList;	//�A�C�e���̃e�N�X�`���̔z��B
	int				m_item = 0;							//�A�C�e���̎莝���̑����B			
	const int		m_maxItem = 12;						//���Ă�A�C�e���̍ő吔�B
	bool			m_isStart = false;					//�X�^�[�g��ʂ��\������Ă��邩�ǂ����B
};

