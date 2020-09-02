/*!
 * @brief	モデルシェーダー。
 */
//#include "..\..\graphics\LightNumData.h"
#include "../../graphics\LightNumData.h"


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
Texture2D<float4> albedoTexture : register(t0);	//アルベドテクスチャ。
Texture2D<float4> g_shadowMap : register(t1);	//シャドウマップ。
Texture2D<float4> g_normalMap : register(t2);	//法線マップ。
Texture2D<float4> g_specMap : register(t3);		//スペキュラマップ。

//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float4x4 mLightView;	//ライトビュー行列。
	float4x4 mLightProj;	//ライトプロジェクション行列。
	int isShadowReciever;	//シャドウレシーバーフラグ。
	int isHasNormalMap;		//法線マップあるかどうか。
	int isHasSpecMap;		//スペキュラマップあるかどうか。
};

/*!
 *@brief	ライト用の定数バッファ。
 */
cbuffer LightCb : register(b1) {
    float3 dligDirection[NUM_DIRECTION_LIG];
    float4 dligColor[NUM_DIRECTION_LIG];
	float3 eyePos;									//カメラの視点。
	float  specPow;									//スペキュラライトの絞り。
	float3 ambientLight;							//環境光。
};

//ポイントライト。
struct SPointLight
{
    float3 position;
    float3 color;
    float pad;
    float range;	
    float3 pad2;
};

//ポイントライト用の定数バッファ。
cbuffer PointLightCB : register(b2){
    SPointLight pointsLights[NUM_POINT_LIG];
}

//スポットライト。
struct SSpotLight
{
    float3 position;
    float3 color;
	float3 direction;
    float pad;
    float range;	
};

//スポットライト用の定数バッファ。
cbuffer SpotLightCB : register(b3){
    SSpotLight spotLights[NUM_SPOT_LIG];
}
/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float3 Tangent  : TANGENT;				//接ベクトル。
    float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float2 TexCoord	: TEXCOORD0;			//UV座標。
    float3 Tangent	: TANGENT;				//接ベクトル。
    uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
    float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};

/// <summary>
/// シャドウマップ用の定数バッファ。
/// </summary>
cbuffer ShadowMapCb : register( b1 ){
	float4x4 lightViewProjMatrix;	//ライトビュープロジェクション行列。
}

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos		: TEXCOORD1;	//ワールド座標。
	float4 posInLVP		: TEXCOORD2;	//ライトビュープロジェクション空間での座標。
};

/// <summary>
/// シャドウマップ用のピクセルシェーダへの入力構造体。
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//座標。
};

/*!
 *@brief	スキン行列を計算。
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
/*!--------------------------------------------------------------------------------------
 * @brief	スキンなしモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);
	//鏡面反射の計算のために、ワールド座標をピクセルシェーダーに渡す。
	psInput.worldPos = pos;
	//ワールド座標系からカメラ座標系に変換する。
	pos = mul(mView, pos);
	//カメラ座標系からスクリーン座標系に変換する。
	pos = mul(mProj, pos);
	psInput.Position = pos;
	//影。
	if(isShadowReciever == 1 ){
		//続いて、ライトビュープロジェクション空間に変換。
		psInput.posInLVP = mul( mLightView, float4(psInput.worldPos.xyz,1.0f));
		psInput.posInLVP = mul( mLightProj, psInput.posInLVP );
	}
	//UV座標はそのままピクセルシェーダーに渡す。
	psInput.TexCoord = In.TexCoord;
	//法線。
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));
	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //最後のボーンを計算する。
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
	    pos = mul(skinning, In.Position);
	}
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
    psInput.worldPos = pos;
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	//影。
	if(isShadowReciever == 1 ){
		//続いて、ライトビュープロジェクション空間に変換。
		psInput.posInLVP = mul( mLightView, float4(psInput.worldPos.xyz,1.0f));
		psInput.posInLVP = mul( mLightProj, psInput.posInLVP );
	}
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	//albedoテクスチャからカラーをフェッチする。
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);

	//法線を計算する。
    float3 normal = 0;
    if (isHasNormalMap == 1)
    {
		//法線マップがある。
		//法線と接ベクトルの外積を計算して、従ベクトルを計算する。
        float3 biNormal = cross(In.Normal, In.Tangent);
        normal = g_normalMap.Sample(Sampler, In.TexCoord);
		//0.0～1.0の範囲になっているタンジェントスペース法線を
		//-1.0～1.0の範囲に変換する。
        normal = (normal * 2.0f) - 1.0f;
		//法線をタンジェントスペースから、ワールドスペースに変換する。
        normal = In.Tangent * normal.x + biNormal * normal.y + In.Normal * normal.z;
    }
    else
    {
		//ない。
        normal = In.Normal;
    }

	////ディレクションライトの拡散反射光を計算する。
	float3 lig = 0.0f;
    for (int i = 0; i < NUM_DIRECTION_LIG; i++)
    {
        lig += max(0.f, dot(In.Normal * -1.f, dligDirection[i])) * dligColor[i];
    }

	////鏡面反射////
    if (isHasSpecMap == 1)
    {
		//1 視点からライトを当てる物体に伸びるVectorEを求める
        float3 E = normalize(eyePos - In.worldPos);
		//2 反射ベクトルRを求める。
        float3 R = 0.0f;
        for (int i = 0; i < NUM_DIRECTION_LIG; i++)
        {
			
			//スペキュラ反射の強さを求める。
            R += reflect(dligDirection[i].xyz, In.Normal);
			//↓同じ結果。
			//R += dligDirection[i].xyz + 2.0f * dot(In.Normal, -dligDirection[i].xyz) * In.Normal;

			//1と2で求まったVectorのない席を計算する。
			//スペキュラ反射の強さを求める。
            float specPower = max(0.0f, dot(R, E));

            float specMap = 0.0f;
            specMap = g_specMap.Sample(Sampler, In.TexCoord);

			//3 スペキュラ反射をライトに加算する。
            lig += dligColor[i].xyz * pow(specPower, specPow) * specMap * dligColor[i].w;
        }
    }


	
	//ポイントライトから光によるランバート拡散反射を計算。
    for (int i = 0; i < NUM_POINT_LIG; i++)
    {
        float3 ligDir = normalize(In.worldPos - pointsLights[i].position.xyz);
        float distance = length(In.worldPos - pointsLights[i].position.xyz);
        float t = max(0.0f, dot(-ligDir, In.Normal));
        float affect = 1.0f - min(1.0f, distance / pointsLights[i].range/*100.0f*/);
        lig += pointsLights[i].color.xyz * t * affect;
    }
	
	//スポットライトから光によるランバート拡散反射を計算。
    for (int i = 0; i < NUM_SPOT_LIG; i++)
    {
        float3 vec = normalize(In.worldPos - spotLights[i].position);

        float Dot = dot(vec, spotLights[i].direction);
        float angle = acos(Dot);
        float degree = angle * 180.0f / 3.14f;
		
        if (degree < 45.0f)
        {
            float3 ligDir = normalize(In.worldPos - spotLights[i].position);
            float distance = length(In.worldPos - spotLights[i].position);
            float t = max(0.0f, dot(-ligDir, In.Normal));
            float affect = 1.0f - min(1.0f, distance / spotLights[i].range);
            lig += spotLights[i].color * t * affect;
        }
    }
	
	//環境光。
    lig += ambientLight;
   
	//影。
    if (isShadowReciever == 1)
    { //シャドウレシーバー。
		//LVP空間から見た時の最も手前の深度値をシャドウマップから取得する。
        float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
        shadowMapUV *= float2(0.5f, -0.5f);
        shadowMapUV += 0.5f;
		//シャドウマップの範囲内かどうかを判定する。
        if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
		)
        {
			
			///LVP空間での深度値を計算。
            float zInLVP = In.posInLVP.z / In.posInLVP.w;
			//シャドウマップに書き込まれている深度値を取得。
            float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);

            if (zInLVP > zInShadowMap + 0.01f)
            {
				//影が落ちているので、光を弱くする
                lig *= 0.5f;
            }
        }
    }
	
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}

/// <summary>
/// シャドウマップ生成用の頂点シェーダー。
/// </summary>
//スキンなし。
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In) 
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
//スキンあり。
PSInput_ShadowMap VSMainSkin_ShadowMap( VSInputNmTxWeights In ) 
{
	PSInput_ShadowMap psInputS = (PSInput_ShadowMap)0;
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //最後のボーンを計算する。
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
	    pos = mul(skinning, In.Position);
	}
	
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInputS.Position = pos;
    return psInputS;
}
/// <summary>
/// ピクセルシェーダーのエントリ関数。
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//射影空間でのZ値を返す。
	return In.Position.z  / In.Position.w;
}