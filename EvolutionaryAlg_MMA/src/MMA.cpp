
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
    ind_permut = new unsigned int[N];

    for(unsigned int j = 0; j < N; j++) {
        permut[j] = j;
        ind_permut[j] = 0;
    }
    crit = 0;
}

Chromosome::Chromosome(const Chromosome& c){
    if (N == 0) return;   //nN - global unsigned int, chromosome length = RSet.nZ
    permut = new unsigned int[N];
    ind_permut = new unsigned int[N];
    for(unsigned int j = 0; j < N; j++) {
        permut[j] = c.permut[j];
        ind_permut[j] = c.ind_permut[j];
    };
    crit = c.crit;
}

Chromosome::~Chromosome() {
    if (permut != NULL)  delete[] permut;
    if (ind_permut != NULL ) delete[] ind_permut;
    crit = 0;
}

Chromosome& Chromosome::operator = (const Chromosome& c){
    if (this == &c) return *this;
    crit = c.crit;
    for (unsigned int i = 0; i < N; i++){
        permut[i] = c.permut[i];
        ind_permut[i] = c.ind_permut[i];
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
    /// Permutation of n elements
    /// Inverse permutation (indices) - for crossover
    /// Fisher–Yates shuffle for random permutation generating
    for (i = 0; i < N; i++) {
        j = (unsigned int) round(Rndm() * i);   // uniform random number from 0 to i
        if(i != j)  {
            v = permut[i];
            permut[i] = permut[j];
            permut[j] = v;
        }
    }
    for( i = 0; i < N; i++)  ind_permut[permut[i]] = i;
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
        k = RS.s[l][j];
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
   /// Comparison of two chromosomes by criterion value
   if ( crit < c.crit ) return -1;
   if ( crit > c.crit ) return +1;
   return 0;
};


Chromosome& Chromosome::Crossover(const Chromosome& c1, const Chromosome& c2) {
    unsigned int i, j1, j2;

    Chromosome h1(c1), h2(c2);

    j1 = 0; j2 = 0;

    for (i = 0; i < N; i++) {
        if (j1 < N) {
            while ( h1.permut[j1] == N ) {
                j1 = j1 + 1;
                if (j1 == N) break;
            }
        }
        if ( j2 < N ) {
            while ( h2.permut[j2] == N ) {
                j2 = j2 + 1;
                if ( j2 == N ) break;
            }
        }
        if ( (j1 < N)  &&  (j2 == N) ) {
            permut[i] = h1.permut[j1];
            h1.permut[j1] = N;
        } else if ( (j2 < N) && (j1 == N) ) {
            permut[i] = h2.permut[j2];
            h2.permut[j2] = N;
        } else if ( (j2 < N) && (j1 < N) ) {
            if ( Rndm() < 0.5 ) {
                permut[i] = h1.permut[j1];
                h2.permut[h2.ind_permut[permut[i]]] = N;
                h1.permut[j1] = N;
            } else {
                permut[i] = h2.permut[j2];
                h1.permut[h1.ind_permut[permut[i]]] = N;
                h2.permut[j2] = N;
            }

        }
        ind_permut[permut[i]] = i;
    }
    return *this;
}

bool Chromosome::Mutation(double p0){
    bool fl = false;
    if ( Rndm() < p0 ) {
        unsigned int j1 = (int)round( Rndm() * (N - 1) );
        unsigned int j2 = (int)round( Rndm() * (N - 1) );
        if ( j1 != j2)  {
            unsigned int v = permut[j1];
            permut[j1] = permut[j2];
            permut[j2] = v;
            ind_permut[permut[j1]] = j1;
            ind_permut[permut[j2]] = j2;
            fl = true;
        }
    }
    return fl;
}

/*************************************TriangularDistribution*********************************************/
/********************************************************************************************************/
/// Constructs triangular distribution with parameters h and m, fills sample[] - sample of size n
TriangularDistribution::TriangularDistribution(double xh, double xm, unsigned int xn){
    h = xh;
    m = xm;
    n = xn;
    sample = new double[n];
    unsigned int i;
    unsigned int s = (unsigned int) m;
    double v;

    for(i = 0; i < n; i++) sample[i] = 0;

    double rn, ri;
    rn = n;
    ///sample[0] = 0 !!! always!/**************************!!!!!!!!!**********/
    for(i = 0; i < n; i++) {
        ri = i;
        if (i <= s )  {
            v =  (2. +  h * ri / m) / rn;
            sample[i] = v * ri / (h + 2.);
        } else {
            v = (2. + h + (h / (rn - m)) * (rn - ri)) / rn;
            sample[i] = v * (ri - m) / (h + 2.) + sample[s];
        }
    }
}

TriangularDistribution::~TriangularDistribution(){
    if (sample != NULL) delete[] sample;
    n = 0;
    m = h = 0.0;
}

unsigned int TriangularDistribution::sampling(double p){
    unsigned int i, i1, j1;
    i1 = 0;
    j1 = n;
    i = (unsigned int) (n / 2);
    while (i != i1) {
        if (p < sample[i]) {
            j1 = i;
        } else {
            i1 = i;
        }
        i = (unsigned int)  (i1 + (j1 - i1) / 2);
    }
    return i;
}

/*******************************************Population***************************************************/
/********************************************************************************************************/
Population::Population(){
    base = descendants = 0;
    h = m = p_mut = 0.0;
    popul = NULL;
    populIndexes = NULL;
    pProb = NULL;
}

Population::~Population(){
    if (populIndexes != NULL ) delete[] populIndexes;
    if (pProb != NULL ) delete pProb;
    base = descendants = 0;
    h = m = p_mut = 0.0;
}

Population::Population(unsigned int b, unsigned int d, double hight, double mean, double pMut){
    base = b;
    descendants = d;
    h = hight;
    m = mean;
    p_mut = pMut;
    popul = new Chromosome[base + descendants];

    if ( popul == NULL ) cout << "Failed to allocate memory for Chromosome* popul\n" << endl;

    populIndexes = new unsigned int[base + descendants];
    if ( populIndexes == NULL ) cout << "Failed to allocate memory for unsigned int* populIndexes\n" << endl;

    for ( unsigned int i = 0; i < (base + descendants); i++ ) populIndexes[i] = i;
    /// Probability distribution for individuals` selecting for crossovering
    pProb = new TriangularDistribution(h * base, m * base, base);
}

bool Population::ChromInsert(unsigned int n, unsigned int m){
    /// Binary search
    /// m -th element of the array looks for its place,  m > n!

    if (m < n) return false;

    unsigned int i, j;
    i = populIndexes[n];
    j = populIndexes[m];
    /// New element is compared with last element of Population, if it is worse then return
    if ( popul[i].CompareCrit(popul[j]) <= 0 ) return true;

    unsigned int k, p = 0, q = n, i0;
    int cmp;

    if (n > 0){
        while ( q >= p ){
            k = p + (unsigned int) ( ( q - p + 1) / 2 );
            i = populIndexes[k];
            cmp = popul[i].CompareCrit(popul[j]);
            if (k == 0){
                if (cmp == 1) i0 = k;
                else i0 = k + 1;
                break;
            }

            if (cmp == 0){
                i0 = k;
                break;
            } else if( cmp == -1 ){
                p = k + 1;
                i0 = k;
            } else {
                q = k - 1;
                i0 = q;
            }
        }
    } else {  // n = 0
        populIndexes[n] = m;        // m = 1
        populIndexes[n + 1] = n;

        return true;
    }


    i = populIndexes[n];

    for (k = n; k > i0; k--)  populIndexes[k] = populIndexes[k - 1];

    populIndexes[i0] = j;
    populIndexes[m] = i;

    return true;
}

void Population::InitPop(const RSet& r){
    for(unsigned int i = 1; i < base; i++) {
        popul[i].UniformDistrRandPermut();
        popul[i].build_core(r);

        /// Element insertion into index array
        ChromInsert(i - 1, i);   // 2 indexes

    };

}

void Population::NextGen(const RSet& r){
    unsigned int i, j, k;
    /// forming of the set of pairs
    for( k = 0; k < descendants; k++) {
        i = pProb->sampling(Rndm());
        j = pProb->sampling(Rndm());

        /// crossover and mutation
        if (i != j)  popul[populIndexes[base + k]].Crossover(popul[populIndexes[i]], popul[populIndexes[j]]);
        else         popul[populIndexes[base + k]] = popul[populIndexes[j]];

        popul[populIndexes[base + k]].Mutation(p_mut);
        popul[populIndexes[base + k]].build_core(r);

    };

    ///  New chromosomes insertion into the array
    for( k = 0; k < descendants; k++) ChromInsert(base - 1, base + k);

}

Chromosome& Population::write(unsigned int k, FILE* f){
    return popul[k].write();
}
