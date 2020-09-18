
#include "include\MMA.h"
#include "include\Public.h"

using namespace std;

bool   LocalSearch_alg(RSet* pR, Chromosome* p_out_Chromosome,  unsigned int nPopul);

unsigned int Chromosome::N;  /// chromosome length = RSet.nZ
unsigned int Chromosome::nObjEstim;  // chromosome length = RSet.nZ

int main(int argc, char* argv[]){
    char tbuf[512];
    char btime[128];
    time_t ttt;

    time(&ttt);
    strftime(btime, sizeof(btime) - 1, "%c", localtime(&ttt));
    sprintf(tbuf, "\nprogramm %s  v = %s  start  %s\n\n", PGM_NAME, PGM_ver, btime);

    printf("%s\nn", tbuf);

    char buf[512];

    ///**************///
    char* NameDir = "321_100t_c20_st4_100s"; //"321_100t_100s_50r_3m_6l"; //"321_10t_c20_st4_100s"; //"321_100t_1000s_500r";  // "321_100t_c100_st9_1000s";
    ///*************///

    char tNameDir[512];
    char tekD[512];
    char** Instances = NULL;  // files of instances

    GetCurrentDirectory(sizeof(tekD), tekD);

    printf("set_of_instances_directory = %s\n", NameDir);
    sprintf(tNameDir, "%s\\%s", tekD, NameDir);
    printf("set_of_instances_full_path = %s\n", tNameDir);
   //********************************//

    if(!SetCurrentDirectory(tNameDir))
    {          printf("*** folder <%s> not found... ***\n", tNameDir);
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
           if (strlen(buf) > 0 ) buf[strlen(buf) - 1] = '\0';
           if (strlen(buf) == 0 ) continue;

           Instances[kk] = (char*) calloc(sizeof(char*), sizeof(buf) + 1);
           if(Instances[kk] == NULL)  {printf("*** There in no memory for Instances[%d]***\n", kk); return 0; };
           strcpy(Instances[kk], buf);
           kk++;
    };

    fclose(h);

//////////////**********************************


    unsigned int Achrom = 1000;  //10;    //–азмер попул€ции
    unsigned int Apair = 200; //10;    // оличество пар дл€ кроссовера
    unsigned int Agen  = 625;   //1;    //„исло поколений
    unsigned int nEVFObjCounter = Achrom + Apair * Agen;    //number of iterations

    cout << "\nAchrom = " << Achrom << " Apair = " << Apair << " Agen = " << Agen << "\n\n";
    cout << "nEVFObjCounter = " << nEVFObjCounter << "\n\n";

    srand((unsigned int) time(NULL)/2);

    sprintf(buf,"ls_%s.%s", NameDir, "csv");

    FILE* out = fopen(buf, "w");
    if(out == 0) cout << "Cannot open Output File! \n";

    unsigned int nS;  // number of sentences = cardinality of T = RSet.nT
    unsigned int nO;  // cardinality of result core + number of isolated sentences (nT - nS)
    unsigned int nObjTask;

    /****************************************/
    for(int nnZ = 0; nnZ < n_inst; nnZ++) {
        nObjTask = Chromosome::nObjEstim;
        FILE* in = fopen(Instances[nnZ], "r"); //Instances[nnZ]
        if(in == 0) cout << "Cannot open Input File! \n";

        RSet* pR = new RSet;                      //set of relations
        pR->read(in);
        fclose(in);
        printf("\n\n*****%s*****\n", Instances[nnZ]);

        Chromosome::N = pR->get_nZ();  // number of relations = cardinality of Z = RSet.nZ
        unsigned int nTranspositions = Chromosome::N * (Chromosome::N - 1) / 2;
        cout << "Number of transpositions = " << nTranspositions << "\n";
        cout << "Number of relations = cardinality of Z = " << Chromosome::N << "\n\n";

        Chromosome* pOpt = new Chromosome;
        pOpt->UniformDistrRandPermut();
        nS = pOpt->build_core(*pR);  ////дл€ первой хромосомы подсчет of total number nS of sentences in Z <= n
        cout << "Number of used sentences nS = " << nS << "\n\n";


        ///********************///
        LocalSearch_alg(pR, pOpt, 4); // (nObjCounter / nTranspositions)  // population`s size
        ///*******************///
        cout << "\n**************Solution*****************\n";

        nO = pOpt->crit + pR->get_nT() - nS;

        delete pR;
        delete pOpt;

        sprintf(tbuf, "%s %3d %9d\n", Instances[nnZ], nO, Chromosome::nObjEstim - nObjTask);
        fprintf(out, "%s", tbuf);

        time(&ttt);
        strftime(btime, sizeof(btime) - 1, "%c", localtime(&ttt));
        sprintf(tbuf, "\ninstance # %d %s   %s\n", nnZ + 1, Instances[nnZ], btime);

        printf("%s\n", tbuf);

        printf("\nNumber of objective LocalSearch-estimates for the instance is %d", Chromosome::nObjEstim - nObjTask);

    }
    printf("\n\nNumber of objective EVF-estimates per instanse is %d", nEVFObjCounter);
    printf("\nNumber of total objective LocalSearch-estimates is %d", Chromosome::nObjEstim);
    fclose(out);
    cout << "\n\n------------Ok-----------\n";
    //getchar();
    return 0;
}
