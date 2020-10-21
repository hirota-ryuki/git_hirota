#pragma once
class Zombie;
/// <summary>
/// �]���r�̃R���|�[�l���g
/// </summary>
class IZombieComponent
{
public:
	/// <summary>
	/// �]���r���֘A�t����B
	/// </summary>
	/// <param name="zombie"></param>
	void BindZombie(Zombie* zombie)
	{
		m_zombie = zombie;
	}
	virtual ~IZombieComponent(){}
	virtual void Start() {}
	virtual void Update() {}
protected:
	Zombie* m_zombie = nullptr;	//���̃R���|�[�l���g�̏��L�҂̃]���r�B
};

