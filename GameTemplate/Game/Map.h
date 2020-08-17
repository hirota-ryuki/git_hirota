#pragma once
class Player;
class Map :
	public IGameObject
{
public:
	Map();
	~Map();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
private:
	Game*			m_game = nullptr;						//Game�N���X�̃C���X�^���X�B
	Player*			m_player = nullptr;						//Player�N���X�̃|�C���^�B
	SpriteRender*	m_mapSprite = nullptr;
	//CVector2		m_mapSpriteSize = { 850.0f,500.0f };	//�\�������摜�̑傫���B
	CVector2		m_mapSpriteSize = { 20.0f*37.0f,20.0f*27.0f };	//�\�������摜�̑傫���B
	CVector2		m_mapSpritePos = { 0.0f,0.0f };			//�\�������摜�̍��W�B
	SpriteRender*	m_playerSprite = nullptr;
	//CVector2		m_mapSize = { 17000.0f,10000.0f };		//���ۂ̃t�B�[���h�̑傫���B
	CVector2		m_mapSize = { 3700.0f,2700.0f };		//���ۂ̃t�B�[���h�̑傫���B
	CVector2		m_ratio = CVector2::Zero();				//�t�B�[���h����摜�ւ̏k�����B�摜/�t�B�[���h�B
	bool			m_isMap = false;						//�}�b�v��ʂ��\������Ă��邩�ǂ����B
};