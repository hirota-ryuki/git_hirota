#pragma once

class SkinModelRender : public IGameObject
{
public:
	SkinModelRender();
	~SkinModelRender();
	void Update() override;
	bool Start() override;
	void Draw() override;
	/// <summary>
	/// ���[���h�s����X�V�B
	/// </summary>
	void UpdateWorldMatrix();
	/// <summary>
	/// �X�L�����f���̏�����
	/// </summary>
	/// <param name="texFilePath">���f���̃t�@�C���p�X</param>
	void Init(const wchar_t* texFilePath);
	/// <summary>
	/// ���W�A��]��ݒ�
	/// �g�嗦�͐G��Ȃ��悤��
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	void SetData(CVector3 pos, CQuaternion rot)
	{
		m_pos = pos;
		m_rot = rot;
	}
	
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(CVector3 pos)
	{
		m_pos = pos;
	}

	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="pos">��]</param>
	void SetRot(CQuaternion rot)
	{
		m_rot = rot;
	}
	/// <summary>
	/// �`����s��Ȃ����[�h�B
	/// </summary>
	void NotModelMode()
	{
		m_isNotModel = false;
	}
	/// <summary>
	/// �X�L�����f�����擾�B
	/// </summary>
	/// <returns>�X�L�����f��</returns>
	SkinModel& GetModel()
	{
		return m_model;
	}
	/*!
	* @brief	���f���̑O���������擾�B
	*/
	const CVector3& GetForward() const
	{
		return m_forward;
	}
	void SetLight(int light, CVector4 direction) {
		m_model.SetLight(light, direction);
	}
	/// <summary>
	/// �e��ݒ肷��֐��B
	/// ��{��true�B
	/// </summary>
	/// <param name="shadow">�e��ݒ肷�邩�ǂ����B</param>
	void SetShadowCaster(bool shadow) {
		m_isShadow = shadow;
	}
	/// <summary>
	/// �V���h�E���V�[�o�[�̃t���O��ݒ肷��B
	/// </summary>
	/// <param name="flag">true��n���ƃV���h�E���V�[�o�[�ɂȂ�</param>
	/// <remarks>
	/// �V���h�E���V�[�o�[�Ƃ͉e�𗎂Ƃ����I�u�W�F�N�g�̂��Ƃł��B
	/// �V���h�E�L���X�^�[�ɂ���Đ������ꂽ�A�V���h�E�}�b�v�𗘗p����
	/// ���g�ɉe�𗎂Ƃ��܂��B
	/// �I�u�W�F�N�g���V���h�E���V�[�o�[���V���h�E�L���X�^�[�ɂȂ��Ă���ꍇ��
	/// �Z���t�V���h�E(�����̉e�������ɗ�����)���s�����Ƃ��ł��܂��B
	/// </remarks>
	void SetShadowReciever(bool flag)
	{
		m_model.SetShadowReciever(flag);
	}
private:
	SkinModel		m_model;								//�X�L�����f���B
	CVector3		m_pos = CVector3::Zero();
	CQuaternion		m_rot = CQuaternion::SpriteRot();
	CVector3		m_scale = CVector3::One();
	bool			m_isNotModel = true;
	bool			m_isUpdate = false;						//Update�֐����Ă΂ꂽ���ǂ����B
	CVector3		m_forward = CVector3::Front();			//�J�����̑O���B
	CMatrix			m_viewMatrixInv = CMatrix::Identity();	//�r���[�̋t�s��B
	bool			m_isShadow = true;						//�e�𗎂Ƃ����ǂ����B
};