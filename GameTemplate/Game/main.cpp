#include "stdafx.h"
#include "system/system.h"
#include "Title.h"

///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//メモ
	//シャドウマップはいつも一つ！シングルトン
	//順番↓
	//Update->シャドウマップのDraw->Draw

	//カレントディレクトリをAssetsに。
	SetCurrentDirectory("Assets");

	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");

	//カメラを初期化。
	g_camera3D.SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D.SetTarget({ 0.0f, 100.0f, 0.0f });
	g_camera3D.SetFar(10000.0f);
	
	//Titleクラス
	NewGO<Title>(GOPrio_Defalut);

	//デバッグモードのオンオフ
	bool m_isDebug = false;
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
		GameObjectManager::GetInstance().Update();

		
		if (g_pad[0].IsTrigger(enButtonSelect))
		{
			m_isDebug = !m_isDebug;
		}
		if (m_isDebug)
		{
			//デバッグモード
			g_physics.DebugDraw();
		}

		//描画終了。
		g_graphicsEngine->EndRender();
	}
}