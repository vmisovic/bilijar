#include"matrica.h"

#include<iostream>
using namespace std;

matrica::matrica()
{
	kolona=3;
	red=3;
}

float** inicijalizuj_memoriju()
{
	float** memorija=new float*[VELICINA_MATRICE];
	for(int i=0;i<VELICINA_MATRICE;i++) memorija[i] = new float[VELICINA_MATRICE];

	return memorija;
}

void matrica::dodeli(int kolona1, int red1, float** mat1)
{
	kolona=kolona1;
	red=red1;
	for(int i=0;i<kolona;i++) for(int j=0;j<red;j++) mat[i][j]=mat1[i][j];
}

matrica matrica::operator * (const matrica druga)
{
	if(kolona!=druga.red)
	{
		cout<<"ne mogu se pomnoziti matrice\n";
		exit(1);
	}

	matrica nova;
	nova.kolona=red;
	nova.red=druga.kolona;
	nova.mat=memorija_nova;

	for(int i=0;i<red;i++)
	{
		for(int j=0;j<druga.kolona;j++)
		{
			float sum=0;
			for(int k=0; k<druga.red; k++) sum+=mat[i][k]*druga.mat[k][j];
			nova.mat[i][j]=sum;
		}
	}

	return nova;
}

void matrica::operator = (const matrica druga)
{
	dodeli(druga.kolona,druga.red,druga.mat);
}
