#ifndef CAMERAFACTORY_H
#define CAMERAFACTORY_H

#include"Camera/ccamerabase.h"
#include"params/dssystemparam.h"


class CameraFactory
{

public:
    //static  CCameraBase * createCamera(ENUMCAMERATYPE t,QString ipAdd,int channel);
    static  CCameraBase * createCamera(ENUMCAMERATYPE t);
private:
    CameraFactory();
};

#endif // CAMERAFACTORY_H
