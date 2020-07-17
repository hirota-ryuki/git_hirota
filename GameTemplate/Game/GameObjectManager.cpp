#include "stdafx.h"
#include "GameObjectManager.h"
#include "RenderState.h"
#include "ShadowMap.h"

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
	StartAndUpdate();

	Render();

	Delete();
}

void GameObjectManager::StartAndUpdate()
{
	//登録されているゲームオブジェクトの更新処理を呼ぶ。
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			if (!go->IsStart()) {
				if (go->Start()) {
					go->StartEnd();
				}
			}
			else {
				if (!go->m_isDelete) {
					go->Update();
				}
			}
				
		}
	}
	//影。
	float p = 1000.0f;
	auto pos = g_camera3D.GetPosition();
	pos.x += p;
	pos.y += p;
	pos.z += p;
	//シャドウマップを更新。
	ShadowMap::GetInstance().UpdateFromLightTarget(
		pos,
		g_camera3D.GetPosition()
	);

	//ポストエフェクトの更新。
	m_postEffect.Update();
}

void GameObjectManager::Delete()
{
	//削除
	for (auto GO : m_DeleteGOList) {
		auto& goExecList = m_goList[GO->prio];
		//ゲームオブジェクトリストから該当のオブジェクトの箇所を探して
		auto it = std::find(goExecList.begin(), goExecList.end(), GO);
		//削除
		delete (*it);
		//ゲームオブジェクトリストから削除
		goExecList.erase(it);
	}
	m_DeleteGOList.clear();
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

void GameObjectManager::ShadowMapRender()
{
	///////////////////////////////////////////////
	//シャドウマップにレンダリング
	///////////////////////////////////////////////
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	//現在のレンダリングターゲットをバックアップしておく。
	ID3D11RenderTargetView* oldRenderTargetView;
	ID3D11DepthStencilView* oldDepthStencilView;
	d3dDeviceContext->OMGetRenderTargets(
		1, 
		&oldRenderTargetView, 
		&oldDepthStencilView
	);
	//ビューポートもバックアップを取っておく。
	unsigned int numViewport = 1;
	D3D11_VIEWPORT oldViewports;
	d3dDeviceContext->RSGetViewports(&numViewport, &oldViewports);

	//シャドウマップにレンダリング。	
	ShadowMap::GetInstance().RenderToShadowMap();

	//元に戻す。
	d3dDeviceContext->OMSetRenderTargets(
		1, 
		&oldRenderTargetView, 
		oldDepthStencilView
	);
	d3dDeviceContext->RSSetViewports(numViewport, &oldViewports);
	//レンダリングターゲットとデプスステンシルの参照カウンタを下す。
	oldRenderTargetView->Release();
	oldDepthStencilView->Release();
}

void GameObjectManager::ForwordRender()
{
	//レンダリングターゲットをメインに変更する。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ChangeRenderTarget(d3dDeviceContext, &m_mainRenderTarget, &m_frameBufferViewports);
	//メインレンダリングターゲットをクリアする。
	float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	m_mainRenderTarget.ClearRenderTarget(clearColor);
	
	//ドロー
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			if (!go->m_isDelete) {
				go->Draw();
			}
		}
	}
}

void GameObjectManager::PostRender()
{
	//レンダーステートの保存。
	g_graphicsEngine->SaveRenderState();
	//ポストエフェクトの描画。
	m_postEffect.Draw();
	//レンダーステートの読み込み。
	g_graphicsEngine->LoadRenderState();


	//レンダリングターゲットをフレームバッファに戻す。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ChangeRenderTarget(
		d3dDeviceContext,
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);
	//ドロー。
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
			if (!go->m_isDelete) {
				go->DrawHUD();
			}
		}
	}
}

void GameObjectManager::FontRender()
{
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			if (!go->m_isDelete) {
				go->DrawFont();
			}
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

	ShadowMapRender();

	ForwordRender();

	PostRender();
	
	HudRender();
	
	FontRender();
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