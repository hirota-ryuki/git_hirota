#pragma once
/*!
 *@brief	グラフィックスエンジン。
 */
#include "RenderTarget.h"

 /// <summary>
 /// レンダリングモード。
 /// </summary>
enum EnRenderMode {
	enRenderMode_Invalid,			//不正なレンダリングモード。
	enRenderMode_CreateShadowMap,	//シャドウマップ生成。
	enRenderMode_Normal,			//通常レンダリング。
	enRenderMode_Num,				//レンダリングモードの数。
};

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	/*!
	 *@brief	初期化。
	 *@param[in]	hWnd		ウィンドウハンドル。
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	解放。
	 */
	void Release();
	/*!
	 *@brief	D3D11デバイスをセット。
	 */
	void SetD3DDevice(ID3D11Device* pd3d) 
	{
		m_pd3dDevice = pd3d;
	}
	/*!
	 *@brief	D3D11デバイスを取得。
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11デバイスコンテキストを取得。
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	/// <summary>
	/// 現在のレンダーステートを保存する関数。
	/// </summary>
	void SaveRenderState()
	{
		m_pd3dDeviceContext->OMGetDepthStencilState(&m_depthStencilState, &m_depthStencilStateRef);
		m_pd3dDeviceContext->OMGetBlendState(&m_blendState, m_blendFactor, &m_blendSampleMask);
		m_pd3dDeviceContext->RSGetState(&m_saveRasterizerState);
	}
	/// <summary>
	/// 保存したステートをロードする関数。
	/// </summary>
	void LoadRenderState()
	{
		m_pd3dDeviceContext->OMSetDepthStencilState(m_depthStencilState, m_depthStencilStateRef);
		m_pd3dDeviceContext->OMSetBlendState(m_blendState, m_blendFactor, m_blendSampleMask);
		m_pd3dDeviceContext->RSSetState(m_saveRasterizerState);
	}
	/*!
	 *@brief	描画開始。
	 */
	void BegineRender();
	/*!
	 *@brief	描画終了。
	 */
	void EndRender();

	////フォント////
	/*!
	*@brief	SpriteBatchの取得。
	*@details
	* ゲーム層では使用しないように。
	*/
	/// <summary>
	/// SpriteBatchの取得
	/// </summary>
	/// <remarks>
	/// エンジン内部で使用されます。ゲーム層では使用しないように。
	/// </remarks>
	/// <returns></returns>
	DirectX::SpriteBatch* GetSpriteBatch() const
	{
		return m_spriteBatch.get();
	}
	/// <summary>
	/// SpriteFontの取得。
	/// </summary>
	/// <remarks>
	/// エンジン内部で使用されます。ゲーム層では使用しないように。
	/// </remarks>
	/// <returns></returns>
	DirectX::SpriteFont* GetSpriteFont() const
	{
		return m_spriteFont.get();
	}
	/// <summary>
		/// フレームバッファの幅を取得。
		/// </summary>
	int GetFrameBufferWidth() const
	{
		return m_frameBufferWidth;
	}
	/// <summary>
	/// フレームバッファの高さを取得。
	/// </summary>
	int GetFrameBufferHeight() const
	{
		return m_frameBufferHeight;
	}
	/// <summary>
		/// 2D空間のスクリーンの幅を取得。
		/// </summary>
	int Get2DSpaceScreenWidth() const
	{
		return m_2dSpaceScreenWidth;
	}
	/// <summary>
	/// 2D空間のスクリーンの高さを取得。
	/// </summary>
	int Get2DSpaceScreenHeight() const
	{
		return m_2dSpaceScreenHeight;
	}
	/// <summary>
	/// レンダリングターゲットの切り替え。
	/// </summary>
	/// <param name="renderTarget">レンダリングターゲット</param>
	/// <param name="viewport">ビューポート</param>
	void ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
private:
	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D デバイスのターゲットとなる機能セット。
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11デバイス。
	IDXGISwapChain*			m_pSwapChain = NULL;		//スワップチェイン。
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11デバイスコンテキスト。
	ID3D11RenderTargetView* m_backBuffer = NULL;		//バックバッファ。
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//ラスタライザステート。
	ID3D11Texture2D*		m_depthStencil = NULL;		//デプスステンシル。
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//デプスステンシルビュー。

	////保存用ステート。////
	ID3D11DepthStencilState*	m_depthStencilState = nullptr;	//!<現在のデプスステンシルステート。
	UINT						m_depthStencilStateRef;
	ID3D11RasterizerState*		m_saveRasterizerState = nullptr;//!<現在のラスタライザステート。
	ID3D11BlendState*			m_blendState = nullptr;			//!<現在のブレンドステート。
	float						m_blendFactor[4];
	UINT						m_blendSampleMask;


	////フォント////
	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;				//!<スプライトバッチ。
	std::unique_ptr<DirectX::SpriteFont>	m_spriteFont;				//!<スプライトフォント。
	int						m_frameBufferWidth = 0;						//!<フレームバッファの幅。これが内部解像度。
	int						m_frameBufferHeight = 0;					//!<フレームバッファの高さ。これが内部解像度。
	const int				m_2dSpaceScreenWidth = 1280;				//!<2D空間のスクリーンの幅。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
	const int				m_2dSpaceScreenHeight = 720;				//!<2D空間のスクリーンの高さ。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
	
};

extern GraphicsEngine* g_graphicsEngine;			//グラフィックスエンジン