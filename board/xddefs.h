#ifndef XDDEFS_H
#define XDDEFS_H

#ifdef __cplusplus
extern "C"
{
#endif
//#ifndef DEFINED_bool
//#define DEFINED_bool
//	typedef bool  bool;
//#endif


#ifndef DEFINED_UINT8
#define DEFINED_UINT8
	typedef unsigned char  UINT8;
#endif

#ifndef DEFINED_UINT16
#define DEFINED_UINT16
	typedef unsigned short UINT16;
#endif

#ifndef DEFINED_UINT32
#define DEFINED_UINT32
	typedef unsigned int   UINT32;
#endif

#ifndef NULL  
#ifdef __cplusplus
#define NULL 0
#else
#define NULL (void *)0
# endif
#endif

#ifdef __cplusplus
}
#endif

#endif  

