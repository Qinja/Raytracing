#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include "bmp.h"
#include "vec3.h"
#include <omp.h>
#include "ray.h"
#include "scatter.h"
#include "lambertian.h"
#include "metal.h"
#include "hitable.h"
#include "hitablelist.h"
#include "sphere.h"
#include <float.h>
#include "camera.h"
#include "random.h"
#include "dielectric.h"
#define MAX_DEPTH 200


vec3 get_color_recursive(const ray& r, const hitable_list *world, const int maxDepth)
{
	hit_record rec;
	if (world->hit(r, 0.1, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (maxDepth > 0 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * get_color_recursive(scattered, world, maxDepth - 1);
		else
			return vec3(0, 0, 0);
	}
	else {
		vec3 unitdir = r.direction().normalize();
		float k = 0.5 *(unitdir.y() + 1.0);
		return (1.0 - k)*vec3(1.0, 1.0, 1.0) + k * vec3(0.4, 0.6, 1.0);
	}
}

vec3 get_color(const ray& r, const hitable_list *world, const int maxDepth)
{
	hit_record tmprec;
	vec3 tmpc(1, 1, 1);
	ray tmpr = r;
	vec3 attenuation;
	ray scattered;
	for (int d = 0; d < maxDepth; d++)
	{
		if (world->hit(tmpr, 0.1, FLT_MAX, tmprec))
		{
			if (tmprec.mat_ptr->scatter(tmpr, tmprec, attenuation, scattered))
			{
				tmpc = tmpc * attenuation;
				tmpr = scattered;
			}
			else
			{
				tmpc = vec3(0, 0, 0);
				break;
			}
		}
		else
		{
			vec3 unitdir = tmpr.direction().normalize();
			float k = 0.5 *(unitdir.y() + 1.0);
			tmpc = tmpc * ((1.0 - k)*vec3(1.0, 1.0, 1.0) + k * vec3(0.4, 0.6, 1.0));
			break;
		}
	}
	return tmpc;
}


hitable_list* random_scene(const int n)
{
	sphere** list = new sphere*[n + 1];
	list[0] = new sphere(vec3(0, -1000, -1), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = drand48();
			vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9*drand48());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)//diffuse
				{
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
				}
				else if(choose_mat <0.95)
				{
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())), 0.5*(1 + drand48())));
				}
				else
				{
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	return new hitable_list(list, i);
}

int main(int argc, char *argv[])
{
	int nx = 800;
	int ny = 480;
	int ns = 10;
	if (argc == 2)
	{
		ns = std::atoi(argv[1]);
	}
	else if (argc == 4)
	{
		nx = std::atoi(argv[1]);
		ny = std::atoi(argv[2]);
		ns = std::atoi(argv[3]);
	}
	printf("RayTracing: \n nx= %d \t ny=%d \t ns=%d\n", nx, ny, ns);
#ifdef _OPENMP
	printf("OpenMP ready!\n");
#else
	printf("OpenMP not supported\n");
#endif
	system("pause");


	sphere *list1[5];
	list1[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list1[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list1[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2),0));
	list1[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list1[4] = new sphere(vec3(-1, 0, -1), -0.48, new dielectric(1.5));

	sphere *list2[2];
	float R = cos(M_PI / 4);
	list2[0] = new sphere(vec3(-R, 0, -1), R, new lambertian(vec3(0, 0, 1)));
	list2[1] = new sphere(vec3(R, 0, -1), R, new lambertian(vec3(1, 0, 0)));

	hitable_list *list3 = random_scene(500);

	hitable_list *world;
	//world= new hitable_list(list1, 4);
	world = list3;

	camera cam(vec3(5, 2, 3), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(nx) / float(ny));
	vec3 *results = new vec3[ny*nx];
	float startTime = omp_get_wtime();
	float pixelTime = omp_get_wtime();

	for (int j = ny - 1; j >= 0; j--)
	{
#pragma omp parallel for
		for (int i = 0; i < nx; i++) {
			vec3 color(0,0,0);
			for (int k = 0; k < ns; k++)
			{
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.get_ray(u, v);
				color += get_color(r, world, MAX_DEPTH);
			}
			color /= float(ns);
			color = vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));		//1/2的伽马校正
			results[nx*j + i] = color;
		}
		float endTime = omp_get_wtime();
		printf("【%d%%】\t第%d行\t执行时间: %2.0f ms", (int)((1.0f - 1.0f*j / ny) * 100), ny - j, (endTime - pixelTime) * 1000);
		printf("    \t已使用时间: %2.0f s",endTime - startTime);
		printf("\t预计剩余时间: %2.0f s\n", (endTime - startTime) / (ny - j)*j);
		pixelTime = endTime;
	}
	float endTime = omp_get_wtime();
	printf("渲染完成，执行总时间: %2.0f s\n", endTime - startTime);
	char filename[20] = {0};
	sprintf(filename, "out%dx%d-%d.bmp", nx, ny, ns);
	WriteBmp(nx, ny, results, filename);
	printf("写入BMP\n"); 
	system("pause");
	system(filename);
	return 0;
}