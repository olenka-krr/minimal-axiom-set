
#include "MMA.h"

using namespace std;


bool   LocalSearch_alg(RSet* R, Chromosome* out_Chromosome, unsigned int nPopul) {

    /*****************
    out_Chromosome - хромосома-выход (массив-перестановка). Размерность - длина хромосомы

    Параметры алгоритма
    nPopul - размер популяции

    ***************/

    cout << "*** LocalSearch: begins......\n";
    unsigned int nN = R->get_nT();
    cout << "nN = " << nN << endl;
    Population P(*R, nPopul, nN);   //


    cout << "*** LocalSearch:  proceeding\n";
    cout << "\nnPopul = " << nPopul;
    for (unsigned int i = 0; i < nPopul; i++) {
        cout << "*";

        P.popul[i].R1SearchSphere(*R);

        if(P.popul[i].crit < P.optSolCrit){
            P.optSolCrit = P.popul[i].crit;
            P.optSolIndx = i;
        }

    }
    cout << "\n";

    *out_Chromosome = P.popul[P.optSolIndx];

    cout << "\n*** LocalSearch: ended......\n";
    return true;
}

/**************************************************************************************************************/

int Vmax(int x, int y)  { return (x > y) ? x : y; }

double Rndm(){
    return (double) (rand() / (double)(RAND_MAX + 1));
}
