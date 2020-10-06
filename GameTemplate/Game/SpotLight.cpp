#include "stdafx.h"
#include "SpotLight.h"

SpotLight::SpotLight()
{
}
SpotLight::~SpotLight()
{
}

void SpotLight::OnDestroy()
{
}

bool SpotLight::Start()
{
	return true;
}

void SpotLight::Update()
{
	SkinModel::SetSpotLight(m_spotList);
}
