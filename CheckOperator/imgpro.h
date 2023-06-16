#ifndef IMGPRO_H
#define IMGPRO_H

#include<list>
#include"CheckOperator.hpp"
#include"ImageBase.hpp"

class ImgPro {
    public:
    typedef struct StuPostionAdjust
    {
    public:
        StuPostionAdjust(int score,cv::Point pt):Score(score),Pt(pt)
        {}
        StuPostionAdjust()
        {}
        int Score;
        cv::Point Pt;
    }StuPostionAdjust;
public:
    ImgPro(bool valid = true, bool isCheck = true, bool existRefImage = true, int dirNum = 0, int refNum = 0, int checkNum = 0, int workPiece = 0);
    ImgPro(const ImgPro& another);


    ImgPro& operator=(ImgPro& another);

    ImgPro* clone();


    void copyto(ImgPro* pCopyto);

    ~ImgPro();

public:
    int ImgIndex;
    bool Valid;
    bool IsCheck;
    bool ExistRefImage;
    int DirNum;
    int RefNum;
    int CheckNum;
    int WorkPiece;
    list<CheckOperatorBasic*> LstCheckOperator;

    OpencvImage CalcOpencvImage;
    OpencvImage CurBadOpencvImage;
    OpencvImage RefOpencvImage;
    std::map<QString,std::map<int,OpencvImage>> TemplateImage;

    //定位算子的原始位置，比如边缘定位（左右边缘，得到左右的X值，y为0；上下边缘得到上下的Y值，左右为0）。对象搜索算子中，对象的位置设置好了，即得到xy
    std::map<int, StuPostionAdjust> MapPostion;

};

#endif // IMGPRO_H
