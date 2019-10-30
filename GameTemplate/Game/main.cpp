#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "level/Level.h"
#include "GameObjectManager.h"

//class Player;

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//カレントディレクトリをAssetsに。
	SetCurrentDirectory("Assets");

	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	//カメラを初期化。
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	
	//Gameクラス
	NewGO<Game>();

	//ゲームループ。
	while (DispatchWindowMessage() == true)
	{
		//描画開始。
		g_graphicsEngine->BegineRender();
		//ゲームパッドの更新。	
		for (auto& pad : g_pad) {
			pad.Update();
		}
		//物理エンジンの更新。
		g_physics.Update();
					
		//GameObjectManagerの更新
		g_goMgr.Update();

		//デバッグモード
		g_physics.DebugDraw();
		
		//描画終了。
		g_graphicsEngine->EndRender();
	}
}