#ifndef CLSPHOTOTELECCONFIG_H
#define CLSPHOTOTELECCONFIG_H

//#include "mainwindow.h"
#include<mutex>
#include"common/XmlHelper.h"
#include<QMap>


class ClsPhototElecConfig
{

public:
    typedef struct StuPhotoElecParam
    {
    public:
        StuPhotoElecParam(){}

        StuPhotoElecParam(unsigned int* CapAngle,unsigned int KickAngle,unsigned int KickStep,unsigned int TrigAngle,
                          unsigned int SelfCheckAngle,unsigned int* ProbThreshold,unsigned int IsSendFaltSig,double Ratio)

        {
            memcpy(capAngle,CapAngle,sizeof(unsigned int)*12);
            kickAngle=KickAngle;
            kickStep=KickStep;
            trigAngle=TrigAngle;
            selfCheckAngle=SelfCheckAngle;
            memcpy(probThreshold,ProbThreshold,sizeof(unsigned int)*20);
            isSendFaltSig=IsSendFaltSig;
            ratio=Ratio;
        }

        unsigned int capAngle[12];
        unsigned int kickAngle;
        unsigned int kickStep;
        unsigned int trigAngle;
        unsigned int selfCheckAngle;
        unsigned int probThreshold[20];
        unsigned int isSendFaltSig;
        double ratio;
    }StuPhotoElecParam;
    
    
    typedef struct StuPhotoElecParamSim
    {
    public:
        StuPhotoElecParamSim(){}

        StuPhotoElecParamSim(double* CurrentArr,unsigned int* GainArr,unsigned int* ProbThreshold,double Ratio)

        {
            memcpy(currentArr,CurrentArr,sizeof(double)*20);
            memcpy(gainArr,GainArr,sizeof(unsigned int)*20);
            memcpy(probThreshold,ProbThreshold,sizeof(unsigned int)*20);
            ratio=Ratio;
        }
         double currentArr[20];
        unsigned int gainArr[20];
        unsigned int probThreshold[20];
        double ratio;
    }StuPhotoElecParamSim;


public:
    ClsPhototElecConfig();


    static ClsPhototElecConfig* getInstance();
    ~ClsPhototElecConfig(){}



    void setCapAngle(int Type,int Index,unsigned int val)
    {
        m_photoElecParamArr[Type].capAngle[Index]=val;
    }
    unsigned int getCapAngle(int Type,int Index)
    {
        return  m_photoElecParamArr[Type].capAngle[Index];
    }

    void setKickAngleAndStep(int Type,unsigned int val1,unsigned int val2)
    {
        m_photoElecParamArr[Type].kickAngle=val1;
        m_photoElecParamArr[Type].kickStep=val2;
    }

    void getKickAngleAndStep(int Type,unsigned int& val1, unsigned int& val2)
    {
        val1=m_photoElecParamArr[Type].kickAngle;
        val2=m_photoElecParamArr[Type].kickStep;
    }

    void setTrigAngle(int Type,unsigned int val1)
    {
        m_photoElecParamArr[Type].trigAngle=val1;
    }

    unsigned int getTrigAngle(int Type)
    {
        return  m_photoElecParamArr[Type].trigAngle;
    }

    void setSelfCheckAngle(int Type,unsigned int val1)
    {
        m_photoElecParamArr[Type].selfCheckAngle=val1;
    }

    unsigned int getSelfCheckAngle(int Type)
    {
        return  m_photoElecParamArr[Type].selfCheckAngle;
    }



    void setProThreshold(int Type,int Index,unsigned int val)
    {
        m_photoElecParamArr[Type].probThreshold[Index]=val;
    }
    unsigned int getProThreshold(int Type,int Index)
    {
        return  m_photoElecParamArr[Type].probThreshold[Index];
    }

    void setIsSendFaltSig(int Type,bool val1)
    {
        m_photoElecParamArr[Type].isSendFaltSig=val1;
    }

    bool getIsSendFaltSig(int Type)
    {
        return  m_photoElecParamArr[Type].isSendFaltSig;
    }

    void setRatio(int Type,double val)
    {
        m_photoElecParamArr[Type].ratio=val;
    }

    double getRatio(int Type)
    {
        return m_photoElecParamArr[Type].ratio;
    }
    
    
    
    //模拟板
    //调整系数
    void setRatioSim(int Type,double val)
    {
        m_photoElecParamArrSim[Type].ratio=val;
    }

    double getRatioSim(int Type)
    {
        return m_photoElecParamArrSim[Type].ratio;
    }
    
    //探头门槛值
    void setProThresholdSim(int Type,int Index,unsigned int val)
    {
        m_photoElecParamArrSim[Type].probThreshold[Index]=val;
    }

    unsigned int getProThresholdSim(int Type,int Index)
    {
        return  m_photoElecParamArrSim[Type].probThreshold[Index];
    }
    
   //探头电流
    void setProCurrentSim(int Type,int Index,double val)
    {
        m_photoElecParamArrSim[Type].currentArr[Index]=val;
    }
    double getProCurrentSim(int Type,int Index)
    {
        return  m_photoElecParamArrSim[Type].currentArr[Index];
    }
    
    //探头增益
    void setProGainSim(int Type,int Index,unsigned int val)
    {
        m_photoElecParamArrSim[Type].gainArr[Index]=val;
    }
    unsigned int getProGainSim(int Type,int Index)
    {
        return  m_photoElecParamArrSim[Type].gainArr[Index];
    }
    
    
    XMLElement* genCurrentXmlElementBlockSim(TinyXmlDocument & doc,int index);

    XMLElement* genGainXmlElementBlockSim(TinyXmlDocument & doc,int index);
    XMLElement* genProbValXmlElementBlockSim(TinyXmlDocument & doc,int index);
    
    
    




    XMLElement* genCapAngleXmlElementBlock(TinyXmlDocument & doc,int index);

    XMLElement* genProbValXmlElementBlock(TinyXmlDocument & doc,int index);





    void save();
    void load();

    void savetoFile(QString dirPath);

    void createPhotoElecFile(QString filename,int cameraNum);

    
    void getPhotoElecCfg(QMap<int,StuPhotoElecParam>& PhotoElecCfg,QMap<int,StuPhotoElecParamSim>& PhotoElecCfgSim)
    {
        PhotoElecCfg.clear();
        PhotoElecCfg=this->m_photoElecParamArr;
        
        PhotoElecCfgSim.clear();
        PhotoElecCfgSim=this->m_photoElecParamArrSim;
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
    static ClsPhototElecConfig* m_Instance;
    QMap<int,StuPhotoElecParam> m_photoElecParamArr;
    
     QMap<int,StuPhotoElecParamSim> m_photoElecParamArrSim;

    void loadFromFile();
};

#endif // CLSPHOTOTELECCONFIG_H
