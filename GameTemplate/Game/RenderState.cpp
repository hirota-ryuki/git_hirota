#include "stdafx.h"
#include "RenderState.h"

ID3D11DepthStencilState* DepthStencilState::disable = nullptr;			//!<すべて無効。
ID3D11DepthStencilState* DepthStencilState::SceneRender = nullptr;		//!<3Dモデルを描画する時の基本的な深度ステンシルステート。
ID3D11DepthStencilState* DepthStencilState::spriteRender = nullptr;		//!<2D描画する時の基本的な深度ステンシルステート。
ID3D11DepthStencilState* DepthStencilState::defferedRender = nullptr;	//!<ディファードレンダリングを行う基本的な深度ステンシルステート。

void DepthStencilState::Init(GraphicsEngine & ge)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	ID3D11Device* pd3d = ge.GetD3DDevice();
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.StencilEnable = false;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	pd3d->CreateDepthStencilState(&desc, &SceneRender);

	desc.DepthEnable = false;
	pd3d->CreateDepthStencilState(&desc, &defferedRender);

	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pd3d->CreateDepthStencilState(&desc, &disable);
	pd3d->CreateDepthStencilState(&desc, &spriteRender);
}