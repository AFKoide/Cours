#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void main()
{
    float* T = malloc(100*sizeof(float));
    int dimension = 50;

    FILE * sortie ;
    sortie = fopen("sortie.txt","w");

    for(int i=0; i<50; i++)
    {
        T[i] = i+1;
        T[i+50] = exp(i/50.0);

        printf("%.0f : %f\n",T[i], T[i+dimension]);

        fprintf(sortie,"%.0f : %f\n",T[i], T[i+dimension]);
    }

    fclose(sortie);
}
