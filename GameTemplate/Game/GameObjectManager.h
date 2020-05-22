#pragma once
#include "RenderTarget.h"
#include "util/Util.h"
#include <list>
#include "graphics/posteffect/PostEffect.h"

class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();
	/*!
	*@brief	�Q�[���I�u�W�F�N�g�̖��O�L�[���쐬�B
	*/
	static unsigned int MakeGameObjectNameKey(const char* objectName)
	{
		static const unsigned int defaultNameKey = Util::MakeHash("Undefined");	//���O�L�[�B
		unsigned int hash;
		if (objectName == nullptr) {
			hash = defaultNameKey;
		}
		else {
			hash = Util::MakeHash(objectName);
		}
		return hash;
	}
public:
	//�V���O���g���B
	static GameObjectManager& GetInstance()
	{
		static GameObjectManager instance;
		return instance;
	}
	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();
	/// <summary>
	/// GameObject�̊J�n�֐��ƍX�V�֐��B
	/// </summary>
	void StartAndUpdate();
	/// <summary>
	/// GameObject����������֐��B
	/// </summary>
	void Delete();
	/// <summary>
	/// �`��B
	/// </summary>
	void Render();
	/// <summary>
	/// �J�����̏������B
	/// </summary>
	void InitCamera();
	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̐؂�ւ��B
	/// </summary>
	/// <param name="d3dDeviceContext">�f�o�C�X�R���e�L�X�g</param>
	/// <param name="renderTarget">�����_�[�^�[�Q�b�g</param>
	/// <param name="viewport">�r���[�|�[�g</param>
	void ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, RenderTarget * renderTarget, D3D11_VIEWPORT * viewport);
	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̐؂�ւ��B
	/// </summary>
	/// <param name="d3dDeviceContext">�f�o�C�X�R���e�L�X�g</param>
	/// <param name="renderTarget">�����_�[�^�[�Q�b�g</param>
	/// <param name="depthStensil">�f�v�X�X�e���V��</param>
	/// <param name="viewport">�r���[�|�[�g</param>
	void ChangeRenderTarget(ID3D11DeviceContext * d3dDeviceContext, ID3D11RenderTargetView * renderTarget, ID3D11DepthStencilView * depthStensil, D3D11_VIEWPORT * viewport);
	/// <summary>
	/// �V���h�E�}�b�v�̃����_�[�B
	/// </summary>
	void ShadowMapRender();
	/// <summary>
	/// �t�H���[�h�����_�����O(�ʏ�̕`�悾�ƍl����OK)�B
	/// </summary>
	void ForwordRender();
	/// <summary>
	/// �|�X�g�G�t�F�N�g�p�̃����_�[�B
	/// </summary>
	void PostRender();
	/// <summary>
	/// HUD�`��B
	/// </summary>
	void HudRender();
	/// <summary>
	/// �t�H���g�̕`��B
	/// </summary>
	void FontRender();
	/// <summary>
	/// �Q�[���I�u�W�F�N�g��ǉ��B
	/// </summary>
	template <class T>
	T* NewGO(int prio, const char* objectName)
	{
		(void*)objectName;
		T* newObj = new T;
		m_goList[prio].push_back(newObj);
		unsigned int hash = MakeGameObjectNameKey(objectName);
		newObj->SetNameKey(hash);
		newObj->prio = prio;
		return newObj;
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�����X�g����폜����B
	/// </summary>
	/// <param name="go">�폜����Q�[���I�u�W�F�N�g</param>
	
	void DeleteGO(IGameObject* go)
	{
		for (int i = 0; i < GOPrio_num; i++) {
			//���X�g���猟�����āA����������폜����B
			for (auto it = m_goList[i].begin(); it != m_goList[i].end(); it++) {
				if ((*it) == go) {
					if (!go->IsDelete()) {
						//���������B
						//�폜���N�G�X�g�𑗂�B
						go->SetDelete();
						m_DeleteGOList.emplace_back(go);
						go->OnDestroy();
					}
					return;
				}
			}
		}
	}
	template<class T>
	void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
	{
		unsigned int nameKey = Util::MakeHash(objectName);
		for (int i = 0; i < GOPrio_num; i++) {
			for (auto go : m_goList[i]) {
				if (go->GetNameKey() == nameKey) {
					//�������B
					T* p = dynamic_cast<T*>(go);
					if (p != nullptr) {
						if (func(p) == false) {
							//�N�G�����f�B
							return;
						}
					}
				}
			}
		}
	}

	/// <summary>
	/// ���C�������_�����O�^�[�Q�b�g���擾�B
	/// </summary>
	/// <returns>���C�������_�����O�^�[�Q�b�g�B</returns>
	RenderTarget* GetMainRenderTarget(){
		return &m_mainRenderTarget;
	}

	RenderTarget m_mainRenderTarget;				//���C�������_�����O�^�[�Q�b�g�B
	Sprite m_copyMainRtToFrameBufferSprite;			//���C�������_�����O�^�[�Q�b�g�ɕ`���ꂽ�G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�B
	D3D11_VIEWPORT m_frameBufferViewports;			//�t���[���o�b�t�@�̃r���[�|�[�g�B
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[�B
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//�t���[���o�b�t�@�̃f�v�X�X�e���V���r���[�B

	//�x�N�^�͐V��������ʂō��z����X�V���Ă������A
	//���X�g�͓����������Ă����̂ō���̏ꍇ�̓��X�g�̂ق����ǂ��B
	std::list< IGameObject* > m_goList[GOPrio_num];		//�Q�[���I�u�W�F�N�g�̃��X�g�B
	std::list< IGameObject* > m_DeleteGOList;			//�폜�\��̃Q�[���I�u�W�F�N�g�̃��X�g�B

	PostEffect m_postEffect;				//�|�X�g�G�t�F�N�g�B
};

//�O������A�N�Z�X����̂ŁAextern�錾���K�v�B
//extern GameObjectManager* g_goMgr;

template <class T>
static inline T* NewGO(int prio, const char* objectName = nullptr)
{
	return GameObjectManager::GetInstance().NewGO<T>(prio, objectName);
}
static inline void DeleteGO(IGameObject* go)
{
	return GameObjectManager::GetInstance().DeleteGO(go);
}

/*!
	*@brief	�Q�[���I�u�W�F�N�g�̌����̃w���p�[�֐��B
	*@details
	* �����̃Q�[���I�u�W�F�N�g�ɑS�Ăɑ΂��āA�N�G�����s�������ꍇ�Ɏg�p���Ă��������B
	*@param[in]	objectName	�Q�[���I�u�W�F�N�g�̖��O�B
	*@param[in]	func		�Q�[���I�u�W�F�N�g�����������Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	*/
template<class T>
static inline void QueryGOs(const char* objectName, std::function<bool(T* go)> func)
{
	return GameObjectManager::GetInstance().FindGameObjects<T>(objectName, func);
}

/*!
	*@brief	�Q�[���I�u�W�F�N�g�𖼑O�w��ō폜�B
	*@details
	* ���O�������s���邽�ߒx���ł��B
	*@param[in]	objectName		�폜����Q�[���I�u�W�F�N�g�̖��O�B
	*/
static inline void DeleteGOs(const char* objectName)
{
	QueryGOs<IGameObject>(objectName, [](auto go) {
		DeleteGO(go);
		return true;
		});
}