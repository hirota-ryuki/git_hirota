/*!
 * @brief	カプセルコライダー。
 */

#include "stdafx.h"
#include "Physics/BoxCollider.h"



/*!
	* @brief	デストラクタ。
	*/
BoxCollider::~BoxCollider()
{
	delete shape;
}
