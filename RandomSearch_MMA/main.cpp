#include "include\MMA.h"
#include "include\Public.h"

using namespace std;

//bool   RndSearch_alg(RSet* pR, Chromosome* p_out_Chromosome, unsigned int nIter);
double Rndm(); // returns uniformly distributed on [0; 1) random number

unsigned int Chromosome::n;  // chromosome length = RSet.nZ

int main(int argc, char* argv[]){
    char tbuf[512];
    char btime[128];
    time_t ttt;

    time(&ttt);
    strftime(btime, sizeof(btime) - 1, "%c", localtime(&ttt));
    sprintf(tbuf, "\nprogramm %s  v = %s  start  %s\n\n", PGM_NAME, PGM_ver, btime);

    printf("%s", tbuf);
    //char fz_name[512];
    char buf[512];

    ///*****************///
    char* NameDir = "321_100t_c20_st4_100s";   //"321_10t_c20_st4_100s"; //"321_100t_c100_st9_1000s";    ///NULL;
    ///****************///

    char tNameDir[512];
    char tekD[512];
    char** Instances = NULL;  // files of instances

    GetCurrentDirectory(sizeof(tekD), tekD);

    printf("set_of_instances_directory = %s\n", NameDir);
    sprintf(tNameDir, "%s\\%s", tekD, NameDir);
    printf("set_of_instances_full_path = %s\n", tNameDir);

    if(!SetCurrentDirectory(tNameDir)){
        printf("*** folder <%s> not found... ***\n", tNameDir);
        return 0;
    };


    GetCurrentDirectory(sizeof(tekD), tekD);
    printf("*** folder <%s> ... ***\n", tekD);
    printf("*** file <%s> ... ***\n", file_zad);

    FILE* h;
    h = fopen(file_zad, "r");

    if(h == NULL) { printf("*** error while opening file %s\n", file_zad); return 0; }

    int n_inst = 0;  // number of instances
    while (fgets(buf, 512, h) != NULL) {
        if (strlen(buf) > 0) buf[strlen(buf) - 1] = '\0'; // "обрежем перевод строки"
        if (strlen(buf) == 0) continue;
        n_inst ++;
    }
    fclose(h);

    h = fopen(file_zad, "rt");
    if(h == NULL) { printf("*** error while opening file %s\n", file_zad); return 0; }

    Instances = (char**) calloc(sizeof(char*), n_inst);
    if(Instances == NULL) {printf("*** There in no memory for Instances *** \n"); return 0; };

    int kk = 0;
    while (fgets(buf, 512, h) != NULL) {
           if (strlen(buf) > 0 ) buf[strlen(buf) - 1] = '\0'; // "обрежем перевод строки"
           if (strlen(buf) == 0 ) continue;

           Instances[kk] = (char*) calloc(sizeof(char*), sizeof(buf) + 1);
           if(Instances[kk] == NULL)  {printf("*** There in no memory for Instances[%d]***\n", kk); return 0; };
           strcpy(Instances[kk], buf);
           kk++;
    };

    fclose(h);

    unsigned int Achrom = 600;    //–азмер попул€ции
    unsigned int Apair = 100;    // оличество пар дл€ кроссовера
    unsigned int Agen  = 500;    //„исло поколений

    ///************///
    unsigned int nIter = 126000;  // Achrom + Apair * Agen;    //number of iterations
    ///***********///

    cout << "Achrom = " << Achrom << " Apair = " << Apair << " Agen = " << Agen << "\n\n";
    cout << "nIter = " << nIter << "\n\n";

    srand((unsigned int) time(NULL)/2);  // «апуск генератора случайных чисел

    sprintf(buf,"rs_%s.%s", NameDir, "csv");

    FILE* out = fopen(buf, "w");
    if(out == 0) cout << "Cannot open Output File! \n";

    unsigned int nS;  // number of sentences = power of T = RSet.nT
    unsigned int nO;  // power of result core + number of isolated sentences (nT - nS)

    /****************************************/
    /****************************************/
    for(int nnZ = 0; nnZ < n_inst; nnZ++) {

        FILE* in = fopen(Instances[nnZ], "r"); //Instances[nnZ]
        if(in == 0) cout << "Cannot open Input File! \n";

        RSet* pR = new RSet;                      //set of relations
        pR->read(in);
        fclose(in);
        printf("\n\n*****%s*****\n", Instances[nnZ]);
        ///pR->write();
        Chromosome::n = pR->get_nZ();  // number of relations = power of Z = RSet.nZ

        cout << "Number of relations = power of Z = " << Chromosome::n << "\n\n";

        Chromosome* pOpt = new Chromosome;
        Chromosome* ptemp = new Chromosome;
        pOpt->UniformDistrRandPermut();
        nS = pOpt->build_core(*pR);  ////дл€ первой хромосомы подсчет of total number nS of sentences in Z <= n
        cout << "Number of used sentences nS = " << nS << "\n\n";

        for(unsigned int nnChr = 1; nnChr < nIter; nnChr++){
            ptemp->UniformDistrRandPermut();
            ptemp->build_core(*pR);
            if(pOpt->CompareCrit(*ptemp) > 0) *pOpt = *ptemp;

        }
        //RndSearch_alg(pR, &opt, nIter);
        cout << "\n**************Solution*****************\n";
        ///pOpt->write();
        nO = pOpt->crit + pR->get_nT() - nS;

        delete pR;
        delete pOpt;
        delete ptemp;

        sprintf(tbuf, "%s %3d\n", Instances[nnZ],  nO);
        fprintf(out, "%s", tbuf);

        time(&ttt);
        strftime(btime, sizeof(btime) - 1, "%c", localtime(&ttt));
        sprintf(tbuf, "\ninstance # %d %s   %s\n", nnZ + 1, Instances[nnZ], btime);
        //fprintf(glog, "%s", tbuf);
        printf("%s", tbuf);
        //temp.~Chromosome();
        //opt.~Chromosome();
    }
    fclose(out);
    cout << "\n------------Ok-----------\n";
    //getchar();
    return 0;
}

double Rndm(){
    return (double) (rand() / (double)(RAND_MAX + 1));
}
