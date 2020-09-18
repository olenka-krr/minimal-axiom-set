#ifndef INSTGENERATE_H
#define INSTGENERATE_H

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <time.h>

extern double Rndm(); /// returns uniformly distributed on [0; 1) random number
extern bool RandomCombination(unsigned int*, unsigned int, unsigned int);

class RSetGen{
    private:
        unsigned int m0;             /// cardinality of core-set
        unsigned int Steps;          /// number of steps
        unsigned int* permut;        /// array of relation numbers

    public:
        RSetGen(){ m0 = Steps = 0; permut = NULL; }
        RSetGen(unsigned int, unsigned int);
       ~RSetGen();

        RSetGen& write(std::FILE* = stdout);		/// Output into the text file or onto the monitor
        RSetGen& write_f(std::FILE*);		        /// Output into the text file or onto the monitor

};

#endif // INSTGENERATE_H
