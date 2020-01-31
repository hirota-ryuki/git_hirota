#include "stdafx.h"
#include "GameObjectManager.h"
#include "RenderState.h"

GameObjectManager::GameObjectManager()
{
	InitCamera();

	//���C���ƂȂ郌���_�����O�^�[�Q�b�g���쐬����B
	m_mainRenderTarget.Create(
		FRAME_BUFFER_W,
		FRAME_BUFFER_H,
		DXGI_FORMAT_R16G16B16A16_FLOAT
	);

	//���C�������_�����O�^�[�Q�b�g�ɕ`���ꂽ�G��
	//�t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g������������B
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
	//�o�^����Ă���Q�[���I�u�W�F�N�g�̍X�V�������ĂԁB
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			if (!go->IsStart()) {
				if (go->Start()) {
					go->StartEnd();
				}
			}
			else {
				go->Update();
			}
		}
	}
}

void GameObjectManager::Delete()
{
	//�폜
	for (auto* GO : m_DeleteGOList) {
		auto& goExecList = m_goList[GO->prio];
		//�Q�[���I�u�W�F�N�g���X�g����Y���̃I�u�W�F�N�g�̉ӏ���T����
		auto it = std::find(goExecList.begin(), goExecList.end(), GO);
		//�폜
		delete (*it);
		//�Q�[���I�u�W�F�N�g���X�g����폜
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
	//�����_�����O�^�[�Q�b�g�̐؂�ւ��B
	d3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//�r���[�|�[�g���w�肳��Ă�����A�r���[�|�[�g���ύX����B
		d3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void GameObjectManager::ForwordRender()
{
	//�����_�����O�^�[�Q�b�g�����C���ɕύX����B
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ChangeRenderTarget(d3dDeviceContext, &m_mainRenderTarget, &m_frameBufferViewports);
	//���C�������_�����O�^�[�Q�b�g���N���A����B
	float clearColor[] = { 0.0f, 0.0f, 255.0f, 1.0f };
	m_mainRenderTarget.ClearRenderTarget(clearColor);
	
	//�h���[
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			go->Draw();
		}
	}
}

void GameObjectManager::PostRender()
{
	//�����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɖ߂��B
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ChangeRenderTarget(
		d3dDeviceContext,
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);
	//�h���[
	m_copyMainRtToFrameBufferSprite.Draw();
	//�f�v�X���N���A�B
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
	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v���Ă����B
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//�r���[�|�[�g���o�b�N�A�b�v������Ă����B
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