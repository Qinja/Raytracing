#pragma once
#include "hitable.h"
#include "sphere.h"

class  hitable_list  {
public:
	hitable_list(){}
	hitable_list(sphere **l, int n): list (l), list_size(n){}
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const;
	sphere** list;
	int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec)const {
	hit_record tmp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->hit(r, t_min, closest_so_far, tmp_rec))	//找到最近的hit，缩小范围。
		{
			hit_anything = true;
			closest_so_far = tmp_rec.t;
			rec = tmp_rec;		//记录的是最近的hit rec
		}
	}
	return hit_anything;
}