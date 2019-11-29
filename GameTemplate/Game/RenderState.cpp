#include "stdafx.h"
#include "RenderState.h"

ID3D11DepthStencilState* DepthStencilState::disable = nullptr;			//!<���ׂĖ����B
ID3D11DepthStencilState* DepthStencilState::SceneRender = nullptr;		//!<3D���f����`�悷�鎞�̊�{�I�Ȑ[�x�X�e���V���X�e�[�g�B
ID3D11DepthStencilState* DepthStencilState::spriteRender = nullptr;		//!<2D�`�悷�鎞�̊�{�I�Ȑ[�x�X�e���V���X�e�[�g�B
ID3D11DepthStencilState* DepthStencilState::defferedRender = nullptr;	//!<�f�B�t�@�[�h�����_�����O���s����{�I�Ȑ[�x�X�e���V���X�e�[�g�B

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