
#include "include\MMA.h"
#include "include\Public.h"

using namespace std;

bool   EVF_alg(RSet* pR, Chromosome* p_out_Chromosome,
               unsigned int Achrom, unsigned int Apair, unsigned int Agen,
               double pMut,  double ProbH, double ProbS);

unsigned int Chromosome::N;          // chromosome length = RSet.nZ
unsigned int Chromosome::nObjEstim;  // chromosome length = RSet.nZ


int main(int argc, char* argv[]){
    char tbuf[512];
    char btime[128];
    time_t ttt;

    time(&ttt);
    strftime(btime, sizeof(btime) - 1, "%c", localtime(&ttt));
    sprintf(tbuf, "\nprogramm %s  v = %s  start  %s\n\n", PGM_NAME, PGM_ver, btime);

    printf("%s", tbuf);

    char buf[512];

    ///**************************///

    char NameDir[] = "321_100t_c100_st9_1000s"; //"321_100t_c20_st4_100s"; //"321_100t_1000s_500r"; //"321_10t_c20_st4_100s";
    ///*************************///

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
        if (strlen(buf) > 0) buf[strlen(buf) - 1] = '\0';
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
// 600 + 100 * 500 = 50 600;  100 000 + 50 000 * 2 500 = 125 100 000;  1000 + 200 * 565 = 114 000; 50 000 + 20 000 * 1 250 = 250 050 000;

    unsigned int Achrom = 100000;  //1000; //50000; //1000;   //100 000;      // population size
    unsigned int Apair = 50000;  //50000; //20000; //200;    //50 000;      // numbers of pairs for crossovering
    unsigned int Agen  = 2500; // 625 //394;500;  //1250;      //565;    //2 500;      // number of generations
    unsigned int nEVFObjCounter = Achrom + Apair * Agen;

    double pMut  = 0.1;     /// mutation probability
    double ProbH = 1;       /// hight of the triangular distribution (proportion of Achrom)
    double ProbS = 0.1;     /// shift in the triangular distribution (proportion of Achrom)
    cout << "Achrom = " << Achrom << " Apair = " << Apair << " Agen = " << Agen << "\n\n";

    srand((unsigned int) time(NULL)/2);

    sprintf(buf,"efm_%s.%s", NameDir, "csv");

    FILE* out = fopen(buf, "w");
    if(out == 0) cout << "Cannot open Output File! \n";

    unsigned int nS;  /// number of sentences = cardinality of T = RSet.nT
    unsigned int nO;  /// cardinality of result core + number of isolated sentences (nT - nS)
    unsigned int nObjTask;

    /****************************************/
    for(int nnZ = 0; nnZ < n_inst; nnZ++) {

        nObjTask = Chromosome::nObjEstim;

        FILE* in = fopen(Instances[nnZ], "r");
        if(in == 0) cout << "Cannot open Input File! \n";

        RSet* pR = new RSet;                      //set of relations
        pR->read(in);
        fclose(in);
        printf("\n*****%s*****\n", Instances[nnZ]);

        Chromosome::N = pR->get_nZ();  // number of relations = cardinality of Z = RSet.nZ

        cout << "Number of relations = power of Z = " << Chromosome::N << "\n\n";

        Chromosome* pOpt = new Chromosome;
        pOpt->UniformDistrRandPermut();
        nS = pOpt->build_core(*pR);  /// for first chromosome count of total number nS of sentences in Z <= n
        cout << "Number of used sentences nS = " << nS << "\n\n";


        EVF_alg(pR, pOpt, Achrom, Apair, Agen, pMut, ProbH, ProbS);
        cout << "\n**************Solution*****************\n";

        nO = pOpt->crit + pR->get_nT() - nS;

        delete pR;
        delete pOpt;

        sprintf(tbuf, "%s %3d\n", Instances[nnZ],  nO);
        fprintf(out, "%s", tbuf);

        time(&ttt);
        strftime(btime, sizeof(btime) - 1, "%c", localtime(&ttt));
        sprintf(tbuf, "\ninstance # %d %s   %s\n", nnZ + 1, Instances[nnZ], btime);

        printf("%s", tbuf);
        printf("\nNumber of objective EVF-estimates for the instance is %d", Chromosome::nObjEstim - nObjTask);

        //opt.~Chromosome();
    }
    fclose(out);
    printf("\n\nNumber of theoretical objective EVF-estimates per instance is %d", nEVFObjCounter);
    printf("\nNumber of total objective EVF-estimates is %d", Chromosome::nObjEstim);
    cout << "\n------------Ok-----------\n";

    return 0;
}
