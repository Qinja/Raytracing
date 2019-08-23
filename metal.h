#pragma once
#include "scatter.h"
#include "random.h"
#include "mathutil.h"

class metal : public material {
public:
	metal(const vec3& a, float f) :albedo(a){ if (f < 1)fuzz = f; else fuzz = 1; }
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 reflected = reflect(r_in.direction().normalize(), rec.normal);
		
		scattered = ray(rec.p, reflected+fuzz *randomInUnitSphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	vec3 albedo;
	float fuzz;
};