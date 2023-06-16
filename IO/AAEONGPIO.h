#pragma once

#define AAEONGpioOUT 0x00
#define AAEONGpioINT 0xFF
#define AAEONDefaultBitmask 0xFFFFFFFF
#define AAEONHighLevel 0xFF
#define AAEONLowLevel 0x00

//#include <aaeonEAPI.h>  20210616
#include "aaeonEAPI.h"
#include"ciobase.h"
class AAEONGPIO : public CIOBase
{
public:

    friend class CIOBase;
    int init();
    ~AAEONGPIO();

    int setDirection(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal);

    int setLevel(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal);


private:
    uint32_t get_level(
        __IN EApiId_t Id, /* GPIO Id */
        __IN uint32_t Bitmask, /* Bit mask of Affected Bits */
        __IN uint32_t *Level /* Level */);
private:
	uint32_t get_caps(
		__IN EApiId_t Id, /* GPIO Id */
		__OUTOPT uint32_t *PinCount, /* PinCount */
		__OUTOPT uint32_t *pDioDisable /* Pin Enable or Disable: 1:Disable; 0:Enable */);


private:
    AAEONGPIO();

};

