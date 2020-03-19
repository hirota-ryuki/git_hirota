#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::OnDestroy()
{
}

bool GameCamera::Start()
{
	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
	//�΂˃J�����̏������B
	m_springCamera.Init(
		g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
		1000.0f,		//�J�����̈ړ����x�̍ő�l�B
		true,			//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		5.0f			//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);
	return true;
}

void GameCamera::Update()
{	
	if (g_pad[0].IsTrigger(enButtonSelect)) {
		//m_isMapMode = !m_isMapMode;
	}

	if (!m_isMapMode) {
		//�ꎞ��~���Ă��Ȃ�������B
		if (!m_game->GetIsPose()) {
			if (m_player != nullptr) {
				//�J�����Ɍ��ݒn���Z�b�g�B
				CVector3 l_setpos = m_player->GetPos();
				l_setpos.y += 50000.0f;
				m_springCamera.SetPosition(l_setpos);

				// ���E�����߂�
				float toCameraPosRotAngleold = toCameraPosRotAngle;
				toCameraPosRotAngleold += (g_pad[0].GetRStickYF()*1.5f);

				float t = 60.0f;

				if (toCameraPosRotAngle > t) {
					toCameraPosRotAngleold = t;
				}
				if (toCameraPosRotAngle < -70.0f) {
					toCameraPosRotAngleold = -70.0f;
				}
				toCameraPosRotAngle = toCameraPosRotAngleold;

				//�v���C���[���璍���_�܂ł̃x�N�g����ݒ�B
				//z���O
				//�v���C���[�̑O�x�N�g���B
				CVector3 l_toCameraTarget;
				l_toCameraTarget.Set(0.0f, 0.0f, m_targetFromPlayer);
				m_player->GetRot().Multiply(l_toCameraTarget);

				//�����_�𓮂������߂̊O�ρB
				CQuaternion qAddRot2;
				CVector3 axis2;
				CVector3 y2 = { 0.0f,1.0f,0.0f };
				axis2.Cross(l_toCameraTarget, y2);
				axis2.Normalize();
				qAddRot2.SetRotationDeg(axis2, toCameraPosRotAngle);
				qAddRot2.Multiply(l_toCameraTarget);


				//�����_���王�_�܂ł̃x�N�g����ݒ�B
				CVector3 l_toCameraPos;
				l_toCameraPos.Set(60.0f, 0.0f, m_targetFromPos);
				m_player->GetRot().Multiply(l_toCameraPos);

				//���_�𓮂������߂̊O�ρB
				CQuaternion qAddRot;
				CVector3 axis;
				CVector3 y = { 0.0f,-1.0f,0.0f };
				axis.Cross(l_toCameraPos, y);
				axis.Normalize();
				qAddRot.SetRotationDeg(axis, toCameraPosRotAngle);
				qAddRot.Multiply(l_toCameraPos);

				CVector3 m_pos = m_player->GetPos();
				m_pos.y += 150.0f;

				//�����_���v�Z����B
				m_target = m_pos + l_toCameraTarget;
				//���_���v�Z����B
				m_pos = m_target + l_toCameraPos;

				//�����_�Ǝ��_��ݒ肷��B
				m_springCamera.SetTarget(m_target);
				m_springCamera.SetPosition(m_pos);

				//�����_���v���C���[�̉E���Ɉڂ��B
				//   �Z ���̒����_(m_target)
				//�@�@ \
				//�@    \
				//�@�@P  �Z�@ //�������ŏI�I�Ȓ����_�I�I�I
				//        \
				//         \
				//         �Z���_
				//�Ȃ����ɂ��邩�Ƃ����ƁA�v���C���[�̑O���������ƁA�O���̕ǂ����蔲���Ă��܂���
				//�J�����̓����蔻��Ō듮�삷�邽�߁B
				{
					//1.�v���C���[�̑O�������Ƌt�����̃x�N�g�� v0 �����߂�B
					CVector3 v0 = l_toCameraTarget;
					v0.Normalize();
					v0 *= -1.0f;
					//2. 1�ŋ��߂�v0�Ɖ��̒����_���王�_�Ɍ������x�N�g���Ƃ̓��ς����߂�B
					//�����_���v���C���[�̉E���Ɉڂ��B
					//   �Z ���̒����_(m_target)
					//�@��|
					//�@��| \
					//�@��P  �Z�@ //�������ŏI�I�Ȓ����_�I�I�I
					//  ��|   \
					//  ��|    \
					//  t |�[�[�[�Z���_
					t = v0.Dot(l_toCameraPos);
					//3.�v���C���[���牼�̒����_�܂ł̋�����t�ōŏI�I�Ȓ����_�܂ł̔䗦�����߂�B
					//2. 1�ŋ��߂�v0�Ɖ��̒����_���王�_�Ɍ������x�N�g���Ƃ̓��ς����߂�B
					//�����_���v���C���[�̉E���Ɉڂ��B
					//   �Z ���̒����_(m_target)
					//����||\   ����
					//����|| \�@�����I�I�I�@
					//�@�@P  �Z�@ //�������ŏI�I�Ȓ����_�I�I�I
					//  �@|   \
					//  �@|    \
					//  �@|�[�[�[�Z���_
					t = m_targetFromPlayer / t;
					//4.���Ƃ͐��`�ق��[��B
					m_target.Lerp(t, m_target, m_pos);
					m_springCamera.SetTarget(m_target);
				}


				//�X�V�B
				m_springCamera.Update();
			}
		}
	}
	//else {
	//	m_target = CVector3::Zero();		
	//	m_pos = CVector3::Zero();		
	//	m_pos.y = 5000.0f;

	//	//�����_�Ǝ��_��ݒ肷��B
	//	g_camera3D.SetTarget(m_target);
	//	g_camera3D.SetPosition(m_pos);

	//	//�X�V�B
	//	g_camera3D.Update();
	//}
}