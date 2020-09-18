#include "include\InstGenerate.h"
#include <cstring>

using namespace std;
bool UniformDistrRandPermut(unsigned int);

int main(){
    char listInst[] = "list_instances.csv";    /// list of instances
    char fnameo[] = "321_0000";                // for name of instance file
    int nnum = 4;
    char numb[nnum + 1];                       // counter for the output file name

    cout << "\n*********Instant generation**************" << endl <<endl;

    /// m0 - core cardinality; nSt - number of steps (core is not including)
    // e.g. m0 = 20, nSt = 4, => nSent = 20 * (4 + 1) = 100 // m0 = 100, nSt = 9; nSent = 100 * (9 + 1) = 1000
    int N, m0 = 20, nSt = 4;
    cout << "Enter number of instances (0 < N <= 100): N = ";
    cin >> N;
    /*cout << "Enter power of core (0 < m0 <= 20): m0 = ";
    cin >> m0;
    cout << "Enter number of steps (0 < nSt <= 5)(core is not including): nSt = ";
    cin >> nSt;*/
    //unsigned int ns, nr;
    //ns = m0 * (nSt + 1);    // Number of sentences
    //nr = m0 * nSt;     // Number of relations

    srand((unsigned int) time(NULL)/2);

    //Probabilities for all possible cardinalities (1..l) of R-sets
    //TriangularDistribution TDist((double) 2 / l, (double) m, l);

    FILE* InstList = fopen(listInst, "w");

    for(int i = 0; i < N; i++){

        sprintf(numb, "%d", i + 1); //itoa(i + 1, numb, 10);
        for(int j = 0; j < nnum; j++) fnameo[4 + j] = numb[j];

        strcat(fnameo, ".csv");

        FILE* out = fopen(fnameo, "w");
        fprintf(InstList, "%s \n", fnameo);
        cout << fnameo << endl;

        RSetGen* pR = new RSetGen(m0, nSt);


        pR->write_f(out);                  // writing into output file

        fclose(out);
        delete pR;
    }
    fclose(InstList);

    puts("\n------- Fin. Press any key---------");
    getchar();
    return 0;
}


double Rndm(){
    return (double)(rand() / (double)(RAND_MAX + 1));
}

/// n - max index, not array size
bool RandomCombination(unsigned int* per, unsigned int n, unsigned int m){
    unsigned int i, j, v;

    /// the last m elements of the array keeps combinations from n elements (0 .. n - 1)

    for (i = n; i > n - m; i--) {
        j = (unsigned int) round(Rndm() * i);   // uniform random number from 0 to i
        if(i != j)  {
            v = per[i];
            per[i] = per[j];
            per[j] = v;
        }
    }
    return true;
}

