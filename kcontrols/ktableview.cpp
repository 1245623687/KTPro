#include "ktableview.h"
#include"dsdebug.h"
KTableView::KTableView(QWidget *parent):QTableView(parent)
{

}


void KTableView::resizeEvent(QResizeEvent * e)
{
    QTableView::resizeEvent(e);
    if(m_WidthFix.size())
    {
        if(m_WidthFix.size()>this->model()->columnCount()) return;

        int noSetNum=0;
        int allWidth=this->width()-2;
        int restWidth=allWidth;
        for (int idx=0;idx<this->m_WidthFix.size();++idx)
        {
            if(m_WidthFix[idx]==0)
                noSetNum++;
            restWidth-=m_WidthFix[idx];
        }

        for (int idx=0;idx<this->m_WidthFix.size();++idx)
        {

             this->setColumnWidth(idx, m_WidthFix[idx]==0? floor(restWidth/noSetNum):m_WidthFix[idx]);
        }
        return;
    }

    if(m_WidthScale.size())
    {
        if(m_WidthScale.size()>this->m_WidthFix.size()) return;

        int alltmp=0;
        for (int idx=0;idx<m_WidthScale.size();++idx)
        {
                     alltmp+=m_WidthScale[idx];
        }

        int allWidth=this->width()-2;
        for (int idx=0;idx<this->m_WidthFix.size();++idx)
        {
            this->setColumnWidth(idx,floor(m_WidthFix[idx]*alltmp/allWidth ));
        }
        return;
    }

}


void KTableView::setColumnScaleWidth(QVector<int>& arrWid)
{
    if(this->m_WidthFix.size()!=0)
    {
        m_WidthFix.clear();
    }

    m_WidthScale=arrWid;


}


void KTableView::setColumnFixWidth(QVector<int>& arrWid)
{

    if(this->m_WidthScale.size()!=0)
    {
        m_WidthScale.clear();
    }

    m_WidthFix=arrWid;
}
