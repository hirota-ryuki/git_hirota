#include "stdafx.h"
#include "system/system.h"
#include "Title.h"

//class Player;

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�J�����g�f�B���N�g����Assets�ɁB
	SetCurrentDirectory("Assets");

	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	//�J�������������B
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	
	//Title�N���X
	NewGO<Title>(GOPrio_Defalt);

	//�Q�[�����[�v�B
	while (DispatchWindowMessage() == true)
	{
		//�`��J�n�B
		g_graphicsEngine->BegineRender();
		//�Q�[���p�b�h�̍X�V�B	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		//�����G���W���̍X�V�B
		g_physics.Update();
					
		//GameObjectManager�̍X�V
		g_goMgr.Update();

		//�f�o�b�O���[�h
		g_physics.DebugDraw();
		
		//�`��I���B
		g_graphicsEngine->EndRender();
	}
}