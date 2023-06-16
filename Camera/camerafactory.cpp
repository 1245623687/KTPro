#include "camerafactory.h"
//#include"cCarShuixingMer231.h"
#include"ccarshuixingmer231c.h"
//#include"chkusbcamera.h"
#include"csimucanera.h"
CameraFactory::CameraFactory()
{


}

//CCameraBase * CameraFactory::createCamera(ENUMCAMERATYPE t,QString ipAddress,int channel)
//{
//    switch (t)
//    {
//    case ENUMCAMERATYPE_AVT:
//    {
//        //CCameraBase* tmpCamera=new AVTCamera(ipAddress.toStdString(),channel);
//        CCameraBase* tmpCamera=new AVTCamera();
//        return  tmpCamera;
//    }
//        break;

//    }

//}


CCameraBase * CameraFactory::createCamera(ENUMCAMERATYPE t)
{
    switch (t)
    {
    case ENUMCAMERATYPE_MER2:
    {
        CCameraBase* tmpCamera=new cCarShuixingMer231C;
        return  tmpCamera;
    }

    case ENUMCAMERATYPE_HKUSB:
    {
//        CCameraBase* tmpCamera=new cHKUSBCamera;
//        return  tmpCamera;
    }
        break;

    case ENUMCAMERATYPE_SIMU:
    {
      cSimuCanera * tmpCamera=new cSimuCanera;
       return  tmpCamera;
    }
        break;

    }

}

