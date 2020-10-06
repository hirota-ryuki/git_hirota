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
		static Inventory instance;
		return instance;
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
	/// �A�C�e���̌���Ԃ��Ă���֐��B
	/// </summary>
	/// <param name="name">�A�C�e���̖��O�B</param>
	/// <returns>���B</returns>
	int FindItem(const wchar_t* name);
	/// <summary>
	/// �A�C�e���f�[�^��Ԃ��Ă���֐��B
	/// </summary>
	/// <returns>�A�C�e���f�[�^�B</returns>
	const std::unordered_map<
		std::wstring,		
		int					
	>& GetItemDataMap() const{
		return m_itemDataMap;
	}
	/// <summary>
	/// �A�C�e���f�[�^���ǉ����ꂽ�ǂ�����Ԃ��֐��B
	/// </summary>
	/// <returns>m_isAddData</returns>
	bool GetIsAddData() const{
		return m_isAddData;
	}
	/// <summary>
	/// �A�C�e���̌����ǉ����ꂽ�ǂ�����Ԃ��֐��B
	/// </summary>
	/// <returns>m_isAddNum</returns>
	bool GetIsAddNum() const{
		return m_isAddNum;
	}
	/// <summary>
	/// �A�C�e���̑�����Ԃ��֐��B
	/// </summary>
	/// <returns>m_itemCount</returns>
	int GetItemCount() const {
		return m_itemDataMap.size();
	}
	/// <summary>
	/// �ǉ��̗L���̔�������Z�b�g����֐��B
	/// </summary>
	void ResetIsAddData() {
		m_isAddData = false;
		m_isAddNum = false;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool GetIsDeleteItem() {
		return m_isDeleteItem;
	}
	/// <summary>
	/// �폜�̗L���̔�������Z�b�g����֐��B
	/// </summary>
	void ResetIsDeleteItem() {
		m_isDeleteItem = false;
	}
private:
	//�A�C�e���f�[�^�B
	std::unordered_map<
		std::wstring,			//�A�C�e���̖��O�B
		int						//�A�C�e���̌��B
	>	m_itemDataMap;
	std::unordered_map<
		std::wstring,			//�A�C�e���̖��O�B
		int						//�A�C�e���̌��B
	>	m_deleteItemDataMap;

	bool	m_isAddData = false;		//�A�C�e���f�[�^���ǉ����ꂽ�ǂ����B
	bool	m_isAddNum = false;			//�A�C�e���̌����ǉ����ꂽ�ǂ����B
	bool	m_isDeleteItem = false;		//�A�C�e�����폜���ꂽ�ǂ����B
};

static inline void Inv_AddItem(const wchar_t* name, int addnum) {
	Inventory::GetInstance().AddItem(name, addnum);
}

static inline std::unordered_map<
	std::wstring,			
	int						
> Inv_GetItemDataMap() {
	return Inventory::GetInstance().GetItemDataMap();
}

static inline int Inv_FindItem(const wchar_t* name) {
	return Inventory::GetInstance().FindItem(name);
}

static inline void Inv_ResetIsAddData() {
	Inventory::GetInstance().ResetIsAddData();
}

static inline bool Inv_GetIsAddData() {
	return Inventory::GetInstance().GetIsAddData();
}

static inline bool Inv_GetIsAddNum() {
	return Inventory::GetInstance().GetIsAddNum();
}

static inline bool Inv_GetIsDeleteItem() {
	return Inventory::GetInstance().GetIsDeleteItem();
}

static inline int Inv_GetItemCount() {
	return Inventory::GetInstance().GetItemCount();
}