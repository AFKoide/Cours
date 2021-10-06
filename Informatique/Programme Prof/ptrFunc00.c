#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void afficheMath(float xmin, float xmax, float pas,double (*f)(double x))
{

  float x;
  for(x = xmin; x < xmax; x = x+pas)
  {
    printf("%f %f\n",x,(*f)(x));
  }

}
int main()
{

afficheMath(-10,10,0.1,exp);


}
