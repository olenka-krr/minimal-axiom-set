#include "InstGenerate.h"

using namespace std;

/*************************************RSet*********************************************/
RSet::RSet(unsigned int k, unsigned int l, TriangularDistribution& t){
    nT = k;
    nZ = l;
    unsigned int i, j;
    f = new unsigned int[nZ];
    na = new unsigned int[nZ];
    s = new unsigned int* [nZ];

    unsigned int* workArr = new unsigned int[nT];
    for (i = 0; i < nT; i++) workArr[i] = i;

    for (i = 0; i < nZ; i++){
        na[i] = t.samling(Rndm());   /// triangle distribution for R-set size (1 .. max)
        l = na[i];

        RandomCombination(workArr, nT - 1, l + 1);

        f[i] = workArr[nT - 1];

        s[i] = new unsigned int[l];

        for (j = 0; j < l; j++){
            s[i][j] = workArr[nT - 2 - j];
            //cout << s[i][j] << " ";
        }
    }

    delete[] workArr;
}

RSet::~RSet(){
    if ( s != NULL ) {
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
            if(r > nT - 1) {cout << "Error! Sentence number > (nT - 1) ! \n"; }
            s[i][j] = r;
        }
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

RSet& RSet::write_f(FILE* ft){
    unsigned int i, j;
    fprintf(ft, "%3d \n", nT);  /// 1st row - number of sentences
    fprintf(ft, "%3d \n", nZ);  /// 2nd row - number of relations
    for (i = 0; i < nZ; i++){
        fprintf(ft, "%3d %3d ", f[i], na[i]);    /// 3rd row - 1st number is number of sentence-consequence, 2nd number is cardinality of the relation set
        for (j = 0; j < na[i]; j++) fprintf(ft, " %3d", s[i][j]);   /// sentences-members of relation
        fprintf(ft, ";\n");
    }
    return *this;
}

 /*************************************TriangularDistribution*********************************************/
/// Constructs triangular distribution with parameters h and m, fills sample[] - sample of size n
TriangularDistribution::TriangularDistribution(double xh, double xm, unsigned int xn){
    h = xh;  /// hight
    m = xm;  /// shift
    n = xn;  /// sample size
    sample = new double[n];

    unsigned int i;
    unsigned int s = (unsigned int) m;

    for(i = 0; i < n; i++) sample[i] = 0;

    double rn, ri;
    rn = n;

    for(i = 0; i < n; i++) {
        ri = i + 1;
        if (i <= s )  {
            sample[i] = ri * ri * h / m / 2.;
        } else {
            sample[i] = (rn - (rn - ri) * (rn - ri) / (rn - m)) * h / 2.;
        }
    }
}

TriangularDistribution::~TriangularDistribution(){
    if (sample != NULL){
        n = 0;
        m = h = 0.0;
        delete[] sample;
    }
}

//returns R-set size (from 1 to max)
unsigned int TriangularDistribution::samling(double p){
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
    return i + 1;
}

TriangularDistribution& TriangularDistribution::write(FILE* ft){
    unsigned int i;
    fprintf(ft, "h = %3f \n", h);
    fprintf(ft, "m = %3f \n", m);
    fprintf(ft, "n = %3d \n", n);
    for (i = 0; i < n; i++){
        fprintf(ft, "%3d: f = %3f ", i, sample[i]);
        fprintf(ft, "\n");
    }
    return *this;
}
