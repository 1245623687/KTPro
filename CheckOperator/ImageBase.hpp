#pragma once

#include <opencv2/opencv.hpp>
#include<QImage>
#include<QPixmap>
#include<QTextCodec>

#include"ImageHandle/opencvimage.h"
template<class ImageType>
class ImageBase
{

public:
    //ImageBase(ImageType& imageRGB=NULL, ImageType& imageR = NULL, ImageType& imageG = NULL, ImageType& imageB = NULL, ImageType& imageGray = NULL, ENUMIMAGEFORMAT format=ENUMIMAGEFORMAT_GRAY)
    //				:ImageRGB(imageRGB),ImageR(imageR), ImageG(imageG), ImageB(imageB),ImageGray(imageGray),Format(format)
    //{
    //}

    ImageBase(int hei,int wid,ENUMDSIMAGEFORMAT format = ENUMDSIMAGEFORMAT_GRAY) : Height(hei),Width(wid),Format(format)
    {

    }

    ImageBase(const ImageBase& another)
    {
        this->Width = another.Width;
        this->Height = another.Height;
        this->ImageRGB = another.ImageRGB;
        this->ImageR = another.ImageR;
        this->ImageG = another.ImageG;
        this->ImageB = another.ImageB;
        this->ImageGray = another.ImageGray;
        this->Format=another.Format;

    }

    ImageBase& operator=(ImageBase& another)
    {
        if (&another != this)
        {
            this->Width = another.Width;
            this->Height = another.Height;
            this->ImageRGB = another.ImageRGB;
            this->ImageR = another.ImageR;
            this->ImageG = another.ImageG;
            this->ImageB = another.ImageB;
            this->ImageGray = another.ImageGray;
            this->Format=another.Format;
        }
        return *this;
    }

    //virtual void getRectImageClone(int x,int y,int width,int height)=0;
public:
    int Height;
    int Width;
    ImageType ImageRGB;
    ImageType ImageR;
    ImageType ImageG;
    ImageType ImageB;
    ImageType ImageGray;
    ENUMDSIMAGEFORMAT Format;
};

//class OpenCvImage :public ImageBase<cv::Mat>{
//public:
//    OpenCvImage(int hei=480, int wid=640, ENUMDSIMAGEFORMAT format = ENUMDSIMAGEFORMAT_GRAY) :ImageBase<cv::Mat>(hei, wid, ENUMDSIMAGEFORMAT_GRAY) {}


//    OpenCvImage(const OpenCvImage& another):ImageBase(another)
//    {
//        this->Width = another.Width;
//        this->Height = another.Height;
//        this->ImageRGB = another.ImageRGB.clone();
//        this->ImageR = another.ImageR.clone();
//        this->ImageG = another.ImageG.clone();
//        this->ImageB = another.ImageB.clone();
//        this->ImageGray = another.ImageGray.clone();
//        this->Format=another.Format;
//    }


//    OpenCvImage& operator=(const OpenCvImage& another)
//    {
//        if (&another != this)
//        {
//            this->Width = another.Width;
//            this->Height = another.Height;
//            this->ImageRGB = another.ImageRGB.clone();
//            this->ImageR = another.ImageR.clone();
//            this->ImageG = another.ImageG.clone();
//            this->ImageB = another.ImageB.clone();
//            this->ImageGray = another.ImageGray.clone();
//            this->Format=another.Format;
//        }
//        return *this;
//    }


//    void loadImage(QString filename)
//    {
//        switch (this->Format)
//        {
//        case ENUMDSIMAGEFORMAT_GRAY:
//        {
//            QTextCodec *code = QTextCodec::codecForName("GB2312");
//            std::string name = code->fromUnicode(filename).data();
//            this->ImageGray=cv::imread(name,0);
//        }
//            break;
//        case ENUMDSIMAGEFORMAT_RGB:
//        {

//        }
//            break;
//        case ENUMDSIMAGEFORMAT_R:
//            break;
//        case ENUMDSIMAGEFORMAT_G:
//            break;
//        case ENUMDSIMAGEFORMAT_B:
//            break;
//        }

//    }

//    //    virtual OpenCvImage* getRectImageClone(int x,int y,int width,int height)
//    //    {
//    //        OpenCvImage * retImage=new OpenCvImage(width,height,ENUMDSIMAGEFORMAT_GRAY);
//    //        switch (Format)
//    //        {

//    //        case ENUMDSIMAGEFORMAT_GRAY:

//    //            retImage->ImageGray=cv::Mat(this->ImageGray,cv::Rect(x,y,width,height));

//    //            break;
//    //        case ENUMDSIMAGEFORMAT_RGB:
//    //            break;
//    //        case ENUMDSIMAGEFORMAT_R:
//    //            break;
//    //        case ENUMDSIMAGEFORMAT_G:
//    //            break;
//    //        case ENUMDSIMAGEFORMAT_B:
//    //            break;

//    //        }
//    //        return retImage;
//    //    }

//    OpenCvImage getRectImage(QRect rect)
//    {
//        switch (this->Format)
//        {
//        case ENUMDSIMAGEFORMAT_GRAY:
//        {
//            OpenCvImage retImg(rect.height(),rect.width(),ENUMDSIMAGEFORMAT_GRAY);
//            retImg.ImageGray=cv::Mat(this->ImageGray,cv::Rect(rect.x(),rect.y(),rect.width(),rect.height())).clone();
//            return retImg;
//        }
//            break;
//        case ENUMDSIMAGEFORMAT_RGB:
//        {
//            OpenCvImage retImg(rect.width(),rect.height(),ENUMDSIMAGEFORMAT_RGB);
//            retImg.ImageRGB=cv::Mat(this->ImageRGB,cv::Rect(rect.x(),rect.y(),rect.width(),rect.height())).clone();
//            return retImg;
//        }
//            break;
//        case ENUMDSIMAGEFORMAT_R:
//            break;
//        case ENUMDSIMAGEFORMAT_G:
//            break;
//        case ENUMDSIMAGEFORMAT_B:
//            break;
//        }
//    }


//    void save(QString path)
//    {
//        QTextCodec *code = QTextCodec::codecForName("GB2312");
//        std::string name = code->fromUnicode(path).data();

//        switch (this->Format)
//        {
//        case ENUMDSIMAGEFORMAT_GRAY:
//        {

//            cv::imwrite(name,this->ImageGray);
//        }
//            break;
//        case ENUMDSIMAGEFORMAT_RGB:
//        {
//            cv::imwrite(name,this->ImageRGB);
//        }
//            break;
//        case ENUMDSIMAGEFORMAT_R:
//            break;
//        case ENUMDSIMAGEFORMAT_G:
//            break;
//        case ENUMDSIMAGEFORMAT_B:
//            break;
//        }
//    }

//    void toQPixmap(QPixmap & pixmap)
//    {
//        QImage* image=new QImage(this->ImageGray.cols,this->ImageGray.rows,QImage::Format_Indexed8);
//        this->toQImage(image);
//        pixmap=QPixmap::fromImage(*image);
//        delete  image;
//    }

//    void toQImage(QImage* qImage)
//    {
//        switch (this->Format)
//        {
//        case ENUMDSIMAGEFORMAT_GRAY:
//        {

//            qImage->setColorCount(256);
//            for(int i = 0; i < 256; i++)
//            {
//                qImage->setColor(i, qRgb(i, i, i));
//            }
//            cv::Mat m=ImageGray;

//            for (int r=0;r<this->ImageGray.rows;r++)
//            {
//                memcpy(qImage->scanLine(r),m.ptr<uchar>(r),qImage->bytesPerLine());
//            }
//        }
//            break;
//        case ENUMDSIMAGEFORMAT_RGB:
//            break;
//        case ENUMDSIMAGEFORMAT_R:
//            break;
//        case ENUMDSIMAGEFORMAT_G:
//            break;
//        case ENUMDSIMAGEFORMAT_B:
//            break;
//        }
//    }
//};



