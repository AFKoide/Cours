#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	float* f;
    f = malloc(100*sizeof(float));
    int dimension = 50;

	for (int i = 0; i < 50; ++i)
	{
        f[i]=i;
		f[i+dimension]=exp(i/50.0);
		printf("%.0f : %f\n",f[i], f[i+dimension]);
	}
}