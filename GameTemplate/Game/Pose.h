#pragma once
//struct ItemInfo {
//	SpriteRender*	m_sprite = nullptr;		//�摜�̃|�C���^�B
//	const wchar_t*  m_texFIlePath;
//	int				m_nomber = 0;			//�o�b�O�̉��Ԗڂɂ��邩�B
//};
struct ItemInfo_Font {
	//�A�C�e���̊�{���B
	const wchar_t*  m_itemName;			//�A�C�e���̖��O�B
	int				m_itemCount = 0;	//���̃A�C�e���̌��B
	int				m_bagNumber = 0;	//�o�b�O�̉��Ԗڂɂ��邩�B
	//�t�H���g��\�����邽�߂̏��B
	FontRender*		m_itemNameFont = nullptr;	//�A�C�e�����̃t�H���g�B
	FontRender*		m_itemCountfont = nullptr;	//�A�C�e���̌��̃t�H���g�B
	bool			m_isNewGO = false;			//�t�H���g��NewGO�������ǂ����B
	bool			m_isChange = false;			//�A�C�e���̌����ύX���ꂽ���ǂ����B
};
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
	//void AddItem(const wchar_t* textureFIlePath) {
	//	/*if (m_item < m_maxItem) {
	//		m_itemTexList.emplace_back(textureFIlePath);
	//		m_item++;
	//	}*/
	//	m_itemInfo.m_texFIlePath = textureFIlePath;
	//}
	//void AddItem(const wchar_t* itemname);
	int GetItemCount(){
		return m_itemCount;
	}
	void AddItemCount(){
		m_itemCount++;
	}
private:
	SpriteRender*	m_sprite = nullptr;			//�摜�̃|�C���^�B
	bool			m_isStart = false;			//�X�^�[�g��ʂ��\������Ă��邩�ǂ����B
	FontRender*		m_itemNameFont = nullptr;	//�A�C�e�����̃t�H���g�B
	FontRender*		m_itemCountfont = nullptr;	//�A�C�e���̌��̃t�H���g�B
	int				m_itemCount = 0;	//���̃A�C�e���̌��B


	//ItemInfo_Font	m_itemList[12];
	//FontPos			m_posList[12];
	//bool			m_isStart = false;			//�X�^�[�g��ʂ��\������Ă��邩�ǂ����B
	//int				m_allItemCount = 0;			//�A�C�e���̎�ނ̐��B

	//SpriteRender*	m_itemSprite = nullptr;				//�A�C�e���̉摜�̃|�C���^�B
	//std::vector< CVector3* >		m_itemPosList;		//�A�C�e���̍��W�z��B
	//std::vector< CVector3* >		m_itemPosList;		//�A�C�e���̍��W�z��B
	//std::vector< ItemInfo* >		m_itemList;			//�A�C�e���̍��W�z��B
	//ItemInfo m_itemInfo;
	////std::vector< const wchar_t* >	m_itemTexList;		//�A�C�e���̃e�N�X�`���̔z��B
	////std::vector< SpriteRender* >	m_itemSpriteList;	//�A�C�e���̃e�N�X�`���̔z��B
	//int				m_item = 0;							//�A�C�e���̎莝���̑����B			
	//const int		m_maxItem = 12;						//���Ă�A�C�e���̍ő吔�B
	//bool			m_isStart = false;					//�X�^�[�g��ʂ��\������Ă��邩�ǂ����B
	//bool			m_isNewSprite = false;				//�X�^�[�g��ʂ��\������Ă��邩�ǂ����B
};

