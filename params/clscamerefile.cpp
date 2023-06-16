#include "clscamerefile.h"

std::mutex ClsCamereFile::m_Mutex;
ClsCamereFile* ClsCamereFile::m_Instance = nullptr;
ClsCamereFile ::GC ClsCamereFile ::gc;

ClsCamereFile::ClsCamereFile()
{
    load();
}
