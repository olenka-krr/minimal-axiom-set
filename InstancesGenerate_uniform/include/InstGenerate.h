#ifndef INSTGENERATE_H
#define INSTGENERATE_H

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <time.h>

extern double Rndm(); /// returns uniformly distributed on [0; 1) random number
extern bool RandomCombination(unsigned int*, unsigned int, unsigned int);

class TriangularDistribution;

class RSet{
    private:
        unsigned int nT;          /// cardinality of set T
        unsigned int nZ;          /// cardinality of set Z

        unsigned int* f;          /// sentences-consequences
        unsigned int* na;         /// A-sets` cardinalities
        unsigned int** s;         /// set of A-sets
    public:
        RSet(){ nT = nZ = 0; f = 0; na = 0; s = 0; }
        RSet(unsigned int, unsigned int, TriangularDistribution&);
       ~RSet();
        unsigned int get_nZ(){return nZ;}
        unsigned int get_nT(){return nT;}

        RSet& write(std::FILE* = stdout);	   /// Output into the text file or into the monitor
        RSet& write_f(std::FILE*);		       /// Output into the text file or into the monitor
        RSet& read(std::FILE*);		           /// Input from the text file

        void gen(TriangularDistribution&);
};

class TriangularDistribution{
    private:
        double h;              /// absolute hight >=0
        double m;              /// absolute shift >=0
        unsigned int n;        /// sample size
        double* sample;        /// sample
    public:
        TriangularDistribution(){h = m = 0.0; n = 0; sample = NULL; }
        TriangularDistribution(double, double, unsigned int);
       ~TriangularDistribution();
        unsigned int samling(double);
        TriangularDistribution& write(std::FILE* = stdout);	   /// Output into the text file or into the monitor
        friend void RSet::gen(TriangularDistribution&);
};

#endif // INSTGENERATE_H
