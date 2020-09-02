#pragma once
class SkinModel;

class SpotLight :
	public IGameObject
{
public:
	SpotLight();
	~SpotLight();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void SetLight(const CVector3& pos, const CVector3& color, const CVector3& direction, const float& range, const int count = 0) {
		SSpotLight sl;
		sl.pos = pos;
		sl.color = color;
		sl.direction = direction;
		sl.range = range;
		m_spotList[count] = sl;
	}
private:
	SSpotLight			m_spotList[NUM_SPOT_LIG];				//ポイントライトクラス。
	//std::vector<SSpotLight> m_spotList;
};

