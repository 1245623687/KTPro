#ifndef CED_BC09_2A_FUN_H
#define CED_BC09_2A_FUN_H

#include <Windows.h>
using namespace std;



#define	DLL_NANE	TEXT("winio64.dll")
typedef bool (*pCED_BC09_2A_InitIO)(void);
typedef void (*pCED_BC09_2A_ShutDownIO)(void);
typedef bool (*pCED_BC09_2A_GetPortVal)(unsigned int ,unsigned int&,unsigned char);
typedef bool(*pCED_BC09_2A_SetPortVal)(unsigned int , unsigned int, unsigned char);




class CED_BC09_2A_Fun
{
public:
    CED_BC09_2A_Fun();
    int init();
    ~CED_BC09_2A_Fun();

    HINSTANCE		hDLL;
    pCED_BC09_2A_InitIO CED_BC09_2A_InitIO;
    pCED_BC09_2A_ShutDownIO CED_BC09_2A_ShutDownIO;
    pCED_BC09_2A_GetPortVal  CED_BC09_2A_GetPortVal;
    pCED_BC09_2A_SetPortVal  CED_BC09_2A_SetPortVal;

};

#endif // CED_BC09_2A_FUN_H
