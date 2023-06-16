#ifdef __cplusplus
#pragma once

#include <string>
#include <list>
#include <string>
#include<map>
#include"opencv.hpp"
#include"dsdebug.h"
using namespace std;



//算子类型
enum ENUMCHECKOPERATORTYPE {
    ENUMCHECKOPERATORTYPE_TEMPLATEMATCH = 0,//对象搜索，模板匹配
    ENUMCHECKOPERATORTYPE_EDGEPOSITIONING = 1,//边缘定位,边缘检测
    ENUMCHECKOPERATORTYPE_DEFECTINSPECT = 2,//缺陷检查
    ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT = 3,//折线检查
    ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT = 4,//灰度测量
    ENUMCHECKOPERATORTYPE_LINEINSPECT = 5, //直线查找
    ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT = 6,//关联检查
    ENUMCHECKOPERATORTYPE_STAYINSPECT = 7,//拉线检查
    ENUMCHECKOPERATORTYPE_DEFECTINSPECT2 =8,//多边形缺陷检查
    ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT = 9,//拉线检查
    ENUMCHECKOPERATORTYPE_CIRCLEINSPECT = 10,//椭圆检查
    ENUMCHECKOPERATORTYPE_DLOBJECTDETECT = 11,//目标检测
    ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2 = 12,//关联检查2
};

//线的颜色
enum ENUMLINECOLOR
{
    ENUMLINECOLOR_GRAY = 0,
    ENUMLINECOLOR_R = 1,
    ENUMLINECOLOR_G = 2,
    ENUMLINECOLOR_B = 3,

};



//rgb->gray数据结构
typedef struct StuGrayConversion {
public:
    StuGrayConversion(int mode = 0, float red = 0.299, float green = 0.587, float blue = 0.114) :GrayMode(mode), RedRate(red), GreenRate(green), BlueRate(blue)
    {}

    StuGrayConversion(const StuGrayConversion& another)
    {
        this->GrayMode = another.GrayMode;
        this->RedRate = another.RedRate;
        this->GreenRate = another.GreenRate;
        this->BlueRate = another.BlueRate;
    }

    StuGrayConversion& operator=(StuGrayConversion& another)
    {
        this->GrayMode = another.GrayMode;
        this->RedRate = another.RedRate;
        this->GreenRate = another.GreenRate;
        this->BlueRate = another.BlueRate;
        return *this;
    }

    int GrayMode;
    double RedRate;
    double GreenRate;
    double BlueRate;
}StuGrayConversion;

//拉线检测中数据结构：霍夫
typedef struct StuHoughLine
{
public:
    StuHoughLine(int stAngle=0, int endAngle=0, int lineNum=0, int edgeThresh=0, int rgnR=0, int rgnQ=0, int threshShift=0) :
        StAngle(stAngle), EndAngle(endAngle), LineNum(lineNum), EdgeThresh(edgeThresh), RgnR(rgnR), RgnQ(rgnQ), ThreshShift(threshShift)
    {}

    void cpoyto(StuHoughLine* pCopyTo)
    {
        pCopyTo->StAngle=this->StAngle;
        pCopyTo->EndAngle=this->EndAngle;
        pCopyTo->LineNum=this->LineNum;
        pCopyTo->EdgeThresh=this->EdgeThresh;
        pCopyTo->RgnR=this->RgnR;
        pCopyTo->RgnQ=this->RgnQ;
        pCopyTo->ThreshShift=this->ThreshShift;

    }

    int StAngle;
    int EndAngle;
    int LineNum;
    int EdgeThresh;
    int RgnR;
    int RgnQ;
    int ThreshShift;
}StuHoughLine;

//折线检查算子中StuLineLenLineLen
typedef struct StuLineLenLineLen
{

public:
    //线的方向
    enum ENUMLINEDIR
    {
        ENUMLINEDIR_HORIZONTAL = 0,
        ENUMLINEDIR_VERTICAL = 1,
    };
public:
    StuLineLenLineLen(ENUMLINECOLOR lineColor= ENUMLINECOLOR_GRAY, int edgeThreshold=0, ENUMLINEDIR lineDir= ENUMLINEDIR_HORIZONTAL, ENUMLINEDIR scanLineDir= ENUMLINEDIR_HORIZONTAL,
                      int fitWeight=0, int lineLength=0) :
        LineColor(lineColor), EdgeThreshold(edgeThreshold), LineDir(lineDir), ScanLineDir(scanLineDir), FitWeight(fitWeight), LineLength(lineLength)
    {}

    void cpoyto(StuLineLenLineLen* pCopyTo)
    {
        pCopyTo->LineColor=this->LineColor;
        pCopyTo->EdgeThreshold=this->EdgeThreshold;
        pCopyTo->LineDir=this->LineDir;
        pCopyTo->ScanLineDir=this->ScanLineDir;
        pCopyTo->FitWeight=this->FitWeight;
        pCopyTo->LineLength=this->LineLength;

    }


    ENUMLINECOLOR LineColor;
    int EdgeThreshold;
    ENUMLINEDIR LineDir;
    ENUMLINEDIR ScanLineDir;
    int FitWeight;
    int LineLength;
};

//basic数据结构
typedef struct StuCheckOperatorBasic
{
public:
    StuCheckOperatorBasic(
            int index=0,
            float RectValid_Xs = 0.0,
            float RectValid_Ys = 0.0,
            float RectValid_Xe = 0.0,
            float RectValid_Ye = 0.0,
            float RectValidInvalid_Xs = 0.0,
            float RectValidInvalid_Ys = 0.0,
            float RectValidInvalid_Xe = 0.0,
            float RectValidInvalid_Ye = 0.0,
            bool ExistInvalidRect = false,
            bool IsCheck = true,
            bool Vaild = true,
            string Ref_X = "",
            string Ref_Y = "",
            float Ref_Angle = 0.0,
            int AsRef = 0,
            int AsDir = 0,
            int AsContinusCheck = 0,
            int isAssociated=0,
            StuGrayConversion* GrayConversion = new StuGrayConversion()
            ) :
        Index(index),
        Basic_RectValid_Xs(RectValid_Xs),
        Basic_RectValid_Ys(RectValid_Ys),
        Basic_RectValid_Xe(RectValid_Xe),
        Basic_RectValid_Ye(RectValid_Ye),
        Basic_RectValidInvalid_Xs(RectValidInvalid_Xs),
        Basic_RectValidInvalid_Ys(RectValidInvalid_Ys),
        Basic_RectValidInvalid_Xe(RectValidInvalid_Xe),
        Basic_RectValidInvalid_Ye(RectValidInvalid_Ye),
        Basic_ExistInvalidRect(ExistInvalidRect),
        Basic_IsCheck(IsCheck),
        Basic_Vaild(Vaild),
        Basic_Ref_X(Ref_X),
        Basic_Ref_Y(Ref_Y),
        Basic_Ref_Angle(Ref_Angle),
        Basic_AsRef(AsRef),
        Basic_AsDir(AsDir),
        Basic_AsContinusCheck(AsContinusCheck),
        IsAssociated(isAssociated),
        Basic_GrayConversion(GrayConversion)
    {
    }

    StuCheckOperatorBasic(const StuCheckOperatorBasic& another)
    {
        this->Index = another.Index;
        this->Basic_RectValid_Xs = another.Basic_RectValid_Xs;
        this->Basic_RectValid_Ys = another.Basic_RectValid_Ys;
        this->Basic_RectValid_Xe = another.Basic_RectValid_Xe;
        this->Basic_RectValid_Ye = another.Basic_RectValid_Ye;
        this->Basic_RectValidInvalid_Xs = another.Basic_RectValidInvalid_Xs;
        this->Basic_RectValidInvalid_Ys = another.Basic_RectValidInvalid_Ys;
        this->Basic_RectValidInvalid_Xe = another.Basic_RectValidInvalid_Xe;
        this->Basic_RectValidInvalid_Ye = another.Basic_RectValidInvalid_Ye;
        this->Basic_ExistInvalidRect = another.Basic_ExistInvalidRect;
        this->Basic_IsCheck = another.Basic_IsCheck;
        this->Basic_Vaild = another.Basic_Vaild;
        this->Basic_Ref_X = another.Basic_Ref_X;
        this->Basic_Ref_Y = another.Basic_Ref_Y;
        this->Basic_Ref_Angle = another.Basic_Ref_Angle;
        this->Basic_AsRef = another.Basic_AsRef;
        this->Basic_AsDir = another.Basic_AsDir;
        this->Basic_AsContinusCheck = another.Basic_AsContinusCheck;
        this->IsAssociated=another.IsAssociated;
        this->Basic_GrayConversion = another.Basic_GrayConversion;
    }





    StuCheckOperatorBasic& operator=(StuCheckOperatorBasic& another)
    {
        this->Index = another.Index;
        this->Basic_RectValid_Xs = another.Basic_RectValid_Xs;
        this->Basic_RectValid_Ys = another.Basic_RectValid_Ys;
        this->Basic_RectValid_Xe = another.Basic_RectValid_Xe;
        this->Basic_RectValid_Ye = another.Basic_RectValid_Ye;
        this->Basic_RectValidInvalid_Xs = another.Basic_RectValidInvalid_Xs;
        this->Basic_RectValidInvalid_Ys = another.Basic_RectValidInvalid_Ys;
        this->Basic_RectValidInvalid_Xe = another.Basic_RectValidInvalid_Xe;
        this->Basic_RectValidInvalid_Ye = another.Basic_RectValidInvalid_Ye;
        this->Basic_ExistInvalidRect = another.Basic_ExistInvalidRect;
        this->Basic_IsCheck = another.Basic_IsCheck;
        this->Basic_Vaild = another.Basic_Vaild;
        this->Basic_Ref_X = another.Basic_Ref_X;
        this->Basic_Ref_Y = another.Basic_Ref_Y;
        this->Basic_Ref_Angle = another.Basic_Ref_Angle;
        this->Basic_AsRef = another.Basic_AsRef;
        this->Basic_AsDir = another.Basic_AsDir;
        this->Basic_AsContinusCheck = another.Basic_AsContinusCheck;
        this->IsAssociated=another.IsAssociated;
        this->Basic_GrayConversion = another.Basic_GrayConversion;
        return *this;
    }

    ~StuCheckOperatorBasic() {

        if(this->Basic_GrayConversion != NULL)
        {
            delete this->Basic_GrayConversion;
            this->Basic_AsContinusCheck = NULL;
        }
    }

    void cpoyto(StuCheckOperatorBasic* pCopyto)
    {

        pCopyto->Index=this->Index;
        pCopyto->Basic_RectValid_Xs=this->Basic_RectValid_Xs;
        pCopyto->Basic_RectValid_Ys=this->Basic_RectValid_Ys;
        pCopyto->Basic_RectValid_Xe=this->Basic_RectValid_Xe;
        pCopyto->Basic_RectValid_Ye=this->Basic_RectValid_Ye;
        pCopyto->Basic_RectValidInvalid_Xs=this->Basic_RectValidInvalid_Xs;
        pCopyto->Basic_RectValidInvalid_Ys=this->Basic_RectValidInvalid_Ys;
        pCopyto->Basic_RectValidInvalid_Xe=this->Basic_RectValidInvalid_Xe;
        pCopyto->Basic_RectValidInvalid_Ye=this->Basic_RectValidInvalid_Ye;
        pCopyto->Basic_ExistInvalidRect=this->Basic_ExistInvalidRect;

        pCopyto->Basic_IsCheck=this->Basic_IsCheck;
        pCopyto->Basic_Vaild=this->Basic_Vaild;
        pCopyto->Basic_Ref_X=this->Basic_Ref_X;
        pCopyto->Basic_Ref_Y=this->Basic_Ref_Y;
        pCopyto->Basic_Ref_Angle=this->Basic_Ref_Angle;
        pCopyto->Basic_AsRef=this->Basic_AsRef;
        pCopyto->Basic_AsDir=this->Basic_AsDir;
        pCopyto->Basic_AsContinusCheck=this->Basic_AsContinusCheck;

        pCopyto->IsAssociated=this->IsAssociated;


        pCopyto->Basic_GrayConversion->RedRate=this->Basic_GrayConversion->RedRate;
        pCopyto->Basic_GrayConversion->BlueRate=this->Basic_GrayConversion->BlueRate;
        pCopyto->Basic_GrayConversion->GrayMode=this->Basic_GrayConversion->GrayMode;
        pCopyto->Basic_GrayConversion->GreenRate=this->Basic_GrayConversion->GreenRate;
    }

public:
    int Index;//当前算子编号索引

    float Basic_RectValid_Xs;
    float Basic_RectValid_Ys;
    float Basic_RectValid_Xe;
    float Basic_RectValid_Ye;

    float Basic_RectValidInvalid_Xs;
    float Basic_RectValidInvalid_Ys;
    float Basic_RectValidInvalid_Xe;
    float Basic_RectValidInvalid_Ye;

    bool Basic_ExistInvalidRect;
    bool Basic_IsCheck;
    bool Basic_Vaild;

    string Basic_Ref_X;
    string Basic_Ref_Y;
    float Basic_Ref_Angle;
    int Basic_AsRef;
    int Basic_AsDir;
    int Basic_AsContinusCheck;
    int IsAssociated;
    StuGrayConversion* Basic_GrayConversion;
}StuCheckOperatorBasic;


//基础算子：基类
class CheckOperatorBasic
{
public:
    CheckOperatorBasic(
            ENUMCHECKOPERATORTYPE checkType,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) : CheckType(checkType),
        stu_CheckOperatorBasic(stuCheckOperatorBasic)
    {
    }

    CheckOperatorBasic(const CheckOperatorBasic& another)
    {
        this->CheckType = another.CheckType;
        this->stu_CheckOperatorBasic = another.stu_CheckOperatorBasic;
    }


    CheckOperatorBasic& operator=(CheckOperatorBasic& another)
    {

        if (&another != this)
        {
            this->CheckType = another.CheckType;
            this->stu_CheckOperatorBasic = another.stu_CheckOperatorBasic;
        }
        return *this;
    }

    virtual void function() {};

    ~CheckOperatorBasic()
    {
        if (this->stu_CheckOperatorBasic!=NULL)
        {
            delete this->stu_CheckOperatorBasic;
            this->stu_CheckOperatorBasic = NULL;
        }
    }

public:
    ENUMCHECKOPERATORTYPE CheckType;//算子类型
    StuCheckOperatorBasic* stu_CheckOperatorBasic;
};


//关联检查算子
class CheckOperatorAssociatedInspect :public CheckOperatorBasic
{
public:
    //线的方向
    enum ENUMLINETYPE
    {
        ENUMLINETYPE_SINGLE = 0,
        ENUMLINETYPE_DOUBLE = 1,
    };

    enum ENUMLINEDIR
    {
        ENUMLINEDIR_HORIZONTAL = 0,
        ENUMLINEDIR_VERTICAL = 1,
    };
public:
    CheckOperatorAssociatedInspect(
            float stdDis = 0.0,
            int maxShift = 10,
            int relaty1 = 0,
            int relaty2 = 0,
            ENUMLINEDIR lineDir = ENUMLINEDIR_HORIZONTAL,
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :StdDis(stdDis), MaxShift(maxShift), Relaty1(relaty1), Relaty2(relaty2), LineDir(lineDir),LineType(ENUMLINETYPE_SINGLE),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }

    CheckOperatorAssociatedInspect(const CheckOperatorAssociatedInspect& another) :CheckOperatorBasic(another)
    {
        this->StdDis = another.StdDis;
        this->MaxShift = another.MaxShift;
        this->Relaty1 = another.Relaty1;
        this->Relaty2 = another.Relaty2;
        this->LineType = another.LineType;
        this->LineDir = another.LineDir;
    }


    CheckOperatorAssociatedInspect& operator=(CheckOperatorAssociatedInspect& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->StdDis = another.StdDis;
            this->MaxShift = another.MaxShift;
            this->Relaty1 = another.Relaty1;
            this->Relaty2 = another.Relaty2;
            this->LineType = another.LineType;
            this->LineDir = another.LineDir;
        }
        return *this;
    }
    virtual void function() {};

    ~CheckOperatorAssociatedInspect() {

    }

    void copyto(CheckOperatorAssociatedInspect* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->StdDis=this->StdDis;
        pCopyto->MaxShift=this->MaxShift;
        pCopyto->Relaty1=this->Relaty1;
        pCopyto->Relaty2=this->Relaty2;
        pCopyto->LineType=this->LineType;
        pCopyto->LineDir=this->LineDir;
    }
public:
    float StdDis;
    int MaxShift;
    int Relaty1;
    int Relaty2;
    ENUMLINETYPE LineType;
    ENUMLINEDIR LineDir;
};

//椭圆检查算子
class CheckOperatorCircleInspect :public CheckOperatorBasic
{
public:
    CheckOperatorCircleInspect(
            int threshLowOut=90,
            int threshHighOut=255,
            int threshLowIn=50,
            int threshHighIn=190,

            int presOut=20,
            int presIn=20,
            int threshInCirlVal=150,
            int threshOutCirlVal=220,

            int minCirleArea=900,
            int maxCirleArea=1800,

            int presCircle=93,

            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_CIRCLEINSPECT,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :ThreshLowOut(threshLowOut),ThreshHighOut(threshHighOut),
        ThreshLowIn(threshLowIn),ThreshHighIn(threshHighIn),
        PresOut(presOut),PresIn(presIn),ThreshInCirlVal(threshInCirlVal),ThreshOutCirlVal(threshOutCirlVal),MinCirleArea(minCirleArea),
        MaxCirleArea(maxCirleArea),PresCircle(presCircle),NoCheck(4),Thrick(6),CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }
    CheckOperatorCircleInspect(const CheckOperatorCircleInspect& another) :CheckOperatorBasic(another)
    {
        this->ThreshLowOut = another.ThreshLowOut;
        this->ThreshHighOut = another.ThreshHighOut;
        this->ThreshLowIn = another.ThreshLowIn;
        this->ThreshHighIn = another.ThreshHighIn;

        this->PresOut = another.PresOut;
        this->PresIn = another.PresIn;

        this->ThreshInCirlVal = another.ThreshInCirlVal;
        this->ThreshOutCirlVal = another.ThreshOutCirlVal;

        this->MinCirleArea = another.MinCirleArea;
        this->MaxCirleArea = another.MaxCirleArea;

        this->PresCircle = another.PresCircle;
        this->NoCheck = another.NoCheck;
        this->Thrick=another.Thrick;


    }

    CheckOperatorCircleInspect& operator=(CheckOperatorCircleInspect& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->ThreshLowOut = another.ThreshLowOut;
            this->ThreshHighOut = another.ThreshHighOut;
            this->ThreshLowIn = another.ThreshLowIn;
            this->ThreshHighIn = another.ThreshHighIn;

            this->PresOut = another.PresOut;
            this->PresIn = another.PresIn;

            this->ThreshInCirlVal = another.ThreshInCirlVal;
            this->ThreshOutCirlVal = another.ThreshOutCirlVal;

            this->MinCirleArea = another.MinCirleArea;
            this->MaxCirleArea = another.MaxCirleArea;

            this->PresCircle = another.PresCircle;
            this->NoCheck = another.NoCheck;
            this->Thrick=another.Thrick;
        }
        return *this;
    }

    virtual void function() {};

    ~CheckOperatorCircleInspect()
    {
        //DSDEBUG<<"enter ~CheckOperatorDefectInspect()"<<endl;
    }

    void copyto(CheckOperatorCircleInspect* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->ThreshLowOut=this->ThreshLowOut;
        pCopyto->ThreshHighOut=this->ThreshHighOut;
        pCopyto->ThreshLowIn=this->ThreshLowIn;
        pCopyto->ThreshHighIn=this->ThreshHighIn;
        pCopyto->PresOut=this->PresOut;
        pCopyto->PresIn=this->PresIn;

        pCopyto->ThreshInCirlVal=this->ThreshInCirlVal;
        pCopyto->ThreshOutCirlVal=this->ThreshOutCirlVal;

        pCopyto->MinCirleArea=this->MinCirleArea;
        pCopyto->MaxCirleArea=this->MaxCirleArea;


        pCopyto->PresCircle=this->PresCircle;
        pCopyto->NoCheck=this->NoCheck;
         pCopyto->Thrick=this->Thrick;
    }

public:
    int ThreshLowOut;
    int ThreshHighOut;

    int ThreshLowIn;
    int ThreshHighIn;

    int PresOut;
    int PresIn;

    int ThreshInCirlVal;
    int ThreshOutCirlVal;

    //
    int MinCirleArea;
    int MaxCirleArea;


    int PresCircle;
    int NoCheck;
    int Thrick;
};




//缺陷检查算子
class CheckOperatorDefectInspect :public CheckOperatorBasic
{
public:
    CheckOperatorDefectInspect(
            int maxArea=300,
            int binThresh=60,
            bool isBlackBlob=0,
            bool isDefectAll=1,
            int minArea=300,
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_DEFECTINSPECT,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :MaxArea(maxArea), BinThresh(binThresh), IsBlackBlob(isBlackBlob), IsDefectAll(isDefectAll),MinArea(minArea),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }
    CheckOperatorDefectInspect(const CheckOperatorDefectInspect& another) :CheckOperatorBasic(another)
    {
        this->MaxArea = another.MaxArea;
        this->BinThresh = another.BinThresh;
        this->IsBlackBlob = another.IsBlackBlob;
        this->IsDefectAll = another.IsDefectAll;
        this->MinArea = another.MinArea;
    }


    CheckOperatorDefectInspect& operator=(CheckOperatorDefectInspect& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->MaxArea = another.MaxArea;
            this->BinThresh = another.BinThresh;
            this->IsBlackBlob = another.IsBlackBlob;
            this->IsDefectAll = another.IsDefectAll;
            this->MinArea = another.MinArea;
        }
        return *this;
    }

    virtual void function() {};

    ~CheckOperatorDefectInspect()
    {
        //DSDEBUG<<"enter ~CheckOperatorDefectInspect()"<<endl;
    }

    void copyto(CheckOperatorDefectInspect* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->MaxArea=this->MaxArea;
        pCopyto->BinThresh=this->BinThresh;
        pCopyto->IsBlackBlob=this->IsBlackBlob;
        pCopyto->IsDefectAll=this->IsDefectAll;
        pCopyto->MinArea=this->MinArea;

    }

public:
    int MaxArea;
    int BinThresh;
    bool IsBlackBlob;
    bool IsDefectAll;
    int MinArea;
};

//目标检测
class CheckOperatorDLObjectDetect:public CheckOperatorBasic
{
public:
    CheckOperatorDLObjectDetect(int confidence=25, int minArea=50,int minVal=110, int valUpLimt=250, int accuracyType=0,ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_DLOBJECTDETECT,
        StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()):m_iConfidence(confidence),m_iMinArea(minArea),m_iMinGrayVal(minVal),m_iValUpLimt(valUpLimt),m_iaccuracyType(accuracyType),
        CheckOperatorBasic(checkType,stuCheckOperatorBasic)
    {

    }
    CheckOperatorDLObjectDetect(const CheckOperatorDLObjectDetect& another):CheckOperatorBasic(another)
    {
        this->m_iConfidence=another.m_iConfidence;
        this->m_iMinArea=another.m_iMinArea;
        this->m_iMinGrayVal=another.m_iMinGrayVal;
        this->m_iValUpLimt=another.m_iValUpLimt;

        this->m_iaccuracyType=another.m_iaccuracyType;
    }
    CheckOperatorDLObjectDetect& operator =( CheckOperatorDLObjectDetect& another)
    {
        if(&another!=this)
        {
            CheckOperatorBasic::operator=(another);
            this->m_iConfidence=another.m_iConfidence;
            this->m_iMinArea=another.m_iMinArea;
            this->m_iValUpLimt=another.m_iValUpLimt;
            this->m_iMinGrayVal=another.m_iMinGrayVal;

            this->m_iaccuracyType=another.m_iaccuracyType;
        }
        return *this;
    }

    void copyto(CheckOperatorDLObjectDetect* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;
        this->stu_CheckOperatorBasic->cpoyto(pCopyto->stu_CheckOperatorBasic);
        pCopyto->m_iConfidence=this->m_iConfidence;
        pCopyto->m_iMinArea=this->m_iMinArea;
        pCopyto->m_iMinGrayVal=this->m_iMinGrayVal;
        pCopyto->m_iValUpLimt=this->m_iValUpLimt;

        pCopyto->m_iaccuracyType=this->m_iaccuracyType;
    }

    virtual void function() {} ;
    ~CheckOperatorDLObjectDetect()
    {

    }

public:
    int m_iConfidence;
    int m_iMinArea;
    int m_iMinGrayVal;
    int m_iValUpLimt;
    int m_iaccuracyType;
};


//缺陷检查算子
class CheckOperatorDefectInspect2 :public CheckOperatorBasic
{
public:
    CheckOperatorDefectInspect2(
            int maxArea=300,
            int binThresh=60,
            bool isBlackBlob=0,
            int minArea=300,
            bool isDefectAll=1,
            int x1=0,int y1=160,
            int x2=0,int y2=160,
            int x3=0,int y3=160,
            int x4=0,int y4=160,
            int x5=0,int y5=160,
            int x6=0,int y6=160,
            int x7=0,int y7=160,
            int x8=0,int y8=160,
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_DEFECTINSPECT2,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :MaxArea(maxArea), BinThresh(binThresh), IsBlackBlob(isBlackBlob), MinArea(minArea),IsDefectAll(isDefectAll),
        X1(x1),Y1(y1),X2(x2),Y2(y2),X3(x3),Y3(y3),X4(x4),Y4(y4), X5(x5),Y5(y5),X6(x6),Y6(y6),X7(x7),Y7(y7),X8(x8),Y8(y8),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }
    CheckOperatorDefectInspect2(const CheckOperatorDefectInspect2& another) :CheckOperatorBasic(another)
    {
        this->MaxArea = another.MaxArea;
        this->BinThresh = another.BinThresh;
        this->IsBlackBlob = another.IsBlackBlob;
        this->MinArea = another.MinArea;
        this->IsDefectAll = another.IsDefectAll;
        this->X1=another.X1;
        this->X2=another.X2;
        this->X3=another.X3;
        this->X4=another.X4;
        this->Y1=another.Y1;
        this->Y2=another.Y2;
        this->Y3=another.Y3;
        this->Y4=another.Y4;
        this->X5=another.X5;
        this->Y5=another.Y5;
        this->X6=another.X6;
        this->Y6=another.Y6;
        this->X7=another.X7;
        this->Y7=another.Y7;
        this->X8=another.X8;
        this->Y8=another.Y8;

    }


    CheckOperatorDefectInspect2& operator=(CheckOperatorDefectInspect2& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->MaxArea = another.MaxArea;
            this->BinThresh = another.BinThresh;
            this->IsBlackBlob = another.IsBlackBlob;
            this->MinArea = another.MinArea;
            this->IsDefectAll = another.IsDefectAll;

            this->X1=another.X1;
            this->X2=another.X2;
            this->X3=another.X3;
            this->X4=another.X4;
            this->Y1=another.Y1;
            this->Y2=another.Y2;
            this->Y3=another.Y3;
            this->Y4=another.Y4;
            this->X5=another.X5;
            this->Y5=another.Y5;
            this->X6=another.X6;
            this->Y6=another.Y6;
            this->X7=another.X7;
            this->Y7=another.Y7;
            this->X8=another.X8;
            this->Y8=another.Y8;
        }
        return *this;
    }

    virtual void function() {};

    ~CheckOperatorDefectInspect2()
    {
        //DSDEBUG<<"enter ~CheckOperatorDefectInspect()"<<endl;
    }

    void copyto(CheckOperatorDefectInspect2* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->MaxArea=this->MaxArea;
        pCopyto->BinThresh=this->BinThresh;
        pCopyto->IsBlackBlob=this->IsBlackBlob;
        pCopyto->MinArea=this->MinArea;

        pCopyto->X1=this->X1;
        pCopyto->Y1=this->Y1;
        pCopyto->X2=this->X2;
        pCopyto->Y2=this->Y2;
        pCopyto->X3=this->X3;
        pCopyto->Y3=this->Y3;
        pCopyto->X4=this->X4;
        pCopyto->Y4=this->Y4;

        pCopyto->X5=this->X5;
        pCopyto->Y5=this->Y5;
        pCopyto->X6=this->X6;
        pCopyto->Y6=this->Y6;
        pCopyto->X7=this->X7;
        pCopyto->Y7=this->Y7;
        pCopyto->X8=this->X8;
        pCopyto->Y8=this->Y8;

    }


public:
    int MaxArea;
    int BinThresh;
    bool IsBlackBlob;
    int MinArea;
     bool IsDefectAll;
    int X1;
    int Y1;
    int X2;
    int Y2;
    int X3;
    int Y3;
    int X4;
    int Y4;
    int X5;
    int Y5;
    int X6;
    int Y6;
    int X7;
    int Y7;
    int X8;
    int Y8;
};


//关联检测算子2
class CheckOperatorAssociatedInspect2 :public CheckOperatorBasic
{
public:
    CheckOperatorAssociatedInspect2(
            int maxArea=300,
            int binThresh=60,
            bool isBlackBlob=0,
            int minArea=300,
            bool isDefectAll=1,
            int x1=0,int y1=160,
            int x2=0,int y2=160,
            int x3=0,int y3=160,
            int x4=0,int y4=160,
            int x5=0,int y5=160,
            int x6=0,int y6=160,
            int x7=0,int y7=160,
            int x8=0,int y8=160,
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :MaxArea(maxArea), BinThresh(binThresh), IsBlackBlob(isBlackBlob), MinArea(minArea),IsDefectAll(isDefectAll),
        X1(x1),Y1(y1),X2(x2),Y2(y2),X3(x3),Y3(y3),X4(x4),Y4(y4), X5(x5),Y5(y5),X6(x6),Y6(y6),X7(x7),Y7(y7),X8(x8),Y8(y8),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }
    CheckOperatorAssociatedInspect2(const CheckOperatorAssociatedInspect2& another) :CheckOperatorBasic(another)
    {
        this->MaxArea = another.MaxArea;
        this->BinThresh = another.BinThresh;
        this->IsBlackBlob = another.IsBlackBlob;
        this->MinArea = another.MinArea;
        this->IsDefectAll = another.IsDefectAll;
        this->X1=another.X1;
        this->X2=another.X2;
        this->X3=another.X3;
        this->X4=another.X4;
        this->Y1=another.Y1;
        this->Y2=another.Y2;
        this->Y3=another.Y3;
        this->Y4=another.Y4;
        this->X5=another.X5;
        this->Y5=another.Y5;
        this->X6=another.X6;
        this->Y6=another.Y6;
        this->X7=another.X7;
        this->Y7=another.Y7;
        this->X8=another.X8;
        this->Y8=another.Y8;

    }


    CheckOperatorAssociatedInspect2& operator=(CheckOperatorAssociatedInspect2& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->MaxArea = another.MaxArea;
            this->BinThresh = another.BinThresh;
            this->IsBlackBlob = another.IsBlackBlob;
            this->MinArea = another.MinArea;
            this->IsDefectAll = another.IsDefectAll;

            this->X1=another.X1;
            this->X2=another.X2;
            this->X3=another.X3;
            this->X4=another.X4;
            this->Y1=another.Y1;
            this->Y2=another.Y2;
            this->Y3=another.Y3;
            this->Y4=another.Y4;
            this->X5=another.X5;
            this->Y5=another.Y5;
            this->X6=another.X6;
            this->Y6=another.Y6;
            this->X7=another.X7;
            this->Y7=another.Y7;
            this->X8=another.X8;
            this->Y8=another.Y8;
        }
        return *this;
    }

    virtual void function() {};

    ~CheckOperatorAssociatedInspect2()
    {
        //DSDEBUG<<"enter ~CheckOperatorDefectInspect()"<<endl;
    }

    void copyto(CheckOperatorAssociatedInspect2* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->MaxArea=this->MaxArea;
        pCopyto->BinThresh=this->BinThresh;
        pCopyto->IsBlackBlob=this->IsBlackBlob;
        pCopyto->MinArea=this->MinArea;

        pCopyto->X1=this->X1;
        pCopyto->Y1=this->Y1;
        pCopyto->X2=this->X2;
        pCopyto->Y2=this->Y2;
        pCopyto->X3=this->X3;
        pCopyto->Y3=this->Y3;
        pCopyto->X4=this->X4;
        pCopyto->Y4=this->Y4;

        pCopyto->X5=this->X5;
        pCopyto->Y5=this->Y5;
        pCopyto->X6=this->X6;
        pCopyto->Y6=this->Y6;
        pCopyto->X7=this->X7;
        pCopyto->Y7=this->Y7;
        pCopyto->X8=this->X8;
        pCopyto->Y8=this->Y8;

    }


public:
    int MaxArea;
    int BinThresh;
    bool IsBlackBlob;
    int MinArea;
     bool IsDefectAll;
    int X1;
    int Y1;
    int X2;
    int Y2;
    int X3;
    int Y3;
    int X4;
    int Y4;
    int X5;
    int Y5;
    int X6;
    int Y6;
    int X7;
    int Y7;
    int X8;
    int Y8;
};


//直线检查算子
class CheckOperatorLineInspect :public CheckOperatorBasic
{
public:
    //线的方向
    enum ENUMLINEDIR
    {
        ENUMLINEDIR_HORIZONTAL = 0,
        ENUMLINEDIR_VERTICAL = 1,
    };
public:
    CheckOperatorLineInspect(
            int binThreshold=0,
            int precision=0,
            int score=0,
            ENUMLINEDIR lineDir= ENUMLINEDIR_HORIZONTAL,
            int maxShift=0,
            int angleMax=0,
            int angleMin=0,
            int angleShift=0,
            bool hasStdLine=0,
            float stdLine_xs=0,
            float stdLine_ys=0,
            float stdLine_xe=0,
            float stdLine_ye=0,
            float stdLine_a=0,
            float stdLine_b=0,
            float stdLine_c=0,
            StuHoughLine* houghLine= new StuHoughLine(),
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_LINEINSPECT,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :BinThreshold(binThreshold), Precision(precision), Score(score), LineDir(lineDir), MaxShift(maxShift), AngleMax(angleMax), AngleMin(angleMin), AngleShift(angleShift), HasStdLine(hasStdLine),
        StdLine_xs(stdLine_xs), StdLine_ys(stdLine_ys), StdLine_xe(stdLine_xe), StdLine_ye(stdLine_ye), StdLine_a(stdLine_a), StdLine_b(stdLine_b), StdLine_c(stdLine_c), StuStuHoughLine(houghLine),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }

    CheckOperatorLineInspect(const CheckOperatorLineInspect& another):CheckOperatorBasic(another)
    {
        this->BinThreshold = another.BinThreshold;
        this->Precision = another.Precision;
        this->Score = another.Score;
        this->LineDir = another.LineDir;
        this->MaxShift = another.MaxShift;
        this->AngleMax = another.AngleMax;
        this->AngleMin = another.AngleMin;
        this->AngleShift = another.AngleShift;
        this->HasStdLine = another.HasStdLine;
        this->StdLine_xs = another.StdLine_xs;
        this->StdLine_ys = another.StdLine_ys;
        this->StdLine_xe = another.StdLine_xe;
        this->StdLine_ye = another.StdLine_ye;
        this->StdLine_a = another.StdLine_a;
        this->StdLine_b = another.StdLine_b;
        this->StdLine_c = another.StdLine_c;
        this->StuStuHoughLine = another.StuStuHoughLine;
    }

    CheckOperatorLineInspect& operator=(CheckOperatorLineInspect& another)
    {
        if (&another != this) {
            CheckOperatorBasic::operator=(another);

            //自有
            this->BinThreshold = another.BinThreshold;
            this->Precision = another.Precision;
            this->Score = another.Score;
            this->LineDir = another.LineDir;
            this->MaxShift = another.MaxShift;
            this->AngleMax = another.AngleMax;
            this->AngleMin = another.AngleMin;
            this->AngleShift = another.AngleShift;
            this->HasStdLine = another.HasStdLine;
            this->StdLine_xs = another.StdLine_xs;
            this->StdLine_ys = another.StdLine_ys;
            this->StdLine_xe = another.StdLine_xe;
            this->StdLine_ye = another.StdLine_ye;
            this->StdLine_a = another.StdLine_a;
            this->StdLine_b = another.StdLine_b;
            this->StdLine_c = another.StdLine_c;
            this->StuStuHoughLine = another.StuStuHoughLine;
        }
        return *this;
    }


    virtual void function() {};
    ~CheckOperatorLineInspect()
    {
        //DSDEBUG<<"enter ~CheckOperatorLineInspect()"<<endl;
        if (this->StuStuHoughLine!=NULL)
        {
            delete this->StuStuHoughLine;
            this->StuStuHoughLine = NULL;
        }
    }
public:

    int BinThreshold;
    int Precision;
    int Score;
    ENUMLINEDIR LineDir;
    int MaxShift;
    int AngleMax;
    int AngleMin;
    int AngleShift;

    bool HasStdLine;
    float StdLine_xs;
    float StdLine_ys;
    float StdLine_xe;
    float StdLine_ye;
    float StdLine_a;
    float StdLine_b;
    float StdLine_c;
    StuHoughLine* StuStuHoughLine;
};


//折线检查算子
class CheckOperatorBrokenlineInspect :public CheckOperatorBasic
{
public:
    CheckOperatorBrokenlineInspect(
            int binThreshold=0,
            int precision=0,
            int score=0,
            double angleMax=90,
            double angleMin=-90,
            int lineLength=0,
            int angleShift=0,
            int line1Seg_xs=0,
            int line1Seg_ys=0,
            int line1Seg_xe=0,
            int line1Seg_ye=0,
            int line2Seg_xs=0,
            int line2Seg_ys=0,
            int line2Seg_xe=0,
            int line2Seg_ye=0,
            bool hasStdLine=0,
            int stdLine_xs=0,
            int stdLine_ys=0,
            int stdLine_xe=0,
            int stdLine_ye=0,
            int stdLine_a=0,
            int stdLine_b=0,
            int stdLine_c=0,
            StuHoughLine* houghLine=new StuHoughLine(),
            StuLineLenLineLen* lineLenLineLen=new StuLineLenLineLen(),
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :BinThreshold(binThreshold), Precision(precision), Score(score), AngleMax(angleMax), AngleMin(angleMin), LineLength(lineLength), AngleShift(angleShift), Line1Seg_xs(line1Seg_xs), Line1Seg_ys(line1Seg_ys),
        Line1Seg_xe(line1Seg_xe), Line1Seg_ye(line1Seg_ye), Line2Seg_xs(line2Seg_xs), Line2Seg_ys(line2Seg_ys), Line2Seg_xe(line2Seg_xe), Line2Seg_ye(line2Seg_ye), HasStdLine(hasStdLine),
        StdLine_xs(stdLine_xs), StdLine_ys(stdLine_ys), StdLine_xe(stdLine_xe), StdLine_ye(stdLine_ye), StdLine_a(stdLine_a), StdLine_b(stdLine_b), StdLine_c(stdLine_c), StuStuHoughLine(houghLine), StuStuLineLenLineLen(lineLenLineLen),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }


    CheckOperatorBrokenlineInspect(const CheckOperatorBrokenlineInspect& another):CheckOperatorBasic(another)
    {
        this->BinThreshold = another.BinThreshold;
        this->Precision = another.Precision;
        this->Score = another.Score;
        this->AngleMax = another.AngleMax;
        this->AngleMin = another.AngleMin;
        this->LineLength = another.LineLength;
        this->AngleShift = another.AngleShift;
        this->Line1Seg_xs = another.Line1Seg_xs;
        this->Line1Seg_ys = another.Line1Seg_ys;
        this->Line1Seg_xe = another.Line1Seg_xe;
        this->Line1Seg_ye = another.Line1Seg_ye;
        this->Line2Seg_xs = another.Line2Seg_xs;
        this->Line2Seg_ys = another.Line2Seg_ys;
        this->Line2Seg_xe = another.Line2Seg_xe;
        this->Line2Seg_ye = another.Line2Seg_ye;
        this->HasStdLine = another.HasStdLine;
        this->StdLine_xs = another.StdLine_xs;
        this->StdLine_ys = another.StdLine_ys;
        this->StdLine_xe = another.StdLine_xe;
        this->StdLine_ye = another.StdLine_ye;
        this->StdLine_a = another.StdLine_a;
        this->StdLine_b = another.StdLine_b;
        this->StdLine_c = another.StdLine_c;
        this->StuStuHoughLine = another.StuStuHoughLine;
        this->StuStuLineLenLineLen = another.StuStuLineLenLineLen;
    }


    CheckOperatorBrokenlineInspect& operator=(CheckOperatorBrokenlineInspect& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->BinThreshold = another.BinThreshold;
            this->Precision = another.Precision;
            this->Score = another.Score;
            this->AngleMax = another.AngleMax;
            this->AngleMin = another.AngleMin;
            this->LineLength = another.LineLength;
            this->AngleShift = another.AngleShift;
            this->Line1Seg_xs = another.Line1Seg_xs;
            this->Line1Seg_ys = another.Line1Seg_ys;
            this->Line1Seg_xe = another.Line1Seg_xe;
            this->Line1Seg_ye = another.Line1Seg_ye;
            this->Line2Seg_xs = another.Line2Seg_xs;
            this->Line2Seg_ys = another.Line2Seg_ys;
            this->Line2Seg_xe = another.Line2Seg_xe;
            this->Line2Seg_ye = another.Line2Seg_ye;
            this->HasStdLine = another.HasStdLine;
            this->StdLine_xs = another.StdLine_xs;
            this->StdLine_ys = another.StdLine_ys;
            this->StdLine_xe = another.StdLine_xe;
            this->StdLine_ye = another.StdLine_ye;
            this->StdLine_a = another.StdLine_a;
            this->StdLine_b = another.StdLine_b;
            this->StdLine_c = another.StdLine_c;
            this->StuStuHoughLine = another.StuStuHoughLine;
            this->StuStuLineLenLineLen = another.StuStuLineLenLineLen;

        }
        return *this;
    }

    virtual void function() {};


    ~CheckOperatorBrokenlineInspect() {

        //DSDEBUG<<"enter ~CheckOperatorBrokenlineInspect()"<<endl;
        if (this->StuStuHoughLine != NULL)
        {
            delete this->StuStuHoughLine;
            this->StuStuHoughLine = NULL;
        }
        if (this->StuStuLineLenLineLen != NULL)
        {
            delete this->StuStuLineLenLineLen;
            this->StuStuLineLenLineLen = NULL;
        }

    }

    void copyto(CheckOperatorBrokenlineInspect* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->BinThreshold=this->BinThreshold;
        pCopyto->Precision=this->Precision;
        pCopyto->Score=this->Score;
        pCopyto->AngleMax=this->AngleMax;

        pCopyto->AngleMin=this->AngleMin;
        pCopyto->LineLength=this->LineLength;
        pCopyto->AngleShift=this->AngleShift;
        pCopyto->Line1Seg_xs=this->Line1Seg_xs;
        pCopyto->Line1Seg_ys=this->Line1Seg_ys;
        pCopyto->Line1Seg_xe=this->Line1Seg_xe;
        pCopyto->Line1Seg_ye=this->Line1Seg_ye;
        pCopyto->Line2Seg_xs=this->Line2Seg_xs;
        pCopyto->Line2Seg_ys=this->Line2Seg_ys;
        pCopyto->Line2Seg_xe=this->Line2Seg_xe;
        pCopyto->Line2Seg_ye=this->Line2Seg_ye;
        pCopyto->HasStdLine=this->HasStdLine;
        pCopyto->StdLine_xs=this->StdLine_xs;
        pCopyto->StdLine_ys=this->StdLine_ys;
        pCopyto->StdLine_xe=this->StdLine_xe;
        pCopyto->StdLine_ye=this->StdLine_ye;
        pCopyto->StdLine_a=this->StdLine_a;
        pCopyto->StdLine_b=this->StdLine_b;
        pCopyto->StdLine_c=this->StdLine_c;

        this->StuStuHoughLine->cpoyto( pCopyto->StuStuHoughLine);
        this->StuStuLineLenLineLen->cpoyto( pCopyto->StuStuLineLenLineLen);
    }

public:

    int BinThreshold;
    int Precision;
    int Score;
    double AngleMax;
    double AngleMin;
    int LineLength;
    int AngleShift;
    int Line1Seg_xs;
    int Line1Seg_ys;
    int Line1Seg_xe;
    int Line1Seg_ye;
    int Line2Seg_xs;
    int Line2Seg_ys;
    int Line2Seg_xe;
    int Line2Seg_ye;
    bool HasStdLine;
    int StdLine_xs;
    int StdLine_ys;
    int StdLine_xe;
    int StdLine_ye;
    int StdLine_a;
    int StdLine_b;
    int StdLine_c;

    StuHoughLine* StuStuHoughLine;
    StuLineLenLineLen* StuStuLineLenLineLen;
};


//边缘定位,边缘检测
class CheckOperatorEdgePositioning :public CheckOperatorBasic
{
public:
    //线的位置
    enum ENUMEDGEPOS
    {
        ENUMEDGEPOS_LEFT = 0,
        ENUMEDGEPOS_UP = 1,
        ENUMEDGEPOS_RIGHT = 2,
        ENUMEDGEPOS_DOWN = 3,
    };

    typedef struct StuEdgeLine
    {
    public:
        //线的方向
        enum ENUMLINEDIR
        {
            ENUMLINEDIR_HORIZONTAL = 0,
            ENUMLINEDIR_VERTICAL = 1,
        };

    public:
        StuEdgeLine(ENUMLINECOLOR lineColor= ENUMLINECOLOR_GRAY, int edgeThreshold=50, ENUMLINEDIR lineDir= ENUMLINEDIR_HORIZONTAL, ENUMLINEDIR scanLineDir= ENUMLINEDIR_HORIZONTAL, int fitWeight=0,int posX=0,int posY=0) :
            LineColor(lineColor), EdgeThreshold(edgeThreshold), LineDir(lineDir), ScanLineDir(scanLineDir), FitWeight(fitWeight),PosX(posX),PosY(posY)
        {}

        void copyto(StuEdgeLine* pCopyto)
        {
            pCopyto->LineColor=this->LineColor;
            pCopyto->EdgeThreshold=this->EdgeThreshold;
            pCopyto->LineDir=this->LineDir;
            pCopyto->ScanLineDir=this->ScanLineDir;
            pCopyto->FitWeight=this->FitWeight;
            pCopyto->PosX=this->PosX;
            pCopyto->PosY=this->PosY;
        }

        ENUMLINECOLOR LineColor;
        int EdgeThreshold;
        ENUMLINEDIR LineDir;
        ENUMLINEDIR ScanLineDir;
        int FitWeight;
        int PosX;
        int PosY;
    }StuEdgeLine;

public:
    CheckOperatorEdgePositioning(
            int score=0,
            int maxPosShift=0,
            int maxAngleShift=0,
            int hasStdLine=0,
            int stdLine_a=0,
            int stdLine_b=0,
            int stdLine_c=0,
            int resetReg =0,
            int binThresh=40,
            ENUMEDGEPOS edgePos= ENUMEDGEPOS_LEFT,
            StuEdgeLine* edgeLine=new StuEdgeLine(),
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_EDGEPOSITIONING,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :Score(score), MaxPosShift(maxPosShift), MaxAngleShift(maxAngleShift), HasStdLine(hasStdLine), StdLine_a(stdLine_a),
        StdLine_b(stdLine_b), StdLine_c(stdLine_c), ResetReg(resetReg), BinThresh(binThresh), EdgePos(edgePos), StuStuEdgeLine(edgeLine),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }


    CheckOperatorEdgePositioning(const CheckOperatorEdgePositioning& another) :CheckOperatorBasic(another)
    {
        this->StuStuEdgeLine = another.StuStuEdgeLine;
        this->EdgePos = another.EdgePos;
        this->Score = another.Score;
        this->MaxPosShift = another.MaxPosShift;
        this->MaxAngleShift = another.MaxAngleShift;
        this->HasStdLine = another.HasStdLine;
        this->StdLine_a = another.StdLine_a;
        this->StdLine_b = another.StdLine_b;
        this->StdLine_c = another.StdLine_c;
        this->ResetReg = another.ResetReg;
        this->BinThresh = another.BinThresh;
    }


    CheckOperatorEdgePositioning& operator=(CheckOperatorEdgePositioning& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->StuStuEdgeLine = another.StuStuEdgeLine;
            this->EdgePos = another.EdgePos;
            this->Score = another.Score;
            this->MaxPosShift = another.MaxPosShift;
            this->MaxAngleShift = another.MaxAngleShift;
            this->HasStdLine = another.HasStdLine;
            this->StdLine_a = another.StdLine_a;
            this->StdLine_b = another.StdLine_b;
            this->StdLine_c = another.StdLine_c;
            this->ResetReg = another.ResetReg;
            this->BinThresh = another.BinThresh;
        }
        return *this;
    }

    virtual void function() {};


    ~CheckOperatorEdgePositioning() {

        //DSDEBUG<<"enter ~CheckOperatorEdgePositioning()"<<endl;
        if (this->StuStuEdgeLine != NULL)
        {
            delete this->StuStuEdgeLine;
            this->StuStuEdgeLine = NULL;
        }

    }

    void copyto(CheckOperatorEdgePositioning* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->EdgePos=this->EdgePos;
        pCopyto->Score=this->Score;
        pCopyto->MaxPosShift=this->MaxPosShift;
        pCopyto->MaxAngleShift=this->MaxAngleShift;
        pCopyto->HasStdLine=this->HasStdLine;
        pCopyto->StdLine_a=this->StdLine_a;
        pCopyto->StdLine_b=this->StdLine_b;
        pCopyto->StdLine_c=this->StdLine_c;
        pCopyto->ResetReg=this->ResetReg;
        pCopyto->BinThresh=this->BinThresh;


        this->StuStuEdgeLine->copyto(pCopyto->StuStuEdgeLine);
    }


public:
    StuEdgeLine* StuStuEdgeLine;
    ENUMEDGEPOS EdgePos;
    int Score;
    int MaxPosShift;
    int MaxAngleShift;
    int HasStdLine;
    int StdLine_a;
    int StdLine_b;
    int StdLine_c;
    int ResetReg;
    int BinThresh;
};


//灰度测量算子
class CheckOperatorGrayMeasurement :public CheckOperatorBasic
{
public:
    CheckOperatorGrayMeasurement(
            int averageStd=0,
            int averageLimited=0,
            int devStd=0,
            bool devLimited=false,
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT,
            StuCheckOperatorBasic* stuCheckOperatorBasic = new StuCheckOperatorBasic()
            ) :AverageStd(averageStd), AverageLimited(averageLimited), DevStd(devStd), DevLimited(devLimited),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }

    CheckOperatorGrayMeasurement(const CheckOperatorGrayMeasurement& another) :CheckOperatorBasic(another)
    {
        this->AverageStd = another.AverageStd;
        this->AverageLimited = another.AverageLimited;
        this->DevStd = another.DevStd;
        this->DevLimited = another.DevLimited;
    }


    CheckOperatorGrayMeasurement& operator=(CheckOperatorGrayMeasurement& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->AverageStd = another.AverageStd;
            this->AverageLimited = another.AverageLimited;
            this->DevStd = another.DevStd;
            this->DevLimited = another.DevLimited;
        }
        return *this;
    }

    virtual void function() {};

    ~CheckOperatorGrayMeasurement() {
        //DSDEBUG<<"enter ~CheckOperatorGrayMeasurement()"<<endl;
    }


    void copyto(CheckOperatorGrayMeasurement* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->AverageStd=this->AverageStd;
        pCopyto->AverageLimited=this->AverageLimited;
        pCopyto->DevStd=this->DevStd;
        pCopyto->DevLimited=this->DevLimited;


    }

public:
    float AverageStd;
    float AverageLimited;
    float DevStd;
    float DevLimited;

};


//拉线检测算子
class CheckOperatorStayInspect :public CheckOperatorBasic
{
public:
    enum ENUMLINTYPE
    {
        ENUMLINTYPE_SINGLE=0,
        ENUMLINTYPE_DOUBLE=1,
        ENUMLINTYPE_DEFAULT=2
    };

    //线的方向
    enum ENUMLINEDIR
    {
        ENUMLINEDIR_HORIZONTAL = 0,
        ENUMLINEDIR_VERTICAL = 1,
    };


    enum ENUMLINECOLOR
    {
        ENUMLINECOLOR_BRIGHT=0,
        ENUMLINECOLOR_DARK=1
    };

public:
    CheckOperatorStayInspect(
            int binThreshold=60,
            int precision=0,
            int score=60,
            ENUMLINEDIR lineDir= ENUMLINEDIR_VERTICAL,
            int angleMax=0,
            int angleMin=0,
            int packWidth=0,
            int grayAve=0,
            int grayStd=0,
            int stdLineNum=0,
            int angleShift=5,
            int stdWidth=0,
            ENUMLINTYPE lineType=ENUMLINTYPE_SINGLE,
            CheckOperatorStayInspect::ENUMLINECOLOR lineColor= ENUMLINECOLOR_BRIGHT,
            int aveShift=0,
            StuHoughLine* houghLine=new  StuHoughLine(0,0,0,0,0,0,0),
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_STAYINSPECT,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :BinThreshold(binThreshold), Precision(precision), Score(score), LineDir(lineDir), AngleMax(angleMax), AngleMin(angleMin), PackWidth(packWidth), GrayAve(grayAve),
        GrayStd(grayStd), StdLineNum(stdLineNum), AngleShift(angleShift), StdWidth(stdWidth), LineType(lineType), LineColor(lineColor), AveShift(aveShift),
        StuStuHoughLine(houghLine),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {

    }


    CheckOperatorStayInspect(const CheckOperatorStayInspect& another) :CheckOperatorBasic(another)
    {
        this->BinThreshold = another.BinThreshold;
        this->Precision = another.Precision;
        this->Score = another.Score;
        this->LineDir = another.LineDir;
        this->AngleMax = another.AngleMax;
        this->AngleMin = another.AngleMin;
        this->PackWidth = another.PackWidth;
        this->GrayAve = another.GrayAve;
        this->GrayStd = another.GrayStd;
        this->StdLineNum = another.StdLineNum;
        this->AngleShift = another.AngleShift;
        this->StdWidth = another.StdWidth;
        this->LineType = another.LineType;
        this->LineColor = another.LineColor;
        this->AveShift = another.AveShift;
        this->StuStuHoughLine = another.StuStuHoughLine;
    }


    CheckOperatorStayInspect& operator=(CheckOperatorStayInspect& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->BinThreshold = another.BinThreshold;
            this->Precision = another.Precision;
            this->Score = another.Score;
            this->LineDir = another.LineDir;
            this->AngleMax = another.AngleMax;
            this->AngleMin = another.AngleMin;
            this->PackWidth = another.PackWidth;
            this->GrayAve = another.GrayAve;
            this->GrayStd = another.GrayStd;
            this->StdLineNum = another.StdLineNum;
            this->AngleShift = another.AngleShift;
            this->StdWidth = another.StdWidth;
            this->LineType = another.LineType;
            this->LineColor = another.LineColor;
            this->AveShift = another.AveShift;
            this->StuStuHoughLine = another.StuStuHoughLine;
        }
        return *this;
    }

    virtual void function() {};

    ~CheckOperatorStayInspect() {
        // DSDEBUG<<"enter ~CheckOperatorStayInspect()"<<endl;
        if (this->StuStuHoughLine != NULL)
        {
            delete this->StuStuHoughLine;
            this->StuStuHoughLine = NULL;
        }

    }
    void copyto(CheckOperatorStayInspect* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->BinThreshold=this->BinThreshold;
        pCopyto->Precision=this->Precision;
        pCopyto->Score=this->Score;
        pCopyto->LineDir=this->LineDir;

        pCopyto->AngleMax=this->AngleMax;
        pCopyto->AngleMin=this->AngleMin;
        pCopyto->PackWidth=this->PackWidth;
        pCopyto->GrayAve=this->GrayAve;
        pCopyto->GrayStd=this->GrayStd;
        pCopyto->center=this->center;
        pCopyto->StdLineNum=this->StdLineNum;
        pCopyto->AngleShift=this->AngleShift;
        pCopyto->StdWidth=this->StdWidth;
        pCopyto->LineType=this->LineType;
        pCopyto->LineColor=this->LineColor;
        pCopyto->AveShift=this->AveShift;
        this->StuStuHoughLine->cpoyto(pCopyto->StuStuHoughLine);

    }

public:
    int BinThreshold;
    int Precision;
    int Score;
    ENUMLINEDIR LineDir;
    int AngleMax;
    int AngleMin;
    int PackWidth;
    int GrayAve;
    int GrayStd;
    int center=0;
    int StdLineNum;
    int AngleShift;
    int StdWidth;
    ENUMLINTYPE LineType;
    CheckOperatorStayInspect::ENUMLINECOLOR LineColor;
    int AveShift;
    StuHoughLine* StuStuHoughLine;
};


//双拉线检测算子
class CheckOperatorDoubleStayInspect :public CheckOperatorBasic
{
public:
    //线的方向
    enum ENUMLINEDIR
    {
        ENUMLINEDIR_HORIZONTAL = 0,
        ENUMLINEDIR_VERTICAL = 1,
    };


public:
    CheckOperatorDoubleStayInspect(
            int binThreshold=10,
            int score=60,
            ENUMLINEDIR lineDir= ENUMLINEDIR_VERTICAL,
            int angleShift=5,
            StuHoughLine* houghLine=new  StuHoughLine(0,0,0,0,0,0,0),
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT,
            StuCheckOperatorBasic* stuCheckOperatorBasic =new StuCheckOperatorBasic()
            ) :BinThreshold(binThreshold),  Score(score), LineDir(lineDir),AngleShift(angleShift),center1(0),center2(0),StuStuHoughLine(houghLine),DisMin(10),DisMax(50),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {

    }


    CheckOperatorDoubleStayInspect(const CheckOperatorDoubleStayInspect& another) :CheckOperatorBasic(another)
    {
        this->BinThreshold = another.BinThreshold;
        this->Score = another.Score;
        this->LineDir = another.LineDir;
        this->AngleShift = another.AngleShift;
        this->center1=another.center1;
        this->center2=another.center2;
        this->StuStuHoughLine = another.StuStuHoughLine;
    }


    CheckOperatorDoubleStayInspect& operator=(CheckOperatorDoubleStayInspect& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->BinThreshold = another.BinThreshold;
            this->Score = another.Score;
            this->LineDir = another.LineDir;
            this->AngleShift = another.AngleShift;
            this->center1=another.center1;
            this->center2=another.center2;
            this->StuStuHoughLine = another.StuStuHoughLine;
        }
        return *this;
    }

    virtual void function() {};

    ~CheckOperatorDoubleStayInspect() {
        // DSDEBUG<<"enter ~CheckOperatorStayInspect()"<<endl;
        if (this->StuStuHoughLine != NULL)
        {
            delete this->StuStuHoughLine;
            this->StuStuHoughLine = NULL;
        }

    }
    void copyto(CheckOperatorDoubleStayInspect* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;

        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->BinThreshold=this->BinThreshold;
        pCopyto->Score=this->Score;
        pCopyto->LineDir=this->LineDir;
        pCopyto->center1=this->center1;
        pCopyto->center2=this->center2;
        pCopyto->AngleShift=this->AngleShift;
        this->StuStuHoughLine->cpoyto(pCopyto->StuStuHoughLine);

    }

public:
    int BinThreshold;
    int Score;
    int DisMin;
    int DisMax;
    ENUMLINEDIR LineDir;
    int center1;
    int center2;
    int X11;
    int Y11;
    int X12;
    int Y12;
    int X21;
    int Y21;
    int X22;
    int Y22;

    int AngleShift;
    StuHoughLine* StuStuHoughLine;
};



//对象搜索模板匹配算子
class CheckOperatorTemplateMatch :public CheckOperatorBasic
{

public:
    //对象搜索模板匹配算子中数据结构
    typedef struct StuGrayMatch
    {
    public:
        StuGrayMatch(int searchCount=0, int searchMode=0, int precision=0, int condition=0, int score1=0, int score2=0, int patternNum=0, int patternRect_Xs=0, int patternRect_Ys=0, int patternRect_Xe=0, int patternRect_Ye=0) :
            SearchCount(searchCount), SearchMode(searchMode), Precision(precision), Condition(condition), Score1(score1), Score2(score2), PatternNum(patternNum), PatternRect_Xs(patternRect_Xs), PatternRect_Ys(patternRect_Ys), PatternRect_Xe(patternRect_Xe), PatternRect_Ye(patternRect_Ye)
        {}

        void copyto(StuGrayMatch* pCopyto)
        {
            pCopyto->SearchCount=this->SearchCount;
            pCopyto->SearchMode=this->SearchMode;
            pCopyto->Precision=this->Precision;
            pCopyto->Condition=this->Condition;
            pCopyto->Score1=this->Score1;
            pCopyto->Score2=this->Score2;
            pCopyto->PatternNum=this->PatternNum;
            pCopyto->PatternRect_Xs=this->PatternRect_Xs;
            pCopyto->PatternRect_Ys=this->PatternRect_Ys;
            pCopyto->PatternRect_Xe=this->PatternRect_Xe;
            pCopyto->PatternRect_Ye=this->PatternRect_Ye;

        }

        int SearchCount;
        int SearchMode;
        int Precision;
        int Condition;
        int Score1;
        int Score2;
        int PatternNum;
        int PatternRect_Xs;
        int PatternRect_Ys;
        int PatternRect_Xe;
        int PatternRect_Ye;


    }StuGrayMatch;

public:
    CheckOperatorTemplateMatch(
            std::string code="",
            int matchThresh=60,
            int searchNum=0,
            StuGrayMatch *grayMatch=new StuGrayMatch(),
            ENUMCHECKOPERATORTYPE checkType = ENUMCHECKOPERATORTYPE_TEMPLATEMATCH,
            StuCheckOperatorBasic *stuCheckOperatorBasic =new  StuCheckOperatorBasic()
            ) :Code(code),
        MatchThresh(matchThresh), SearchNum(searchNum), StuStuGrayMatch(grayMatch),
        CheckOperatorBasic(checkType, stuCheckOperatorBasic)
    {
    }



    CheckOperatorTemplateMatch(const CheckOperatorTemplateMatch& another) :CheckOperatorBasic(another)
    {
        this->Code=another.Code;
        this->MatchThresh = another.MatchThresh;
        this->SearchNum = another.SearchNum;
        this->StuStuGrayMatch = another.StuStuGrayMatch;
    }


    CheckOperatorTemplateMatch& operator=(CheckOperatorTemplateMatch& another)
    {
        if (&another != this)
        {
            CheckOperatorBasic::operator=(another);
            this->Code=another.Code;
            this->MatchThresh = another.MatchThresh;
            this->SearchNum = another.SearchNum;
            this->StuStuGrayMatch = another.StuStuGrayMatch;
        }
        return *this;
    }
    virtual void function() {};


    ~CheckOperatorTemplateMatch() {
        // DSDEBUG<<"enter ~CheckOperatorTemplateMatch()"<<endl;
        if (this->StuStuGrayMatch != NULL)
        {
            delete this->StuStuGrayMatch;
            this->StuStuGrayMatch = NULL;
        }
        //        if(this->m_Template.size()>0)
        //        {
        //            this->m_Template.clear();
        //        }


    }

    void copyto(CheckOperatorTemplateMatch* pCopyto)
    {
        pCopyto->CheckType=this->CheckType;
        pCopyto->Code=this->Code;
        this->stu_CheckOperatorBasic->cpoyto( pCopyto->stu_CheckOperatorBasic);
        pCopyto->MatchThresh=this->MatchThresh;
        pCopyto->SearchNum = this->SearchNum;
        this->StuStuGrayMatch->copyto(pCopyto->StuStuGrayMatch);
    }


public:
    int MatchThresh;
    int SearchNum;
    std::string Code;
    StuGrayMatch* StuStuGrayMatch;
    //std::map<int,cv::Mat> m_Template;
};



#endif
