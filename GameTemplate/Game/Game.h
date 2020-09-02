#pragma once
#include "level/Level.h"
//��������Ƃ��ɕs�K�v�Ȏ菇���ȗ����邽�߂̃��[�h�B
//Title�ł͂Ȃ�Game���ŏ���NewGO����B
//�I�[�v�j���O���ȗ�����B
#define CREATE_MODE	

class GameCamera;
class Floor;
class ObjRigidbody;
class Ceiling;
class Player;
class Goal;
class Zombie;
class Key;
class RockDoor;
class BulletStack;
class Result;
class Menu;
class Opening;
class UI;
class Map;
class PointLight;
class SpotLight;

class Game :
	public IGameObject
{
public:
	Game();
	~Game();	
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �Q�[���̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�Q�[���̃C���X�^���X</returns>
	static Game* GetGame()
	{
		return m_game;
	}
	/// <summary>
	/// �v���C���[�̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�v���C���[�̃C���X�^���X</returns>
	Player* GetPlayer()
	{
		return m_player;
	}
	/// <summary>
	/// �J�����̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�J�����̃C���X�^���X</returns>
	GameCamera* GetCamera()
	{
		return m_gamecamera;
	}
	/// <summary>
	/// ���̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>���̃C���X�^���X</returns>
	Floor* GetFloor()
	{
		return m_floor;
	}
	/// <summary>
	/// �|�[�Y���j���[�̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�|�[�Y���j���[�̃C���X�^���X</returns>
	Menu* GetMenu()
	{
		return m_menu;
	}
	/// <summary>
	/// UI�̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>UI�̃C���X�^���X</returns>
	UI* GetUI()
	{
		return m_ui;
	}
	/// <summary>
	/// PointLight�̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>PointLight�̃C���X�^���X</returns>
	PointLight* GetPointLight() {
		return m_pl;
	}
	/// <summary>
	/// SpotLight�̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>SpotLight�̃C���X�^���X</returns>
	SpotLight* GetSpotLight() {
		return m_sl;
	}
	/// <summary>
	/// �N���A�����Ƃ��ɌĂяo���֐��B
	/// </summary>
	void Clear(){
		m_isClear = true;
	}
	/// <summary>
	/// �Q�[���I�[�o�[�����Ƃ��ɌĂяo���֐��B
	/// </summary>
	void GameOver(){
		m_isGameOver = true;
	}
	/// <summary>
	/// �ꎞ��~�������Ƃ��ɌĂяo���֐��B
	/// </summary>
	void ChangePose(){
		m_isPose = !m_isPose;
	}
	/// <summary>
	/// �ꎞ��~�������Ƃ��ɌĂяo���֐��B
	/// true�Ȃ�ꎞ��~�Afalse�Ȃ�ꎞ��~�����B
	/// </summary>
	void IsPose(bool flag){
		m_isPose = flag;
	}
	/// <summary>
	/// �ꎞ��~���Ă��邩�ǂ������擾�B
	/// </summary>
	/// <returns>�ꎞ��~���Ă��邩�ǂ����B</returns>
	bool GetIsPose() {
		return m_isPose;
	}
private:
	//�C���X�^���X�֌W�B
	static Game*	m_game;					//Game�N���X�̃C���X�^���X�B
	GameCamera*		m_gamecamera = nullptr;	//GameCamera�N���X�̃|�C���^�B
	Level			m_level;				//���x���B
	Floor*			m_floor = nullptr;		//Floor�N���X�̃|�C���^�B
	ObjRigidbody*	m_objrb = nullptr;		//ObjRigidbody�N���X�̃|�C���^�B
	Ceiling*		m_ceiling = nullptr;	//Ceiling�N���X�̃|�C���^�B
	Player*			m_player = nullptr;		//Player�N���X�̃|�C���^�B
	Goal*			m_goal = nullptr;		//Goal�N���X�̃|�C���^�B
	Zombie*			m_zombie = nullptr;		//Zombie�N���X�̃|�C���^�B
	Key*			m_key = nullptr;		//Ball�N���X�̃|�C���^�B
	RockDoor*		m_rockdoor = nullptr;	//RockDoor�N���X�̃|�C���^�B
	BulletStack*	m_bs = nullptr;			//BulletStack�N���X�̃|�C���^�B
	Menu*			m_menu = nullptr;		//Menu�N���X�̃|�C���^�B
	Opening*		m_op = nullptr;			//Opening�N���X�̃|�C���^�B
	Result*			m_result = nullptr;		//Result�N���X�̃|�C���^�B
	UI*				m_ui = nullptr;			//UI�N���X�̃|�C���^�B
	Map*			m_map = nullptr;		//Map�N���X�̃|�C���^�B
	PointLight*		m_pl = nullptr;			//PointLight�N���X�̃|�C���^�B
	SpotLight*		m_sl = nullptr;			//SpotLight�N���X�̃|�C���^�B
	//��Ԋ֌W�B
	bool m_isClear = false;				//�N���A�������ǂ����B
	bool m_isGameOver = false;			//�Q�[���I�[�o�[�������ǂ����B
	bool m_isPose = false;				//�ꎞ��~�������ǂ����B
	SkinModelRender* m_PSOmodel;
	SkinModelRender* model;

};

static inline Game* GetGame()
{
	return Game::GetGame();
}