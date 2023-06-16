#pragma once


#include"ipp.h "
#include "opencv2/opencv.hpp"

typedef  struct StuRect
{
public:
    int X;
    int Y;
    int Width;
    int Height;
}StuRect;


class DSIppImage
{
public:
    //均值计算
    static int ippiMean_8u_C1R_CV(cv::Mat & src, double * pMean);

    //均值和标准差计算
    static int ippiMean_StdDev_8u_C1R_CV(cv::Mat & src, double * pMean,double* pStddev);

    static int ippiCompareC_8u_C1R_CV(cv::Mat & src, int threshold, cv::Mat& dstComp);

    static int ippiSum_8u_C1R_CV(cv::Mat& src, double * pSum);

   static cv::Rect getInsertSquare(cv::Mat & src, const cv::Point center);
    //************************************
    // Method:    stayInspect
    // FullName:  stayInspect
    // Access:    public
    // Returns:   int  -1：没有线  0：有线
    // Qualifier:
    // Parameter: cv::Mat src
    // Parameter: int linedir  0：水平  1：垂直
    // Parameter: int thresh
    // Parameter: int * p1x
    // Parameter: int * p1y
    // Parameter: int * p2x
    // Parameter: int * p2y
    //************************************
    static int stayLineInspect(cv::Mat src,int linedir,int thresh,int * p1x,int* p1y,int * p2x,int* p2y);

    static int doublestayLineInspect(cv::Mat src,int linedir,int thresh,int dismin,int dismax,int * p1x1,int* p1y1,int * p1x2,int* p1y2,int * p2x1,int* p2y1,int * p2x2,int* p2y2);

    static int brokenLineInspect(cv::Mat src,
                                      int line1_x1,int line1_y1, int line1_x2, int line1_y2,
                                      int line2_x1, int line2_y1, int line2_x2, int line2_y2,
                                      int thresh,int minLineThresh,int minLineLen,int maxLineGap ,
                                      int angleMin,int angleMax,int disMin,bool hasRef,
                                      int * p1x, int* p1y, int * p2x, int* p2y);



    static int brokenLineInspect2(cv::Mat src,
        int line1_x1, int line1_y1, int line1_x2, int line1_y2,
        int line2_x1, int line2_y1, int line2_x2, int line2_y2,
        int thresh, int minLineThresh, int minLineLen, int maxLineGap,
        int angleMin, int angleMax, int disMin, bool hasRef,
        int * p1x, int* p1y, int * p2x, int* p2y);

    //************************************
    // Method:    findEdget
    // FullName:  边缘查找，用于查找烟条的水平和垂直边缘
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: Mat & img
    // Parameter: int dir 搜索方向：0：left->right  1:right->left  2:up->down  3:down->up
    // Parameter: int thres:  sobel之后的二值化阈值
    // Parameter: int * ptx：当找垂直方向边缘生效 找到的坐标
    // Parameter: int * pty：当找水平方向边缘生效 找到的坐标
    // Parameter: int * score： 得分 拟合的结果值
    //************************************
    static  int findEdget(cv::Mat & img, int dir, int thres,int * ptx,int * pty,int* score);
    static  int findEdget2(cv::Mat & img, int dir, int thres,int * ptx,int * pty,int* score);

    //获取掩膜后的图像
    static void getMaskedImage(cv::Mat& src,std::vector<cv::Point>& pts,unsigned char fillPix=100);


   static void DSIppImage::getCircleMaskedImage(cv::Mat& src,unsigned char fillPix=100);
    //根据顶点获取掩膜图像
    static void getMaskImage(cv::Mat& mask,int img_hei,int img_wid,std::vector<cv::Point>& pts,unsigned char pixMask=255);

    static int DSMatchTemplateAbsNormalIPP(cv::Mat& imgSearch, cv::Mat& imgTarget, cv::Point* dstPnt, float * norm1);


     static void RemoveSmallRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode);
};

