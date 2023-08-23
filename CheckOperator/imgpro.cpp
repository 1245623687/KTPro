#include "imgpro.h"

ImgPro::ImgPro(bool valid, bool isCheck , bool existRefImage, int dirNum , int refNum , int checkNum, int workPiece) :Valid(valid), IsCheck(isCheck), ExistRefImage(existRefImage),
    DirNum(dirNum), RefNum(refNum), CheckNum(checkNum), WorkPiece(workPiece)
{
}

ImgPro::ImgPro(const ImgPro& another)
{
    this->ImgIndex = another.ImgIndex;
    this->Valid = another.Valid;
    this->IsCheck = another.IsCheck;
    this->ExistRefImage = another.ExistRefImage;
    this->DirNum = another.DirNum;
    this->RefNum = another.RefNum;
    this->CheckNum = another.CheckNum;
    this->WorkPiece = another.WorkPiece;
    this->m_bIsUseGPU= another.m_bIsUseGPU;


    if (LstCheckOperator.size() != 0)
    {
        for (list<CheckOperatorBasic*>::iterator itor = this->LstCheckOperator.begin(); itor != this->LstCheckOperator.end();)
        {
            CheckOperatorBasic* checkOperator = *itor;
            //内存添加
            switch (checkOperator->CheckType)
            {
            case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
            {
                CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch = dynamic_cast<CheckOperatorTemplateMatch*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorTemplateMatch);
            }
                break;
            case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
            {
                CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning = dynamic_cast<CheckOperatorEdgePositioning*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorEdgePositioning);
            }
                break;
            case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
            {
                CheckOperatorDefectInspect* pCheckOperatorDefectInspect = dynamic_cast<CheckOperatorDefectInspect*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorDefectInspect);
            }
                break;
            case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
            {
                CheckOperatorCircleInspect* pCheckOperatorCircleInspect = dynamic_cast<CheckOperatorCircleInspect*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorCircleInspect);
            }
                break;
            case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
            {
                CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect = dynamic_cast<CheckOperatorBrokenlineInspect*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorBrokenlineInspect);
            }
                break;
            case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
            {
                CheckOperatorGrayMeasurement* pCheckOperatorBrokenlineInspect = dynamic_cast<CheckOperatorGrayMeasurement*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorBrokenlineInspect);
            }
                break;
            case ENUMCHECKOPERATORTYPE_LINEINSPECT:
            {
                CheckOperatorLineInspect* pCheckOperatorLineInspect = dynamic_cast<CheckOperatorLineInspect*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorLineInspect);
            }
                break;
            case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
            {
                CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect = dynamic_cast<CheckOperatorAssociatedInspect*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorAssociatedInspect);
            }
                break;
            case ENUMCHECKOPERATORTYPE_STAYINSPECT:
            {
                CheckOperatorStayInspect* pCheckOperatorStayInspect = dynamic_cast<CheckOperatorStayInspect*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorStayInspect);
            }
                break;
            case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
            {
                CheckOperatorDoubleStayInspect* pCheckOperatorDoubleStayInspect = dynamic_cast<CheckOperatorDoubleStayInspect*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorDoubleStayInspect);
            }
                break;
            case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
            {
                CheckOperatorDLObjectDetect* pCheckOperatorDLObjectDetect = dynamic_cast<CheckOperatorDLObjectDetect*>(checkOperator);
                this->LstCheckOperator.push_back(pCheckOperatorDLObjectDetect);
            }
                break;
            }

        }
    }
}


void ImgPro::copyto(ImgPro* pCopyto)
{

    pCopyto->ImgIndex=this->ImgIndex;
    pCopyto->Valid=this->Valid;
    pCopyto->IsCheck=this->IsCheck;
    pCopyto->ExistRefImage=this->ExistRefImage;
    pCopyto->DirNum=this->DirNum;
    pCopyto->RefNum=this->RefNum;
    pCopyto->CheckNum=this->CheckNum;
    pCopyto->WorkPiece=this->WorkPiece;

    pCopyto->RefOpencvImage=this->RefOpencvImage;
//    pCopyto->CalcOpencvImage=this->CalcOpencvImage;
     pCopyto->m_bIsUseGPU= this->m_bIsUseGPU;



    auto itorTemplate=TemplateImage.begin();
    for(;itorTemplate!=TemplateImage.end();++itorTemplate)
    {
         auto itorTemplate2=itorTemplate->second.begin();
         std::map<int,OpencvImage> insert2;
         for(;itorTemplate2!=itorTemplate->second.end();++itorTemplate2)
         {

             insert2[itorTemplate2->first]=itorTemplate2->second;

         }

         pCopyto->TemplateImage[itorTemplate->first]=insert2;
    }



    list<CheckOperatorBasic*>::iterator itor= LstCheckOperator.begin();

    for(;itor!=this->LstCheckOperator.end();++itor)
    {
        switch ((*itor)->CheckType)
        {
        case  ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
        {
            CheckOperatorTemplateMatch* addCo=   static_cast<CheckOperatorTemplateMatch*>(*itor);
            CheckOperatorTemplateMatch* padd=new CheckOperatorTemplateMatch;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;
        case  ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
        {
            CheckOperatorEdgePositioning* addCo=   static_cast<CheckOperatorEdgePositioning*>(*itor);
            CheckOperatorEdgePositioning* padd=new CheckOperatorEdgePositioning;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;
        case  ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
        {
            CheckOperatorDefectInspect* addCo=   static_cast<CheckOperatorDefectInspect*>(*itor);
            CheckOperatorDefectInspect* padd=new CheckOperatorDefectInspect;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;
        case  ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
        {
            CheckOperatorCircleInspect* addCo=   static_cast<CheckOperatorCircleInspect*>(*itor);
            CheckOperatorCircleInspect* padd=new CheckOperatorCircleInspect;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;
        case  ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
        {
            CheckOperatorBrokenlineInspect* addCo=   static_cast<CheckOperatorBrokenlineInspect*>(*itor);
            CheckOperatorBrokenlineInspect* padd=new CheckOperatorBrokenlineInspect;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;
        case  ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
        {
            CheckOperatorGrayMeasurement* addCo=   static_cast<CheckOperatorGrayMeasurement*>(*itor);
            CheckOperatorGrayMeasurement* padd=new CheckOperatorGrayMeasurement;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;
        case  ENUMCHECKOPERATORTYPE_LINEINSPECT:
            //            CheckOperatorTemplateMatch* addCo=   static_cast<CheckOperatorTemplateMatch*>(*itor);
            //            CheckOperatorTemplateMatch* padd=new CheckOperatorTemplateMatch;
            //            addCo->copyto(padd);
            //            pCopyto->LstCheckOperator.push_back(padd);
            break;
        case  ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
        {
            CheckOperatorAssociatedInspect* addCo=   static_cast<CheckOperatorAssociatedInspect*>(*itor);
            CheckOperatorAssociatedInspect* padd=new CheckOperatorAssociatedInspect;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;
        case  ENUMCHECKOPERATORTYPE_STAYINSPECT :
        {
            CheckOperatorStayInspect* addCo=   static_cast<CheckOperatorStayInspect*>(*itor);
            CheckOperatorStayInspect* padd=new CheckOperatorStayInspect;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;
        case  ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT :
        {
            CheckOperatorDoubleStayInspect* addCo=   static_cast<CheckOperatorDoubleStayInspect*>(*itor);
            CheckOperatorDoubleStayInspect* padd=new CheckOperatorDoubleStayInspect;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;
        case  ENUMCHECKOPERATORTYPE_DEFECTINSPECT2:
        {
            CheckOperatorDefectInspect2* addCo=   static_cast<CheckOperatorDefectInspect2*>(*itor);
            CheckOperatorDefectInspect2* padd=new CheckOperatorDefectInspect2;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;

        case  ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT2:
        {
            CheckOperatorAssociatedInspect2* addCo=   static_cast<CheckOperatorAssociatedInspect2*>(*itor);
            CheckOperatorAssociatedInspect2* padd=new CheckOperatorAssociatedInspect2;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;


        case  ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
        {
            CheckOperatorDLObjectDetect* addCo=   static_cast<CheckOperatorDLObjectDetect*>(*itor);
            CheckOperatorDLObjectDetect* padd=new CheckOperatorDLObjectDetect;
            addCo->copyto(padd);
            pCopyto->LstCheckOperator.push_back(padd);
        }
            break;

        }

    }


}



ImgPro& ImgPro::operator=(ImgPro& another)
{
    if (&another != this)
    {
        this->ImgIndex = another.ImgIndex;
        this->Valid = another.Valid;
        this->IsCheck = another.IsCheck;
        this->ExistRefImage = another.ExistRefImage;
        this->DirNum = another.DirNum;
        this->RefNum = another.RefNum;
        this->CheckNum = another.CheckNum;
        this->WorkPiece = another.WorkPiece;
       this->m_bIsUseGPU= another.m_bIsUseGPU;

        if (LstCheckOperator.size() != 0)
        {
            for (list<CheckOperatorBasic*>::iterator itor = this->LstCheckOperator.begin(); itor != this->LstCheckOperator.end();)
            {
                CheckOperatorBasic* checkOperator = *itor;
                //内存添加
                switch (checkOperator->CheckType)
                {
                case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
                {
                    CheckOperatorTemplateMatch* pCheckOperatorTemplateMatch = dynamic_cast<CheckOperatorTemplateMatch*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorTemplateMatch);
                }
                    break;
                case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
                {
                    CheckOperatorEdgePositioning* pCheckOperatorEdgePositioning = dynamic_cast<CheckOperatorEdgePositioning*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorEdgePositioning);
                }
                    break;
                case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
                {
                    CheckOperatorDefectInspect* pCheckOperatorDefectInspect = dynamic_cast<CheckOperatorDefectInspect*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorDefectInspect);
                }
                    break;
                case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
                {
                    CheckOperatorCircleInspect* pCheckOperatorCircleInspect = dynamic_cast<CheckOperatorCircleInspect*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorCircleInspect);
                }
                    break;
                case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
                {
                    CheckOperatorBrokenlineInspect* pCheckOperatorBrokenlineInspect = dynamic_cast<CheckOperatorBrokenlineInspect*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorBrokenlineInspect);
                }
                    break;
                case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT:
                {
                    CheckOperatorGrayMeasurement* pCheckOperatorBrokenlineInspect = dynamic_cast<CheckOperatorGrayMeasurement*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorBrokenlineInspect);
                }
                    break;
                case ENUMCHECKOPERATORTYPE_LINEINSPECT:
                {
                    CheckOperatorLineInspect* pCheckOperatorLineInspect = dynamic_cast<CheckOperatorLineInspect*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorLineInspect);
                }
                    break;
                case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
                {
                    CheckOperatorAssociatedInspect* pCheckOperatorAssociatedInspect = dynamic_cast<CheckOperatorAssociatedInspect*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorAssociatedInspect);
                }
                    break;
                case ENUMCHECKOPERATORTYPE_STAYINSPECT:
                {
                    CheckOperatorStayInspect* pCheckOperatorStayInspect = dynamic_cast<CheckOperatorStayInspect*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorStayInspect);
                }
                    break;
                case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT:
                {
                    CheckOperatorDoubleStayInspect* pCheckOperatorDoubleStayInspect = dynamic_cast<CheckOperatorDoubleStayInspect*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorDoubleStayInspect);
                }
                    break;

                case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT:
                {
                    CheckOperatorDLObjectDetect* pCheckOperatorDLObjectDetect = dynamic_cast<CheckOperatorDLObjectDetect*>(checkOperator);
                    this->LstCheckOperator.push_back(pCheckOperatorDLObjectDetect);
                }
                    break;

                }
            }
        }
    }
    return *this;
}


ImgPro::~ImgPro()
{
    if (LstCheckOperator.size()!=0)
    {
        for (list<CheckOperatorBasic*>::iterator itor=this->LstCheckOperator.begin();itor!=this->LstCheckOperator.end();itor++)
        {
            switch ((*itor)->CheckType)
            {
            case ENUMCHECKOPERATORTYPE_TEMPLATEMATCH:
            {
                CheckOperatorTemplateMatch* p=static_cast<CheckOperatorTemplateMatch*>((*itor));
                delete p;
            }
                break;
            case ENUMCHECKOPERATORTYPE_EDGEPOSITIONING:
            {
                CheckOperatorEdgePositioning* p=static_cast<CheckOperatorEdgePositioning*>((*itor));
                delete p;
            }
                break;
            case ENUMCHECKOPERATORTYPE_DEFECTINSPECT:
            {
                CheckOperatorDefectInspect* p=static_cast<CheckOperatorDefectInspect*>((*itor));
                delete p;
            }
                break;
            case ENUMCHECKOPERATORTYPE_CIRCLEINSPECT:
            {
                CheckOperatorCircleInspect* p=static_cast<CheckOperatorCircleInspect*>((*itor));
                delete p;
            }
                break;
            case ENUMCHECKOPERATORTYPE_BROKENLINEINSPECT:
            {
                CheckOperatorBrokenlineInspect* p=static_cast<CheckOperatorBrokenlineInspect*>((*itor));
                delete p;
            }
                break;
            case ENUMCHECKOPERATORTYPE_GRAYMEASUREMENT :
            {
                CheckOperatorGrayMeasurement* p=static_cast<CheckOperatorGrayMeasurement*>((*itor));
                delete p;
            }
                break;
            case ENUMCHECKOPERATORTYPE_LINEINSPECT :
            {
                CheckOperatorLineInspect* p=static_cast<CheckOperatorLineInspect*>((*itor));
                delete p;
            }
                break;
            case ENUMCHECKOPERATORTYPE_ASSOCIATEDINSPECT:
            {
                CheckOperatorAssociatedInspect* p=static_cast<CheckOperatorAssociatedInspect*>((*itor));
                delete p;
            }
                break;
            case ENUMCHECKOPERATORTYPE_STAYINSPECT :
            {
                CheckOperatorStayInspect* p=static_cast<CheckOperatorStayInspect*>((*itor));
                delete p;
            }
                break;
            case ENUMCHECKOPERATORTYPE_DOUBLESTAYINSPECT :
            {
                CheckOperatorDoubleStayInspect* p=static_cast<CheckOperatorDoubleStayInspect*>((*itor));
                delete p;
            }
                break;

            case ENUMCHECKOPERATORTYPE_DLOBJECTDETECT :
            {
                CheckOperatorDLObjectDetect* p=static_cast<CheckOperatorDLObjectDetect*>((*itor));
                delete p;
            }
                break;

            }

            //            itor = LstCheckOperator.erase(itor);
        }
        //LstCheckOperator.clear();
    }
}
