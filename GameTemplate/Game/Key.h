#pragma once
#include "IGameObject.h"

class Key :
	public IItem
{
public:
	Key();
	~Key();
	void ItemDestroy() override;
	void ItemStart() override;
	void ItemUpdate() override;
	void OnGet() override;	
	/// <summary>
	/// ボールのポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ボールの回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// ボールの識別番号をセット。
	/// </summary>
	/// <param name="pos">識別番号。</param>
	void SetNomber(int no)
	{
		m_nomber = no;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="name"></param>
	void SetName(std::wstring name) {
		m_name = name;
	}
private:
	SkinModelRender*	m_model = nullptr;						//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	int					m_nomber = 0;							//識別番号。		
	Game*				m_game = nullptr;						//Gameのポインタ。
	Player*				m_player = nullptr;						//プレイヤークラスのポインタ。

	//ポップアップ画像関係。
	SpriteRender*		m_sprite = nullptr;
	CVector2			m_spritePos = FRAME_OUT_POS;

	//アイテムデータ。
	const wchar_t* NAME = L"鍵";
	std::wstring		m_name;
	const int      ADD = 1;

};

