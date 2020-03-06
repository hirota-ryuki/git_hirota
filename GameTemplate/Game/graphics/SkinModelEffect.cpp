#include "stdafx.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/SkinModelShaderConst.h"

void __cdecl ModelEffect::Apply(ID3D11DeviceContext* deviceContext)
{
	switch (m_renderMode) {
	case enRenderMode_Normal: {
		deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
		deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
		
		//deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &m_albedoTex);
		
		//�V�F�[�_�[���\�[�X�r���[����C�ɐݒ肷��B
		ID3D11ShaderResourceView* srvArray[] = {
			m_albedoTex,								//�A���x�h�e�N�X�`���B
			ShadowMap::GetInstance().GetShadowMapSRV()	//�V���h�E�}�b�v�B
		};
		deviceContext->PSSetShaderResources(0, 2, srvArray);
		
	}break;
	case enRenderMode_CreateShadowMap:
		//�V���h�E�}�b�v�����B
		deviceContext->VSSetShader((ID3D11VertexShader*)m_vsShadowMap.GetBody(), NULL, 0);
		deviceContext->PSSetShader((ID3D11PixelShader*)m_psShadowMap.GetBody(), NULL, 0);
		break;
	}
}