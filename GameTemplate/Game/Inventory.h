#pragma once
#include "IGameObject.h"
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
	bool Start() override {};
	void Update() override {};

	/// <summary>
	/// �A�C�e���̌���ǉ�����֐��B
	/// </summary>
	/// <param name="name">�A�C�e���̖��O�B</param>
	/// <param name="addnum">�ǉ�����֐��B</param>
	void AddItem(const wchar_t* name, int addnum);
	/// <summary>
	/// �A�C�e���f�[�^��Ԃ��Ă���֐��B
	/// </summary>
	/// <returns>�A�C�e���f�[�^�B</returns>
	std::unordered_map<
		std::wstring,			//�A�C�e���̖��O�B
		int						//�A�C�e���̌��B
	>* GetItemDataMap() {
		return &m_itemDataMap;
	}
private:
	//�A�C�e���f�[�^�B
	std::unordered_map<
		std::wstring,			//�A�C�e���̖��O�B
		int						//�A�C�e���̌��B
	>	m_itemDataMap;
};

static inline 	void AddItem(const wchar_t* name, int addnum) {
	Inventory::GetInstance().AddItem(name, addnum);
}

static inline std::unordered_map<
	std::wstring,			//�A�C�e���̖��O�B
	int						//�A�C�e���̌��B
>* GetItemDataMap() {
	return Inventory::GetInstance().GetItemDataMap();
}

