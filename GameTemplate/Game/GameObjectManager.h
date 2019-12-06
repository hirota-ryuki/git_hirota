#pragma once
#include "RenderTarget.h"

//NewGO�̗D�揇��
enum prio {
	GOPrio_Defalut = 2,
	GOPrio_Sprite,
	GOPrio_num = 5,
};

class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();
public:
	//�V���O���g��
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
	/// �t�H���[�h�����_�����O(�ʏ�̕`�悾�ƍl����OK)
	/// </summary>
	void ForwordRender();
	/// <summary>
	/// �|�X�g�G�t�F�N�g�p�̃����_�[
	/// </summary>
	void PostRender();
	/// <summary>
	/// HUD�`��B
	/// </summary>
	void HudRender();
	/// <summary>
	/// �Q�[���I�u�W�F�N�g��ǉ��B
	/// </summary>
	template <class T>
	T* NewGO(int prio)
	{
		T* newObj = new T;
		m_goList[prio].push_back(newObj);
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
			for (auto it = m_goList[i].begin();
				it != m_goList[i].end();
				it++
				) {
				if ((*it) == go) {
					//���������B
					//�폜���N�G�X�g�𑗂�B
					go->RequestDelete();
					//�폜�ł����̂ŏI���B
					return;
				}
			}
		}
	}
private:

	RenderTarget m_mainRenderTarget;				//���C�������_�����O�^�[�Q�b�g�B
	Sprite m_copyMainRtToFrameBufferSprite;			//���C�������_�����O�^�[�Q�b�g�ɕ`���ꂽ�G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�B
	D3D11_VIEWPORT m_frameBufferViewports;			//�t���[���o�b�t�@�̃r���[�|�[�g�B
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[�B
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//�t���[���o�b�t�@�̃f�v�X�X�e���V���r���[�B

	std::vector< IGameObject* > m_goList[GOPrio_num];		//�Q�[���I�u�W�F�N�g�̃��X�g�B

};

//�O������A�N�Z�X����̂ŁAextern�錾���K�v�B
//extern GameObjectManager* g_goMgr;

template <class T>
static inline T* NewGO(int prio)
{
	return GameObjectManager::GetInstance().NewGO<T>(prio);
}
static inline void DeleteGO(IGameObject* go)
{
	return GameObjectManager::GetInstance().DeleteGO(go);
}