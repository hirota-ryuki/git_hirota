#pragma once
#define BAG_MODE

#ifdef BAG_MODE
struct ItemInfo {
	const wchar_t*		name;				//�A�C�e���̖��O�B
	SpriteRender*		sprite = nullptr;	//�摜�̃|�C���^�B
	int					nomber = 0;			//�o�b�O�̉��Ԗڂɂ��邩�B
};
#endif // BAG_MODE

struct FontPos {
	CVector2	namepos = CVector2::Zero();
	CVector2	countpos = CVector2::Zero();
};
class Pose :
	public IGameObject
{
public:
	Pose();
	~Pose();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
#ifdef BAG_MODE
	void AddItem(const wchar_t* name, const wchar_t* textureFIlePath) {
		ItemInfo itemData;
		itemData.name = name;
		itemData.sprite = NewGO<SpriteRender>(GOPrio_Sprite);
		itemData.sprite->Init(textureFIlePath, 640.f, 360.f);
	}
#endif // BAG_MODE


	int GetItemCount(){
		return m_itemCount;
	}
	void AddItemCount(){
		m_itemCount++;
	}
private:
	bool			m_isStart = false;			//�X�^�[�g��ʂ��\������Ă��邩�ǂ����B
	FontRender*		m_itemNameFont = nullptr;	//�A�C�e�����̃t�H���g�B
	FontRender*		m_itemCountfont = nullptr;	//�A�C�e���̌��̃t�H���g�B
	int				m_itemCount = 0;			//���̃A�C�e���̌��B

#ifdef BAG_MODE
	SpriteRender*			m_spriteMenu = nullptr;		//���j���[��ʂ̉摜�B
	std::vector<ItemInfo>	m_itemList;					//�A�C�e���̃��X�g�B

#endif // BAG_MODE
};

