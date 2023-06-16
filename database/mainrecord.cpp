#include "mainrecord.h"

MainRecord::MainRecord()
{
    m_NumAll=0;
    memset(m_NumNg,0,sizeof (int)*CAMERANUM_MAX);
}
