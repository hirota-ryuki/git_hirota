#pragma once

const CVector2 NAME_TOP_POS  = { 0.0f,0.0f };	//�t�H���g�����Ԉ�ԏ�̖��O�̍��W�B
const CVector2 NUM_TOP_POS   = { 250.0f,0.0f };	//�t�H���g�����Ԉ�ԏ�̌��̍��W�B
const CVector2 ADD_BELOW_POS = { 0.0f,50.0f };	//���ڂ̍��W�̒ǉ��ʁB���̒萔�����W���������Ă����B
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
class Pose :
	public IGameObject
{
public:
	Pose();
	~Pose();
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

	std::list<ItemFontData>	m_fontList;
	CVector2	m_position = CVector2::Zero();
	CVector3	m_position2 = CVector3::Zero();
};

