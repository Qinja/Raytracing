#pragma once
#include "vec3.h"

#define M_PI       3.14159265358979323846


inline vec3 reflect(const vec3& v, const vec3& n) { return v - 2 * dot(v, n)*n; }

inline bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
	vec3 uv = v.normalize();
	float dt = dot(uv, n);			//cos theta 1
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);		//square of cos theta 2
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);			//refract
		return true;
	}
	else
		return false;
}

//·ÆÄù¶û½üËÆ¹«Ê½
inline float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 *= r0;
	return r0 + (1 - r0) *pow((1 - cosine), 5);
}