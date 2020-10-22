#pragma once
#include "character/CharacterController.h"

class Bullet;
class UI;
class SpotLight;
class SkinModel;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void Update_NotPause() override;
	
	/// <summary>
	/// �v���C���[�̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
		m_charaCon.SetPosition(m_position);
	}
	/// <summary>
	/// �v���C���[�̃|�W�V�������擾�B
	/// </summary>
	const CVector3& GetPos() const
	{
		return m_position;
	}
	/// <summary>
	/// �����œn���ꂽ���W�ƃv���C���[�̋�����2����v�Z���܂��B
	/// </summary>
	/// <param name="pos">���W</param>
	/// <returns>������2��</returns>
	float CalcDistanceSQFrom(const CVector3& pos) const
	{
		CVector3 diff = m_position - pos;
		return diff.LengthSq();
	}
	/// <summary>
	/// �����œn���ꂽ���W����v���C���[�Ɍ������ĐL�т�
	/// XZ���ʂł̌����x�N�g��(�傫���P)���v�Z�B
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	CVector3 CalcDirectionXZFrom(const CVector3& pos) const
	{
		CVector3 moveDirection = m_position - pos;
		moveDirection.y = 0.0f;
		moveDirection.Normalize();
		return moveDirection;
	}
	/// <summary>
	/// �v���C���[�̃��[�u�X�s�[�h���擾�B
	/// </summary>
	const CVector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	/// <summary>
	/// �v���C���[�̉�]���Z�b�g�B
	/// </summary>
	/// <param name="rot">��]�B</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �v���C���[�̉�]���擾�B
	/// </summary>
	/// <returns>��]�B</returns>
	const CQuaternion& GetRot() const
	{
		return m_rotation;
	}
	/// <summary>
	/// �v���C���[�̎c�e�����擾�B
	/// </summary>
	const int GetCapacity() const
	{
		return m_capacity;
	}
	/// <summary>
	/// �v���C���[�̑��e�����擾�B
	/// </summary>
	const int GetMaxCapacity() const
	{
		return m_maxCapacity;
	}
	/// <summary>
	/// �v���C���[�̎莝���̑��e�����擾�B
	/// </summary>
	const int GetStack() const
	{
		return m_stack;
	}
	/// <summary>
	/// �v���C���[�̎莝���̑��e���𑫂��B
	/// </summary>
	void AddStack(const int bullet)
	{
		m_stack += bullet;
	}
	/// <summary>
	/// �v���C���[�̎莝���̑��e�����擾�B
	/// </summary>
	const SkinModelRender* GetSkinModelRender() const
	{
		return m_model;
	}
	/// <summary>
	/// ���܂ꂽ���ǂ������Z�b�g�B
	/// </summary>
	/// <param name="flag">���܂ꂽ���ǂ����B</param>
	void SetIsBiteconst (bool flag) {
		m_isBite = flag;
	}
	/// <summary>
	/// �L�����R����Ԃ��֐��B
	/// </summary>
	/// <returns>�L�����R���B</returns>
	const CharacterController* GetCharaCon()  const{
		return &m_charaCon;
	}
	const CharacterController& GetCharaCon_Ghorst()  const{
		return m_charaCon;
	}
	/// <summary>
	/// ���f���̑O�����B
	/// </summary>
	/// <returns>���f���̑O�����B</returns>
	const CVector3& GetForward() const{
		return m_forward;
	}
	/// <summary>
	/// �e�p�̎��_�B
	/// </summary>
	/// <returns>�e�p�̎��_�B</returns>
	const CVector3& GetShadowPos() const{
		return m_shadowPos;
	}
private:
	/// <summary>
	/// �A�j���[�V�����N���b�v���������B
	/// </summary>
	void InitAnimationClips();
	/// <summary>
	/// ���f�����������B
	/// </summary>
	void InitModel();
	/// <summary>
	/// �_���[�W�\����2D��������
	/// </summary>
	void InitDamageSprite();
	/// <summary>
	/// �Ə�2D���������B
	/// </summary>
	void InitAimSprite();
	/// <summary>
	/// GameCamera�̃C���X�^���X���擾����֐��B
	/// </summary>
	void GetGameCameraInst() {
		if (m_game->GetCamera() != nullptr
			&& !m_isGameCamera) {
			m_gamecamera = m_game->GetCamera();
			m_isGameCamera = true;
		}
	}
	/// <summary>
	/// �ҋ@�X�e�[�g�֐��B
	/// </summary>
	void En_Idle();
	/// <summary>
	/// �����X�e�[�g�֐��B
	/// </summary>
	void En_Walk();
	/// <summary>
	/// ����X�e�[�g�֐��B
	/// </summary>
	void En_Run();
	/// <summary>
	/// �G�C���X�e�[�g�֐��B
	/// </summary>
	void En_Aim();
	/// <summary>
	/// ���X�e�[�g�֐��B
	/// </summary>
	void En_Shot();
	/// <summary>
	/// �����[�h����X�e�[�g�֐��B
	/// </summary>
	void En_Reload();
	/// <summary>
	/// ���݂����X�e�[�g�֐��B
	/// </summary>
	void En_Lie();
	/// <summary>
	/// �X�e�[�g��ύX����֐��B
	/// ����ȏꍇ�������B
	/// </summary>
	void ChangeState();
	/// <summary>
	/// �v���C���[�̓����B
	/// </summary>
	void Move();
	/// <summary>
	/// �v���C���[�̉�]�B
	/// </summary>
	void Rotation();
	/// <summary>
	/// ���Ԃɂ���ĉ񕜂���֐��B
	/// </summary>
	void Heal();
	/// <summary>
	/// �_���[�W��^����֐��B
	/// </summary>
	void Damage();
	/// ���ʊ֐��B
	/// </summary>
	void Death();
	/// ���D���C�g�֐��B
	/// </summary>
	void ActressLight();
	/// <summary>
	/// �����d���B
	/// </summary>
	void LightUpdate();
private:
	SkinModelRender*	m_model = nullptr;						//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CharacterController m_charaCon;								//�L�����N�^�[�R���g���[���[�B
	CVector3			m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	UI*					m_ui = nullptr;							//UI�̃|�C���^�B
	SpotLight*			m_sl = nullptr;							//UI�̃|�C���^�B
	float				m_speed = 210.0f;			//�L�����������X�s�[�h�B
	float				m_runSpeed = 280.0f;		//�L����������X�s�[�h�B
	
	//�A�j���[�V�����֌W�B
	enum {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_walk_left,
		enAnimationClip_walk_right,
		enAnimationClip_back,
		enAnimationClip_run,
		enAnimationClip_aim,
		enAnimationClip_shot,
		enAnimationClip_reload,
		enAnimationClip_lie,
		enAnimationClip_num,
	};
	AnimationClip m_animationClip[enAnimationClip_num];		//�A�j���[�V�����N���b�v�B
	Animation m_animation;//�A�j���[�V�����̃C���X�^���X�B
	//�X�e�[�g�֌W�B
	enum State {
		enState_idle,
		enState_walk,
		enState_run,
		enState_aim,
		enState_shot,
		enState_reload,
		enState_lie,
		enState_death,
		enState_num,
	};
	State			m_state = enState_idle;
	bool			m_isRun = false;		//�����Ă��邩�ǂ����B
	bool			m_isShot = false;		//�����Ă��邩�ǂ����B
	//Shot�֌W�B
	Bullet*			m_bullet = nullptr;		//Bullet�N���X�̃|�C���^�B
	int				m_timer = 0;			//�^�C�}�[�B
	bool			m_isBullet = false;		//���������ǂ����B
	bool			m_isEmpty = false;		//��C�����������ǂ����B
	int				m_capacity = 12;		//�c�e���B
	const int		m_maxCapacity = 12;		//���e���B
	int				m_stack = 24;			//�莝���̑��e���B
	//�_���[�W�֌W�B
	bool			m_isBite = false;			//���܂ꂽ���ǂ����B
	bool			m_isOldCameraInfo = false;	//�J�����̏����L�^�������ǂ����B
	bool			m_isGameCamera = false;		//GameCamera�N���X�̃|�C���^���擾�������ǂ����B
	GameCamera*		m_gamecamera = nullptr;		//GameCamera�N���X�̃|�C���^�B
	//�J�������̃o�b�N�A�b�v�B
	float			m_oldplayer = 0.0f;			//�|���O�̃J�����̎��_�B
	float			m_oldpos = 0.0f;			//�|���O�̃J�����̒����_�B
	float			m_oldAddY = 0.0f;			//�|���O�̓_��n�ʂ���㏸������ʁB
	//�_���[�W�摜�֌W�B
	SpriteRender*	m_sprite = nullptr;
	float			m_alpha = 0.0f;
	//�Ə��摜�֌W�B
	SpriteRender*	m_aimSprite = nullptr;
	//�񕜊֌W�B
	bool			m_isDamage = false;			//�_���[�W���󂯂����ǂ����B
	int 			m_healTimer = 0;			//�񕜂���܂ł̃^�C�}�[�B
	const int		m_healStartTime = 600;		//�񕜂���܂ł̎��ԁB
	int 			m_heaIntervalTimer = 0;		//�񕜂Ɖ񕜂̊Ԃ̃^�C�}�[�B���X�ɉ񕜂���悤�ɂ���B
	int				m_healIntervalTime = 30;	//�񕜂Ɖ񕜂̊Ԃ̎��ԁB���X�ɉ񕜂���悤�ɂ���B
	//�X�e�[�^�X�֌W�B
	const float		m_maxhp = 5.0f;		//�ő�̗́B
	float			m_hp = m_maxhp;		//�̗́B
	//���C�g�B
	bool			m_isLight = true;			//���C�g�����Ă��邩�ǂ����B
	CVector3		m_shadowPos = CVector3::One();

	//���f���̑O�����B
	CVector3		m_forward = CVector3::AxisX();
};