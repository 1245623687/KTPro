#ifndef CIOBASE_H
#define CIOBASE_H

#include<mutex>
#include"params/dssystemparam.h"


class CIOBase
{


public:

   static CIOBase* getInstance(ENUMIOTYPE type);

   virtual int init()=0;
   
   virtual int unInitialize(){return  0;}


   virtual int setDirection(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal)=0;

   virtual int setLevel(unsigned char iPin,unsigned int idTypeVal,unsigned int setVal)=0;
   
   virtual int getDirection(int iPin, int &setVal){return  0;}
   virtual int getLevel(int iPin, int& setVal) {return  0;}

   virtual ~CIOBase()=0{}


 protected:
    CIOBase();

private:
   static std::mutex m_Mutex;
   static CIOBase* m_Instance;

};

#endif // CIOBASE_H
