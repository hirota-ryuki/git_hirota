#pragma once
#include "RenderTarget.h"
#include "util/Util.h"
#include <list>
#include "graphics/posteffect/PostEffect.h"

class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();
	/*!
	*@brief	ゲームオブジェクトの名前キーを作成。
	*/
	static unsigned int MakeGameObjectNameKey(const char* objectName)
	{
		static const unsigned int defaultNameKey = Util::MakeHash("Undefined");	//名前キー。
		unsigned int hash;
		if (objectName == nullptr) {
			hash = defaultNameKey;
		}
		else {
			hash = Util::MakeHash(objectName);
		}
		return hash;
	}
public:
	//シングルトン。
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
	/// GameObjectの開始関数と更新関数。
	/// </summary>
	void StartAndUpdate();
	/// <summary>
	/// GameObjectを消去する関数。
	/// </summary>
	void Delete();
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
	/// シャドウマップのレンダー。
	/// </summary>
	void ShadowMapRender();
	/// <summary>
	/// フォワードレンダリング(通常の描画だと考えてOK)。
	/// </summary>
	void ForwordRender();
	/// <summary>
	/// ポストエフェクト用のレンダー。
	/// </summary>
	void PostRender();
	/// <summary>
	/// HUD描画。
	/// </summary>
	void HudRender();
	/// <summary>
	/// フォントの描画。
	/// </summary>
	void FontRender();
	/// <summary>
	/// ゲームオブジェクトを追加。
	/// </summary>
	template <class T>
	T* NewGO(int prio, const char* objectName)
	{
		(void*)objectName;
		T* newObj = new T;
		m_goList[prio].push_back(newObj);
		unsigned int hash = MakeGameObjectNameKey(objectName);
		newObj->SetNameKey(hash);
		newObj->prio = prio;
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
			for (auto it = m_goList[i].begin(); it != m_goList[i].end(); it++) {
				if ((*it) == go) {
					if (!go->IsDelete()) {
						//見つかった。
						//削除リクエストを送る。
						go->SetDelete();
						m_DeleteGOList.emplace_back(go);
						go->OnDestroy();
					}
					return;
				}
			}
		}
	}
	template<class T>
	void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
	{
		unsigned int nameKey = Util::MakeHash(objectName);
		for (int i = 0; i < GOPrio_num; i++) {
			for (auto go : m_goList[i]) {
				if (go->GetNameKey() == nameKey) {
					//見つけた。
					T* p = dynamic_cast<T*>(go);
					if (p != nullptr) {
						if (func(p) == false) {
							//クエリ中断。
							return;
						}
					}
				}
			}
		}
	}

	/// <summary>
	/// メインレンダリングターゲットを取得。
	/// </summary>
	/// <returns>メインレンダリングターゲット。</returns>
	RenderTarget* GetMainRenderTarget(){
		return &m_mainRenderTarget;
	}

	RenderTarget m_mainRenderTarget;				//メインレンダリングターゲット。
	Sprite m_copyMainRtToFrameBufferSprite;			//メインレンダリングターゲットに描かれた絵をフレームバッファにコピーするためのスプライト。
	D3D11_VIEWPORT m_frameBufferViewports;			//フレームバッファのビューポート。
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//フレームバッファのレンダリングターゲットビュー。
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//フレームバッファのデプスステンシルビュー。

	//ベクタは新しい道を別で作りつつ配列を更新していくが、
	//リストは道を延長していくので今回の場合はリストのほうが良い。
	std::list< IGameObject* > m_goList[GOPrio_num];		//ゲームオブジェクトのリスト。
	std::list< IGameObject* > m_DeleteGOList;			//削除予定のゲームオブジェクトのリスト。

	PostEffect m_postEffect;				//ポストエフェクト。
};

//外部からアクセスするので、extern宣言も必要。
//extern GameObjectManager* g_goMgr;

template <class T>
static inline T* NewGO(int prio, const char* objectName = nullptr)
{
	return GameObjectManager::GetInstance().NewGO<T>(prio, objectName);
}
static inline void DeleteGO(IGameObject* go)
{
	return GameObjectManager::GetInstance().DeleteGO(go);
}

/*!
	*@brief	ゲームオブジェクトの検索のヘルパー関数。
	*@details
	* 同名のゲームオブジェクトに全てに対して、クエリを行いたい場合に使用してください。
	*@param[in]	objectName	ゲームオブジェクトの名前。
	*@param[in]	func		ゲームオブジェクトが見つかったときに呼ばれるコールバック関数。
	*/
template<class T>
static inline void QueryGOs(const char* objectName, std::function<bool(T* go)> func)
{
	return GameObjectManager::GetInstance().FindGameObjects<T>(objectName, func);
}

/*!
	*@brief	ゲームオブジェクトを名前指定で削除。
	*@details
	* 名前検索が行われるため遅いです。
	*@param[in]	objectName		削除するゲームオブジェクトの名前。
	*/
static inline void DeleteGOs(const char* objectName)
{
	QueryGOs<IGameObject>(objectName, [](auto go) {
		DeleteGO(go);
		return true;
		});
}