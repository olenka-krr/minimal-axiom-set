
#if !defined(_extern)

#define PU
#else
#define PU extern
#endif

#include "Define_file_name.h"


#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#define _extern

PU  double * k_time_limit;
PU  double * time_cpu; // ����� ����
PU  double * time_all; //  ����� �����

// ��������� ������������ ������
///  ��� EVFalg -�������� ����� �������� ��������� - �� ��������� �� Public_init.h ///
PU int    Ahrom, Apair, Agen;
PU double pMut, ProbH, probS;



