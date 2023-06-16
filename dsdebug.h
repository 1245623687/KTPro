#ifndef DSDEBUG_H
#define DSDEBUG_H

#include<QDebug>

#define DSDEBUG qDebug()<<"file_"<<__FILE__<<" line_"<<__LINE__<<" function:"<<__FUNCTION__<<" :"

#define DSDEBUG_ qDebug()<<"MYDEBUG_"<<" :"

#define DSDEBUG__ qDebug()<<"MYDEBUG__"<<" :"

#endif // DSDEBUG_H


