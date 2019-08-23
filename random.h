#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "vec3.h"
#include <stdlib.h>  

#define drand_m 0x100000000LL  
#define drand_c 0xB16  
#define drand_a 0x5DEECE66DLL

static unsigned long long seed = 1;

double drand48(void)
{
	seed = (drand_a * seed + drand_c) & 0xFFFFFFFFFFFFLL;
	unsigned int x = seed >> 16;
	return  ((double)x / (double)drand_m);

}

void srand48(unsigned int i)
{
	seed = (((long long int)i) << 16) | rand();
}

vec3 randomInUnitSphere() {
	vec3 p;
	do
	{
		p = vec3(drand48(), drand48(), drand48());
	} while (dot(p,p) >= 1.0f);
	return p;
}

#endif