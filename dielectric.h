#pragma once
#include "scatter.h"
#include "mathutil.h"
class dielectric : public material {
public:
	dielectric(float ri):ref_idx(ri){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const {
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1, 1, 1);
		float reflect_prob;
		float cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
		vec3 refracted;
		if (dot(r_in.direction(), rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -cosine;
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_idx);
		}
		else 
		{
			scattered = ray(rec.p, reflected);
			reflect_prob = 1.0;
		}
		if(drand48() < reflect_prob)		//概率做反射折射的选择
			scattered = ray(rec.p, reflected);
		else
			scattered = ray(rec.p, refracted);
		return true;
	}
	float ref_idx;
};