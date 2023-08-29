#ifndef FITGPIO_H
#define FITGPIO_H
#include "ciobase.h"
#include <iostream>
#include <Windows.h>
#include"dsdebug.h"
using namespace std;

typedef int (*GETUINTPROC)(unsigned int*);
typedef int (*SETVOIDPROC)();
typedef int (*GETINT2UCHARPROC)(int, int*, unsigned char);
typedef int (*SETINT2UCHARPROC)(int, int, unsigned char);
typedef int (*SETINT2PROC)(int, int);
typedef bool(*SETUCHAR2PROC)(unsigned char, unsigned char);
typedef bool(*GETUCHAR2PROC)(unsigned char, unsigned char*);
typedef int (*GETINTUCHAR2PROC)(int, UCHAR*);
typedef int (*SETINTPROC)(int);



class Fitgpio:public CIOBase
{
public:

    Fitgpio();
    friend class CIOBase;

    int init();
   ~ Fitgpio();

    int setDirection(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal);

    int setLevel(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal);



     int getLevel(int iPin, int& getVal);


private:
    HINSTANCE hinstLib;
    SETVOIDPROC ProcAdd;
    GETUCHAR2PROC GPIO_Int;
    SETUCHAR2PROC GPIO_Out;

    int m_cnt;
};

#endif // FITGPIO_H
