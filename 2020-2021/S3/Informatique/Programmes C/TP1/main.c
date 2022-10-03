#include <stdio.h>
#include <stdlib.h>
#include <string.h>
float** CreerMatrice(int dimension)
{
    float** M;

    M=malloc(dimension*sizeof(float*));

    for (int i = 0; i < dimension; ++i)
    {
    	M[i]=malloc(dimension*sizeof(float));
    }

    // OU
    // float** M[dimension*sizeof(float)][dimension*sizeof(float)];


	return M;
}

void SaisieMatrice(float** M, int dimension,char* nom)
{
	printf("Saisir les valeurs de la matrice %s (%d x %d)",nom, dimension, dimension);

    float a;

	for (int i = 0; i < dimension; ++i)
	{
		for (int j = 0; j < dimension; ++j)
		{
		    printf("\nValeur : \t");
			scanf("%f",&a);

			M[i][j]=a;
		}
	}
}

void ProduitMatrice(float** A,float** B, float** C,int dimension)
{
    float temporaire;

	for (int i = 0; i < dimension; ++i)
	{
		for (int j = 0; j < dimension; ++j)
		{
		    temporaire = 0;

			for (int k = 0; k < dimension; ++k)
			{
				temporaire=temporaire+A[i][k]*B[k][j];
			}

			C[i][j]=temporaire;
		}
	}
}

void AfficheMatrice(float** C, int dimension)
{
	printf("\n");
	for(int i=0; i<dimension; i++)
    {
        for(int j=0; j<dimension; j++)
        {
            printf("%.2f\t",C[i][j]);
        }
        printf("\n");
	    }
}



int main()
{
	int dimension=2;

	float** A = CreerMatrice(dimension);
	float** B = CreerMatrice(dimension);
	float** C = CreerMatrice(dimension);

	SaisieMatrice(A, dimension, "A");
	SaisieMatrice(B, dimension, "B");

	ProduitMatrice(A,B,C, dimension);

	AfficheMatrice(C, dimension);

	return 0;
}
