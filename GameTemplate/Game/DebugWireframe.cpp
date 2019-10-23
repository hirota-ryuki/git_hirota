#include "stdafx.h"
#include "DebugWireframe.h"


DebugWireframe::DebugWireframe()
{
}


DebugWireframe::~DebugWireframe()
{
}

void DebugWireframe::Prepare()
{
	////���_�o�b�t�@////
	//�\����
	//Description �Ӗ�:����
	D3D11_BUFFER_DESC desc{};
	//�g�����@�ǂݕ�:�䂣������
	desc.Usage = D3D11_USAGE_DEFAULT;
	//�T�C�Y �ǂݕ�:�o�C�g�E�B�Y
	desc.ByteWidth = sizeof(Vertex) * 2;
	//���_�p�ɂ���
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//0��CPU����A�N�Z�X�ł��Ȃ��B
	//0�Ȃ珑���Ȃ����ėǂ��B
	desc.CPUAccessFlags = 0;
	//�d�グ
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, nullptr, &m_vertexBuffer);
	////���_�I���b�b�b�I�I�I////
	
	////�萔�u�o�b�t�@////�v
	//�r���[�s��ƃv���W�F�N�V�����s��𑗂�p
	//�T�C�Y �ǂݕ�:�o�C�g�E�B�Y
	desc.ByteWidth = sizeof(CMatrix);
	//�萔�iCONSTANT�j�p�ɂ���
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//�d�グ
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, nullptr, &m_constantBuffer);
	////�萔�I���b�b�b�I�I�I////
	
	//�V�F�[�_�[�̐ݒ�
	//wchar_t����Ȃ�����L�͂���Ȃ�
	m_Vshader.Load("shader/DebugMode.fx", "VSMain", Shader::EnType::VS);
	m_Pshader.Load("shader/DebugMode.fx", "PSMain", Shader::EnType::PS);
}



void DebugWireframe::Context()
{
	//�f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	
	//���_�V�F�[�_�[���f�o�C�X�R���e�L�X�g�ɐݒ�
	//Shader�͐搶�̃��b�p�[�N���X
	//���ۂɎg��GetBody�Ŏ擾����m_shader��void*�Ȃ̂�
	//ID3D11VertexShader*�ɃL���X�g����K�v������
	d3dDeviceContext->VSSetShader((ID3D11VertexShader*) m_Vshader.GetBody(), nullptr, 0);

	//�s�N�Z���V�F�[�_�[���f�o�C�X�R���e�L�X�g�ɐݒ�
	//���_�V�F�[�_�[�Ɠ��l
	d3dDeviceContext->PSSetShader((ID3D11PixelShader*) m_Pshader.GetBody(), nullptr, 0);

	//�萔�o�b�t�@�̍X�V
	//mVP�̍X�V
	//�r���[�ƃv���W�F�N�V�����̊|���Z
	mVP.Mul(g_camera3D.GetViewMatrix(), g_camera3D.GetProjectionMatrix());
	//�萔�o�b�t�@���f�o�C�X�R���e�L�X�g�ɐݒ�
	d3dDeviceContext->UpdateSubresource(m_constantBuffer, 0, nullptr, &mVP, 0, 0);

	//���_�o�b�t�@�̍X�V

}

//1�t���[������drawLine�͐��̐������s��
void DebugWireframe::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{

}