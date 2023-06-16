#include "DSIppImage.h"
#include"dsdebug.h"
#ifndef PI
#define PI 3.14159265
#endif
#include<QThread>
#include<QPainter>
#include<QImage>
#include"glog/logging.h"
int DSIppImage::ippiMean_8u_C1R_CV(cv::Mat & src, double * pMean)
{

    return 0;
}


int DSIppImage::ippiMean_StdDev_8u_C1R_CV(cv::Mat & src, double * pMean, double* pStddev)
{

    return 0;
}


void DSIppImage::RemoveSmallRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode)
{

}


int DSIppImage::ippiCompareC_8u_C1R_CV(cv::Mat & src,int threshold,cv::Mat& dstComp)
{

    return 0;
}

bool expandEdge(const cv::Mat & img, int edge[], const int edgeID)
{

    int nc = img.cols;
    int nr = img.rows;
    switch (edgeID) {
    case 0:
        if (edge[0] > nr)
            return false;
        for (int i = edge[3]; i <= edge[1]; ++i)
        {
            if (img.at<uchar>(edge[0], i) == 0)
                return false;
        }
        edge[0]++;
        return true;
        break;
    case 1:
        if (edge[1] > nc)
            return false;
        for (int i = edge[2]; i <= edge[0]; ++i)
        {
            if (img.at<uchar>(i, edge[1]) == 0)
                return false;
        }
        edge[1]++;
        return true;
        break;
    case 2:
        if (edge[2] < 0)
            return false;
        for (int i = edge[3]; i <= edge[1]; ++i)
        {
            if (img.at<uchar>(edge[2], i) == 0)
                return false;
        }
        edge[2]--;
        return true;
        break;
    case 3:
        if (edge[3] < 0)
            return false;
        for (int i = edge[2]; i <= edge[0]; ++i)
        {
            if (img.at<uchar>(i, edge[3]) == 0)
                return false;
        }
        edge[3]--;
        return true;
        break;
    default:
        return false;
        break;
    }
}

cv::Rect DSIppImage::getInsertSquare(cv::Mat & img, const cv::Point center)
{

    if (img.empty() ||
            img.channels() > 1
            || img.depth() > 8)
        return cv::Rect();

    int edge[4];
    edge[0] = center.y + 1;
    edge[1] = center.x + 1;
    edge[2] = center.y - 1;
    edge[3] = center.x - 1;

    bool EXPAND[4] = { 1,1,1,1 };
    int n = 0;
    while (EXPAND[0] || EXPAND[1] || EXPAND[2] || EXPAND[3])
    {
        int edgeID = n % 4;
        EXPAND[edgeID] = expandEdge(img, edge, edgeID);
        n++;
    }


    cv::Point tl = cv::Point(edge[3], edge[0]);
    cv::Point br = cv::Point(edge[1], edge[2]);
    return cv::Rect(tl, br);
}


int DSIppImage::ippiSum_8u_C1R_CV(cv::Mat& src, double * pSum)
{
    int step = src.step;
    unsigned char * pSrc = src.ptr<uchar>(0);
    IppStatus ret = ippiSum_8u_C1R(pSrc, step,{ src.cols,src.rows },pSum);
    if (ret != ippStsNoErr)
    {
        return -1;
    }
    return 0;
}

void DSIppImage::getMaskedImage(cv::Mat& src,std::vector<cv::Point>& pts,unsigned char fillPix)
{

    cv::Mat tmp=cv::Mat(src.rows, src.cols, CV_8UC1, cv::Scalar(0, 0, 0));
    std::vector<std::vector<cv::Point>> vpts1;
    std::vector<cv::Point> pts1;
    pts1.push_back(pts[0]);
    pts1.push_back(pts[1]);
    pts1.push_back(pts[2]);
    pts1.push_back(pts[3]);
    pts1.push_back(pts[4]);
    pts1.push_back(pts[5]);
    pts1.push_back(pts[6]);
    pts1.push_back(pts[7]);
    vpts1.push_back(pts1);

    fillPoly(tmp, vpts1, cv::Scalar(255, 255, 255));

    for(int r=0;r<src.rows;++r)
        for(int c=0;c<src.cols;++c)
            src.at<unsigned char>(r,c)=tmp.at<unsigned char>(r,c)?src.at<unsigned char>(r,c):fillPix;


}


void DSIppImage::getCircleMaskedImage(cv::Mat& src,unsigned char fillPix)
{

    //    cv::Mat tmp=cv::Mat(src.rows, src.cols, CV_8UC1, cv::Scalar(0, 0, 0));
    //    fillConvexPoly(tmp, pts, cv::Scalar(1, 1, 1));
    //    for(int r=0;r<src.rows;++r)
    //        for(int c=0;c<src.cols;++c)
    //            src.at<unsigned char>(r,c)=tmp.at<unsigned char>(r,c)?src.at<unsigned char>(r,c):fillPix;
    //    QImage img(src.cols,src.rows,QImage::Format_Grayscale8);
    //    QPainter painter(&img);
    //    painter.drawEllipse(0,0,src.cols,src.rows);
    //    for(int r=0;r<src.rows;++r)
    //        for(int c=0;c<src.cols;++c)
    //            src.at<unsigned char>(r,c)=tmp.at<unsigned char>(r,c)?src.at<unsigned char>(r,c):fillPix;


}


void DSIppImage::getMaskImage(cv::Mat& mask,int img_hei,int img_wid,std::vector<cv::Point>& pts,unsigned char pixMask)
{

    mask=cv::Mat(img_hei,img_wid,CV_8UC1,cv::Scalar(0,0,0));
    std::vector<std::vector<cv::Point>> vpts1;
    std::vector<cv::Point> pts1;

    pts1.push_back(pts[0]);
    pts1.push_back(pts[1]);
    pts1.push_back(pts[2]);
    pts1.push_back(pts[3]);
    pts1.push_back(pts[4]);
    pts1.push_back(pts[5]);
    pts1.push_back(pts[6]);
    pts1.push_back(pts[7]);
    vpts1.push_back(pts1);


    fillPoly(mask, vpts1, cv::Scalar(255, 255, 255));

}


void HoughLinesProbabilistic12(cv::Mat& image, float rho, float theta, int threshold, int lineLength, int lineGap, std::vector<cv::Vec4i>& lines, int linesMax)
{

}

int DSIppImage::stayLineInspect(cv::Mat src,int linedir,int thresh,int * p1x,int* p1y,int * p2x,int* p2y)
{

    return  0;
}


int DSIppImage::doublestayLineInspect(cv::Mat src,int linedir,int thresh,int dismin,int dismax,int * p1x1,int* p1y1,int * p1x2,int* p1y2,int * p2x1,int* p2y1,int * p2x2,int* p2y2)
{

    return  0;
}


int DSIppImage::brokenLineInspect(cv::Mat src,
                                  int line1_x1,int line1_y1, int line1_x2, int line1_y2,
                                  int line2_x1, int line2_y1, int line2_x2, int line2_y2,
                                  int thresh,int minLineThresh,int minLineLen,int maxLineGap ,
                                  int angleMin,int angleMax,int disMin,bool hasRef,
                                  int * p1x, int* p1y, int * p2x, int* p2y)
{


    return 0;
}

int DSIppImage::brokenLineInspect2(cv::Mat src,
                                   int line1_x1, int line1_y1, int line1_x2, int line1_y2,
                                   int line2_x1, int line2_y1, int line2_x2, int line2_y2,
                                   int thresh, int minLineThresh, int minLineLen, int maxLineGap,
                                   int angleMin, int angleMax, int disMin, bool hasRef,
                                   int * p1x, int* p1y, int * p2x, int* p2y)
{

    return 0;
}



int DSIppImage::findEdget2(cv::Mat & img, int dir, int thres, int * ptx, int * pty, int* score)
{

    return 0;
}



int DSIppImage::findEdget(cv::Mat & img, int dir, int thres, int * ptx, int * pty, int* score)
{
    return  0;
}



int  DSIppImage::DSMatchTemplateAbsNormalIPP(cv::Mat& imgSearch, cv::Mat& imgTarget, cv::Point* dstPnt, float * norm1)
{

    return 0;


}



