//#include <iostream>
//#include <stdio.h>
#include "MMA.h"
//#include "Define_file_name.h"

using namespace std;
/***********************************************RSet*****************************************************/
/********************************************************************************************************/

RSet::~RSet(){
    if (s != NULL){
        for (unsigned int i = 0; i < nZ; i++)  delete[] s[i];
        delete[] s;
        delete[] na;
        delete[] f;
        nT = nZ = 0;
        na = 0;
        f = 0;
        s = 0;
    }
}

/*
RSet& RSet::operator << (FILE* in){
    read(in);
    return *this;
}

RSet& RSet::operator >> (FILE* out){
    write(out);
    return *this;
}
*/
RSet& RSet::read(FILE* in){
    unsigned int i, j, k, v, r;
    char c;
    fscanf(in, "%d", &nT);
    fscanf(in, "%d", &nZ);
    f = new unsigned int[nZ];
    na = new unsigned int[nZ];
    s = new unsigned int* [nZ];
    for (i = 0; i < nZ; i++){
        fscanf(in, "%d %d", &v, &k);
        f[i] = v;
        na[i] = k;
        s[i] = new unsigned int[k];
        for (j = 0; j < k; j++){
            fscanf(in, "%d", &r);
            if(r > nT - 1) {cout << "Error! Sentence number > (nT - 1) ! j = " << j << " nT = " << nT << "\n"; }
            s[i][j] = r;
        }
        ///
        fscanf(in, "%c", &c);
        if(c != ';') cout << "Error: Endline semicolon is not found" << endl;
    }
    return *this;
}

RSet& RSet::write(FILE* ft){
    unsigned int i, j;
    fprintf(ft, "nT = %3d \n", nT);
    fprintf(ft, "nZ = %3d \n", nZ);
    for (i = 0; i < nZ; i++){
        fprintf(ft, "%3d f = %3d   %3dA: ", i, f[i], na[i]);
        for (j = 0; j < na[i]; j++) fprintf(ft, "%3d ", s[i][j]);
        fprintf(ft, "\n");
    }
    return *this;
}

/********************************************Chromosome**************************************************/
/********************************************************************************************************/
Chromosome::Chromosome() {
    if (n == 0) return;   // nN - global unsigned int
    permut = new unsigned int[n];
    //ind_permut = new unsigned int[n];
    //cout << "Constructor Chromosome\n";
    for(unsigned int j = 0; j < n; j++) {
        permut[j] = j;
        //ind_permut[j] = 0;
        //cout << j << " ";
    };
    crit = 0;
    //cout << "\n";
};

Chromosome::Chromosome(const Chromosome& c){
    if (n == 0) return;   //nN - global unsigned int, chromosome length = RSet.nZ
    permut = new unsigned int[n];
    //ind_permut = new unsigned int[n];
    for(unsigned int j = 0; j < n; j++) {
        permut[j] = c.permut[j];
        //ind_permut[j] = c.ind_permut[j];
    };
    crit = c.crit;
}

Chromosome::~Chromosome() {
    //cout << "~Chromosome here\n";
    if (permut != NULL)  delete[] permut;
    //if (ind_permut != NULL ) delete[] ind_permut;
    crit = 0;
};

Chromosome& Chromosome::operator = (const Chromosome& c){
    //cout << "\noperator = , crit = " << crit << " before\n";
    if (this == &c) return *this;
    crit = c.crit;
    //cout << "\noperator = , crit copied\n";
    for (unsigned int i = 0; i < n; i++){
        permut[i] = c.permut[i];
        //ind_permut[i] = c.ind_permut[i];
    }
    return *this;
}

Chromosome& Chromosome::write(FILE* ft){
    unsigned int i;
    fprintf(ft, "%3d \n", n);
    for (i = 0; i < n; i++) fprintf(ft, "%3d  ", permut[i]);
    fprintf(ft, "\ncrit = %10d\n", crit);
    fprintf(ft, "\n");

    return *this;
}

bool Chromosome::UniformDistrRandPermut(){
    unsigned int i, j, v;
    // Перестановка из n элементов
    // //Обратная перестановка (индексы) - для кроссовера
    // алгоритм Фишера-Йетса генерации случайной перестановки
    for (i = 0; i < n; i++) {
        j = (unsigned int) round(Rndm() * i);   // uniform random number from 0 to i
        if(i != j)  {
            v = permut[i];
            permut[i] = permut[j];
            permut[j] = v;
        }
    }
    //for( i = 0; i < n; i++)  ind_permut[permut[i]] = i;
    return true;
}

unsigned int Chromosome::build_core(const RSet& RS){
    unsigned int i, j, k, l;
    unsigned int t;
    unsigned int* T = new unsigned int[RS.nT];

    for(i = 0; i < RS.nT; i++) T[i] = RS.nT;

    l = permut[0];
    for(j = 0; j < RS.na[l]; j++){
        k = RS.s [l][j];
        T[k] = 0;
    }
    crit = RS.na[l];
    T[RS.f[l]] = 1;
    t = RS.na[l] + 1;

    for(i = 1; i < RS.nZ; i++){
        l = permut[i];
        for(j = 0; j < RS.na[l]; j++){
            k = RS.s[l][j];
            if(T[k] == RS.nT){
                T[k] = 0;
                crit++;
                t++;
            }

        }
        // add f-sentence to T1-set, if not yet
        if (T[RS.f[l]] == RS.nT) {
            T[RS.f[l]] = 1;
            t++;
        }
        if(t == RS.nT) break;

    }
    /*
    // Output of core-set T0
    cout << endl << "nS = " << t << endl;
    cout << "\n T0 = {";
    for(i = 0; i < RS.nT; i++)  if(T[i] == 0) cout << "  " << i;
    cout << "  }\n";
    */
    delete[] T;
    return t;
}

int Chromosome::CompareCrit(const Chromosome& c){
   // Сравниние двух хромосом по значению критерия
   if ( crit < c.crit ) return -1;
   if ( crit > c.crit ) return +1;
   return 0;
};

