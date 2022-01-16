#ifndef MATRICA_H
#define MATRICA_H

#define VELICINA_MATRICE 10


extern float** memorija_nova;
float** inicijalizuj_memoriju();

struct matrica
{
    int kolona,red;
    float** mat;

    matrica();
    void dodeli(int,int,float**);
    matrica operator * (const matrica druga);
    void operator = (const matrica druga);
};

#endif
