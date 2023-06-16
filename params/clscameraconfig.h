#ifndef CLSCAMERACONFIG_H
#define CLSCAMERACONFIG_H

#include<mutex>
#include"common/XmlHelper.h"
#include<QMap>


class ClsCameraConfig
{
public:
    typedef struct StuCameraParam
    {
    public:
        StuCameraParam(){}

        StuCameraParam(QString ip,int exposeTime,int offset,int wb_r,int wb_g,int wb_b,unsigned int lightCode):
            IPAddress(ip),
            ExposeTime(exposeTime),
            Offset(offset),
            WB_R(wb_r),
            WB_G(wb_g),
            WB_B(wb_b),
            Light_Code(lightCode)
        {}

        QString IPAddress;
        int ExposeTime;
        int Offset;
        int WB_R;
        int WB_G;
        int WB_B;
        unsigned int Light_Code;
    }StuCameraParam;



public:
    static ClsCameraConfig* getInstance();

    ~ClsCameraConfig(){

    }
    void save();

    unsigned int getLightCode(int cameraNo)
    {
        return  m_CameraParamArr[cameraNo].Light_Code;
    }

    void setLightCode(int cameraNo,unsigned int lightCode)
    {
        m_CameraParamArr[cameraNo].Light_Code=lightCode;
    }

    int getGain(int cameraNo)
    {
        return m_CameraParamArr[cameraNo].Offset;
    }

    int getExposureTime(int cameraNo)
    {
        return m_CameraParamArr[cameraNo].ExposeTime;
    }

    QString getIpaddress(int cameraNo)
    {
        return m_CameraParamArr[cameraNo].IPAddress;
    }

    void setGain(int cameraNo,int igain)
    {
        m_CameraParamArr[cameraNo].Offset=igain;
    }

    void setExposureTime(int cameraNo,int iet)
    {
        m_CameraParamArr[cameraNo].ExposeTime=iet;
    }

    void load();

    void savetoFile(QString dirPath);

    void createCameraConfigFile(QString filename,int cameraNum);

    void getCameraCfg(QMap<int,StuCameraParam>& cameraCfg)
    {
        cameraCfg.clear();
        cameraCfg=this->m_CameraParamArr;
    }



private:

    class GC
    {
    public :
        ~GC()
        {

            if (m_Instance != nullptr )
            {
                delete m_Instance;
                m_Instance = nullptr ;
            }
        }
    };
    static GC gc;


    static std::mutex m_Mutex;
    static ClsCameraConfig* m_Instance;
    ClsCameraConfig();
    void loadFromFile();



private:


    QMap<int,StuCameraParam> m_CameraParamArr;
    QString m_AllLightCode;
};

#endif // CLSCAMERACONFIG_H
