#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float Re;
    float Im;
} complexe;

int main()
{
    int i, j, k;
    complexe C1, C2, C3;

    C1.Re = 1.5;
    C1.Im = -3;

    C2.Re=-7;
    C2.Im=4;

    C3.Im = C1.Im + C2.Im;
    C3.Re = C1.Re + C2.Re;

    printf("%f + %f i",C3.Re,C3.Im);

    return 0;
}