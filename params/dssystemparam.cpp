#include "dssystemparam.h"
#include<QCoreApplication>
#include<QTime>
#include"packagechecker.h"

QString DSSystemParam::AppPath=QCoreApplication::applicationDirPath();
QString DSSystemParam::ParamsConfig="参数配置";
QString DSSystemParam::BrandName="Default";
QString DSSystemParam::BrandDirPath="品牌模板";
int DSSystemParam::CameraNum=1;
//ENUMSHIFT DSSystemParam::CurShift=ENUMSHIFT_NULL;
DSSystemParam::ENUMSYSTEMSTATE DSSystemParam::SystemState=ENUMSYSTEMSTATE_STOPPING;

ENUMSHIFT DSSystemParam::getCurrentShift()
{
    QTime curDate=QTime::currentTime();
    if(curDate>=PackageChecker::getInstance()->Options->ShiftMorning()&&curDate<PackageChecker::getInstance()->Options->ShiftAfternoon())
    {
        return ENUMSHIFT_MORNING;
    }
    else if(curDate>=PackageChecker::getInstance()->Options->ShiftAfternoon()&&curDate<PackageChecker::getInstance()->Options->ShiftNight())
    {
        return ENUMSHIFT_AFTERNOON;
    }
    else
    {
        return ENUMSHIFT_NIGHT;
    }
    return  ENUMSHIFT_NULL;
}
