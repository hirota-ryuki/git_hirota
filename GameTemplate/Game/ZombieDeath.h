#pragma once
#include "IZombieComponent.h"

/// <summary>
/// ゾンビの死んだときの処理。
/// </summary>
class ZombieDeath : public IZombieComponent
{
public:
	void Update()override;
};

