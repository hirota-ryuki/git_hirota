#include "stdafx.h"
#include "Sprite.h"


//���_�\���́B
struct Vertex {
	CVector4	pos;		//���W�B
	CVector2	uv;			//UV���W�B
};

/// <summary>
/// 2D�`��Ŏg�p����萔�o�b�t�@�\���́B
/// </summary>
struct SSpriteCB {
	CMatrix mWVP;		//���[���h�~�r���[�~�v���W�F�N�V�����s��B
	float mAlpha;
};

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
	if (m_vertexBuffer != nullptr) {
		m_vertexBuffer->Release();
	}
	if (m_indexBuffer != nullptr) {
		m_indexBuffer->Release();
	}
	if (m_cbGPU != nullptr) {
		m_cbGPU->Release();
	}
	if (m_texture != nullptr) {
		m_texture->Release();
	}
	if (m_samplerState != nullptr) {
		m_samplerState->Release();
	}
}

void Sprite::Init(const wchar_t* textureFilePath, float w, float h)
{
	//�V�F�[�_�[�����[�h�B
	LoadShader();
	//���_�o�b�t�@���쐬�B
	CreateVertexBuffer(w, h);
	//�C���f�b�N�X�o�b�t�@���쐬�B
	CreateIndexBuffer();
	//�萔�o�b�t�@���쐬�B
	CreateConstantBuffer();
	//�T���v���X�e�[�g���쐬�B
	CreateSamplerState();
	//�e�N�X�`�������[�h�B
	LoadTexture(textureFilePath);
}

void Sprite::Init(ID3D11ShaderResourceView* srv, float w, float h)
{
	//���ʂ̏������������Ăяo���B
	InitCommon(w, h);
	m_texture = srv;
	m_texture->AddRef();	//�Q�ƃJ�E���^�𑝂₷�B
}

void Sprite::InitCommon(float w, float h)
{
	m_size.x = w;
	m_size.y = h;
	//���_�o�b�t�@�̏������B
	InitVertexBuffer(w, h);
	//�C���f�b�N�X�o�b�t�@�̏������B
	InitIndexBuffer();
	//�T���v���X�e�[�g�̏������B
	InitSamplerState();
	//�V�F�[�_�[�̃��[�h�B
	m_vs.Load("shader/sprite.fx", "VSMain", Shader::EnType::VS);
	m_ps.Load("shader/sprite.fx", "PSMain", Shader::EnType::PS);

	//�萔�o�b�t�@���������B
	InitConstantBuffer();

}

void Sprite::InitVertexBuffer(float w, float h)
{
	//��Œ�`�������_���g�p���Ē��_�o�b�t�@���쐬����B
	//���_�o�b�t�@���쐬���邽�߂ɂ�D3D11_BUFFER_DESC��D3D11_SUBRESOURCE_DATA��ݒ肷��K�v������B
	float halfW = w * 0.5f;
	float halfH = h * 0.5f;
	SVertex vertex[4] = {
		//���_�P
		{
			//���W�@position[4]
			-halfW,  -halfH, 0.0f, 1.0f,
			//UV���W uv[2]
			0.0f, 1.0f
		},
		//���_�Q
		{
			//���W�@position[4]
			halfW, -halfH, 0.0f, 1.0f,
			//UV���W uv[2]
			1.0f, 1.0f
		},
		//���_�R
		{
			//���W�@position[4]
			-halfW,  halfH, 0.0f, 1.0f,
			//UV���W uv[2]
			0.0f, 0.0f
		},
		//���_�S
		{
			//���W�@position[4]
			halfW,  halfH, 0.0f, 1.0f,
			//UV���W uv[2]
			1.0f, 0.0f
		},
	};
	//��Œ�`�������_���g�p���Ē��_�o�b�t�@���쐬����B
	//���_�o�b�t�@���쐬���邽�߂ɂ�D3D11_BUFFER_DESC��D3D11_SUBRESOURCE_DATA��ݒ肷��K�v������B
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//�\���̂�0�ŏ���������B
	bd.Usage = D3D11_USAGE_DEFAULT;				//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@�B
												//��肠������D3D11_USAGE_DEFAULT�ł悢�B
	bd.ByteWidth = sizeof(vertex);				//���_�o�b�t�@�̃T�C�Y�B���_�̃T�C�Y�~���_���ƂȂ�B
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���ꂩ��쐬����o�b�t�@�����_�o�b�t�@�ł��邱�Ƃ��w�肷��B

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	//���_�o�b�t�@�̍쐬�B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
}

void Sprite::InitIndexBuffer()
{
	//���ꂪ�C���f�b�N�X�B���_�ԍ��B
	int index[6] = {
		0,1,2,		//�O�p�`���
		2,1,3		//�O�p�`���
	};
	//��Œ�`�����C���f�b�N�X���g�p���ăC���f�b�N�X�o�b�t�@���쐬����B
	//�C���f�b�N�X�o�b�t�@�̍쐬�����_�o�b�t�@�Ɠ��l�ɁA
	//D3D11_BUFFER_DESC��D3D11_SUBRESOURCE_DATA��ݒ肷��K�v������B
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//�\���̂�0�ŏ���������B
	bd.Usage = D3D11_USAGE_DEFAULT;				//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@�B
												//��肠������D3D11_USAGE_DEFAULT�ł悢�B
	bd.ByteWidth = sizeof(index);				//�C���f�b�N�X�o�b�t�@�̃T�C�Y�B
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//���ꂩ��쐬����o�b�t�@���C���f�b�N�X�o�b�t�@�ł��邱�Ƃ��w�肷��B

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = index;

	//�C���f�b�N�X�o�b�t�@�̍쐬�B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(
		&bd, &InitData, &m_indexBuffer
	);
}

void Sprite::InitSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}

void Sprite::InitConstantBuffer()
{
	D3D11_BUFFER_DESC desc;

	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = (((sizeof(ConstantBuffer) - 1) / 16) + 1) * 16;	//16�o�C�g�A���C�����g�ɐ؂肠����B
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, NULL, &m_cb);
}

void Sprite::LoadShader()
{
	//�V�F�[�_�[�����[�h����B
	m_ps.Load("shader/sprite_hud.fx", "PSMain", Shader::EnType::PS);
	m_vs.Load("shader/sprite_hud.fx", "VSMain", Shader::EnType::VS);
}

void Sprite::CreateConstantBuffer()
{
	int bufferSize = sizeof(SSpriteCB);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC desc = {0};
	desc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
	desc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�萔�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
															//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
															//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
	desc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
															//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
	//�쐬�B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&desc, NULL, &m_cbGPU);
}

void Sprite::CreateVertexBuffer( float w, float h)
{
	float halfW = w * 0.5f;
	float halfH = h * 0.5f;

	//���_�o�b�t�@�̌��f�[�^�B
	//�����Ŏw�肳�ꂽ�c�A���̃T�C�Y�̔|���B
	Vertex vertices[] = {
		{
			CVector4(-halfW, -halfH, 0.0f, 1.0f),
			CVector2(0.0f, 1.0f),
		},
		{
			CVector4(halfW, -halfH, 0.0f, 1.0f),
			CVector2(1.0f, 1.0f),
		},
		{
			CVector4(-halfW, halfH, 0.0f, 1.0f),
			CVector2(0.0f, 0.0f)
		},
		{
			CVector4(halfW, halfH, 0.0f, 1.0f),
			CVector2(1.0f, 0.0f)
		}
	};

	//���_�o�b�t�@���쐬���邽�߂̏���ݒ肷��B
	D3D11_BUFFER_DESC desc = { 0 };					//�\���̂̃����o��0�ŏ���������B
	desc.Usage = D3D11_USAGE_DEFAULT;				//�C�ɂ��Ȃ��Ă悢�B
	desc.StructureByteStride = sizeof(Vertex);		//�\���̃o�b�t�@�̂P�v�f�̃T�C�Y�B(���_����̃T�C�Y�ł��B)
	desc.ByteWidth = sizeof(Vertex) * 4;			//���_�o�b�t�@�̃T�C�Y�B
													//�l�p�`�̃|���S����`�悷��̂ŁA�S���_�K�v�B
													//�Ȃ̂ŁA��̒��_�̃T�C�Y�~�S�̃T�C�Y���K�v�ƂȂ�B
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//�쐬����o�b�t�@�����_�o�b�t�@�ł��邱�Ƃ��w�肷��B
	desc.CPUAccessFlags = 0;						//CPU���璸�_�o�b�t�@�ɃA�N�Z�X���邩�̃t���O�B
													//�ҏW������͂��Ȃ��̂ŁA�O�ɂ��Ă����B
	
	//�����āA���f�[�^�̏������B
	//���f�[�^���Ȃ��Ȃ炱��͂���Ȃ��B
	//����͕K�v�B
	D3D11_SUBRESOURCE_DATA initData = { 0 };		//�\���̂̃����o��0�ŏ���������B
	initData.pSysMem = vertices;					//���f�[�^�̃A�h���X�����B

	//�����܂Őݒ肵�Ă��������g���āAVRAM��ɒ��_�o�b�t�@���쐬����B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&desc, &initData, &m_vertexBuffer);
}

void Sprite::CreateIndexBuffer()
{
	//�C���f�b�N�X�o�b�t�@�̌��f�[�^�B
	short indices[] = { 0,1,2,3 };

	//�C���f�b�N�X�o�b�t�@����邽�߂̏���ݒ肷��B
	D3D11_BUFFER_DESC desc = { 0 };					//�\���̂̃����o��0�ŏ���������B
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.StructureByteStride = 2;					//�C���f�b�N�X����̃T�C�Y�Bshort�Ȃ̂�2�o�C�g�B
	desc.ByteWidth = sizeof( indices );				//�C���f�b�N�X�o�b�t�@�̃T�C�Y�B
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;		//�쐬����o�b�t�@���C���f�b�N�X�o�b�t�@�ł��邱�Ƃ��w�肷��B
	desc.CPUAccessFlags = 0;						//CPU���璸�_�o�b�t�@�ɃA�N�Z�X���邩�̃t���O�B
													//�ҏW������͂��Ȃ��̂ŁA�O�ɂ��Ă����B

	D3D11_SUBRESOURCE_DATA initData = {0};			//�\���̂̃����o��0�ŏ���������B
	initData.pSysMem = indices;						//���f�[�^�̃A�h���X��������B

	//�����܂Őݒ肵�Ă��������g���āAVRAM��ɃC���f�b�N�X�o�b�t�@���쐬����B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&desc, &initData, &m_indexBuffer);
}

void Sprite::CreateSamplerState()
{
	//�e�N�X�`���̃T���v�����O���@���w�肷�邽�߂̃T���v���X�e�[�g���쐬�B
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	//Clamp+�o�C���j�A�t�B���^��������T���v���X�e�[�g���쐬����B
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}
void Sprite::LoadTexture(const wchar_t* textureFIlePath)
{
	//D3D�f�o�C�X���擾�B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();

	DirectX::CreateDDSTextureFromFileEx(
		d3dDevice, 
		textureFIlePath, 
		0,
		D3D11_USAGE_DEFAULT, 
		D3D11_BIND_SHADER_RESOURCE, 
		0, 
		0,
		false, 
		nullptr, 
		&m_texture );
}

void Sprite::UpdateWorldMatrix(CVector3 pos, CQuaternion rot, CVector3 scale)
{
	//���[���h�s����v�Z����
	CMatrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(pos);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world.Mul( mScale, mRot );
	m_world.Mul( m_world, mTrans );
}
void Sprite::Draw(CMatrix mView, CMatrix mProj)
{
	//�f�o�C�X�R���e�L�X�g�����������Ă���B
	auto deviceContext = g_graphicsEngine->GetD3DDeviceContext();

	//�萔�o�b�t�@���X�V�B
	SSpriteCB cb;
	//���[���h�~�r���[�~�v���W�F�N�V�����s����v�Z�B
	cb.mWVP.Mul( m_world, mView );
	cb.mWVP.Mul( cb.mWVP, mProj ) ;
	float alpha = 0.0f;
	cb.mAlpha = alpha;

	//�萔�o�b�t�@�̓��e�����C������������VRAM�ɃR�s�[�B
	deviceContext->UpdateSubresource(m_cbGPU, 0, nullptr, &cb, 0, 0);
	//�萔�o�b�t�@�����W�X�^b0�Ƀo�C���h����B
	deviceContext->VSSetConstantBuffers(0, 1, &m_cbGPU);
	//�e�N�X�`�������W�X�^t0�Ƀo�C���h����B
	deviceContext->PSSetShaderResources(0, 1, &m_texture);
	//�T���v���X�e�[�g�����W�X�^s0�Ƀo�C���h����B
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	//���_�o�b�t�@��ݒ�B
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@��ݒ�B
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//���_���C�A�E�g��ݒ�B
	deviceContext->IASetInputLayout(m_vs.GetInputLayout());
	//���_�V�F�[�_�[��ݒ�B
	deviceContext->VSSetShader( (ID3D11VertexShader *)m_vs.GetBody(), nullptr, 0);
	//�s�N�Z���V�F�[�_��ݒ�B
	deviceContext->PSSetShader((ID3D11PixelShader *)m_ps.GetBody(), nullptr, 0);
	//�v���~�e�B�u�̃g�|���W�[��ݒ�B
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�����܂Őݒ肵�����e�Ńh���[
	deviceContext->DrawIndexed(4, 0, 0);
}

void Sprite::Draw()
{
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

	unsigned int vertexSize = sizeof(SVertex);	//���_�̃T�C�Y�B
	unsigned int offset = 0;
	d3dDeviceContext->IASetVertexBuffers(	//���_�o�b�t�@��ݒ�B
		0,					//StartSlot�ԍ��B����0�ł����B
		1,					//�o�b�t�@�̐��B����1�ł����B
		&m_vertexBuffer,	//���_�o�b�t�@�B
		&vertexSize,		//���_�̃T�C�Y�B
		&offset				//�C�ɂ��Ȃ��Ă悢�B
	);
	d3dDeviceContext->IASetIndexBuffer(	//�C���f�b�N�X�o�b�t�@��ݒ�B
		m_indexBuffer,			//�C���f�b�N�X�o�b�t�@�B
		DXGI_FORMAT_R32_UINT,	//�C���f�b�N�X�̃t�H�[�}�b�g�B
								//�����32bit�Ȃ̂ŁADXGI_FORMAT_R32_UINT�ł����B
		0						//�I�t�Z�b�g0�ł����B
	);
	//�V�F�[�_�[��ݒ�B
	d3dDeviceContext->VSSetShader(
		(ID3D11VertexShader*)m_vs.GetBody(),
		nullptr,
		0
	);
	d3dDeviceContext->PSSetShader(
		(ID3D11PixelShader*)m_ps.GetBody(),
		nullptr,
		0
	);
	d3dDeviceContext->IASetInputLayout(m_vs.GetInputLayout());
	//�e�N�X�`����ݒ�B
	d3dDeviceContext->PSSetShaderResources(0, 1, &m_texture);
	//�T���v���X�e�[�g��ݒ�B
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//���[���h�r���[�v���W�F�N�V�����s����쐬�B
	ConstantBuffer cb;
	cb.WVP = m_world;
	cb.WVP.Mul(cb.WVP, g_camera2D.GetViewMatrix());
	cb.WVP.Mul(cb.WVP, g_camera2D.GetProjectionMatrix());
	cb.alpha = m_alpha;

	d3dDeviceContext->UpdateSubresource(m_cb, 0, NULL, &cb, 0, 0);
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//�v���~�e�B�u�̃g�|���W�[��
	//�g���C�A���O���X�g���b�v��ݒ肷��B
	d3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dDeviceContext->DrawIndexed(	//�`�施�߁B
		6,				//�C���f�b�N�X���B
		0,				//�J�n�C���f�b�N�X�ԍ��B0�ł����B
		0				//�J�n���_�ԍ��B0�ł����B
	);
}