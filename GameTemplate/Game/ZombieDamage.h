#pragma once
#include "IZombieComponent.h"

/// <summary>
/// ゾンビのダメージ処理。
/// </summary>
class ZombieDamage : public IZombieComponent
{
public:
	void Update()override;
private:
	const int		HEAD_SHOT_DAMAGE = 5;					//ヘッドショットのダメージ量。
	const int		NORMAL_DAMAGE = 1;						//通常のダメージ量。
	const float		RENGE_OF_HEAD_COLLISION = 20.0f;		//頭の当たり判定の大きさ。
	const float		RENGE_OF_BODY_COLLISION = 60.0f;		//体の当たり判定の大きさ。
	const float		RAISE_CENTER_BODY = 70.0f;				//当たり判定は球体なので足元から体の中心に上げるための上昇量。

};

