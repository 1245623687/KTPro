#include "ltgfunc.h"

ltgFunc::ltgFunc()
{
}

ltgFunc::~ltgFunc()
{

}

int ltgFunc::init()
{
	// Load library
	hDLL = NULL;
	if (NULL == (hDLL = LoadLibraryEx(DLL_NANE, NULL, NULL)))
	{
        //AfxMessageBox (TEXT("[ERROR] Load LTGLIB.DLL failed !"));
		return 1;
	}
	// Define LTGLIB library function
	LTGInit = (pLTGInit)GetProcAddress(hDLL, ("LTG_Init"));
	LTGUninit = (pLTGUninit)GetProcAddress(hDLL, ("LTG_Uninit"));
	LTGEnableLogFile = (pLTGEnableLogFile)GetProcAddress(hDLL, ("LTG_EnableLogFile"));
	LTGOpenCOMPort = (pLTGOpenCOMPort)GetProcAddress(hDLL, ("LTG_OpenCOMPort"));
	LTGCloseCOMPort = (pLTGCloseCOMPort)GetProcAddress(hDLL, ("LTG_CloseCOMPort"));
	LTGGetFWVersion = (pLTGGetFWVersion)GetProcAddress(hDLL, ("LTG_GetFWVersion"));
	LTGDoCtrlWrite = (pLTGDoCtrlWrite)GetProcAddress(hDLL, ("LTG_DoCtrl_Write"));
	LTGDoCtrlRead = (pLTGDoCtrlRead)GetProcAddress(hDLL, ("LTG_DoCtrl_Read"));
	LTGDoCtrlWriteBit = (pLTGDoCtrlWriteBit)GetProcAddress(hDLL, ("LTG_DoCtrl_WriteBit"));
	LTGDoCtrlReadBit = (pLTGDoCtrlReadBit)GetProcAddress(hDLL, ("LTG_DoCtrl_ReadBit"));
	LTGDiCtrlRead = (pLTGDiCtrlRead)GetProcAddress(hDLL, ("LTG_DiCtrl_Read"));
	LTGDiCtrlReadBit = (pLTGDiCtrlReadBit)GetProcAddress(hDLL, ("LTG_DiCtrl_ReadBit"));
	LTGMcuReset = (pLTGMcuReset)GetProcAddress(hDLL, ("LTG_McuReset"));
	LTGPrarameterReset = (pLTGPrarameterReset)GetProcAddress(hDLL, ("LTG_ParameterReset"));
	LTGLedCtrlWrite = (pLTGLedCtrlWrite)GetProcAddress(hDLL, ("LTG_LedCtrl_Write"));
	LTGLedCtrlRead = (pLTGLedCtrlRead)GetProcAddress(hDLL, ("LTG_LedCtrl_Read"));
	LTGGetLibVersion = (pLTGGetLibVersion)GetProcAddress(hDLL, ("LTG_GetLibVersion"));
	LTGPwmCtrlWrite = (pLTGPwmCtrlWrite)GetProcAddress(hDLL, ("LTG_PwmCtrl_Write"));
	LTGPwmCtrlRead = (pLTGPwmCtrlRead)GetProcAddress(hDLL, ("LTG_PwmCtrl_Read"));
	LTGPwmCtrlEnable = (pLTGPwmCtrlEnable)GetProcAddress(hDLL, ("LTG_PwmCtrl_Enable"));
	LTGPulseCtrlWrite = (pLTGPulseCtrlWrite)GetProcAddress(hDLL, ("LTG_PulseCtrl_Write"));
	LTGPulseCtrlRead = (pLTGPulseCtrlRead)GetProcAddress(hDLL, ("LTG_PulseCtrl_Read"));
	LTGPulseCtrlEnable = (pLTGPulseCtrlEnable)GetProcAddress(hDLL, ("LTG_PulseCtrl_Enable"));
	LTGInputCtrlWrite = (pLTGInputCtrlWrite)GetProcAddress(hDLL, ("LTG_InputCtrl_Write"));
	LTGInputCtrlRead = (pLTGInputCtrlRead)GetProcAddress(hDLL, ("LTG_InputCtrl_Read"));
	LTGInputTriggerCountRead = (pLTGInputTriggerCountRead)GetProcAddress(hDLL, ("LTG_InputTriggerCount_Read"));
	LTGRXWaitMsWrite = (pLTGRXWaitMsWrite)GetProcAddress(hDLL, ("LTG_RXWaitMs_Write"));
	LTGRXWaitMsRead = (pLTGRXWaitMsRead)GetProcAddress(hDLL, ("LTG_RXWaitMs_Read"));
	LTGFastInputCtrlWrite = (pLTGFastInputCtrlWrite)GetProcAddress(hDLL, ("LTG_FastInputCtrl_Write"));
	LTGFastInputCtrlRead = (pLTGFastInputCtrlRead)GetProcAddress(hDLL, ("LTG_FastInputCtrl_Read"));
	LTGNPNPNPSet = (pLTGNPNPNPSet)GetProcAddress(hDLL, ("LTG_NPNPNP_Set"));

	// Call init function
	LTGInit();

	return 0;
}

