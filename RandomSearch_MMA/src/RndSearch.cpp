//#include <cstdlib>
#include "include\MMA.h"
using namespace std;

//extern RSet R;

//double Rndm(); // returns uniformly distributed on [0; 1) random number

/*
bool   RndSearch_alg(RSet* R, Chromosome* out_Chromosome, unsigned int nIter) {

    //*****************
    out_Chromosome - хромосома-выход (массив-перестановка). Размерность - длина хромосомы

    nIter - размер популяции
    //***************

    cout << "*** RandomSearch: begins......\n";

    Population P(Achrom, Apair, ProbH, ProbS, pMut);   // популяции (base + descendants)



    P.setElement0(*out_Chromosome);
    cout << 0 << " Record:\n";
    ///P.write(0);
    P.InitPop(*R);


    cout << "\n";
    P.getElementIndx0(*out_Chromosome);

    //delete Prob;

    cout << "\n*** RandomSearch: ended......\n";
    return true;
}
*/
/**************************************************************************************************************/

int Vmax(int x, int y)  { return (x > y) ? x : y; }

double Rndm(){
    return (double) (rand() / (double)(RAND_MAX + 1));
}
