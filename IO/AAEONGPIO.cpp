
#include "AAEONGPIO.h"
#include"dsdebug.h"


AAEONGPIO::AAEONGPIO()
{

    DSDEBUG<<"gpio start init";

   if(init())
   {
       DSDEBUG<<"GPIO 初始化失败!";
   }
     DSDEBUG<<"gpio start init  suc!";
}



 int AAEONGPIO::init()
 {
     uint32_t err = EApiLibInitialize();
     return err;
 }

 AAEONGPIO::~AAEONGPIO()
 {
     int err = EApiLibUnInitialize();

 }



uint32_t AAEONGPIO::get_caps(
	__IN EApiId_t Id, /* AAEONGPIO Id */
	__OUTOPT uint32_t *PinCount, /* PinCount */
	__OUTOPT uint32_t *pDioDisable /* Pin Enable or Disable: 1:Disable; 0:Enable */)
{
	uint32_t err = EApiGPIOGetCaps(Id, PinCount, pDioDisable);
	return err;
}


int AAEONGPIO::setDirection(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal)
{
    uint32_t err = EApiGPIOSetDirection((unsigned int)iPin, idTypeVal, setVal);   //ModeΪOUtput
    return err;
}

int AAEONGPIO::setLevel(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal)
{
    uint32_t err = EApiGPIOSetLevel((unsigned int)iPin, idTypeVal, setVal);
    return err;
}

int AAEONGPIO::getLevel(int iPin, int &pvalue)
{
    iPin=iPin+0x04;

    uint32_t getVal=0;

    uint32_t err = EApiGPIOGetLevel(iPin, 0xFFFFFFFF, &getVal);
    pvalue=getVal;
    return err;
}


uint32_t AAEONGPIO::get_level(
	__IN EApiId_t Id, /* GPIO Id */
	__IN uint32_t Bitmask, /* Bit mask of Affected Bits */
	__IN uint32_t *Level /* Level */)
{
	uint32_t err = EApiGPIOGetLevel(Id, Bitmask, Level);
	return err;
}




