#pragma once
#include "RenderTarget.h"

//NewGOの優先順位
enum prio {
	GOPrio_Defalut = 2,
	GOPrio_Sprite,
	GOPrio_num = 5,
};

class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();
public:
	//シングルトン
	static GameObjectManager& GetInstance()
	{
		static GameObjectManager instance;
		return instance;
	}
	/// <summary>
	/// 初期化。
	/// </summary>
	void Init();
	/// <summary>
	/// 更新。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画。
	/// </summary>
	void Render();
	/// <summary>
	/// カメラの初期化。
	/// </summary>
	void InitCamera();
	/// <summary>
	/// レンダリングターゲットの切り替え。
	/// </summary>
	/// <param name="d3dDeviceContext">デバイスコンテキスト</param>
	/// <param name="renderTarget">レンダーターゲット</param>
	/// <param name="viewport">ビューポート</param>
	void ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, RenderTarget * renderTarget, D3D11_VIEWPORT * viewport);
	/// <summary>
	/// レンダリングターゲットの切り替え。
	/// </summary>
	/// <param name="d3dDeviceContext">デバイスコンテキスト</param>
	/// <param name="renderTarget">レンダーターゲット</param>
	/// <param name="depthStensil">デプスステンシル</param>
	/// <param name="viewport">ビューポート</param>
	void ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, ID3D11RenderTargetView * renderTarget, ID3D11DepthStencilView * depthStensil, D3D11_VIEWPORT * viewport);
	/// <summary>
	/// フォワードレンダリング(通常の描画だと考えてOK)
	/// </summary>
	void ForwordRender();
	/// <summary>
	/// ポストエフェクト用のレンダー
	/// </summary>
	void PostRender();
	/// <summary>
	/// HUD描画。
	/// </summary>
	void HudRender();
	/// <summary>
	/// ゲームオブジェクトを追加。
	/// </summary>
	template <class T>
	T* NewGO(int prio)
	{
		T* newObj = new T;
		m_goList[prio].push_back(newObj);
		return newObj;
	}

	/// <summary>
	/// ゲームオブジェクトをリストから削除する。
	/// </summary>
	/// <param name="go">削除するゲームオブジェクト</param>
	void DeleteGO(IGameObject* go)
	{
		for (int i = 0; i < GOPrio_num; i++) {
			//リストから検索して、見つかったら削除する。
			for (auto it = m_goList[i].begin();
				it != m_goList[i].end();
				it++
				) {
				if ((*it) == go) {
					//見つかった。
					//削除リクエストを送る。
					go->RequestDelete();
					//削除できたので終わり。
					return;
				}
			}
		}
	}
private:

	RenderTarget m_mainRenderTarget;				//メインレンダリングターゲット。
	Sprite m_copyMainRtToFrameBufferSprite;			//メインレンダリングターゲットに描かれた絵をフレームバッファにコピーするためのスプライト。
	D3D11_VIEWPORT m_frameBufferViewports;			//フレームバッファのビューポート。
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//フレームバッファのレンダリングターゲットビュー。
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//フレームバッファのデプスステンシルビュー。

	std::vector< IGameObject* > m_goList[GOPrio_num];		//ゲームオブジェクトのリスト。

};

//外部からアクセスするので、extern宣言も必要。
//extern GameObjectManager* g_goMgr;

template <class T>
static inline T* NewGO(int prio)
{
	return GameObjectManager::GetInstance().NewGO<T>(prio);
}
static inline void DeleteGO(IGameObject* go)
{
	return GameObjectManager::GetInstance().DeleteGO(go);
}