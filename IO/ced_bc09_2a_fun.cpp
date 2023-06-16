#include "ced_bc09_2a_fun.h"

CED_BC09_2A_Fun::CED_BC09_2A_Fun()
{

}

int CED_BC09_2A_Fun::init()
{
	int ret = 0;
    hDLL = NULL;
    if (NULL == (hDLL = LoadLibraryEx(TEXT("winio64.dll"), NULL, NULL)))
    {
        //AfxMessageBox (TEXT("[ERROR] Load LTGLIB.DLL failed !"));
        return 1;
    }

    CED_BC09_2A_InitIO = (pCED_BC09_2A_InitIO)GetProcAddress(hDLL, "InitializeWinIo");
    CED_BC09_2A_GetPortVal = (pCED_BC09_2A_GetPortVal)GetProcAddress(hDLL, "GetPortVal");
    CED_BC09_2A_SetPortVal = (pCED_BC09_2A_SetPortVal)GetProcAddress(hDLL, "SetPortVal");
    CED_BC09_2A_ShutDownIO = (pCED_BC09_2A_ShutDownIO)GetProcAddress(hDLL, "ShutdownWinIo");
	return ret;
}

CED_BC09_2A_Fun::~CED_BC09_2A_Fun()
{

}
