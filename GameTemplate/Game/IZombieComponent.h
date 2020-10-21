#pragma once
class Zombie;
/// <summary>
/// ゾンビのコンポーネント
/// </summary>
class IZombieComponent
{
public:
	/// <summary>
	/// ゾンビを関連付ける。
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
	Zombie* m_zombie = nullptr;	//このコンポーネントの所有者のゾンビ。
};

