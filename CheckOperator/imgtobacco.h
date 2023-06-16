#ifndef IMGTOBACCO_H
#define IMGTOBACCO_H

#include<string>
#include<list>
#include<map>
#include"imgpro.h"
#include"ImageBase.hpp"
#include"dsbase.h"

class ImgTobacco
{
public:
    ImgTobacco(std::string productName="Default", int imgProObjNum=0, bool revertEnable = false, bool patternsSets = true);

    ImgTobacco(const ImgTobacco& another);

    ImgTobacco& operator=(ImgTobacco& another);

    ~ImgTobacco();

    void copyto(ImgTobacco* pCopyto);

public:
    std::string ProductName;
    int ImgProObjNum;
    bool RevertEnable;
    std::string Code;
    int PatternsSets;
    std::list<ImgPro*> LstImgPro;
    int ImgWid;
    int ImgHei;


    int ImgWids[CAMERANUM_MAX];
    int ImgHeis[CAMERANUM_MAX];

public:
    //vector<OpenCvImage> VCalcOpenCvImage;
    //vector<OpenCvImage> VRefOpenCvImage;

    //一个相机有多个模板算子，每个模板算子有多个模板,用于模板匹配
//    std::map<int,std::map<int,std::map<int,OpenCvImage>>> m_Template;//对应的模板图;
    //std::map<int,OpenCvImage> RefImage;

};
#endif // IMGTOBACCO_H
