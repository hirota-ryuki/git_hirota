/*!
 * @brief	カプセルコライダー。
 */

#pragma once

#include "Physics/ICollider.h"


class BoxCollider : public ICollider
{
public:
	/*!
	* @brief	デストラクタ。
	*/
	~BoxCollider();
	/*!
	* @brief	作成。
	*@param[in] radius	カプセルの半径。
	*@param[in]	height	カプセルの高さ。
	*/
	void Create(CVector3 halfSize)
	{
		btVector3 btSize;
		btSize.setX(halfSize.x);
		btSize.setY(halfSize.y);
		btSize.setZ(halfSize.z);
	
		shape = new btBoxShape(btSize);
		this->radius = radius;
		this->height = height;
	}
	btCollisionShape* GetBody() const override
	{
		return shape;
	}
	float GetRadius() const
	{
		return radius;
	}
	float GetHeight() const
	{
		return height;
	}
private:
	btBoxShape*		shape = nullptr;
	float radius = 0.0f;
	float height = 0.0f;
};
