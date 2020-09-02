#pragma once

#include "Skeleton.h"
#include "LightNumData.h"
//#include "Light.h"

/// <summary>
///	ポイントライト用の構造体。
/// </summary>
struct SPointLight {
	CVector4	pos;
	CVector4	color;
	float		range;
	float		pad[3];
};

/// <summary>
///	スポットライト用の構造体。
/// </summary>
struct SSpotLight {
	CVector4	pos;
	CVector4	color;
	CVector4	direction;
	float		range;
	float		pad[3];
};

/*!
*@brief	FBXの上方向。
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};



/*!
*@brief	スキンモデルクラス。
*/
class SkinModel
{
public:
	//メッシュが見つかったときのコールバック関数。
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	デストラクタ。
	*/
	~SkinModel();
	
	/*!
	*@brief	初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*@param[in] enFbxUpAxis		fbxの上軸。デフォルトはenFbxUpAxisZ。
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	/*!
	*@brief	モデルをワールド座標系に変換するためのワールド行列を更新する。
	*@param[in]	position	モデルの座標。
	*@param[in]	rotation	モデルの回転。
	*@param[in]	scale		モデルの拡大率。
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/// <summary>
	/// 法線マップの初期化処理。
	/// </summary>
	/// <param name="filePath">法線マップのファイルパス。</param>
	void InitNormalMap(const wchar_t* filePath);
	/// <summary>
	/// スペキュラマップの初期化処理。
	/// </summary>
	/// <param name="filePath">スペキュラマップのファイルパス。</param>
	void InitSpecMap(const wchar_t* filePath);
	/*!
	*@brief	ボーンを検索。
	*@param[in]		boneName	ボーンの名前。
	*@return	見つかったボーン。見つからなかった場合はnullptrを返します。
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	モデルを描画。
	*@param[in]	viewMatrix		カメラ行列。
	*  ワールド座標系の3Dモデルをカメラ座標系に変換する行列です。
	*@param[in]	projMatrix		プロジェクション行列。
	*  カメラ座標系の3Dモデルをスクリーン座標系に変換する行列です。
	*/
	void Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix );
	/*!
	*@brief	スケルトンの取得。
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	メッシュを検索する。
	*@param[in] onFindMesh		メッシュが見つかったときのコールバック関数
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}
	void SetLight(int light,CVector4 direction) {
		m_light.directionLight.direction[light] = direction;
		m_light.directionLight.direction[light].Normalize();
	}
	/*!
	*@brief	SRVのレジスタ番号。
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<ディフューズテクスチャ。
		enSkinModelSRVReg_BoneMatrix,				//!<ボーン行列。
	};
	CMatrix& GetWorldMatrix()
	{
		return m_worldMatrix;
	}
	/// <summary>
	/// シャドウレシーバーのフラグを設定する。
	/// </summary>
	/// <param name="flag">trueを渡すとシャドウレシーバーになる</param>
	/// <remarks>
	/// シャドウレシーバーとは影を落とされるオブジェクトのことです。
	/// シャドウキャスターによって生成された、シャドウマップを利用して
	/// 自身に影を落とします。
	/// オブジェクトがシャドウレシーバーかつシャドウキャスターになっている場合は
	/// セルフシャドウ(自分の影が自分に落ちる)を行うことができます。
	/// </remarks>
	void SetShadowReciever(bool flag)
	{
		m_isShadowReciever = flag;
	}
	/// <summary>
	/// ディレクションライトの方向をセット。
	/// </summary>
	/// <param name="LDir">方向</param>
	void SetDirectionLightDirection(CVector4 LDir ) {
		m_light.directionLight.direction[0] = LDir;
		m_light.directionLight.direction[0].Normalize();
	}
	/// <summary>
	///ディレクションライトの色をセット。
	/// </summary>
	/// <param name="LColor">色</param>
	void SetDirectionLightColor(CVector4 LColor ) {
		m_light.directionLight.color[0] = LColor;
	}
	/// <summary>
	/// スペキュラの累乗数をセット。
	/// </summary>
	/// <param name="spec">累乗数</param>
	void SetSpec(float spec ) {
		m_light.specPow = spec;

	}
	/// <summary>
	/// アンビエントライトをセット。
	/// </summary>
	/// <param name="spec">アンビエントライト</param>
	void SetAmbientLight(CVector3 ambientLight) {
		m_light.ambientLight = ambientLight;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="spotlist"></param>
	void SetPointLight(SPointLight pointlist[NUM_POINT_LIG]) {
		for (int i = 0; i < NUM_POINT_LIG; i++) {
			m_pointLight[i] = pointlist[i];
		}
	}
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="spotlist"></param>
	void SetSpotLight(SSpotLight spotlist[NUM_SPOT_LIG]) {
		for (int i = 0; i < NUM_SPOT_LIG; i++) {
			m_spotLight[i] = spotlist[i];
		}
	}
	
private:
	/*!
	*@brief	サンプラステートの初期化。
	*/
	void InitSamplerState();
	/*!
	*@brief	定数バッファの作成。
	*/
	void InitConstantBuffer();
	/// <summary>
	/// ディレクションライトの初期化。
	/// </summary>
	void InitDirectionLight();
	/// <summary>
	/// ポイントライトの初期化。
	/// </summary>
	void InitPointLight();
	/// <summary>
	/// スポットライトの初期化。
	/// </summary>
	void InitSpotLight();
	/*!
	*@brief	スケルトンの初期化。
	*@param[in]	filePath		ロードするcmoファイルのファイルパス。
	*/
	void InitSkeleton(const wchar_t* filePath);
	
private:
	//定数バッファ。
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CMatrix mLightView;		//ライトビュー行列。
		CMatrix mLightProj;		//ライトプロジェクション行列。
		int isShadowReciever;	//シャドウレシーバーのフラグ。
		int isHasNormalMap;		//法線マップを保持していかどうか。
		int isHasSpecMap;		//法線マップを保持していかどうか。
	};
	/*!
	*@brief	ディレクションライト。
	*/
	struct SDirectionLight {
		CVector4 direction[NUM_DIRECTION_LIG];		//ライトの方向。
		CVector4 color[NUM_DIRECTION_LIG];			//ライトのカラー。
	};
	
	/// <summary>
	/// ライト構造体。
	/// </summary>
	struct SLight {
		SDirectionLight		directionLight;		//ディレクションライト
		CVector3			eyePos;				//視点の座標。
		float				specPow;			//鏡面反射の絞り。
		CVector3			ambientLight;		//環境光。
	};

	
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//FBXの上方向。
	ID3D11Buffer*		m_cb = nullptr;					//定数バッファ。
	Skeleton			m_skeleton;						//スケルトン。
	CMatrix				m_worldMatrix;					//ワールド行列。
	DirectX::Model*		m_modelDx;						//DirectXTKが提供するモデルクラス。
	ID3D11SamplerState* m_samplerState = nullptr;		//サンプラステート。
	//ライト
	ID3D11Buffer*		m_lightCb = nullptr;			//ライト用の定数バッファ。
	SLight				m_light;						//ライトクラス。
	//ポイントライト
	ID3D11Buffer*		m_pointlightCb = nullptr;		//ポイントライト用の定数バッファ。
	static SPointLight	m_pointLight[NUM_POINT_LIG];				//ポイントライトクラス。
	//スポットライト
	ID3D11Buffer*		m_spotlightCb = nullptr;		//スポットライト用の定数バッファ。
	static SSpotLight	m_spotLight[NUM_SPOT_LIG];		//スポットライトクラス。
	//影。
	bool				m_isShadowReciever = false;		//シャドウレシーバーのフラグ。
	//法線マップ。
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;	//法線マップのSRV。
	//スペキュラマップ。
	ID3D11ShaderResourceView* m_specMapSRV = nullptr;	//スペキュラマップのSRV。
};