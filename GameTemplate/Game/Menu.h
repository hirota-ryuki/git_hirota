#pragma once

const CVector2 NAME_TOP_POS = { 200.0f,250.0f };	//�t�H���g�����Ԉ�ԏ�̖��O�̍��W�B
const CVector2 NUM_TOP_POS = { 450.0f,250.0f };	//�t�H���g�����Ԉ�ԏ�̌��̍��W�B
const CVector2 ADD_BELOW_POS = { 0.0f,77.0f };	//���ڂ̍��W�̒ǉ��ʁB���̒萔�����W���������Ă����B
const CVector3 ADD2_BELOW_POS = { 0.0f,5.0f,0.0f };	//���ڂ̍��W�̒ǉ��ʁB���̒萔�����W���������Ă����B

//�A�C�e���̃t�H���g�f�[�^�B
struct ItemFontData {
	FontRender* nameFR = nullptr;
	FontRender* numFR = nullptr;
};

struct FontPos {
	CVector2	namepos = CVector2::Zero();
	CVector2	countpos = CVector2::Zero();
};
class Menu :
	public IGameObject
{
public:
	Menu();
	~Menu();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �t�H���g�����_�[���X�g�̍X�V�֐��B
	/// �t�H���g�����_�[�̒ǉ���z�u�̐������s���B
	/// </summary>
	void FontRenderUpdate();
	/// <summary>
	/// ������̔z�u�𐮗�����֐��B
	/// </summary>
	void SortingFontRnderPos();
	/// <summary>
	/// ResetIsAddData�֐����ĂԂ��ǂ����𔻒肵�Ă���֐��B
	/// </summary>
	void ResetIsAdd();
private:
	/// <summary>
	/// ��̃A�C�e�������̂��߂̃t�H���g�����_���[���쐬�B
	/// </summary>
	void CreateItemListFontRendersFirst();
	/// <summary>
	/// �A�C�e���̐����̂��߂̃t�H���g�����_���[���쐬�B
	/// </summary>
	/// <param name="itemDataIt">�쐬����A�C�e���̃f�[�^</param>
	void CreateItemListFontRenders(
		std::unordered_map<
			std::wstring,
			int
		>::iterator itemDataIt
	);
	/// <summary>
	/// ������0�̃A�C�e���̃t�H���g�����_���[���폜����B
	/// </summary>
	void DeleteFontRendererOnZeroItem();
	/// <summary>
	/// �A�C�e�����X�g�̕\�����A�N�e�B�u�ɂ���B
	/// </summary>
	void ActivateDispItemList();
private:
	bool			m_isStart = false;			//�X�^�[�g��ʂ��\������Ă��邩�ǂ����B
	FontRender*		m_itemNameFont = nullptr;	//�A�C�e�����̃t�H���g�B
	FontRender*		m_itemCountfont = nullptr;	//�A�C�e���̌��̃t�H���g�B
	SpriteRender*	m_sprite = nullptr;
	std::list<ItemFontData>	m_fontList;
	CVector2	m_position = CVector2::Zero();
	CVector3	m_position2 = CVector3::Zero();
};