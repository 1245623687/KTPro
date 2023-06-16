#ifndef ADVANTENCHGPIO_H
#define ADVANTENCHGPIO_H

#include"ciobase.h"
#include<mutex>


/*--------
    初始化时,2号PIN脚置1
    停止运行,2号Pin脚置0
--------*/
class AdvantenchGPIO : public CIOBase
{

    friend class CIOBase;

#define NAME_MAXIMUM_LENGTH 32
#define SUSIDEMO_MAIN_PAGE_MAX 10
    char GPIOName[NAME_MAXIMUM_LENGTH];
    /* 1: available; 0: unavailable */
    /* 1:可用；0:不可用*/
    int8_t available[SUSIDEMO_MAIN_PAGE_MAX];
#define SUSIDEMO_BANK_MAX 4
#define SUSIDEMO_GPIO_FUNCTION_MAX 5
    int8_t func[SUSIDEMO_GPIO_FUNCTION_MAX];

    enum funcRank
    {
        funcPin,
        funcIdType,
        funcDirection,
        funcLevel,
        funcGet
    };
    typedef struct
    {
        uint8_t supPinNum;	/* 0 ~ 31 */
        uint32_t supInput;
        uint32_t supOutput;
    } GPIOInfo, *PGPIOInfo;

    GPIOInfo infol[SUSIDEMO_BANK_MAX];
    int8_t page[SUSIDEMO_MAIN_PAGE_MAX];

    enum pageRank
    {
        wdog,
        hwm,
        smartfan,
        gpio,
        vga,
        smb,
        iic,
        storage,
        thermalprotection,
        info
    };
    typedef struct
    {
        uint32_t idType;
        uint32_t directionVal;
        uint32_t levelVal;
    } GPIOConfig, *PGPIOConfig;
    enum idtypeRank
    {
        idtypeSingle,
        idtypeMulti
    };

public:


    int init();

    int setDirection(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal);

    int setLevel(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal);


    ~AdvantenchGPIO();
private:
    int get_available(void);
    uint8_t GPIOInitialize();
    uint8_t gpio_init(int8_t *pavailable);
private:

    AdvantenchGPIO();


};

#endif // ADVANTENCHGPIO_H
