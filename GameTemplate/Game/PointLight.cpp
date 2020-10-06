#include "stdafx.h"
#include "PointLight.h"
#include <random>

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}

void PointLight::OnDestroy()
{
}

bool PointLight::Start()
{
	SkinModel::SetPointLight(m_pointList);
	return true;
}

void PointLight::Update()
{
	
}
