#include "include\InstGenerate.h"
#include <cstring>

using namespace std;


int main(){
    char listInst[] = "list_instances.csv";    /// list of instances
    char fnameo[] = "321_0000";                /// for name of instance file
    int nnum = 4;
    char numb[nnum + 1];                       /// counter for output file name (instead 000)

    cout << "\n*********Instant generation**************" << endl <<endl;

    int N = 0;
    cout << "Enter number of instances (0 < N <= 100): N = ";
    cin >> N;
    unsigned int ns, nr, m, l;
    ns = 100; //1000;    // Number of sentences
    nr = 50;  //500;     // Number of relations
    m = 3;    //10;       // Mean number of sentences in relation
    l = 6;    //20;       // Maximal number of sentences in relation
    srand((unsigned int) time(NULL)/2);

    /// Probabilities for all possible cardinalities (1..l) R-sets
    TriangularDistribution TDist((double) 2 / l, (double) m, l);

    FILE* InstList = fopen(listInst, "w");

    for(int i = 0; i < N; i++){

        sprintf(numb, "%d", i + 1);
        for(int j = 0; j < nnum; j++) fnameo[4 + j] = numb[j];

        strcat(fnameo, ".csv");

        FILE* out = fopen(fnameo, "w");
        fprintf(InstList, "%s \n", fnameo);
        cout << fnameo << endl;

        RSet* pR = new RSet(ns, nr, TDist) ;


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
