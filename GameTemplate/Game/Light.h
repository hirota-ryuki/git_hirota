#pragma once

#ifdef _CPP_
//CPP���̃R�[�h

using float4x4 = CMatrix;
using float4   = CVector4;

#endif

struct SSpotLight {
	float4 pos;
	float4 color;
	float4 direction;
	float range;
	float pad[3];
};