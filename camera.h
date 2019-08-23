#pragma once
#include "ray.h"

class camera {
public:
	camera(vec3 position, vec3 lookat, vec3 up, float vfov, float aspect)
	{
		vec3 u, v, w;
		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = position;
		w = (position - lookat).normalize();
		u = cross(up, w).normalize();
		v = cross(w, u);
		bottom_left = origin - half_width * u - half_height * v - w;
		horizontal = 2 * half_width * u;
		vertical = 2 * half_height * v;
	}
	ray get_ray(float u, float v) { return ray(origin, bottom_left + u * horizontal + v * vertical - origin); }
	vec3 bottom_left;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
};