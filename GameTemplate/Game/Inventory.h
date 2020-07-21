#pragma once
#include "IGameObject.h"
#include <unordered_map>

//NewGO����K�v������̂�����
class Inventory :
	public IGameObject
{
private:
	Inventory();
	~Inventory();
public:
	//�V���O���g���B
	static Inventory& GetInstance() {
		static Inventory inctance;
		return inctance;
	}
	bool Start() override { return true; };
	void Update() override {};

	/// <summary>
	/// �A�C�e����ǉ�������A
	/// ���̕ϓ����s���֐��B
	/// </summary>
	/// <param name="name">�A�C�e���̖��O�B</param>
	/// <param name="addnum">�ǉ�����֐��B</param>
	void AddItem(const wchar_t* name, int addnum);
	/// <summary>
	/// �A�C�e���f�[�^��Ԃ��Ă���֐��B
	/// </summary>
	/// <returns>�A�C�e���f�[�^�B</returns>
	std::unordered_map<
		std::wstring,		
		int					
	> GetItemDataMap() {
		return m_itemDataMap;
	}
	/// <summary>
	/// �A�C�e���f�[�^���ǉ����ꂽ�ǂ�����Ԃ��֐��B
	/// </summary>
	/// <returns>m_isAddData</returns>
	bool GetIsAddData() {
		return m_isAddData;
	}
	/// <summary>
	/// �A�C�e���̌����ǉ����ꂽ�ǂ�����Ԃ��֐��B
	/// </summary>
	/// <returns>m_isAddNum</returns>
	bool GetIsAddNum() {
		return m_isAddNum;
	}
	/// <summary>
	/// �A�C�e���̑�����Ԃ��֐��B
	/// </summary>
	/// <returns>m_itemCount</returns>
	int GetItemCount() {
		return m_itemCount;
	}
	/// <summary>
	/// �ǉ��̗L���̔�������Z�b�g����֐��B
	/// </summary>
	void ResetIsAddData() {
		m_isAddData = false;
		m_isAddNum = false;
	}
private:
	//�A�C�e���f�[�^�B
	std::unordered_map<
		std::wstring,			//�A�C�e���̖��O�B
		int						//�A�C�e���̌��B
	>	m_itemDataMap;

	bool	m_isAddData = false;	//�A�C�e���f�[�^���ǉ����ꂽ�ǂ����B
	bool	m_isAddNum = false;		//�A�C�e���̌����ǉ����ꂽ�ǂ����B
	int		m_itemCount = 0;		//�A�C�e���̑����B
};

static inline 	void AddItem(const wchar_t* name, int addnum) {
	Inventory::GetInstance().AddItem(name, addnum);
}

static inline std::unordered_map<
	std::wstring,			
	int						
> GetItemDataMap() {
	return Inventory::GetInstance().GetItemDataMap();
}

static inline void ResetIsAddData() {
	Inventory::GetInstance().ResetIsAddData();
}

static inline bool GetIsAddData() {
	return Inventory::GetInstance().GetIsAddData();
}

static inline bool GetIsAddNum() {
	return Inventory::GetInstance().GetIsAddNum();
}

static inline int GetItemCount() {
	return Inventory::GetInstance().GetItemCount();
}