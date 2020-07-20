#pragma once
class Player;
class Pose;
class BulletStack :
	public IItem
{
public:
	BulletStack();
	~BulletStack();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void OnGet() override;
	/// <summary>
	/// �|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �|�W�V�������擾�B
	/// </summary>
	CVector3 GetPos()
	{
		return m_position;
	}
	/// <summary>
	/// ��]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
	
private:
	//���f���֌W�B
	SkinModelRender*	m_model = nullptr;
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	//�C���X�^���X�֌W�B
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	Player*				m_player = nullptr;						//�v���C���[�N���X�̃|�C���^�B
	//Pose*				m_pose = nullptr;
	//���C�g�֌W�B
	CVector4			m_directionLightDirection = { 1.0f, -1.0f, 0.0f, 0.0f };
	CVector4			m_directionLightColor = { 0.5f, 0.5f, 0.5f, 10.2f };
	float				m_spec = 5.0f;
	CVector3			m_ambientLight = CVector3::Zero();

	//�A�C�e�����֌W�B
	const int			ADD_BULLET_STACK = 10;					//�����Ă���e��̐��B		
	bool				m_isAddBullet = false;					//�e�e���ǉ����ꂽ���ǂ����B

	//�|�b�v�A�b�v�摜�֌W�B
	SpriteRender*		m_sprite = nullptr;
	CVector2			m_spritePos = FRAME_OUT_POS;

	//�A�C�e���f�[�^�B
	const wchar_t* NAME = L"bulletstack";
	const int      ADD = 10;
};

