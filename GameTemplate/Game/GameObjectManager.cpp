#include "stdafx.h"
#include "GameObjectManager.h"
#include "RenderState.h"

//GameObjectManagerクラスのインスタンス。
//GameObjectManager* g_goMgr = nullptr;

GameObjectManager::GameObjectManager()
{
	InitCamera();

	//メインとなるレンダリングターゲットを作成する。
	m_mainRenderTarget.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);

	//メインレンダリングターゲットに描かれた絵を
	//フレームバッファにコピーするためのスプライトを初期化する。
	m_copyMainRtToFrameBufferSprite.Init(
		m_mainRenderTarget.GetRenderTargetSRV(),
		FRAME_BUFFER_W,
		FRAME_BUFFER_H
	);
}

GameObjectManager::~GameObjectManager()
{
	if (m_frameBufferRenderTargetView != nullptr) {
		m_frameBufferRenderTargetView->Release();
	}
	if (m_frameBufferDepthStencilView != nullptr) {
		m_frameBufferDepthStencilView->Release();
	}
}

void GameObjectManager::Init()
{
	
}

void GameObjectManager::Update()
{
	//登録されているゲームオブジェクトの更新処理を呼ぶ。
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			go->Update();
		}
	}

	Render();

	//全てのゲームオブジェクトの1フレーム分の処理が終わってから、削除する。
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto it = m_goList[i].begin(); it != m_goList[i].end();) {
			if ((*it)->IsRequestDelete()) {
				//削除リクエストを受けているので削除。
				delete* it;
				it = m_goList[i].erase(it);
			}
			else {
				//リクエストを受けていないので。
				it++;
			}
		}
	}
}

void GameObjectManager::ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, RenderTarget* renderTarget, D3D11_VIEWPORT* viewport)
{
	ChangeRenderTarget(
		d3dDeviceContext,
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}
void GameObjectManager::ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport)
{
	ID3D11RenderTargetView* rtTbl[] = {
		renderTarget
	};
	//レンダリングターゲットの切り替え。
	d3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//ビューポートが指定されていたら、ビューポートも変更する。
		d3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void GameObjectManager::ForwordRender()
{
	//レンダリングターゲットをメインに変更する。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ChangeRenderTarget(d3dDeviceContext, &m_mainRenderTarget, &m_frameBufferViewports);
	//メインレンダリングターゲットをクリアする。
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_mainRenderTarget.ClearRenderTarget(clearColor);
	
	//
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			go->Draw();
		}
	}
}

void GameObjectManager::PostRender()
{
	//レンダリングターゲットをフレームバッファに戻す。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ChangeRenderTarget(
		d3dDeviceContext,
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);
	//ドロー
	m_copyMainRtToFrameBufferSprite.Draw();
	//デプスをクリア。
	//d3dDeviceContext->ClearDepthStencilView(m_frameBufferDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	d3dDeviceContext->OMSetDepthStencilState(DepthStencilState::spriteRender, 0);


	m_frameBufferRenderTargetView->Release();
	m_frameBufferDepthStencilView->Release();
}
void GameObjectManager::HudRender()
{
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			go->DrawHUD();
		}
	}
}
void GameObjectManager::Render()
{
	//フレームバッファのレンダリングターゲットをバックアップしておく。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);

	ForwordRender();

	PostRender();
	
	HudRender();
}
void GameObjectManager::InitCamera()
{
	g_camera3D.SetPosition({ 0.0f, 1000.0f, 2200.0f });
	g_camera3D.SetTarget({ 0.0f, 200.0f, 0.0f });
	g_camera3D.Update();
	g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	g_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();
}