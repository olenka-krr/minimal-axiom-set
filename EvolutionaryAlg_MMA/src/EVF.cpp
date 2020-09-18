
#include "MMA.h"
using namespace std;

/************
EVF-algorithm
Popul - population array
IndxChhroms - array of indices
*************/

bool   EVF_alg(RSet* R, Chromosome* out_Chromosome,
               unsigned int Achrom, unsigned int Apair, unsigned int Agen,
               double pMut,  double ProbH, double ProbS) {  //, double t_lim

    /*****************
    out_Chromosome - array-permutation. Size is chromosome`s length

    Parameters of evolutionary algorithm
    Achrom - population size
    Apair - numbers of pairs for crossovering
    Agen  - number of generations
    pMut - mutation probability
    PrîbH, PrîbS - parameters of distribution for pair selecting
    PrîbH - relative hight, ProbS - relative shift
    Achrom*ProbH - absolute hight, Achrom*ProbS - absolute shift
    ***************/

    cout << "*** EVF: begins......\n";

    Population P(Achrom, Apair, ProbH, ProbS, pMut);   // population size (base + descendants)

    cout << "*** EVF: Beginning population\n";

    P.setElement0(*out_Chromosome);

    P.InitPop(*R);

    cout << "*** EVF: Generation growing\n";
    // Generation growth
    for (unsigned int i = 1; i <= Agen; i++) {
        cout << "*";
        P.NextGen(*R);
    }
    cout << "\n";
    P.getElementIndx0(*out_Chromosome);

    cout << "\n*** EVF: ended......\n";
    return true;
}

/**************************************************************************************************************/

int Vmax(int x, int y)  { return (x > y) ? x : y; }

double Rndm(){
    return (double) (rand() / (double)(RAND_MAX + 1));
}
