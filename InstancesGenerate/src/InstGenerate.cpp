#include "InstGenerate.h"

using namespace std;

/*************************************RSet*********************************************/
RSetGen::RSetGen(unsigned int k, unsigned int l){
    m0 = k;
    Steps = l;
    unsigned int i, j, v;
    unsigned int n = m0 * Steps;     /// Number of relations
    permut = new unsigned int[n];
    for (i = 0; i < n; i++) permut[i] = i;
    /// Permutation of n elements
    /// Inverse permutation (indices) - for crossover
    /// Fisher–Yates shuffle for random permutation generating
    for (i = 0; i < n; i++) {
        j = (unsigned int) round(Rndm() * i);   // uniform random number from 0 to i
        if(i != j)  {
            v = permut[i];
            permut[i] = permut[j];
            permut[j] = v;
        }
    }
}

RSetGen::~RSetGen(){
    if ( permut != NULL ) {
        delete[] permut;
        m0 = Steps = 0;
        permut = 0;
    }
}

RSetGen& RSetGen::write(FILE* ft){
    unsigned int i, s, na = 1;           // na = 1 for compatibility
    unsigned int nT = m0 * (Steps + 1);  // Number of sentences
    unsigned int nZ = m0 * Steps;        // Number of relations
    fprintf(ft, "nT = %3d \n", nT);
    fprintf(ft, "nZ = %3d \n", nZ);
    for (i = 0; i < nZ; i++){
        s = (permut[i] + 1) / Steps * (Steps + 1) + ((permut[i] + 1) % Steps ? (permut[i] + 1) % Steps : -1);
        fprintf(ft, "%3d f = %3d   %3dA: %3d\n", i, s, na, s - 1);
    }
    return *this;
}

RSetGen& RSetGen::write_f(FILE* ft){
    unsigned int i, s, na = 1;                // na = 1 for compatibility
    unsigned int nZ = m0 * Steps;             // Number of relations
    fprintf(ft, "%3d \n", m0 * (Steps + 1));  // 1st row - number of sentences
    fprintf(ft, "%3d \n", nZ);                // 2nd row - number of relations
    for (i = 0; i < nZ; i++){
        s = (permut[i] + 1) / Steps * (Steps + 1) + ((permut[i] + 1) % Steps ? (permut[i] + 1) % Steps : -1);
        fprintf(ft, "%3d %3d %3d;\n", s, na, s - 1);    // 3rd row - 1st number sentence-following, 2nd number - cardinality of the relation set
    }
    return *this;
}

