#pragma once

#include "IGameObject.h"

//NewGO�̗D�揇��
enum prio {
	GOPrio_Defalut = 2,
	GOPrio_Sprite,
	GOPrio_num = 5,
};

class GameObjectManager
{
public:
	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();
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
	std::vector< IGameObject* > m_goList[5];		//�Q�[���I�u�W�F�N�g�̃��X�g�B
};

//�O������A�N�Z�X����̂ŁAextern�錾���K�v�B
extern GameObjectManager g_goMgr;

template <class T>
static inline T* NewGO(int prio)
{
	return g_goMgr.NewGO<T>(prio);
}
static inline void DeleteGO(IGameObject* go)
{
	return g_goMgr.DeleteGO(go);
}