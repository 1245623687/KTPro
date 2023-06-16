#ifndef LTGFUNC_H
#define LTGFUNC_H



#include <Windows.h>
#include <Windows.h>
using namespace std;


// LTGLIB library definition
#define	DLL_NANE	TEXT("ltglib.dll")
typedef	int			(*pLTGInit)(void);
typedef	int			(*pLTGUninit)(void);
typedef int			(*pLTGEnableLogFile)(bool en);
typedef int			(*pLTGOpenCOMPort)(int port);
typedef int			(*pLTGCloseCOMPort)(void);
typedef int			(*pLTGGetFWVersion)(BYTE* ver_major, BYTE* ver_minor, BYTE* ver_minor2, int* built_year, int* built_month, int* built_day);
typedef int			(*pLTGDoCtrlWrite)(BYTE port, BYTE level);
typedef int			(*pLTGDoCtrlRead)(BYTE port, BYTE* level);
typedef int			(*pLTGDoCtrlWriteBit)(BYTE port0, BYTE port1);
typedef int			(*pLTGDoCtrlReadBit)(BYTE* port0, BYTE* port1);
typedef int			(*pLTGDiCtrlRead)(BYTE port, BYTE* level);
typedef int			(*pLTGDiCtrlReadBit)(BYTE* port0, BYTE* port1);
typedef int			(*pLTGMcuReset)(void);
typedef int			(*pLTGPrarameterReset)(void);
typedef int			(*pLTGLedCtrlWrite)(BYTE port, int luminance, BYTE on_off, BYTE trigger_mode, int period);
typedef int			(*pLTGLedCtrlRead)(BYTE port, int* luminance, BYTE* on_off, BYTE* trigger_mode, int* period);
typedef int			(*pLTGGetLibVersion)(BYTE* ver_major, BYTE* ver_minor, BYTE* ver_minor2);
typedef int			(*pLTGPwmCtrlWrite)(BYTE port, int cycle_ms, int duty_ms, int pwmpulse_amount);
typedef int			(*pLTGPwmCtrlRead)(BYTE port, int* cycle_ms, int* duty_ms, int* pwmpulse_amount);
typedef int			(*pLTGPwmCtrlEnable)(BYTE port, BYTE flag);
typedef int			(*pLTGPulseCtrlWrite)(BYTE port, int pulse_width_ms);
typedef int			(*pLTGPulseCtrlRead)(BYTE port, int* pulse_width_ms);
typedef int			(*pLTGPulseCtrlEnable)(BYTE port, BYTE flag);
typedef	int			(*pLTGInputCtrlWrite)(BYTE port, BYTE edge_mode, BYTE start_switch, int delay_period);
typedef int			(*pLTGInputCtrlRead)(BYTE port, BYTE* edge_mode, BYTE* start_switch, int* delay_period);
typedef int			(*pLTGInputTriggerCountRead)(BYTE port, BYTE* edge, unsigned int* count_value);
typedef int			(*pLTGRXWaitMsWrite)(unsigned int value);
typedef int			(*pLTGRXWaitMsRead)(unsigned int* value);
typedef	int			(*pLTGFastInputCtrlWrite)(BYTE port, int count_num, BYTE mode_switch, BYTE output_port);
typedef int			(*pLTGFastInputCtrlRead)(BYTE port, unsigned int* count_num, BYTE* mode_switch, BYTE* output_port);
typedef int			(*pLTGNPNPNPSet)(BYTE selection);


class ltgFunc
{
public:
	ltgFunc();
	~ltgFunc();
	int init();

	// LTGLIB library definition
	HINSTANCE					hDLL;
	pLTGInit					LTGInit;
	pLTGUninit					LTGUninit;
	pLTGEnableLogFile			LTGEnableLogFile;
	pLTGOpenCOMPort				LTGOpenCOMPort;
	pLTGCloseCOMPort			LTGCloseCOMPort;
	pLTGGetFWVersion			LTGGetFWVersion;
	pLTGDoCtrlWrite				LTGDoCtrlWrite;
	pLTGDoCtrlRead				LTGDoCtrlRead;
	pLTGDoCtrlWriteBit			LTGDoCtrlWriteBit;
	pLTGDoCtrlReadBit			LTGDoCtrlReadBit;
	pLTGDiCtrlRead				LTGDiCtrlRead;
	pLTGDiCtrlReadBit			LTGDiCtrlReadBit;
	pLTGMcuReset				LTGMcuReset;
	pLTGPrarameterReset			LTGPrarameterReset;
	pLTGLedCtrlWrite			LTGLedCtrlWrite;
	pLTGLedCtrlRead				LTGLedCtrlRead;
	pLTGGetLibVersion			LTGGetLibVersion;
	pLTGPwmCtrlWrite			LTGPwmCtrlWrite;
	pLTGPwmCtrlRead				LTGPwmCtrlRead;
	pLTGPwmCtrlEnable			LTGPwmCtrlEnable;
	pLTGPulseCtrlWrite			LTGPulseCtrlWrite;
	pLTGPulseCtrlRead			LTGPulseCtrlRead;
	pLTGPulseCtrlEnable			LTGPulseCtrlEnable;
	pLTGInputCtrlWrite			LTGInputCtrlWrite;
	pLTGInputCtrlRead			LTGInputCtrlRead;
	pLTGInputTriggerCountRead	LTGInputTriggerCountRead;
	pLTGRXWaitMsWrite			LTGRXWaitMsWrite;
	pLTGRXWaitMsRead			LTGRXWaitMsRead;
	pLTGFastInputCtrlWrite		LTGFastInputCtrlWrite;
	pLTGFastInputCtrlRead		LTGFastInputCtrlRead;
	pLTGNPNPNPSet				LTGNPNPNPSet;
};

#endif
