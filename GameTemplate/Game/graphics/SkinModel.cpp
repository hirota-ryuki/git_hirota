#include "stdafx.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"
#include "SkinModelEffect.h"
#include <random>


SSpotLight SkinModel::m_spotLight[NUM_SPOT_LIG];
SPointLight SkinModel::m_pointLight[NUM_POINT_LIG];
SkinModel::~SkinModel()
{
	if (m_cb != nullptr) {
		//�萔�o�b�t�@������B
		m_cb->Release();
	}
	if (m_samplerState != nullptr) {
		//�T���v���X�e�[�g������B
		m_samplerState->Release();
	}
	//�@���}�b�v������B
	if (m_normalMapSRV != nullptr) {
		m_normalMapSRV->Release();
	}
}
void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
{
	//�X�P���g���̃f�[�^��ǂݍ��ށB
	InitSkeleton(filePath);

	//�萔�o�b�t�@�̍쐬�B
	InitConstantBuffer();

	//�f�B���N�V�������C�g�̏������B
	InitDirectionLight();
	//InitPointLight();
	//�T���v���X�e�[�g�̏������B
	InitSamplerState();

	//SkinModelDataManager���g�p����CMO�t�@�C���̃��[�h�B
	m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

	m_enFbxUpAxis = enFbxUpAxis;
}

//�f�B���N�V�������C�g�̏������B
void SkinModel::InitDirectionLight()
{
	m_light.directionLight.direction[0] = { 1.0f, -1.0f, 0.0f, 0.0f };
	m_light.directionLight.direction[0].Normalize();
	float color = 0.0f;
	m_light.directionLight.color[0] = { color, color, color, 0.2f };
	float light = 0.1f;
	m_light.ambientLight.Set(light, light, light);
	m_light.specPow = 5.0f;
}

//�|�C���g���C�g�̏������B
void SkinModel::InitPointLight()
{
	std::random_device seed_gen;
	std::mt19937 random(seed_gen());
	//for (int i = 0; i < POINTLIGHT_NUMBER; i++)  {
	//	m_pointLight[i].pos = CVector3::Zero();
	//	//m_pointLight[i].color = CVector3{ 0.5f, 0.0f, 0.0f };
	//	//m_pointLight[i].color.x = static_cast<float>(random() % 255) / 255.0f;
	//	//m_pointLight[i].color.y = static_cast<float>(random() % 255) / 255.0f;
	//	//m_pointLight[i].color.z = static_cast<float>(random() % 255) / 255.0f;
	//	m_pointLight[i].color = CVector3{ 1.0f,0.0f,0.0f };
	//	m_pointLight[i].range = 200.0f;
	//}
	for (auto& pt : m_pointLight) {
		pt.pos.x = 0.0f;
		pt.pos.y = 120.0f;
		pt.pos.z = 0.0f;
		//pt.pos = CVector3::Zero();
		pt.range = 100.0f;
		pt.color.x = 0.0f;
		pt.color.y = 0.0f;
		pt.color.z = 1.0f;
	}
}

void SkinModel::InitSpotLight()
{
	for (auto& st : m_spotLight) {
		st.pos.x = 0.0f;
		st.pos.y = 120.0f;
		st.pos.z = 0.0f;

		CVector3 dir = CVector3::AxisZ();
		st.direction.x = dir.x;
		st.direction.y = dir.y;
		st.direction.z = dir.z;
		st.direction.w = 0.0f;

		st.color.x = 0.0f;
		st.color.y = 1.0f;
		st.color.z = 0.0f;

		st.range = 500.0f;

	}
}

void SkinModel::InitSkeleton(const wchar_t* filePath)
{
	//�X�P���g���̃f�[�^��ǂݍ��ށB
	//cmo�t�@�C���̊g���q��tks�ɕύX����B
	std::wstring skeletonFilePath = filePath;
	//�����񂩂�.cmo�t�@�C���n�܂�ꏊ�������B
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmo�t�@�C����.tks�ɒu��������B
	skeletonFilePath.replace(pos, 4, L".tks");
	//tks�t�@�C�������[�h����B
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if ( result == false ) {
		//�X�P���g�����ǂݍ��݂Ɏ��s�����B
		//�A�j���[�V�������Ȃ����f���́A�X�P���g�����s�v�Ȃ̂�
		//�ǂݍ��݂Ɏ��s���邱�Ƃ͂���̂ŁA���O�o�͂����ɂ��Ă����B
#ifdef _DEBUG
		char message[256];
		sprintf(message, "tks�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
}
void SkinModel::InitConstantBuffer()
{
	//�쐬����o�b�t�@�̃T�C�Y��sizeof���Z�q�ŋ��߂�B
	int bufferSize = sizeof(SVSConstantBuffer);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//�O�ŃN���A�B
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
	bufferDesc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
																//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
	//�쐬�B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);

	//�����āA���C�g�p�̒萔�o�b�t�@���쐬�B
	//�쐬����o�b�t�@�̃T�C�Y��ύX���邾���B
	bufferSize = sizeof(SLight);
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);

	//�|�C���g���C�g�p�̒萔�o�b�t�@�B
	bufferSize = sizeof(SPointLight) * NUM_POINT_LIG;
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_pointlightCb);
	
	//�X�|�b�g���C�g�p�̒萔�o�b�t�@�B
	bufferSize = sizeof(SSpotLight) * NUM_SPOT_LIG;
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_spotlightCb);
}

void SkinModel::InitSamplerState()
{
	//�e�N�X�`���̃T���v�����O���@���w�肷�邽�߂̃T���v���X�e�[�g���쐬�B
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}
void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale)
{
	//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//���s�ړ��s����쐬����B
	transMatrix.MakeTranslation( position );
	//��]�s����쐬����B
	rotMatrix.MakeRotationFromQuaternion( rotation );
	rotMatrix.Mul(mBias, rotMatrix);
	//�g��s����쐬����B
	scaleMatrix.MakeScaling(scale);
	//���[���h�s����쐬����B
	//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
	//���Ԃ��ԈႦ���猋�ʂ��ς���B
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//�X�P���g���̍X�V�B
	m_skeleton.Update(m_worldMatrix);
}

void SkinModel::InitNormalMap(const wchar_t * filePath)
{
	//�t�@�C�������g���āA�e�N�X�`�������[�h���āAShaderResrouceView���쐬����B
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), filePath, 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_normalMapSRV
	);
}

void SkinModel::InitSpecMap(const wchar_t * filePath)
{
	//�t�@�C�������g���āA�e�N�X�`�������[�h���āAShaderResrouceView���쐬����B
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), filePath, 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_specMapSRV
	);
}

void SkinModel::Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix)
{
	//���C�g���񂷁B
	/*CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 2.0f);
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		qRot.Multiply(m_light.directionLight.direction[i]);
	}*/

	//���_��ݒ�B
	m_light.eyePos = g_camera3D.GetPosition();

	////���C�g���񂷁B
	//CQuaternion qRot2;
	//qRot2.SetRotationDeg(CVector3::AxisY(), g_pad[0].GetLStickXF());
	//qRot2.Multiply(m_light.directionLight.direction);
	
	//�X�y�L�����̍i��B
	/*if (g_pad[0].IsPress(enButtonLeft)) {
		m_light.specPow = max(0.0f, m_light.specPow - 0.5f);
	}
	if (g_pad[0].IsPress(enButtonRight)) {
		m_light.specPow = min(100.0f, m_light.specPow + 0.5f);
	}*/

	DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());

	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

	//�萔�o�b�t�@�̓��e���X�V�B
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_worldMatrix;
	vsCb.mProj = projMatrix;
	vsCb.mView = viewMatrix;
	//���C�g�J�����̃r���[�A�v���W�F�N�V�����s��𑗂�B
	vsCb.mLightView = ShadowMap::GetInstance().GetLighViewMatrix();
	vsCb.mLightProj = ShadowMap::GetInstance().GetLightProjMatrix();
	//�e�𗎂Ƃ����ǂ����B
	if (m_isShadowReciever == true) {
		vsCb.isShadowReciever = 1;
	}
	else {
		vsCb.isShadowReciever = 0;
	}
	//�@���}�b�v���g�p���邩�ǂ����̃t���O�𑗂�B
	if (m_normalMapSRV != nullptr) {
		vsCb.isHasNormalMap = true;
	}
	else {
		vsCb.isHasNormalMap = false;
	}
	//�X�y�L�����}�b�v���g�p���邩�ǂ����̃t���O�𑗂�B
	if (m_specMapSRV != nullptr) {
		vsCb.isHasSpecMap = true;
	}
	else {
		vsCb.isHasSpecMap = false;
	}
	//�萔�o�b�t�@���X�V�B
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//���C�g�p�̒萔�o�b�t�@���X�V�B
	d3dDeviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_light, 0, 0);
	//�|�C���g���C�g�p�̒萔�o�b�t�@���X�V�B
	d3dDeviceContext->UpdateSubresource(m_pointlightCb, 0, nullptr, &m_pointLight, 0, 0);
	//�X�|�b�g���C�g�p�̒萔�o�b�t�@���X�V�B
	d3dDeviceContext->UpdateSubresource(m_spotlightCb, 0, nullptr, &m_spotLight, 0, 0);
	//�萔�o�b�t�@��GPU�ɓ]���B
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
	d3dDeviceContext->PSSetConstantBuffers(2, 1, &m_pointlightCb);
	d3dDeviceContext->PSSetConstantBuffers(3, 1, &m_spotlightCb);
	//�T���v���X�e�[�g��ݒ�B
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//�{�[���s���GPU�ɓ]���B
	m_skeleton.SendBoneMatrixArrayToGPU();
	//�G�t�F�N�g�ɃN�G�����s���B
	m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
		auto modelMaterial = reinterpret_cast<SkinModelEffect*>(material);
		modelMaterial->SetRenderMode(renderMode);
		});
	if (m_normalMapSRV != nullptr) {
		//�@���}�b�v���ݒ肳��Ă���������W�X�^t2�ɐݒ肷��B
		d3dDeviceContext->PSSetShaderResources(2, 1, &m_normalMapSRV);
	}
	if (m_specMapSRV != nullptr) {
		//�X�y�L�����}�b�v���ݒ肳��Ă���������W�X�^t3�ɐݒ肷��B
		d3dDeviceContext->PSSetShaderResources(3, 1, &m_specMapSRV);
	}
	//�`��B
	m_modelDx->Draw(
		d3dDeviceContext,
		state,
		m_worldMatrix,
		viewMatrix,
		projMatrix
	);
}