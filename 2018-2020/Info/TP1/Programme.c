#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//**********************************************************************************
//Exercice 1.1.1
int main()
{
	float tableau[5000];
	int modulo;
	for (int i = 0; i < 5000; i++)
	{
		tableau[i]=i*i;

		modulo=i%100;
		if(modulo==0)
			printf("%f \t",tableau[i]);
	}
	return 0;
}

//**********************************************************************************
//Exercice 1.1.2
int main()
{
    float* T;
    T=malloc(5000*sizeof(float));

	int modulo;
	for (int i = 0; i < 5000; i++)
	{
        T[i]=i*i;

		modulo=i%100;
		if(modulo==0)
			printf("%f \n",T[i]);
	}
	return 0;
}

//**********************************************************************************
//Exercice 1.2.1
int nbnote()
{
	int note;
	printf("Combien de note à rentrer ?\n");
	scanf("%d", &note);

	return note;
}

int main()
{
	int nombre_note, i, note;
    float moyenne;
	nombre_note=nbnote();
	printf("\n%d note a rentrer.\n",nombre_note);

	float* T;
	T=malloc(nombre_note*sizeof(float));

	for (i = 0; i < nombre_note; ++i)
	{
		printf("\n%d eme note : ",i);
		scanf("%d",&note);

		T[i]=note;
		printf("%f\t",T[i]);

		moyenne=T[i]+moyenne;
		printf("%f\t",moyenne);
	}

	moyenne=moyenne/nombre_note;

	printf("\nLa moyenne de ce tableau est %f \n", moyenne);
}

//**********************************************************************************
//Exercice 1.3
void jms(int temps_init, int* jour, int* heure, int* minute, int* seconde)	//C'est des pointeurs, donc *
{
	int reste_jour, reste_heure, reste_minute;

	*jour=temps_init/(24*60*60);	//Pour réécrire le pointeur, on doit mettre un * devant le nom du pointeur.
	reste_jour=temps_init%(24*60*60);

	*heure=reste_jour/(60*60);
	reste_heure=reste_jour%(60*60);

	*minute=reste_heure/(60);
	reste_minute=reste_heure%(60);

	*seconde=reste_minute;
}

int main()
{
	int temps_init, jour, heure, minute, seconde;	//Initialisation des variables

	printf("Le nombre de secondes à calculer est :  ");
	scanf("%d",&temps_init);
	jms(temps_init, &jour, &heure, &minute, &seconde);
//On récupère les adresses des variables des jours, minutes et secondes et on les donne a la fonction jms.	

	printf("calcule terminé :\n %d jours \n %d heures \n %d minutes \n %d secondes",jour, heure, minute, seconde);
}


//**********************************************************************************
//Exercice de 1.4.1 à 1.4.4
void Taille_Chaine(int* taille, char* chaine)
{
    int i=0;

    while(chaine[i]!=0)
    {
        i++;
    }
    *taille=i;
}

void Inverse_Chaine(int* taille, char* chaine, char* inverse)
{
	int i=0, longueur=*taille;

    longueur --;
	while(chaine[i]!=0)
    {
        inverse[i] = chaine[longueur];
        longueur --;
        i++;
    }
    inverse[i]=0;
}

void Concatenation(int* taille, char* chaine1, char* chaine2)
{
	int i, longueur=*taille,;

	for (i = longueur ; chaine2[j]!=0 ; ++i)
	{
		j++;
		chaine1[i]=chaine2[j];
	}
}

int main()
{
	int taille;
//	char chaine[] = "BONJOUR";
	char chaine[500] ;

	printf("Inserer texte :\n");
	scanf("%s",chaine);

	Taille_Chaine(&taille, chaine);
	printf("\nLa chaine %s fait %d caractères\n",chaine, taille);

	char inverse[taille];

	Inverse_Chaine(&taille, chaine, inverse);
	printf("L'inverse de cette chaine est %s",inverse);
}



//**********************************************************************************
// Exercice 1.4.5
void Taille_Chaine(int* taille, char* chaine)
{
    int i=0;

    while(chaine[i]!=0)
    {
        i++;
    }
    *taille=i;
}

void Inverse_Chaine_V2(int* taille, char* chaine)
{
	int i=0, longueur=*taille;
	char temporaire;

	longueur --;
	while(i!=longueur)
	{
		temporaire=chaine[i];

		chaine[i]=chaine[longueur];
		chaine[longueur]=temporaire;

		i++;
		longueur--;
	}
}

int main()
{
	int taille;
//	char chaine[] = "BONJOUR";
	char chaine[500] ;

	printf("Inserer texte :\n");
	scanf("%s",chaine);

	Taille_Chaine(&taille, chaine);
	printf("\nLa chaine %s fait %d caractères\n",chaine, taille);

	Inverse_Chaine(&taille, chaine);
	printf("L'inverse de cette chaine est %s",chaine);
}


//**********************************************************************************
// Exercice 1.4.6
void Taille_Chaine(int* taille, char* chaine)
{
    int i=0;

    while(chaine[i]!=0)
    {
        i++;
    }
    *taille=i;
}

void Concatenation(int* taille, char* chaine1, char* chaine2, char* chainefinal)
{
	int i, longueur=*taille;
    int j=0;

	for (i = 0 ; chaine1[i]!=0 ; ++i)
	{
		chainefinal[i]=chaine1[i];
	}
	i++;
	for (j = 0; chaine2[j]; ++j)
	{
		chainefinal[i]=chaine2[j];
	}
}


int main()
{
	int taille1, taille2;
	char chaine1[], chaine2[];

	printf("Inserer texte :\n");
	scanf("%s",chaine1);
    printf("\nInserer texte :\n");
	scanf("%s",chaine2);

	Taille_Chaine(&taille1, chaine1);
	printf("\nLa chaine %s fait %d caractères\n",chaine1, taille1);
    Taille_Chaine(&taille2, chaine2);
	printf("\nLa chaine %s fait %d caractères\n",chaine2, taille2);


	char chainefinal[];
	chainefinal=malloc((taille1+taille2+1)*sizeof(char));


	Concatenation(&taille1, chaine1, chaine2, chainefinal);
	printf("Ensemble, on obtient : %s \n",chaine1);
}





//**********************************************************************************
//Exercice 1.5
int main(void)
{
    unsigned int i;
    int T[20];
    int* pL;
    unsigned short* pS;
    unsigned char* pC;

    pL=T;
    pS=(unsigned short*)T;
    pC=(unsigned char*)T;



    for(i=0;i<5;i++)
    {
        T[i]=253+i;
    }

    printf("\nResultats :\n");
    for(i=0;i<20;i++)
	{
        printf("\n%20d%20d%20d",pL[i],(int)pS[i],(int)pC[i]);
	}

	return 0;
}

//**********************************************************************************
//Exercice 1.6.1
float* CreerMatrice( int dimension)
{
    float* M;
	M = malloc(dimension*dimension*sizeof(float));

	return M;
}

void SaisieMatrice(float* M, int dimension,char* nom)
{
	printf("Valeur de la matrice %s (%d x %d) :\n",nom, dimension, dimension);

	float a;

	for (int i = 0; i < dimension; ++i)
	{
		for (int j = 0; j < dimension; ++j)
		{
			printf("Valeur : \t");
			scanf("%f",&a);

			M[i*dimension+j] = a;
		}
	}
}

void ProduitMatrice(float* A,float* B, float* C,int dimension)
{
    float temporaire;

	for (int i = 0; i < dimension; ++i)
	{
		for (int j = 0; j < dimension; ++j)
		{
		    temporaire = 0;

			for (int k = 0; k < dimension; ++k)
			{
				temporaire=temporaire+A[i*dimension+k]*B[dimension*k+j];
			}

			C[i*dimension+j]=temporaire;
		}
	}
}

void AfficheMatrice(float* C, int dimension)
{

    printf("\n");
	for(int i=0; i<dimension; i++)
    {
        for(int j=0; j<dimension; j++)
        {
            printf("%f\t",C[i*dimension+j]);
        }
        printf("\n");

    }
}



int main()
{
	int dimension = 2;

	float* A = CreerMatrice( dimension);
    float* B = CreerMatrice( dimension);
    float* C = CreerMatrice(dimension);


	SaisieMatrice(A, dimension,"A");
	SaisieMatrice(B, dimension,"B");

	ProduitMatrice(A,B,C, dimension);

	AfficheMatrice(C, dimension);


	return 0;
}

//**********************************************************************************
//1.6.2
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
