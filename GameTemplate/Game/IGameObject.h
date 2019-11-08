#pragma once
class IGameObject
{
public:
	//���N���X�ɂ͉��z�f�X�g���N�^���K�v
	//�N�\
	virtual ~IGameObject(){}
	/// <summary>
	/// �X�V�֐��B
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	///  �`��֐��B
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �폜���N�G�X�g���s���B
	/// </summary>
	void RequestDelete()
	{
		isRequestDelete = true;
	}

	/// <summary>
	/// �폜���N�G�X�g���󂯂Ă��邩���ׂ�B
	/// </summary>
	/// <returns>
	/// true���Ԃ��Ă�����A���N�G�X�g���󂯂Ă���B
	/// </returns>
	bool IsRequestDelete()
	{
		return isRequestDelete;
	}
private:
	bool isRequestDelete = false;		//�폜�t���O
};

