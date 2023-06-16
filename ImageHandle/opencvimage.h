#ifndef OPENCVIMAGE_H
#define OPENCVIMAGE_H
#include<string>
#include <opencv2/opencv.hpp>
#include<QPixmap>
#include<QTextCodec>

enum ENUMDSIMAGEFORMAT
{
    ENUMDSIMAGEFORMAT_GRAY = 0,
    ENUMDSIMAGEFORMAT_RGB = 1,
    ENUMDSIMAGEFORMAT_R = 2,
    ENUMDSIMAGEFORMAT_G = 3,
    ENUMDSIMAGEFORMAT_B = 4,
};


class OpencvImage
{
public:
    OpencvImage(){}

public:
    OpencvImage(ENUMDSIMAGEFORMAT format ,int hei=480, int wid=640):
        Format(format),
        Height(hei),
        Width(wid)
    {
        ImageRGB=cv::Mat::zeros(hei,wid,CV_8UC3);
    }

    OpencvImage* clone()
    {

        OpencvImage *another=new OpencvImage();
        {
            another->Width =this->Width;
            another->Height=this->Height;
            another->Format=this->Format;
            switch (this->Format)
            {
            case ENUMDSIMAGEFORMAT_GRAY:
            {
                another->ImageGray=this->ImageGray.clone();
            }
                break;
            case ENUMDSIMAGEFORMAT_RGB:
            {

                another->ImageRGB=this->ImageRGB.clone();
            }
                break;
            case ENUMDSIMAGEFORMAT_R:
            {


                another->ImageB=this->ImageB.clone();
            }
                break;
            case ENUMDSIMAGEFORMAT_G:
            {


                another->ImageG=this->ImageG.clone();
            }
                break;
            case ENUMDSIMAGEFORMAT_B:
            {

                another->ImageB=this->ImageB.clone();

            }
                break;
            }


        }
        return another;
    }



    OpencvImage(const OpencvImage& another)
    {
        this->Width = another.Width;
        this->Height = another.Height;
        this->Format=another.Format;
        switch (this->Format)
        {
        case ENUMDSIMAGEFORMAT_GRAY:
        {
            ImageGray=another.ImageGray.clone();
        }
            break;
        case ENUMDSIMAGEFORMAT_RGB:
        {
            ImageRGB=another.ImageRGB.clone();
        }
            break;
        case ENUMDSIMAGEFORMAT_R:
        {
            ImageB=another.ImageB.clone();
        }
            break;
        case ENUMDSIMAGEFORMAT_G:
        {
            ImageG=another.ImageG.clone();
        }
            break;
        case ENUMDSIMAGEFORMAT_B:
        {
            ImageB=another.ImageB.clone();
        }
            break;
        }


    }

    OpencvImage& operator=(const OpencvImage& another)
    {
        if (&another != this)
        {
            this->Width = another.Width;
            this->Height = another.Height;
            this->Format=another.Format;
            switch (this->Format)
            {
            case ENUMDSIMAGEFORMAT_GRAY:
            {
                ImageGray=another.ImageGray.clone();
            }
                break;
            case ENUMDSIMAGEFORMAT_RGB:
            {
                ImageRGB=another.ImageRGB.clone();
            }
                break;
            case ENUMDSIMAGEFORMAT_R:
            {
                ImageB=another.ImageB.clone();
            }
                break;
            case ENUMDSIMAGEFORMAT_G:
            {
                ImageG=another.ImageG.clone();
            }
                break;
            case ENUMDSIMAGEFORMAT_B:
            {
                ImageB=another.ImageB.clone();
            }
                break;
            }


        }
        return *this;
    }

    void* data()
    {
        switch (this->Format)
        {
        case ENUMDSIMAGEFORMAT_GRAY:
        {
            return this->ImageGray.data;
        }

        case ENUMDSIMAGEFORMAT_RGB:
        {
            return this->ImageRGB.data;

        }

        case ENUMDSIMAGEFORMAT_R:
        {
            return this->ImageR.data;

        }

        case ENUMDSIMAGEFORMAT_G:
        {
            return this->ImageG.data;

        }

        case ENUMDSIMAGEFORMAT_B:
        {
            return this->ImageB.data;

        }

        }


    }

    OpencvImage getRoi(int x,int y,int width,int height)
    {
        OpencvImage retImg;
        retImg.Format=this->Format;
        switch (this->Format)
        {
        case ENUMDSIMAGEFORMAT_GRAY:
        {
            retImg.ImageGray=cv::Mat(this->ImageGray,cv::Rect(x,y,width,height));
        }
            break;
        case ENUMDSIMAGEFORMAT_RGB:
        {
            retImg.ImageRGB=cv::Mat(this->ImageRGB,cv::Rect(x,y,width,height));
        }
            break;
        case ENUMDSIMAGEFORMAT_R:
        {
            retImg.ImageR=cv::Mat(this->ImageR,cv::Rect(x,y,width,height));
        }
            break;
        case ENUMDSIMAGEFORMAT_G:
        {
            retImg.ImageG=cv::Mat(this->ImageG,cv::Rect(x,y,width,height));
        }
            break;
        case ENUMDSIMAGEFORMAT_B:
        {
            retImg.ImageB=cv::Mat(this->ImageB,cv::Rect(x,y,width,height));
        }
            break;
        }

        return retImg;

    }

    void save(QString path)
    {
        QTextCodec *code = QTextCodec::codecForName("GB2312");
        std::string name = code->fromUnicode(path).data();
        switch (this->Format)
        {

        case ENUMDSIMAGEFORMAT_GRAY:
        {
            cv::imwrite(name,this->ImageGray);
        }
            break;
        case ENUMDSIMAGEFORMAT_RGB:
        {
            cv::imwrite(name,this->ImageRGB);
        }
            break;
        case ENUMDSIMAGEFORMAT_R:

            break;
        case ENUMDSIMAGEFORMAT_G:

            break;
        case ENUMDSIMAGEFORMAT_B:

            break;
        default:
        {
            cv::imwrite(name,this->ImageRGB);
        }
            break;
        }

    }

    void loadImage(std::string filename)
    {
        switch (this->Format)
        {
        case ENUMDSIMAGEFORMAT_GRAY:
        {
            this->ImageGray=cv::imread(filename,0);
        }
            break;
        case ENUMDSIMAGEFORMAT_RGB:
        {
            this->ImageRGB=cv::imread(filename);
        }
            break;
        case ENUMDSIMAGEFORMAT_R:
        {
            this->ImageR=cv::imread(filename,0);
        }
            break;
        case ENUMDSIMAGEFORMAT_G:
        {
            this->ImageG=cv::imread(filename,0);
        }
            break;
        case ENUMDSIMAGEFORMAT_B:
        {
            this->ImageB=cv::imread(filename,0);
        }
            break;
        }
    }


    void toQImage(QImage* qImage)
    {
        switch (this->Format)
        {
        case ENUMDSIMAGEFORMAT_GRAY:
        {

            qImage->setColorCount(256);
            for(int i = 0; i < 256; i++)
            {
                qImage->setColor(i, qRgb(i, i, i));
            }
            cv::Mat m=ImageGray;

            for (int r=0;r<this->ImageGray.rows;r++)
            {
                memcpy(qImage->scanLine(r),m.ptr<uchar>(r),qImage->bytesPerLine());
            }

        }
            break;
        case ENUMDSIMAGEFORMAT_RGB:
        {

            cv::Mat m=ImageRGB;

            int ncounts=qImage->bytesPerLine();
            for (int r=0;r<this->ImageRGB.rows;r++)
            {
                memcpy(qImage->scanLine(r),m.ptr<uchar>(r),ncounts);
            }
            //            cv::imwrite("d:/mat.bmp",m);
            //            qImage->save("d:/qimg.bmp");
        }
            break;
        case ENUMDSIMAGEFORMAT_R:
            break;
        case ENUMDSIMAGEFORMAT_G:
            break;
        case ENUMDSIMAGEFORMAT_B:
            break;
        }
    }

    OpencvImage getRectImage(QRect rect)
    {
        switch (this->Format)
        {
        case ENUMDSIMAGEFORMAT_GRAY:
        {
            OpencvImage retImg(ENUMDSIMAGEFORMAT_GRAY,rect.height(),rect.width());
            retImg.ImageGray=cv::Mat(this->ImageGray,cv::Rect(rect.x(),rect.y(),rect.width(),rect.height())).clone();
            return retImg;
        }
            break;
        case ENUMDSIMAGEFORMAT_RGB:
        {
            OpencvImage retImg(ENUMDSIMAGEFORMAT_RGB,rect.height(),rect.width());
            retImg.ImageRGB=cv::Mat(this->ImageRGB,cv::Rect(rect.x(),rect.y(),rect.width(),rect.height())).clone();
            return retImg;
        }
            break;
        case ENUMDSIMAGEFORMAT_R:
            break;
        case ENUMDSIMAGEFORMAT_G:
            break;
        case ENUMDSIMAGEFORMAT_B:
            break;
        }
    }

    void toQPixmap(QPixmap & pixmap)
    {

        switch (this->Format)
        {
        case ENUMDSIMAGEFORMAT_GRAY:
        {
            QImage* image=new QImage(this->ImageGray.cols,this->ImageGray.rows,QImage::Format_Indexed8);
            this->toQImage(image);
            pixmap=QPixmap::fromImage(*image);
            delete  image;
        }
            break;
        case ENUMDSIMAGEFORMAT_RGB:
        {
            QImage* image=new QImage(this->ImageRGB.cols,this->ImageRGB.rows,QImage::Format_RGB888);
            this->toQImage(image);
            pixmap=QPixmap::fromImage(*image);
        }
            break;
        case ENUMDSIMAGEFORMAT_R:
        {

        }
            break;
        case ENUMDSIMAGEFORMAT_G:
        {

        }
            break;
        case ENUMDSIMAGEFORMAT_B:
        {

        }
            break;
        }

        //        QImage* image=new QImage(this->Width,this->Height,QImage::Format_Indexed8);
        //        this->toQImage(image);
        //        pixmap=QPixmap::fromImage(*image);
        //        delete  image;
    }


public:
#ifdef IMG_TYPE_GRAY
    ENUMDSIMAGEFORMAT Format=ENUMDSIMAGEFORMAT_GRAY;
#endif
#ifdef IMG_TYPE_RGB
    ENUMDSIMAGEFORMAT Format=ENUMDSIMAGEFORMAT_RGB;
#endif

    int Height;
    int Width;
    cv::Mat ImageGray;
    cv::Mat ImageRGB;
    cv::Mat ImageR;
    cv::Mat ImageG;
    cv::Mat ImageB;
};

#endif // OPENCVIMAGE_H
