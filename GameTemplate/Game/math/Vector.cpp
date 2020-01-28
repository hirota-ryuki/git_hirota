/*!
 * @brief	ベクトルクラス。
 */

#include "stdafx.h"
#include "Vector.h"
#include "Matrix.h"




/*!
*@brief	行列からクォータニオンを作成。
*/
void CQuaternion::SetRotation(const CMatrix& m)
{
	DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
}

/*!
*@brief	fromベクトルからtoベクトルに回転させるクォータニオンを作成。
*/
void CQuaternion::SetRotation(CVector3 from, CVector3 to)
{
	from.Normalize();
	to.Normalize();
	auto t = from.Dot(to);
	CVector3 rotAxis;
	if (t > 0.998f) {
		//ほぼ同じ向きなので単位クォータニオンにする。
		*this = CQuaternion::Identity();
	}
	else if (t < -0.998f) {
		//ほぼ逆向きなので、
		if (fabsf(to.x) < 1.0f) {
			//
			rotAxis.Cross(CVector3::AxisX(), to);
		}
		else {
			rotAxis.Cross(CVector3::AxisY(), to);
		}
	}
	else {
		rotAxis.Cross(from, to);
	}
	rotAxis.Normalize();
	SetRotation(rotAxis, acos(t));
}
