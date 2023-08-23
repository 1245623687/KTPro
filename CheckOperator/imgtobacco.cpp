#include "imgtobacco.h"

ImgTobacco::ImgTobacco(std::string productName, int imgProObjNum, bool revertEnable , bool patternsSets):ProductName(productName),
    ImgProObjNum(imgProObjNum),RevertEnable(revertEnable),PatternsSets(patternsSets),ImgWid(656),ImgHei(336)
{
    memset(ImgHeis,0,sizeof(int)*CAMERANUM_MAX);
    memset(ImgWids,0,sizeof(int)*CAMERANUM_MAX);

}

ImgTobacco::ImgTobacco(const ImgTobacco& another)
{
    this->ProductName = another.ProductName;
    this->ImgProObjNum = another.ImgProObjNum;
    this->RevertEnable = another.RevertEnable;
    this->PatternsSets = another.PatternsSets;
    this->LstImgPro = another.LstImgPro;
    this->Code=another.Code;
    this->ImgHei = another.ImgHei;
    this->ImgWid = another.ImgWid;

    memcpy(this->ImgHeis,another.ImgHeis,sizeof(int)*CAMERANUM_MAX);
    memcpy(this->ImgWids,another.ImgHeis,sizeof(int)*CAMERANUM_MAX);
}


void ImgTobacco::copyto(ImgTobacco* pCopyto)
{

    pCopyto->ProductName=this->ProductName;
    pCopyto->ImgProObjNum=this->ImgProObjNum;
    pCopyto->RevertEnable=this->RevertEnable;
    pCopyto->PatternsSets = this->PatternsSets;
    pCopyto->Code=this->Code;
    pCopyto->ImgHei = this->ImgHei;
    pCopyto->ImgWid = this->ImgWid;

    memcpy(pCopyto->ImgHeis,this->ImgHeis,sizeof(int)*CAMERANUM_MAX);
    memcpy(pCopyto->ImgWids,this->ImgHeis,sizeof(int)*CAMERANUM_MAX);

    std::list<ImgPro*>::iterator itor= pCopyto->LstImgPro.begin();
    for(;itor!=pCopyto->LstImgPro.end();++itor)
    {
        ImgPro* imgdel=(*itor);
        delete imgdel;
    }

    pCopyto->LstImgPro.clear();



    itor= this->LstImgPro.begin();
    for(;itor!=this->LstImgPro.end();++itor)
    {
        ImgPro* imgProCopy=(*itor);

        ImgPro* addImgPro=new ImgPro;
        imgProCopy->copyto(addImgPro);
        pCopyto->LstImgPro.push_back(addImgPro);
    }
}


ImgTobacco& ImgTobacco::operator=(ImgTobacco& another)
{
    if (&another != this)
    {
        this->ProductName = another.ProductName;
        this->ImgProObjNum = another.ImgProObjNum;
        this->RevertEnable = another.RevertEnable;
        this->PatternsSets = another.PatternsSets;
        this->LstImgPro = another.LstImgPro;
        this->Code=another.Code;
        this->ImgHei = another.ImgHei;
        this->ImgWid = another.ImgWid;

        memcpy(this->ImgHeis,another.ImgHeis,sizeof(int)*CAMERANUM_MAX);
        memcpy(this->ImgWids,another.ImgHeis,sizeof(int)*CAMERANUM_MAX);
    }
    return *this;
}


ImgTobacco::~ImgTobacco()
{
    if (LstImgPro.size() != 0)
    {
        for (std::list<ImgPro*>::iterator itor = this->LstImgPro.begin(); itor != this->LstImgPro.end();)
        {
            delete (*itor);
            itor = LstImgPro.erase(itor);
        }
    }
}
