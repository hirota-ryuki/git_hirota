#pragma once

#include "Skeleton.h"
#include "LightNumData.h"
//#include "Light.h"

/// <summary>
///	�|�C���g���C�g�p�̍\���́B
/// </summary>
struct SPointLight {
	CVector4	pos;
	CVector4	color;
	float		range;
	float		pad[3];
};

/// <summary>
///	�X�|�b�g���C�g�p�̍\���́B
/// </summary>
struct SSpotLight {
	CVector4	pos;
	CVector4	color;
	CVector4	direction;
	float		range;
	float		pad[3];
};

/*!
*@brief	FBX�̏�����B
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};



/*!
*@brief	�X�L�����f���N���X�B
*/
class SkinModel
{
public:
	//���b�V�������������Ƃ��̃R�[���o�b�N�֐��B
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~SkinModel();
	
	/*!
	*@brief	�������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	/*!
	*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
	*@param[in]	position	���f���̍��W�B
	*@param[in]	rotation	���f���̉�]�B
	*@param[in]	scale		���f���̊g�嗦�B
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/// <summary>
	/// �@���}�b�v�̏����������B
	/// </summary>
	/// <param name="filePath">�@���}�b�v�̃t�@�C���p�X�B</param>
	void InitNormalMap(const wchar_t* filePath);
	/// <summary>
	/// �X�y�L�����}�b�v�̏����������B
	/// </summary>
	/// <param name="filePath">�X�y�L�����}�b�v�̃t�@�C���p�X�B</param>
	void InitSpecMap(const wchar_t* filePath);
	/*!
	*@brief	�{�[���������B
	*@param[in]		boneName	�{�[���̖��O�B
	*@return	���������{�[���B������Ȃ������ꍇ��nullptr��Ԃ��܂��B
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	���f����`��B
	*@param[in]	viewMatrix		�J�����s��B
	*  ���[���h���W�n��3D���f�����J�������W�n�ɕϊ�����s��ł��B
	*@param[in]	projMatrix		�v���W�F�N�V�����s��B
	*  �J�������W�n��3D���f�����X�N���[�����W�n�ɕϊ�����s��ł��B
	*/
	void Draw(EnRenderMode renderMode, CMatrix viewMatrix, CMatrix projMatrix );
	/*!
	*@brief	�X�P���g���̎擾�B
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	���b�V������������B
	*@param[in] onFindMesh		���b�V�������������Ƃ��̃R�[���o�b�N�֐�
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
	*@brief	SRV�̃��W�X�^�ԍ��B
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<�f�B�t���[�Y�e�N�X�`���B
		enSkinModelSRVReg_BoneMatrix,				//!<�{�[���s��B
	};
	CMatrix& GetWorldMatrix()
	{
		return m_worldMatrix;
	}
	/// <summary>
	/// �V���h�E���V�[�o�[�̃t���O��ݒ肷��B
	/// </summary>
	/// <param name="flag">true��n���ƃV���h�E���V�[�o�[�ɂȂ�</param>
	/// <remarks>
	/// �V���h�E���V�[�o�[�Ƃ͉e�𗎂Ƃ����I�u�W�F�N�g�̂��Ƃł��B
	/// �V���h�E�L���X�^�[�ɂ���Đ������ꂽ�A�V���h�E�}�b�v�𗘗p����
	/// ���g�ɉe�𗎂Ƃ��܂��B
	/// �I�u�W�F�N�g���V���h�E���V�[�o�[���V���h�E�L���X�^�[�ɂȂ��Ă���ꍇ��
	/// �Z���t�V���h�E(�����̉e�������ɗ�����)���s�����Ƃ��ł��܂��B
	/// </remarks>
	void SetShadowReciever(bool flag)
	{
		m_isShadowReciever = flag;
	}
	/// <summary>
	/// �f�B���N�V�������C�g�̕������Z�b�g�B
	/// </summary>
	/// <param name="LDir">����</param>
	void SetDirectionLightDirection(CVector4 LDir ) {
		m_light.directionLight.direction[0] = LDir;
		m_light.directionLight.direction[0].Normalize();
	}
	/// <summary>
	///�f�B���N�V�������C�g�̐F���Z�b�g�B
	/// </summary>
	/// <param name="LColor">�F</param>
	void SetDirectionLightColor(CVector4 LColor ) {
		m_light.directionLight.color[0] = LColor;
	}
	/// <summary>
	/// �X�y�L�����̗ݏ搔���Z�b�g�B
	/// </summary>
	/// <param name="spec">�ݏ搔</param>
	void SetSpec(float spec ) {
		m_light.specPow = spec;

	}
	/// <summary>
	/// �A���r�G���g���C�g���Z�b�g�B
	/// </summary>
	/// <param name="spec">�A���r�G���g���C�g</param>
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
	*@brief	�T���v���X�e�[�g�̏������B
	*/
	void InitSamplerState();
	/*!
	*@brief	�萔�o�b�t�@�̍쐬�B
	*/
	void InitConstantBuffer();
	/// <summary>
	/// �f�B���N�V�������C�g�̏������B
	/// </summary>
	void InitDirectionLight();
	/// <summary>
	/// �|�C���g���C�g�̏������B
	/// </summary>
	void InitPointLight();
	/// <summary>
	/// �X�|�b�g���C�g�̏������B
	/// </summary>
	void InitSpotLight();
	/*!
	*@brief	�X�P���g���̏������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*/
	void InitSkeleton(const wchar_t* filePath);
	
private:
	//�萔�o�b�t�@�B
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CMatrix mLightView;		//���C�g�r���[�s��B
		CMatrix mLightProj;		//���C�g�v���W�F�N�V�����s��B
		int isShadowReciever;	//�V���h�E���V�[�o�[�̃t���O�B
		int isHasNormalMap;		//�@���}�b�v��ێ����Ă����ǂ����B
		int isHasSpecMap;		//�@���}�b�v��ێ����Ă����ǂ����B
	};
	/*!
	*@brief	�f�B���N�V�������C�g�B
	*/
	struct SDirectionLight {
		CVector4 direction[NUM_DIRECTION_LIG];		//���C�g�̕����B
		CVector4 color[NUM_DIRECTION_LIG];			//���C�g�̃J���[�B
	};
	
	/// <summary>
	/// ���C�g�\���́B
	/// </summary>
	struct SLight {
		SDirectionLight		directionLight;		//�f�B���N�V�������C�g
		CVector3			eyePos;				//���_�̍��W�B
		float				specPow;			//���ʔ��˂̍i��B
		CVector3			ambientLight;		//�����B
	};

	
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//FBX�̏�����B
	ID3D11Buffer*		m_cb = nullptr;					//�萔�o�b�t�@�B
	Skeleton			m_skeleton;						//�X�P���g���B
	CMatrix				m_worldMatrix;					//���[���h�s��B
	DirectX::Model*		m_modelDx;						//DirectXTK���񋟂��郂�f���N���X�B
	ID3D11SamplerState* m_samplerState = nullptr;		//�T���v���X�e�[�g�B
	//���C�g
	ID3D11Buffer*		m_lightCb = nullptr;			//���C�g�p�̒萔�o�b�t�@�B
	SLight				m_light;						//���C�g�N���X�B
	//�|�C���g���C�g
	ID3D11Buffer*		m_pointlightCb = nullptr;		//�|�C���g���C�g�p�̒萔�o�b�t�@�B
	static SPointLight	m_pointLight[NUM_POINT_LIG];				//�|�C���g���C�g�N���X�B
	//�X�|�b�g���C�g
	ID3D11Buffer*		m_spotlightCb = nullptr;		//�X�|�b�g���C�g�p�̒萔�o�b�t�@�B
	static SSpotLight	m_spotLight[NUM_SPOT_LIG];		//�X�|�b�g���C�g�N���X�B
	//�e�B
	bool				m_isShadowReciever = false;		//�V���h�E���V�[�o�[�̃t���O�B
	//�@���}�b�v�B
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;	//�@���}�b�v��SRV�B
	//�X�y�L�����}�b�v�B
	ID3D11ShaderResourceView* m_specMapSRV = nullptr;	//�X�y�L�����}�b�v��SRV�B
};