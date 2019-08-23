#pragma once
#include "hitable.h"

class sphere {
public:
	sphere(){}
	sphere(vec3 c, float r,material *mat_p) :center(c), radius(r), mat(mat_p){}
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const;
	vec3 center;
	float radius;
	material *mat;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());	//∞—1/2‘ºµÙ¡À
	float c = dot(oc, oc) - radius * radius;
	float dis = b * b - a*c;
	if (dis < 0)
		return false;
	float tmp = (-b - sqrt(b*b - a * c)) / a;
	if (tmp < t_max&& tmp>t_min)
	{
		rec.t = tmp;
		rec.p = r.point_at_parameter(tmp);
		rec.normal = (rec.p - center) / radius;
		rec.mat_ptr = mat;
		return true;
	}
	tmp = (-b + sqrt(b*b - a * c)) / a;
	if (tmp < t_max&& tmp>t_min)
	{
		rec.t = tmp;
		rec.p = r.point_at_parameter(tmp);
		rec.normal = (rec.p - center) / radius;
		rec.mat_ptr = mat;
		return true;
	}
	return false;
}