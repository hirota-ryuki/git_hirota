#pragma once
class Player;
class UI :
	public IGameObject
{
public:
	UI();
	~UI();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �t�H���g�̕\���A��\�����Ǘ�����֐��B
	/// </summary>
	void CangeActives(bool active) {
		m_fontCapacity->ActiveMode(active);
		m_fontStack->ActiveMode(active);
		m_fontLine->ActiveMode(active);
	}
private:
	//�c�e���֌W�B
	Game*			m_game = nullptr;				//Game�N���X�̃C���X�^���X�B
	Player*			m_player = nullptr;				//Player�N���X�̃|�C���^�B
	FontRender*		m_fontCapacity = nullptr;		//�c�e���̃t�H���g�����_�[�B
	FontRender*		m_fontStack = nullptr;			//���e���̃t�H���g�����_�[�B
	FontRender*		m_fontLine = nullptr;			//��؂���̃t�H���g�����_�[�B
};

