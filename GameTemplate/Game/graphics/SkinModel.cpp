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
		//定数バッファを解放。
		m_cb->Release();
	}
	if (m_samplerState != nullptr) {
		//サンプラステートを解放。
		m_samplerState->Release();
	}
	//法線マップを解放。
	if (m_normalMapSRV != nullptr) {
		m_normalMapSRV->Release();
	}
}
void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
{
	//スケルトンのデータを読み込む。
	InitSkeleton(filePath);

	//定数バッファの作成。
	InitConstantBuffer();

	//ディレクションライトの初期化。
	InitDirectionLight();
	//InitPointLight();
	//サンプラステートの初期化。
	InitSamplerState();

	//SkinModelDataManagerを使用してCMOファイルのロード。
	m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

	m_enFbxUpAxis = enFbxUpAxis;
}

//ディレクションライトの初期化。
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

//ポイントライトの初期化。
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
	//スケルトンのデータを読み込む。
	//cmoファイルの拡張子をtksに変更する。
	std::wstring skeletonFilePath = filePath;
	//文字列から.cmoファイル始まる場所を検索。
	int pos = (int)skeletonFilePath.find(L".cmo");
	//.cmoファイルを.tksに置き換える。
	skeletonFilePath.replace(pos, 4, L".tks");
	//tksファイルをロードする。
	bool result = m_skeleton.Load(skeletonFilePath.c_str());
	if ( result == false ) {
		//スケルトンが読み込みに失敗した。
		//アニメーションしないモデルは、スケルトンが不要なので
		//読み込みに失敗することはあるので、ログ出力だけにしておく。
#ifdef _DEBUG
		char message[256];
		sprintf(message, "tksファイルの読み込みに失敗しました。%ls\n", skeletonFilePath.c_str());
		OutputDebugStringA(message);
#endif
	}
}
void SkinModel::InitConstantBuffer()
{
	//作成するバッファのサイズをsizeof演算子で求める。
	int bufferSize = sizeof(SVSConstantBuffer);
	//どんなバッファを作成するのかをせてbufferDescに設定する。
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																//アライメントって→バッファのサイズが16の倍数ということです。
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
	bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
																//CPUアクセスが不要な場合は0。
	//作成。
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);

	//続いて、ライト用の定数バッファを作成。
	//作成するバッファのサイズを変更するだけ。
	bufferSize = sizeof(SLight);
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_lightCb);

	//ポイントライト用の定数バッファ。
	bufferSize = sizeof(SPointLight) * NUM_POINT_LIG;
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_pointlightCb);
	
	//スポットライト用の定数バッファ。
	bufferSize = sizeof(SSpotLight) * NUM_SPOT_LIG;
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_spotlightCb);
}

void SkinModel::InitSamplerState()
{
	//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
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
	//3dsMaxと軸を合わせるためのバイアス。
	CMatrix mBias = CMatrix::Identity();
	if (m_enFbxUpAxis == enFbxUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(CMath::PI * -0.5f);
	}
	CMatrix transMatrix, rotMatrix, scaleMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation( position );
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion( rotation );
	rotMatrix.Mul(mBias, rotMatrix);
	//拡大行列を作成する。
	scaleMatrix.MakeScaling(scale);
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算するように！
	//順番を間違えたら結果が変わるよ。
	m_worldMatrix.Mul(scaleMatrix, rotMatrix);
	m_worldMatrix.Mul(m_worldMatrix, transMatrix);

	//スケルトンの更新。
	m_skeleton.Update(m_worldMatrix);
}

void SkinModel::InitNormalMap(const wchar_t * filePath)
{
	//ファイル名を使って、テクスチャをロードして、ShaderResrouceViewを作成する。
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), filePath, 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_normalMapSRV
	);
}

void SkinModel::InitSpecMap(const wchar_t * filePath)
{
	//ファイル名を使って、テクスチャをロードして、ShaderResrouceViewを作成する。
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), filePath, 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_specMapSRV
	);
}

void SkinModel::Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix)
{
	//ライトを回す。
	/*CQuaternion qRot;
	qRot.SetRotationDeg(CVector3::AxisY(), 2.0f);
	for (int i = 0; i < NUM_DIRECTION_LIG; i++) {
		qRot.Multiply(m_light.directionLight.direction[i]);
	}*/

	//視点を設定。
	m_light.eyePos = g_camera3D.GetPosition();

	////ライトを回す。
	//CQuaternion qRot2;
	//qRot2.SetRotationDeg(CVector3::AxisY(), g_pad[0].GetLStickXF());
	//qRot2.Multiply(m_light.directionLight.direction);
	
	//スペキュラの絞り。
	/*if (g_pad[0].IsPress(enButtonLeft)) {
		m_light.specPow = max(0.0f, m_light.specPow - 0.5f);
	}
	if (g_pad[0].IsPress(enButtonRight)) {
		m_light.specPow = min(100.0f, m_light.specPow + 0.5f);
	}*/

	DirectX::CommonStates state(g_graphicsEngine->GetD3DDevice());

	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

	//定数バッファの内容を更新。
	SVSConstantBuffer vsCb;
	vsCb.mWorld = m_worldMatrix;
	vsCb.mProj = projMatrix;
	vsCb.mView = viewMatrix;
	//ライトカメラのビュー、プロジェクション行列を送る。
	vsCb.mLightView = ShadowMap::GetInstance().GetLighViewMatrix();
	vsCb.mLightProj = ShadowMap::GetInstance().GetLightProjMatrix();
	//影を落とすかどうか。
	if (m_isShadowReciever == true) {
		vsCb.isShadowReciever = 1;
	}
	else {
		vsCb.isShadowReciever = 0;
	}
	//法線マップを使用するかどうかのフラグを送る。
	if (m_normalMapSRV != nullptr) {
		vsCb.isHasNormalMap = true;
	}
	else {
		vsCb.isHasNormalMap = false;
	}
	//スペキュラマップを使用するかどうかのフラグを送る。
	if (m_specMapSRV != nullptr) {
		vsCb.isHasSpecMap = true;
	}
	else {
		vsCb.isHasSpecMap = false;
	}
	//定数バッファを更新。
	d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
	//ライト用の定数バッファを更新。
	d3dDeviceContext->UpdateSubresource(m_lightCb, 0, nullptr, &m_light, 0, 0);
	//ポイントライト用の定数バッファを更新。
	d3dDeviceContext->UpdateSubresource(m_pointlightCb, 0, nullptr, &m_pointLight, 0, 0);
	//スポットライト用の定数バッファを更新。
	d3dDeviceContext->UpdateSubresource(m_spotlightCb, 0, nullptr, &m_spotLight, 0, 0);
	//定数バッファをGPUに転送。
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(1, 1, &m_lightCb);
	d3dDeviceContext->PSSetConstantBuffers(2, 1, &m_pointlightCb);
	d3dDeviceContext->PSSetConstantBuffers(3, 1, &m_spotlightCb);
	//サンプラステートを設定。
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
	//ボーン行列をGPUに転送。
	m_skeleton.SendBoneMatrixArrayToGPU();
	//エフェクトにクエリを行う。
	m_modelDx->UpdateEffects([&](DirectX::IEffect* material) {
		auto modelMaterial = reinterpret_cast<SkinModelEffect*>(material);
		modelMaterial->SetRenderMode(renderMode);
		});
	if (m_normalMapSRV != nullptr) {
		//法線マップが設定されていたらをレジスタt2に設定する。
		d3dDeviceContext->PSSetShaderResources(2, 1, &m_normalMapSRV);
	}
	if (m_specMapSRV != nullptr) {
		//スペキュラマップが設定されていたらをレジスタt3に設定する。
		d3dDeviceContext->PSSetShaderResources(3, 1, &m_specMapSRV);
	}
	//描画。
	m_modelDx->Draw(
		d3dDeviceContext,
		state,
		m_worldMatrix,
		viewMatrix,
		projMatrix
	);
}