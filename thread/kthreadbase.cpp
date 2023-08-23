#include "kthreadbase.h"

KThreadBase::KThreadBase()
{

}


void KThreadBase::stop()
{
    requestInterruption();
    quit();

//    terminate();
    wait();
}
