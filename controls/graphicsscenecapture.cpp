#include"graphicsscenecapture.h"


GraphicsSceneCapture::GraphicsSceneCapture(int hei,int wid)
{

//    this->m_Image=new QImage(wid,hei,QImage::Format_Indexed8);
//    m_Image->setColorCount(256);
//    for(int i = 0; i < 256; i++)
//    {
//        m_Image->setColor(i, qRgb(i, i, i));
//    }

//    m_ItemImage=new ItemImage(m_Image,nullptr);
//    this->addItem(m_ItemImage);

#ifdef IMG_TYPE_GRAY
      this->m_Image=new QImage(wid,hei,QImage::Format_Grayscale8);
#endif
#ifdef IMG_TYPE_RGB
      this->m_Image=new QImage(wid,hei,QImage::Format_RGB888);
#endif


    m_ItemImage=new ItemImage(m_Image,nullptr);
    this->addItem(m_ItemImage);

}

void GraphicsSceneCapture::updateImage(OpencvImage& images)
{
//    cv::imwrite("d:/images.bmp",images.ImageRGB);
    images.toQImage(this->m_Image);
//    this->m_Image->save("d:/qimg.bmp");
    this->update();
}

void GraphicsSceneCapture::slotUpdateImage(OpencvImage& images)
{
    images.toQImage(this->m_Image);
    this->update();
}
