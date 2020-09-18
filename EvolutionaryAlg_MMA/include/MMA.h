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
        unsigned int* ind_permut;

    public:
        static unsigned int N;                               // chromosome length = RSet.nZ
        static unsigned int nObjEstim;
        unsigned int crit;

        Chromosome();
        Chromosome(const Chromosome&);
       ~Chromosome();

        unsigned int build_core(const RSet&);                // return number of sentences in Z
        Chromosome& operator = (const Chromosome&);		     // dynamic assignment
        Chromosome& Crossover(const Chromosome&, const Chromosome&);
        bool Mutation(double);
        bool UniformDistrRandPermut();
        int CompareCrit(const Chromosome&);
        Chromosome& write(std::FILE* = stdout);		         // output into the text file or onto the monitor
};


class RSet{
    private:
        unsigned int nT;          /// set T cardinality
        unsigned int nZ;          /// set Z cardinality

        unsigned int* f;          /// sentences-consequences
        unsigned int* na;         /// A-sets` cardinalities
        unsigned int** s;         /// set of A-sets
    public:
        RSet(){ nT = nZ = 0; f = 0; na = 0; s = 0; }
       ~RSet();
        unsigned int get_nZ(){return nZ;}
        unsigned int get_nT(){return nT;}

        RSet& write(std::FILE* = stdout);		      /// output into the text file or onto the monitor
        RSet& read(std::FILE*);		                  /// input from the text file
        friend unsigned int Chromosome::build_core(const RSet&);

};


class TriangularDistribution{
    private:
        double h;              /// hight >=0
        double m;              /// shift >=0
        unsigned int n;        /// sample size
        double* sample;        /// sample
    public:
        TriangularDistribution(){h = m = 0.0; n = 0; sample = NULL; }
        TriangularDistribution(double, double, unsigned int);
       ~TriangularDistribution();
        unsigned int sampling(double);
};


class Population{
    private:
        unsigned int base;               /// base population size
        unsigned int descendants;        /// number of descendants
        double h;                        /// hight >=0
        double m;                        /// shift >=0
        double p_mut;                    /// mutation probability >=0
        Chromosome* popul;               /// population (base + descendants)
        unsigned int* populIndexes;      /// index array
        TriangularDistribution* pProb;   /// object with probabilities of the triangular distribution
    public:
        Population();
        Population(unsigned int, unsigned int, double, double, double);
       ~Population();
        void InitPop(const RSet&);
        void NextGen(const RSet&);
        void setElement0(Chromosome& c) { popul[0] = c; }     // set zero element

        void getElementIndx0(Chromosome& c) {
            c = popul[0];
            //c.write();
            std::cout << std::endl;
        };
        bool ChromInsert(unsigned int, unsigned int);
        Chromosome& write(unsigned int, std::FILE* = stdout); /// output of the chromosome into the text file or onto the monitor
};


#endif // MMA_H
