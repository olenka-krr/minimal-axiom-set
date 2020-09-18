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
        //unsigned int* ind_permut;
        //static unsigned int n; // chromosome length = RSet.nZ
        //unsigned int crit;
    public:
        static unsigned int n;                               // chromosome length = RSet.nZ
        unsigned int crit;

        Chromosome();
        Chromosome(const Chromosome&);                       // ����������� �����������
       ~Chromosome();

        unsigned int build_core(const RSet&);                // return number of sentenses in Z
        Chromosome& operator = (const Chromosome&);		     // �������. ������������

        bool UniformDistrRandPermut();
        int CompareCrit(const Chromosome&);
        Chromosome& write(std::FILE* = stdout);		         // ����� � ��������� ���� ��� �� �����
};


class RSet{
    private:
        unsigned int nT;          // set T power
        unsigned int nZ;          // set Z power
        //unsigned int t;           // total number of sentences in Z <= nT
        unsigned int* f;          // following sentence
        unsigned int* na;         // A-sets powers
        unsigned int** s;         // set of A-sets
    public:
        RSet(){ nT = nZ = 0; f = 0; na = 0; s = 0; }         // ����������� ��� ����������;
       ~RSet();
        unsigned int get_nZ(){return nZ;}
        unsigned int get_nT(){return nT;}

        //RSet& operator << (FILE*);  	// ���� �� ���������� �����
        //RSet& operator >> (FILE*);	    // ����� � ��������� ����
        RSet& write(std::FILE* = stdout);		       // ����� � ��������� ���� ��� �� �����
        RSet& read(std::FILE*);		        // ���� �� ���������� �����
        friend unsigned int Chromosome::build_core(const RSet&);

};

#endif // MMA_H
