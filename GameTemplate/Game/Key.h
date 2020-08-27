#pragma once
#include "IGameObject.h"

class Key :
	public IItem
{
public:
	Key();
	~Key();
	void ItemDestroy() override;
	void ItemStart() override;
	void ItemUpdate() override;
	void OnGet() override;	
	/// <summary>
	/// �{�[���̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �{�[���̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �{�[���̎��ʔԍ����Z�b�g�B
	/// </summary>
	/// <param name="pos">���ʔԍ��B</param>
	void SetNomber(int no)
	{
		m_nomber = no;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="name"></param>
	void SetName(std::wstring name) {
		m_name = name;
	}
private:
	SkinModelRender*	m_model = nullptr;						//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	int					m_nomber = 0;							//���ʔԍ��B		
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	Player*				m_player = nullptr;						//�v���C���[�N���X�̃|�C���^�B

	//�|�b�v�A�b�v�摜�֌W�B
	SpriteRender*		m_sprite = nullptr;
	CVector2			m_spritePos = FRAME_OUT_POS;

	//�A�C�e���f�[�^�B
	const wchar_t* NAME = L"��";
	std::wstring		m_name;
	const int      ADD = 1;

};

