#pragma once
#define BAG_MODE

#ifdef BAG_MODE
struct ItemInfo {
	SpriteRender*	m_sprite = nullptr;		//�摜�̃|�C���^�B
	const wchar_t*  m_texFIlePath;
	int				m_nomber = 0;			//�o�b�O�̉��Ԗڂɂ��邩�B
};
#endif // BAG_MODE


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
#ifdef BAG_MODE
	
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
	SpriteRender*	m_spriteMenu = nullptr;		//���j���[��ʂ̉摜�B

#endif // BAG_MODE
};

