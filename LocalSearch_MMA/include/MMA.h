#ifndef MMA_H
#define MMA_H
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <time.h>

extern double Rndm();          // returns uniformly distributed on [0; 1) random number
extern int Vmax(int, int);     // returns max value

class RSet;

class Chromosome{
    private:
        unsigned int* permut;

    public:
        static unsigned int N;                               // chromosome length = RSet.nZ
        static unsigned int nObjEstim;
        unsigned int crit;

        Chromosome();
        Chromosome(const Chromosome&);
       ~Chromosome();

        Chromosome& operator = (const Chromosome&);

        unsigned int build_core(const RSet&);                // returns number of sentenses in Z, counts crit
        bool UniformDistrRandPermut();
        int CompareCrit(const Chromosome&);
        int R1SearchSphere(const RSet&);
        Chromosome& write(std::FILE* = stdout);		         // Вывод в текстовый файл или на экран
};


class RSet{
    private:
        unsigned int nT;          // set T cardinality
        unsigned int nZ;          // set Z cardinality

        unsigned int* f;          // sentences-consequences
        unsigned int* na;         // A-sets cardinalities
        unsigned int** s;         // set of A-sets
    public:
        RSet(){ nT = nZ = 0; f = 0; na = 0; s = 0; }
       ~RSet();
        unsigned int get_nZ(){return nZ;}
        unsigned int get_nT(){return nT;}

        RSet& write(std::FILE* = stdout);		       // Вывод в текстовый файл или на экран
        RSet& read(std::FILE*);		                   // Ввод из текстового файла
        friend unsigned int Chromosome::build_core(const RSet&);

};


class Population{
    private:
        unsigned int base;               // base population size


    public:
        Chromosome* popul;               // популяция размера base
        unsigned int optSolIndx;    // best solution index
        unsigned int optSolCrit;    // best criterion value
        Population();
        Population(const RSet&, unsigned int, unsigned int);
       ~Population();

        Chromosome& write(unsigned int, std::FILE* = stdout); // Вывод заданной хромосомы в текстовый файл или на экран
};


#endif // MMA_H
