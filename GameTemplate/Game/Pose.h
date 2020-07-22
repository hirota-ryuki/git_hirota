#pragma once
//#define BAG_MODE

const CVector2 NAME_TOP_POS  = { 0.0f,0.0f };	//�t�H���g�����Ԉ�ԏ�̖��O�̍��W�B
const CVector2 NUM_TOP_POS   = { 10.0f,0.0f };	//�t�H���g�����Ԉ�ԏ�̌��̍��W�B
const CVector2 ADD_BELOW_POS = { 0.0f,5.0f };	//���ڂ̍��W�̒ǉ��ʁB���̒萔�����W���������Ă����B
const CVector3 ADD2_BELOW_POS = { 0.0f,5.0f,0.0f };	//���ڂ̍��W�̒ǉ��ʁB���̒萔�����W���������Ă����B

#ifdef BAG_MODE
struct ItemInfo {
	const wchar_t*		name;				//�A�C�e���̖��O�B
	SpriteRender*		sprite = nullptr;	//�摜�̃|�C���^�B
	int					nomber = 0;			//�o�b�O�̉��Ԗڂɂ��邩�B
};
#endif // BAG_MODE

//�A�C�e���̃t�H���g�f�[�^�B
struct ItemFontData {
	FontRender* nameFR = nullptr;
	FontRender* numFR = nullptr;
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
	void DrawFontRender();
#ifdef BAG_MODE
	/// <summary>
	/// �A�C�e����ǉ�����֐��B
	/// ���łɎ����Ă���A�C�e���Ȃ�A�������ǉ�����
	/// </summary>
	/// <param name="name"></param>
	/// <param name="textureFIlePath"></param>
	void AddItem(const wchar_t* name, const wchar_t* textureFIlePath);
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
	int						m_totalItemCount = 0;		//���A�C�e�����B
#endif // BAG_MODE
	std::vector<ItemFontData>	m_fontList;
	CVector2	m_position = CVector2::Zero();
	CVector3	m_position2 = CVector3::Zero();
};

