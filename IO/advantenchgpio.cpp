#include "IO/advantenchgpio.h"
#include"Susi4.h"
#include"dsdebug.h"
#include "glog/logging.h"

AdvantenchGPIO::AdvantenchGPIO()
{
    GPIOInitialize();
}



uint8_t AdvantenchGPIO::GPIOInitialize()
{
    uint32_t status = 0;
    uint8_t result;
    int32_t op;
    //QString str;
    LOG(INFO)<<"start initialize GPIO"<<endl;
    DSDEBUG<<"start initialize GPIO";
    status = SusiLibInitialize();

    if(status==SUSI_STATUS_NOT_INITIALIZED)
    {
        LOG(INFO)<<"initial susi4 failed";
        DSDEBUG<<"initial susi4 failed";
    }
    if (status != SUSI_STATUS_SUCCESS)
    {
        return status;
    }
    LOG(INFO)<<"initialize GPIO  sucess"<<endl;
    DSDEBUG<<"initialize GPIO  sucess";
    if (get_available() != 0)
    {
        status = SusiLibUninitialize();
        return status;
    }
}

uint8_t AdvantenchGPIO::gpio_init(int8_t *pavailable)
{
    uint32_t status, supportPin, id;
    uint8_t index, i, j;

    *pavailable = 0;
    index = 0;
    for (i = 0; i < SUSIDEMO_BANK_MAX; i++)
    {
        id = SUSI_ID_GPIO_BANK(i);

        status = SusiGPIOGetCaps(id, SUSI_ID_GPIO_INPUT_SUPPORT, &infol[i].supInput);
        if (status != SUSI_STATUS_SUCCESS)
            continue;

        status = SusiGPIOGetCaps(id, SUSI_ID_GPIO_OUTPUT_SUPPORT, &infol[i].supOutput);
        if (status != SUSI_STATUS_SUCCESS)
            continue;

        supportPin = infol[i].supInput | infol[i].supOutput;

        if (supportPin > 0)
        {
            for (j = 32; (supportPin & (1 << (j - 1))) == 0; j--);
            infol[i].supPinNum = j;
        }
        else
        {
            infol[i].supPinNum = 0;
        }

        index++;
    }

    if (index == 0)
        return 0;

    *pavailable = 1;

    func[funcPin] = funcPin;
    func[funcIdType] = funcIdType;
    func[funcDirection] = funcDirection;
    func[funcLevel] = funcLevel;
    func[funcGet] = funcGet;

    return 0;
}

int AdvantenchGPIO::get_available(void)
{
    uint8_t index, i;

    gpio_init(&available[gpio]);

    index = 0;
    for (i = 0; i < SUSIDEMO_MAIN_PAGE_MAX; i++)
    {
        if (available[i] == 1)
        {
            page[index] = i;
            index++;
        }
    }

    if (index == 0)
    {
        return 1;
    }
    for (i = index; i < SUSIDEMO_MAIN_PAGE_MAX; i++)
        page[i] = -1;

    return 0;
}

int AdvantenchGPIO::init()
{
    uint32_t status = 0;
    uint8_t result;
    int32_t op;
    //QString str;

    status = SusiLibInitialize();

    if (status != SUSI_STATUS_SUCCESS)
    {
        DSDEBUG<<"研华GPIO SusiLibInitialize失败： "<<status<<"  "<<endl;
        throw status;
        return status;
    }
    if (get_available() != 0)
    {
        DSDEBUG<<"研华GPIO get_available 失败"<<endl;
        status = SusiLibUninitialize();
        throw status;
        return status;
    }
}


AdvantenchGPIO::~AdvantenchGPIO()
{
    SusiLibUninitialize();
}


int AdvantenchGPIO::setDirection(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal)
{
    uint32_t status, id, mask;
    uint8_t iBank;

    //printf("\nSet Direction:\n\n");

    if (idTypeVal == idtypeSingle)
    {
        id = SUSI_ID_GPIO(iPin);
        mask = 1;

        /*if (input_uint(&setVal, 10, 1, 0) != 0)
         {
            return 1;
         }*/
    }
    else
    {
        return 1;
    }

    status = SusiGPIOSetDirection(id, mask, setVal);

    if (status != SUSI_STATUS_SUCCESS)
    {
        //printf("SusiGPIOSetDirection() failed. (0x%08X)\n", status);
        return 1;
    }

    //printf("SusiGPIOSetDirection() succeed.\n");
    return 0;
}

int AdvantenchGPIO::setLevel(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal)
{
    uint32_t status, id, mask;
    uint8_t iBank;

    //show_information(iPin, idTypeVal);

    //printf("\nSet Level:\n\n");

    if (idTypeVal == idtypeSingle)
    {
        id = SUSI_ID_GPIO(iPin);
        mask = 1;
        /*if (input_uint(&setVal, 10, 1, 0) != 0)
         {
             return 1;
         }*/
    }
    else
    {
        return 1;
    }

    status = SusiGPIOSetLevel(id, mask, setVal);
    if (status != SUSI_STATUS_SUCCESS)
    {
        //printf("SusiGPIOSetLevel() failed. (0x%08X)\n", status);
        return 1;
    }

    //printf("SusiGPIOSetLevel() succeed.\n");
    return 0;
}
