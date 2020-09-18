
#include "MMA.h"


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
    if (N == 0) return;   // nN - global unsigned int
    permut = new unsigned int[N];

    //cout << "Constructor Chromosome\n";
    for(unsigned int j = 0; j < N; j++) {
        permut[j] = j;
    }
    crit = 0;

}

Chromosome::Chromosome(const Chromosome& c){
    if (N == 0) return;   //nN - global unsigned int, chromosome length = RSet.nZ
    permut = new unsigned int[N];
    for(unsigned int j = 0; j < N; j++) {
        permut[j] = c.permut[j];

    }
    crit = c.crit;
}

Chromosome::~Chromosome() {
    if (permut != NULL)  delete[] permut;
    crit = 0;
}

Chromosome& Chromosome::operator = (const Chromosome& c){
    if (this == &c) return *this;
    crit = c.crit;
    for (unsigned int i = 0; i < N; i++){
        permut[i] = c.permut[i];

    }
    return *this;
}

Chromosome& Chromosome::write(FILE* ft){
    unsigned int i;
    fprintf(ft, "%3d \n", N);
    for (i = 0; i < N; i++) fprintf(ft, "%3d  ", permut[i]);
    fprintf(ft, "\ncrit = %10d\n", crit);
    fprintf(ft, "\n");

    return *this;
}

bool Chromosome::UniformDistrRandPermut(){
    unsigned int i, j, v;
    // Перестановка из n элементов
    // Обратная перестановка (индексы) - для кроссовера
    // алгоритм Фишера-Йетса генерации случайной перестановки
    for (i = 0; i < N; i++) {
        j = (unsigned int) round(Rndm() * i);   // uniform random number from 0 to i
        if(i != j)  {
            v = permut[i];
            permut[i] = permut[j];
            permut[j] = v;
        }
    }
    return true;
}

unsigned int Chromosome::build_core(const RSet& RS){
    unsigned int i, j, k, l;
    unsigned int t;
    unsigned int* T = new unsigned int[RS.nT];

    nObjEstim++;

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

    delete[] T;
    return t;
}

int Chromosome::CompareCrit(const Chromosome& c){
   // Сравниние двух хромосом по значению критерия
   if ( crit < c.crit ) return -1;
   if ( crit > c.crit ) return +1;
   return 0;
};

int Chromosome::R1SearchSphere(const RSet& r){
    unsigned int i, j, t;  // , k = 0;
    bool o = 1;    // flag - the better solution is found
    Chromosome better(*this);

    while(o){
        o = 0;
        for(i = 0; i < N - 1; i++)
            for(j = i + 1; j < N; j++){
                t = permut[j];
                permut[j] = permut[i];
                permut[i] = t;
                build_core(r);

                if(better.crit > this->crit) {better = *this; better.crit = this->crit; o = 1;}
                permut[i] = permut[j];   // rebuilt permutation but crit value
                permut[j] = t;
            }
        *this = better;
    }

    return 0;
}


/*******************************************Population***************************************************/
/********************************************************************************************************/
Population::Population(){
    base = optSolCrit = optSolIndx = 0;
    popul = NULL;
}

Population::~Population(){
    if (popul != NULL ) delete popul;
    base = optSolCrit = optSolIndx = 0;
 }

Population::Population(const RSet& r, unsigned int b, unsigned int n){
    base = b;
    optSolCrit = n;
    optSolIndx = b;  // (b + 1)-th, out of range
    popul = new Chromosome[base];

    if ( popul == NULL ) cout << "Failed to allocate memory for Chromosome* popul\n" << endl;

    for(unsigned int i = 0; i < base; i++) {
        popul[i].UniformDistrRandPermut();
        popul[i].build_core(r);

        if (popul[i].crit <= optSolCrit){
            optSolCrit = popul[i].crit;
            optSolIndx = i;
        }
    };
    cout << "optSolIndx = " << optSolIndx << endl;
    cout << "optSolCrit = " << optSolCrit << endl;
}

Chromosome& Population::write(unsigned int k, FILE* f){
    return popul[k].write();
}
