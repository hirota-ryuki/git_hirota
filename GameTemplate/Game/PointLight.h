#pragma once
//#include "graphics/SkinModel.h"
class SkinModel;
class PointLight :
	public IGameObject
{
public:
	PointLight();
	~PointLight();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void SetLight(const CVector3& pos, const CVector3& color, const float& range, const int count) {
		SPointLight pl;
		pl.pos = pos;
		pl.color = color;
		pl.range = range;
		m_pointList[count] = pl;
		//m_pointList.push_back(sl);
	}
private:
	SPointLight			m_pointList[NUM_POINT_LIG];				//ポイントライトクラス。
	//std::vector<SPointLight> m_pointList;
};

