/*!
 * @brief	���f���V�F�[�_�[�B
 */
//#include "..\..\graphics\LightNumData.h"
#include "../../graphics\LightNumData.h"


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
Texture2D<float4> albedoTexture : register(t0);	//�A���x�h�e�N�X�`���B
Texture2D<float4> g_shadowMap : register(t1);	//�V���h�E�}�b�v�B
Texture2D<float4> g_normalMap : register(t2);	//�@���}�b�v�B
Texture2D<float4> g_specMap : register(t3);		//�X�y�L�����}�b�v�B

//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// �萔�o�b�t�@�B
/////////////////////////////////////////////////////////////
/*!
 * @brief	���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float4x4 mLightView;	//���C�g�r���[�s��B
	float4x4 mLightProj;	//���C�g�v���W�F�N�V�����s��B
	int isShadowReciever;	//�V���h�E���V�[�o�[�t���O�B
	int isHasNormalMap;		//�@���}�b�v���邩�ǂ����B
	int isHasSpecMap;		//�X�y�L�����}�b�v���邩�ǂ����B
};

/*!
 *@brief	���C�g�p�̒萔�o�b�t�@�B
 */
cbuffer LightCb : register(b1) {
    float3 dligDirection[NUM_DIRECTION_LIG];
    float4 dligColor[NUM_DIRECTION_LIG];
	float3 eyePos;									//�J�����̎��_�B
	float  specPow;									//�X�y�L�������C�g�̍i��B
	float3 ambientLight;							//�����B
};

//�|�C���g���C�g�B
struct SPointLight
{
    float3 position;
    float3 color;
    float pad;
    float range;	
    float3 pad2;
};

//�|�C���g���C�g�p�̒萔�o�b�t�@�B
cbuffer PointLightCB : register(b2){
    SPointLight pointsLights[NUM_POINT_LIG];
}

//�X�|�b�g���C�g�B
struct SSpotLight
{
    float3 position;
    float3 color;
	float3 direction;
    float pad;
    float range;	
};

//�X�|�b�g���C�g�p�̒萔�o�b�t�@�B
cbuffer SpotLightCB : register(b3){
    SSpotLight spotLights[NUM_SPOT_LIG];
}
/////////////////////////////////////////////////////////////
//�e��\����
/////////////////////////////////////////////////////////////
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float3 Tangent  : TANGENT;				//�ڃx�N�g���B
    float2 TexCoord : TEXCOORD0;			//UV���W�B
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float2 TexCoord	: TEXCOORD0;			//UV���W�B
    float3 Tangent	: TANGENT;				//�ڃx�N�g���B
    uint4  Indices  : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
    float4 Weights  : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};

/// <summary>
/// �V���h�E�}�b�v�p�̒萔�o�b�t�@�B
/// </summary>
cbuffer ShadowMapCb : register( b1 ){
	float4x4 lightViewProjMatrix;	//���C�g�r���[�v���W�F�N�V�����s��B
}

/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos		: TEXCOORD1;	//���[���h���W�B
	float4 posInLVP		: TEXCOORD2;	//���C�g�r���[�v���W�F�N�V������Ԃł̍��W�B
};

/// <summary>
/// �V���h�E�}�b�v�p�̃s�N�Z���V�F�[�_�ւ̓��͍\���́B
/// </summary>
struct PSInput_ShadowMap {
	float4 Position 			: SV_POSITION;	//���W�B
};

/*!
 *@brief	�X�L���s����v�Z�B
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
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);
	//���ʔ��˂̌v�Z�̂��߂ɁA���[���h���W���s�N�Z���V�F�[�_�[�ɓn���B
	psInput.worldPos = pos;
	//���[���h���W�n����J�������W�n�ɕϊ�����B
	pos = mul(mView, pos);
	//�J�������W�n����X�N���[�����W�n�ɕϊ�����B
	pos = mul(mProj, pos);
	psInput.Position = pos;
	//�e�B
	if(isShadowReciever == 1 ){
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
		psInput.posInLVP = mul( mLightView, float4(psInput.worldPos.xyz,1.0f));
		psInput.posInLVP = mul( mLightProj, psInput.posInLVP );
	}
	//UV���W�͂��̂܂܃s�N�Z���V�F�[�_�[�ɓn���B
	psInput.TexCoord = In.TexCoord;
	//�@���B
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));
	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //�Ō�̃{�[�����v�Z����B
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
	    pos = mul(skinning, In.Position);
	}
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
    psInput.worldPos = pos;
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	//�e�B
	if(isShadowReciever == 1 ){
		//�����āA���C�g�r���[�v���W�F�N�V������Ԃɕϊ��B
		psInput.posInLVP = mul( mLightView, float4(psInput.worldPos.xyz,1.0f));
		psInput.posInLVP = mul( mLightProj, psInput.posInLVP );
	}
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain( PSInput In ) : SV_Target0
{
	//albedo�e�N�X�`������J���[���t�F�b�`����B
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);

	//�@�����v�Z����B
    float3 normal = 0;
    if (isHasNormalMap == 1)
    {
		//�@���}�b�v������B
		//�@���Ɛڃx�N�g���̊O�ς��v�Z���āA�]�x�N�g�����v�Z����B
        float3 biNormal = cross(In.Normal, In.Tangent);
        normal = g_normalMap.Sample(Sampler, In.TexCoord);
		//0.0�`1.0�͈̔͂ɂȂ��Ă���^���W�F���g�X�y�[�X�@����
		//-1.0�`1.0�͈̔͂ɕϊ�����B
        normal = (normal * 2.0f) - 1.0f;
		//�@�����^���W�F���g�X�y�[�X����A���[���h�X�y�[�X�ɕϊ�����B
        normal = In.Tangent * normal.x + biNormal * normal.y + In.Normal * normal.z;
    }
    else
    {
		//�Ȃ��B
        normal = In.Normal;
    }

	////�f�B���N�V�������C�g�̊g�U���ˌ����v�Z����B
	float3 lig = 0.0f;
    for (int i = 0; i < NUM_DIRECTION_LIG; i++)
    {
        lig += max(0.f, dot(In.Normal * -1.f, dligDirection[i])) * dligColor[i];
    }

	////���ʔ���////
    if (isHasSpecMap == 1)
    {
		//1 ���_���烉�C�g�𓖂Ă镨�̂ɐL�т�VectorE�����߂�
        float3 E = normalize(eyePos - In.worldPos);
		//2 ���˃x�N�g��R�����߂�B
        float3 R = 0.0f;
        for (int i = 0; i < NUM_DIRECTION_LIG; i++)
        {
			
			//�X�y�L�������˂̋��������߂�B
            R += reflect(dligDirection[i].xyz, In.Normal);
			//���������ʁB
			//R += dligDirection[i].xyz + 2.0f * dot(In.Normal, -dligDirection[i].xyz) * In.Normal;

			//1��2�ŋ��܂���Vector�̂Ȃ��Ȃ��v�Z����B
			//�X�y�L�������˂̋��������߂�B
            float specPower = max(0.0f, dot(R, E));

            float specMap = 0.0f;
            specMap = g_specMap.Sample(Sampler, In.TexCoord);

			//3 �X�y�L�������˂����C�g�ɉ��Z����B
            lig += dligColor[i].xyz * pow(specPower, specPow) * specMap * dligColor[i].w;
        }
    }


	
	//�|�C���g���C�g������ɂ�郉���o�[�g�g�U���˂��v�Z�B
    for (int i = 0; i < NUM_POINT_LIG; i++)
    {
        float3 ligDir = normalize(In.worldPos - pointsLights[i].position.xyz);
        float distance = length(In.worldPos - pointsLights[i].position.xyz);
        float t = max(0.0f, dot(-ligDir, In.Normal));
        float affect = 1.0f - min(1.0f, distance / pointsLights[i].range/*100.0f*/);
        lig += pointsLights[i].color.xyz * t * affect;
    }
	
	//�X�|�b�g���C�g������ɂ�郉���o�[�g�g�U���˂��v�Z�B
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
	
	//�����B
    lig += ambientLight;
   
	//�e�B
    if (isShadowReciever == 1)
    { //�V���h�E���V�[�o�[�B
		//LVP��Ԃ��猩�����̍ł���O�̐[�x�l���V���h�E�}�b�v����擾����B
        float2 shadowMapUV = In.posInLVP.xy / In.posInLVP.w;
        shadowMapUV *= float2(0.5f, -0.5f);
        shadowMapUV += 0.5f;
		//�V���h�E�}�b�v�͈͓̔����ǂ����𔻒肷��B
        if (shadowMapUV.x < 1.0f
			&& shadowMapUV.x > 0.0f
			&& shadowMapUV.y < 1.0f
			&& shadowMapUV.y > 0.0f
		)
        {
			
			///LVP��Ԃł̐[�x�l���v�Z�B
            float zInLVP = In.posInLVP.z / In.posInLVP.w;
			//�V���h�E�}�b�v�ɏ������܂�Ă���[�x�l���擾�B
            float zInShadowMap = g_shadowMap.Sample(Sampler, shadowMapUV);

            if (zInLVP > zInShadowMap + 0.01f)
            {
				//�e�������Ă���̂ŁA�����キ����
                lig *= 0.5f;
            }
        }
    }
	
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}

/// <summary>
/// �V���h�E�}�b�v�����p�̒��_�V�F�[�_�[�B
/// </summary>
//�X�L���Ȃ��B
PSInput_ShadowMap VSMain_ShadowMap(VSInputNmTxVcTangent In) 
{
	PSInput_ShadowMap psInput = (PSInput_ShadowMap)0;
	float4 pos = mul(mWorld, In.Position);
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	return psInput;
}
//�X�L������B
PSInput_ShadowMap VSMainSkin_ShadowMap( VSInputNmTxWeights In ) 
{
	PSInput_ShadowMap psInputS = (PSInput_ShadowMap)0;
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //�Ō�̃{�[�����v�Z����B
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
	    pos = mul(skinning, In.Position);
	}
	
	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInputS.Position = pos;
    return psInputS;
}
/// <summary>
/// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
/// </summary>
float4 PSMain_ShadowMap(PSInput_ShadowMap In) : SV_Target0
{
	//�ˉe��Ԃł�Z�l��Ԃ��B
	return In.Position.z  / In.Position.w;
}