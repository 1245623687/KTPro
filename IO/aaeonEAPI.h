// aaeonEAPI.h : main header file for the aaeonEAPI DLL
//

#pragma once


extern "C" {
/***********************************************************************/
/* __IN
* Arg Type		  | Characteristics
* ----------------+-----------------------------------------------------
* Immediate value | Input value that must be specified and is essential
*				  | to function operation.
*				  |
* Pointer		  | Valid pointer to initialized buffer/variable.
*				  |
*/
#ifndef __IN
# define __IN
#endif

/* __INOPT
* Arg Type		  | Characteristics
* ----------------+-----------------------------------------------------
* Pointer		  | Valid pointer to initialized buffer/variable, or
*				  | NULL Pointer.
*				  | Note: refer to function specification for specifics.
*/
#ifndef __INOPT
# define __INOPT
#endif

/* __OUT
* Arg Type		  | Characteristics
* ----------------+-----------------------------------------------------
* Pointer		  | Valid pointer to destination buffer/variable
*/
#ifndef __OUT
# define __OUT
#endif

/* __OUTOPT
* Arg Type		  | Characteristics
* ----------------+-----------------------------------------------------
* Pointer		  | Valid pointer to destination buffer/variable, or
*				  | NULL Pointer.
*				  | Note: refer to function specification for specifics.
*/
#ifndef __OUTOPT
# define __OUTOPT
#endif

/* __INOUT
* Arg Type		  | Characteristics
* ----------------+-----------------------------------------------------
* Pointer		  | Valid pointer to initialized buffer/variable
*				  | Contents of buffer/variable updated before return.
*/
#ifndef __INOUT
# define __INOUT
#endif

#ifndef EAPI_CALLTYPE
# define EAPI_CALLTYPE __declspec(dllexport)
#endif

typedef unsigned __int8	 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;

#ifndef EAPI_UINT32_C
# define EAPI_UINT8_C(x)	((uint8_t)(x))
# define EAPI_UINT16_C(x)	((uint16_t)(x))
# define EAPI_UINT32_C(x)	((uint32_t)(x))
#endif
/***********************************************************************/
/* All version numbers
* +=========+===============================+
* | Bits	| Descriptions					|
* +=========+===============================+
* | 24 - 31 | Version Number				|
* +---------+-------------------------------+
* | 16 - 23 | Revision Number				|
* +---------+-------------------------------+
* | 0 - 15	| Build Number					|
* +=========+===============================+
*/
#define EAPI_VER_MASK_VER		EAPI_UINT32_C(0xFF000000)
#define EAPI_VER_MASK_REV		EAPI_UINT32_C(0x00FF0000)
#define EAPI_VER_MASK_BUILD		EAPI_UINT32_C(0x0000FFFF)
#define EAPI_VER_GET_VER(x)		EAPI_UINT8_C (((x)>>24)&UINT8_MAX )
#define EAPI_VER_GET_REV(x)		EAPI_UINT8_C (((x)>>16)&UINT8_MAX )
#define EAPI_VER_GET_BUILD(x)	EAPI_UINT16_C(((x)>> 0)&UINT16_MAX)
#define EAPI_VER_CREATE(Version, Revision, Build) (	EAPI_UINT32_C((((Version )&UINT8_MAX )<<24)|(((Revision)&UINT8_MAX )<<16)|(((Build )&UINT16_MAX)<< 0)))

/* Embedded API Standard Revision */
#define EAPI_VER		1
#define EAPI_REVISION	0
#define EAPI_VERSION	EAPI_VER_CREATE(EAPI_VER, EAPI_REVISION, 0)
/***********************************************************************/

/* Embedded Application System Interface */
/*
* EApi Types
*/
typedef uint32_t EApiStatus_t;
typedef uint32_t EApiId_t;

/*-------------------------------Library-------------------------------*/
/*
* EApiLibInitialize
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Already initialized			 | EAPI_STATUS_INITIALIZED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@brief 	Should be called before
*			calling any other API
*			function is called
*/
EApiStatus_t
EAPI_CALLTYPE
EApiLibInitialize(void);	/* Should be called before
                                                        * calling any other API
                                                        * function is called
                                                        */
/*
* EApiVGALibInitialize
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Already initialized			 | EAPI_STATUS_INITIALIZED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
EApiStatus_t
EAPI_CALLTYPE
EApiVGALibInitialize(void); /* Should be called before
                                                        *  calling any other VIDEO API
                                                        *  function is called
                                                        */

/*
* EApiLibUnInitialize
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@brief 	Should be called before
*			program exit
*/
EApiStatus_t
EAPI_CALLTYPE
EApiLibUnInitialize(void); /* Should be called before
                                                        * program exit
                                                        */
/*
* EApiVGALibUnInitialize
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
EApiStatus_t
EAPI_CALLTYPE
EApiVGALibUnInitialize(void);	/* Should be called before VIDEO backlight
                                                                *  program exit
                                                                */


/*
* Plug and Play Identifiers
*/
/*
* The following MACROS are for Creating OEM IDs
* OEM ID macros should be named
* EAPI_[PNPID]_ID_[TYPE]_[NAME]
* E.G.
* EAPI_PMG_ID_BOARD_CUSTOMERID
*/
#define EAPI_BYTE_SWAP_W(a) EAPI_UINT16_C((((a)<<8)&0xFF00)|(((a)>>8)&0x00FF))
#define EAPI_CREATE_PNPID(a, b, c) EAPI_BYTE_SWAP_W(((( a -'A'+1)&0x1F)<<10)|((( b -'A'+1)&0x1F)<< 5)|((( c -'A'+1)&0x1F)<< 0))
#define EAPI_CREATE_CUST_ID(a, b, c, Id) EAPI_UINT32_C((0xF<<28)|(EAPI_CREATE_PNPID(a, b, c)<<12)|(Id&0xFFF))
#define EAPI_PNPID_PICMG EAPI_CREATE_PNPID('P', 'M', 'G')
/* PICMG Should register this. */
/*
*
* B O A R D I N F O M A T I O N S T R I N G S
*
*/
/* IDS */
#define EAPI_ID_BOARD_MANUFACTURER_STR	EAPI_UINT32_C(0)
                                                                                /* Board Manufacturer
                                                                                * Name String
                                                                                */
#define EAPI_ID_BOARD_NAME_STR			EAPI_UINT32_C(1) /* Board Name String */
#define EAPI_ID_BOARD_REVISION_STR		EAPI_UINT32_C(2) /* Board Name String */
#define EAPI_ID_BOARD_SERIAL_STR		EAPI_UINT32_C(3) /* Board ID */
#define EAPI_ID_BOARD_BIOS_REVISION_STR EAPI_UINT32_C(4)
                                                                                /* Board Bios Revision
                                                                                * String
                                                                                */
#define EAPI_ID_BOARD_HW_REVISION_STR	EAPI_UINT32_C(5)
                                                                                /* Board Hardware
                                                                                * Revision String
                                                                                */
#define EAPI_ID_BOARD_PLATFORM_TYPE_STR	EAPI_UINT32_C(6)
                                                                                /* Platform ID
                                                                                * (ETX, COM Express,
                                                                                * etc...)
                                                                                */
#define EAPI_ID_EC_REVISION_STR EAPI_UINT32_C(7)
                                                                                /* EC Revision
                                                                                *
                                                                                *
                                                                                */
#define	EAPI_ID_BASEBOARD_SERIAL_STR EAPI_UINT32_C(8)
                                                                                /* SMBIOS Baseboard Serial number
                                                                                *
                                                                                *
                                                                                */
#define EAPI_ID_BASEBOARD_PRODUCTNAME_STR EAPI_UINT32_C(9)
                                                                                /* SMBIOS Baseboard Product Name
                                                                                *
                                                                                *
                                                                                */
/*
* EApiBoardGetStringA
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* pBufLen==NULL							 | EAPI_STATUS_INVALID_PARAMETER
* pBufLen!=NULL&&*pBufLen&&pBuffer==NULL | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* strlen(Id)+1>*pBufLen					 | EAPI_STATUS_MORE_DATA
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@param Id:			Name Id: EAPI_ID_BOARD_NAME_STR
*	@param Id:			Name Id: EAPI_ID_BOARD_SERIAL_STR
*	@param Id:          Name Id: EAPI_ID_BOARD_MANUFACTURER_STR
*	@param Id:			Name Id: EAPI_ID_BOARD_BIOS_REVISION_STR
*	@param Id:			Name Id: EAPI_ID_EC_REVISION_STR
*	@param Id:			Name Id: EAPI_ID_BASEBOARD_SERIAL_STR
*	@retval *pBuffer:	Destination pBuffer
*	@param  *pBufLen:	pBuffer Length
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: pBufLen==NULL/pBufLen!=NULL&&*pBufLen&&pBuffer==NULL
*	@return EAPI_STATUS_UNSUPPORTED: unknown Id
*	@return EAPI_STATUS_MORE_DATA: strlen(Id)+1>*pBufLen

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiBoardGetStringA(
        __IN EApiId_t Id,		/* Name Id */
        __OUT char *pBuffer,	/* Destination pBuffer */
        __INOUT uint32_t *pBufLen	/* pBuffer Length */
        );

/*
*
* B O A R D I N F O M A T I O N V A L U E S
*
*/
/* IDS */
#define EAPI_ID_GET_EAPI_SPEC_VERSION			EAPI_UINT32_C(0)
                                                                                                                                        /* EAPI Specification
                                                                                                                                        * Revision I.E. The
                                                                                                                                        * EAPI Spec Version
                                                                                                                                        * Bits 31-24, Revision
                                                                                                                                        * 23-16, 15-0 always 0
                                                                                                                                        * Used to implement
                                                                                                                                        * this interface
                                                                                                                                        */
#define EAPI_ID_BOARD_BOOT_COUNTER_VAL			EAPI_UINT32_C(1)	/* Units = Boots */
#define EAPI_ID_BOARD_RUNNING_TIME_METER_VAL	EAPI_UINT32_C(2)	/* Units = Minutes */
#define EAPI_ID_BOARD_PNPID_VAL					EAPI_UINT32_C(3)
                                                                                                                                        /* Encoded PNP ID
                                                                                                                                        * Format
                                                                                                                                        * (Compressed ASCII)
                                                                                                                                        */
#define EAPI_ID_BOARD_PLATFORM_REV_VAL			EAPI_UINT32_C(4)
                                                                                                                                        /* Platform Revision
                                                                                                                                        * I.E. The PICMG Spec
                                                                                                                                        * Version Bits 31-24,
                                                                                                                                        * Revision 23-16,
                                                                                                                                        * 15-0 always 0
                                                                                                                                        */
#define EAPI_ID_AONCUS_HISAFE_FUCTION			EAPI_UINT32_C(5)
                                                                                                                                        /* Check function need
                                                                                                                                        * to turn on or off
                                                                                                                                        */

#define EAPI_ID_BOARD_DRIVER_VERSION_VAL	EAPI_UINT32_C(0x10000)
                                                                                                                                        /* Vendor Specific
                                                                                                                                        * (Optional)
                                                                                                                                        */
#define EAPI_ID_BOARD_LIB_VERSION_VAL		EAPI_UINT32_C(0x10001)
                                                                                                                                        /* Vendor Specific
                                                                                                                                        * (Optional)
                                                                                                                                        */

#define EAPI_ID_HWMON_CPU_TEMP		EAPI_UINT32_C(0x20000) /* 0.1 Kelvins */
#define EAPI_ID_HWMON_CHIPSET_TEMP	EAPI_UINT32_C(0x20001) /* 0.1 Kelvins */
#define EAPI_ID_HWMON_SYSTEM_TEMP	EAPI_UINT32_C(0x20002) /* 0.1 Kelvins */

#define EAPI_KELVINS_OFFSET 2731
#define EAPI_ENCODE_CELCIUS(Celsius) EAPI_UINT32_C((((Celsius)*10))+EAPI_KELVINS_OFFSET)
#define EAPI_DECODE_CELCIUS(Celsius) ((Celsius)- EAPI_KELVINS_OFFSET)/10

#define EAPI_ID_HWMON_VOLTAGE_VCORE	EAPI_UINT32_C(0x21004) /* millivolts */
#define EAPI_ID_HWMON_VOLTAGE_2V5	EAPI_UINT32_C(0x21008) /* millivolts */
#define EAPI_ID_HWMON_VOLTAGE_3V3	EAPI_UINT32_C(0x2100C) /* millivolts */
#define EAPI_ID_HWMON_VOLTAGE_VBAT	EAPI_UINT32_C(0x21010) /* millivolts */
#define EAPI_ID_HWMON_VOLTAGE_5V	EAPI_UINT32_C(0x21014) /* millivolts */
#define EAPI_ID_HWMON_VOLTAGE_5VSB	EAPI_UINT32_C(0x21018) /* millivolts */
#define EAPI_ID_HWMON_VOLTAGE_12V	EAPI_UINT32_C(0x2101C) /* millivolts */
#define EAPI_ID_HWMON_VOLTAGE_DIMM	EAPI_UINT32_C(0x21020) /* millivolts */
#define EAPI_ID_HWMON_VOLTAGE_3VSB  EAPI_UINT32_C(0x21024) /* millivolts */		//modify by Tim 0429
#define EAPI_ID_HWMON_FAN_CPU		EAPI_UINT32_C(0x22000) /* RPM */
#define EAPI_ID_HWMON_FAN_CHIPSET	EAPI_UINT32_C(0x22001) /* RPM */
#define EAPI_ID_HWMON_FAN_SYSTEM	EAPI_UINT32_C(0x22002) /* RPM */
#define EAPI_ID_AONCUS_HISAFE_HWMON_GET_BASEADDR	EAPI_UINT32_C(0x23000)

/*
* EApiBoardGetValue
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* pValue==NULL							 | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@param  Id:			Value Id: EAPI_ID_HWMON_FAN_CPU
*	@param  Id:			Value Id: EAPI_ID_HWMON_FAN_CHIPSET
*	@param  Id:			Value Id: EAPI_ID_HWMON_FAN_SYSTEM

*	@param  Id:			Value Id: EAPI_ID_HWMON_CPU_TEMP
*	@param  Id:			Value Id: EAPI_ID_HWMON_CHIPSET_TEMP
*	@param  Id:			Value Id: EAPI_ID_HWMON_SYSTEM_TEMP

*	@param  Id:			Value Id: EAPI_ID_HWMON_VOLTAGE_VCORE
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_2V5
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_3V3
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_VBAT
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_5V
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_5VSB
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_12V
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_DIMM
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_3VSB

*	@param  Id:			Value Id: EAPI_ID_BOARD_DRIVER_VERSION_VAL
*	@param  Id:			Value Id: EAPI_ID_AONCUS_HISAFE_FUCTION
*	@retval	*pValue:	Return Value
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: pValue==NULL
*	@return EAPI_STATUS_UNSUPPORTED: unknown Id

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiBoardGetValue(
        __IN EApiId_t Id, /* Value Id */
        __OUT uint32_t *pValue /* Return Value */
        );

/*
*
* B A C K L I G H T
*
*/
/* IDS */
#define EAPI_ID_BACKLIGHT_1 EAPI_UINT32_C(0)
#define EAPI_ID_BACKLIGHT_2 EAPI_UINT32_C(1)
#define EAPI_ID_BACKLIGHT_3 EAPI_UINT32_C(2)
#define EAPI_ID_BACKLIGHT_4 EAPI_UINT32_C(3)

/* Backlight Values */
#define EAPI_BACKLIGHT_SET_ON			EAPI_UINT32_C(0)
#define EAPI_BACKLIGHT_SET_OFF			EAPI_UINT32_C(0xFFFFFFFF)
#define EAPI_BACKLIGHT_SET_DIMEST		EAPI_UINT32_C(0)
#define EAPI_BACKLIGHT_SET_BRIGHTEST	EAPI_UINT32_C(255)

/*
* EApiVgaGetBacklightEnable
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* pEnable==NULL							 | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiVgaGetBacklightEnable(
        __IN EApiId_t Id, /* Backlight Id */
        __OUT uint32_t *pEnable /* Backlight Enable */
        );
/*
* EApiVgaSetBacklightEnable
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* Enable!=EAPI_BACKLIGHT_SET_ON&&
* Enable!=EAPI_BACKLIGHT_SET_OFF		 | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiVgaSetBacklightEnable(
        __IN EApiId_t Id, /* Backlight Id */
        __IN uint32_t Enable /* Backlight Enable */
        );
/*
*
* EApiVgaGetBacklightBrightness
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* pBright==NULL							 | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@param  Id:		Backlight Id: EAPI_ID_BACKLIGHT_1
*	@param  Id:		Backlight Id: EAPI_ID_BACKLIGHT_2
*	@param  Id:		Backlight Id: EAPI_ID_BACKLIGHT_3
*	@retval *pBright:	Get the value of brightness, the value is from 0 to 255
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: pBright==NULL
*	@return EAPI_STATUS_UNSUPPORTED: unknown Id

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiVgaGetBacklightBrightness(
        __IN EApiId_t Id, /* Backlight Id */
        __OUT uint32_t *pBright /* Backlight Brightness */
        );
/*
* EApiVgaSetBacklightBrightness
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* Bright>EAPI_BACKLIGHT_SET_BRIGHTEST	 | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@param  Id:		Backlight Id: EAPI_ID_BACKLIGHT_1
*	@param  Id:		Backlight Id: EAPI_ID_BACKLIGHT_2
*	@param  Id:		Backlight Id: EAPI_ID_BACKLIGHT_3
*	@param	Bright:	Set the value of brightness, the value is from 0 to 255
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: Bright>EAPI_BACKLIGHT_SET_BRIGHTEST
*	@return EAPI_STATUS_UNSUPPORTED: unknown Id

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiVgaSetBacklightBrightness(
        __IN EApiId_t Id, /* Backlight Id */
        __IN uint32_t Bright /* Backlight Brightness */
        );

/*
*
* S T O R A G E
*
*/
/* IDs */
#define EAPI_ID_STORAGE_STD EAPI_UINT32_C(0)
/* Dummy Example */
#define EAPI_PMG_ID_STORAGE_SAMPLE EAPI_CREATE_CUST_ID('P', 'M', 'G', 0)

/*
* EApiStorageCap
*
* Condition									   | Return Values
* ---------------------------------------------+------------------------------
* Library Uninitialized						   | EAPI_STATUS_NOT_INITIALIZED
* ((pStorageSize==NULL)&&(pBlockLength==NULL)) | EAPI_STATUS_INVALID_PARAMETER
* Unsupported Id							   | EAPI_STATUS_UNSUPPORTED
* Common Error								   | Common Error Code
* Else										   | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiStorageCap(
        __IN EApiId_t Id, /* Storage Area Id */
        __OUT uint32_t *pStorageSize, /* Total */
        __OUT uint32_t *pBlockLength /* Write Block Length & Alignment */
        );
/*
* EApiStorageAreaRead
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* pBuffer==NULL							 | EAPI_STATUS_INVALID_PARAMETER
* ByteCnt==0							 | EAPI_STATUS_INVALID_PARAMETER
* BufLen==0								 | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Offset+ByteCnt>pStorageSize			 | EAPI_STATUS_INVALID_BLOCK_LENGTH
* Read Error							 | EAPI_STATUS_READ_ERROR
* ByteCnt>BufLen						 | EAPI_STATUS_MORE_DATA
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiStorageAreaRead(
        __IN EApiId_t Id, /* Storage Area Id */
        __IN uint32_t Offset, /* Byte Offset */
        __OUT void *pBuffer, /* Pointer to Data pBuffer */
        __IN uint32_t BufLen, /* Data pBuffer Size in bytes */
        __IN uint32_t ByteCnt /* Number of bytes to read */
        );
/*
* EApiStorageAreaWrite
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* pBuffer==NULL							 | EAPI_STATUS_INVALID_PARAMETER
* ByteCnt==0							 | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Offset%pBlockLength					 | EAPI_STATUS_INVALID_BLOCK_ALIGNMENT
* ByteCnt%pBlockLength					 | EAPI_STATUS_INVALID_BLOCK_ALIGNMENT
* Offset+ByteCnt>pStorageSize			 | EAPI_STATUS_INVALID_BLOCK_LENGTH
* Write Error							 | EAPI_STATUS_WRITE_ERROR
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiStorageAreaWrite(
        __IN EApiId_t Id, /* Storage Area Id */
        __IN uint32_t Offset, /* Byte Offset */
        __IN void *pBuffer, /* Pointer to Data pBuffer */
        __IN uint32_t ByteCnt /* Number of bytes to write */
        );

/*
*
* I 2 C
*
*/
/* IDs */
#define EAPI_ID_I2C_EXTERNAL			EAPI_UINT32_C(0) /* Baseboard first I2C Interface */
#define EAPI_ID_I2C_LVDS_1				EAPI_UINT32_C(1) /* LVDS/EDP 1 Interface (optional) */
#define EAPI_ID_I2C_LVDS_2				EAPI_UINT32_C(2) /* LVDS/EDP 2 Interface (optional) */
#define EAPI_ID_AONCUS_I2C_EXTERNAL_2	EAPI_UINT32_C(3) /* Baseboard 2nd I2C Interface */
#define EAPI_ID_AONCUS_I2C_EXTERNAL_3	EAPI_UINT32_C(4) /* Baseboard 3rd I2C Interface */
#define EAPI_ID_AONCUS_I2C_EXTERNAL_4	EAPI_UINT32_C(5) /* Baseboard 4rd I2C Interface */
#define EAPI_ID_AONCUS_I2C_EXTERNAL_5	EAPI_UINT32_C(6) /* Baseboard 5rd I2C Interface */
#define EAPI_ID_AONCUS_I2C_EXTERNAL_6	EAPI_UINT32_C(7) /* Baseboard 6rd I2C Interface */
#define EAPI_ID_AONCUS_I2C_EXTERNAL_7	EAPI_UINT32_C(8) /* Baseboard 7rd I2C Interface */
#define EAPI_ID_AONCUS_I2C_EXTERNAL_8	EAPI_UINT32_C(9) /* Baseboard 8rd I2C Interface */
#define EAPI_ID_AONCUS_SMBUS_EXTERNAL_1	EAPI_UINT32_C(10) /* Baseboard first SMBUS Interface */
#define EAPI_ID_AONCUS_SMBUS_EXTERNAL_2	EAPI_UINT32_C(11) /* Baseboard 2nd SMBUS Interface */
#define EAPI_ID_AONCUS_SMBUS_EXTERNAL_3	EAPI_UINT32_C(12) /* Baseboard 3rd SMBUS Interface */

/*
* I2C Address Format
*
* L = Set to 0
* H = Set to 1
* X = Don't Care(Direction Bit)
* 0-F Address Bit
*
* Bits 31-16 are Reserved and should be set to 0
*
* Bit Offset	  | F E D C B A 9 8 7 6 5 4 3 2 1 0
* ----------------+--------------------------------
* 7 Bit Address   | L L L L L L L L 6 5 4 3 2 1 0 X
* 10 Bit Address  | H H H H L 9 8 X 7 6 5 4 3 2 1 0
*
* Examples where Don't Care bits set to 0
*			Encoded Encoded
* Address	7Bit	10Bit
*	0x01	0x02	0xF001
*	0x58	0xA0	0xF058
*	0x59	0xA2	0xF059
*	0x77	0xEE	0xF077
*	0x3FF			0xF6FF
*
*/
#define EAPI_I2C_ENC_7BIT_ADDR(x) EAPI_UINT8_C(((x)&0x07F)<<1)
#define EAPI_I2C_DEC_7BIT_ADDR(x) EAPI_UINT8_C(((x)>>1)&0x07F)

/*
* EApiI2CGetBusCap
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* pMaxBlkLen==NULL						 | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiI2CGetBusCap(
        __IN EApiId_t Id, /* I2C Bus Id */
        __OUT uint32_t *pMaxBlkLen /* Max Block Length Supported on this interface */
        );
/*
* EApiI2CWriteRead
*
* Condition								   | Return Values
* -----------------------------------------+------------------------------
* Library Uninitialized					   | EAPI_STATUS_NOT_INITIALIZED
* (WriteBCnt>1)&&(pWBuffer==NULL)		   | EAPI_STATUS_INVALID_PARAMETER
* (ReadBCnt>1)&&(pRBuffer==NULL)		   | EAPI_STATUS_INVALID_PARAMETER
* (ReadBCnt>1)&&(RBufLen==0)			   | EAPI_STATUS_INVALID_PARAMETER
* ((WriteBCnt==0)&&(ReadBCnt==0))		   | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							   | EAPI_STATUS_UNSUPPORTED
* WriteBCnt>(pMaxBlkLen+1)				   | EAPI_STATUS_INVALID_BLOCK_LENGTH
* ReadBCnt>(pMaxBlkLen+1)				   | EAPI_STATUS_INVALID_BLOCK_LENGTH
* Bus Busy SDA/SDC low					   | EAPI_STATUS_BUSY_COLLISION
* Arbitration Error/Collision Error		   | EAPI_STATUS_BUSY_COLLISION
* On Write 1 write cycle				   |
* SDA Remains low						   |
* Timeout due to clock stretching		   | EAPI_STATUS_TIMEOUT
* start<Addr Byte><W>Nak				   | EAPI_STATUS_NOT_FOUND
* start<Addr Byte><R>Nak				   | EAPI_STATUS_NOT_FOUND
* start<Addr Byte><W>Ack<Data Byte 1>Nak   | EAPI_STATUS_WRITE_ERROR
* ReadBCnt>(RBufLen+1)					   | EAPI_STATUS_MORE_DATA
* Common Error							   | Common Error Code
* Else									   | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiI2CWriteReadRaw(
        __IN EApiId_t Id, /* I2C Bus Id */
        __IN uint8_t Addr, /* Encoded 7Bit I2C Device Address */
        __INOPT void *pWBuffer, /* Write Data pBuffer */
        __IN uint32_t WriteBCnt, /* Number of Bytes to write plus 1 */
        __OUTOPT void *pRBuffer, /* Read Data pBuffer */
        __IN uint32_t RBufLen, /* Data pBuffer Length */
        __IN uint32_t ReadBCnt /* Number of Bytes to Read plus 1 */
        );

#define EApiI2CWriteRaw(Id, Addr, pBuffer, ByteCnt) \
                        EApiI2CWriteReadRaw(Id, Addr, pBuffer, ByteCnt, NULL, 0, 0)
#define EApiI2CReadRaw(Id, Addr, pBuffer, BufLen, ByteCnt) \
                        EApiI2CWriteReadRaw(Id, Addr, NULL, 0, pBuffer, BufLen, ByteCnt)
#define EAPI_I2C_ENC_10BIT_ADDR(x)	EAPI_UINT32_C(((x)&0xFF)|(((x)&0x0300)<<1)|0xF000)
#define EAPI_I2C_DEC_10BIT_ADDR(x)	EAPI_UINT32_C(((x)&0xFF)|(((x)>>1)&0x300))
#define EAPI_I2C_IS_10BIT_ADDR(x)	(((x)&0xF800)==0xF000)
#define EAPI_I2C_IS_7BIT_ADDR(x)	(!EAPI_I2C_IS_10BIT_ADDR(x))

/*
* I2C Transfer Types
* Bits 31 & 30 Selects Command Type
*
* Transfer Type 1:
* Address Format : 7Bit
* Command Type : None
* Data Direction : Write
* Start<Addr Byte><W>Ack<Data Byte 1>Ack Stop
*
* Transfer Type 2:
* Address Format : 7Bit
* Command Type : None
* Data Direction : Read
* Start<Addr Byte><R>Ack<Data Byte 1>Nak Stop
*
* Transfer Type 3:
* Address Format : 7Bit
* Command Type : Standard
* Data Direction : Write
* Start<Addr Byte><W>Ack<CMD Byte>Ack<Data Byte 1>Ack Stop
*
* Transfer Type 4:
* Address Format : 7Bit
* Command Type : Standard
* Data Direction : Read
* Start<Addr Byte><W>Ack<CMD Byte>Ack
* Start<Addr Byte><R>Ack<Data Byte 1>Nak Stop
*
* Transfer Type 5:
* Address Format : 7Bit
* Command Type : Extended
* Data Direction : Write
* Start<Addr Byte><W>Ack<CMD Byte MSB>Ack<CMD Byte LSB>Ack<Data Byte 1>Ack Stop
*
* Transfer Type 6:
* Address Format : 7Bit
* Command Type : Extended
* Data Direction : Read
* Start<Addr Byte><W>Ack<CMD Byte MSB>Ack<CMD Byte LSB>Ack
* Start<Addr Byte><R>Ack<Data Byte 1>Nak Stop
*
* Transfer Type 7:
* Address Format : 10Bit
* Command Type : None
* Data Direction : Write
* Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<Data Byte 1>Ack Stop
*
* Transfer Type 8:
* Address Format : 10Bit
* Command Type : None
* Data Direction : Read
* Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack
* Start<Addr Byte MSB><R>Ack<Data Byte 1>Nak Stop
*
* Transfer Type 9:
* Address Format : 10Bit
* Command Type : Standard
* Data Direction : Write
* Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<CMD Byte>Ack<Data Byte 1>Ack Stop
*
* Transfer Type 10:
* Address Format : 10Bit
* Command Type : Standard
* Data Direction : Read
* Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<CMD Byte>Ack
* Start<Addr Byte MSB><R>Ack<Data Byte 1>Nak Stop
*
* Transfer Type 11:
* Address Format : 10Bit
* Command Type : Extended
* Data Direction : Write
* Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<CMD Byte MSB>Ack<CMD Byte LSB>Ack<Data Byte1>Ack Stop
*
* Transfer Type 12:
* Address Format : 10Bit
* Command Type : Extended
* Data Direction : Read
* Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack<CMD Byte MSB>Ack<CMD Byte LSB>Ack
* Start<Addr Byte MSB><R>Ack<Data Byte 1>Nak Stop
*
*/
#define EAPI_I2C_STD_CMD		EAPI_UINT32_C(0<<30)
#define EAPI_I2C_EXT_CMD		EAPI_UINT32_C(2<<30)
#define EAPI_I2C_NO_CMD			EAPI_UINT32_C(1<<30)
#define EAPI_I2C_CMD_TYPE_MASK	EAPI_UINT32_C(3<<30)

#define EAPI_I2C_ENC_STD_CMD(x)	EAPI_UINT32_C(((x)&0xFF)|EAPI_I2C_STD_CMD)
#define EAPI_I2C_ENC_EXT_CMD(x)	EAPI_UINT32_C(((x)&0xFFFF)|EAPI_I2C_EXT_CMD)
#define EAPI_I2C_IS_EXT_CMD(x) (EAPI_UINT32_C((x)&(EAPI_I2C_CMD_TYPE_MASK))==EAPI_I2C_EXT_CMD)
#define EAPI_I2C_IS_STD_CMD(x) (EAPI_UINT32_C((x)&(EAPI_I2C_CMD_TYPE_MASK))==EAPI_I2C_STD_CMD)
#define EAPI_I2C_IS_NO_CMD(x) (EAPI_UINT32_C((x)&(EAPI_I2C_CMD_TYPE_MASK))==EAPI_I2C_NO_CMD)

/*
* EApiI2CReadTransfer
* Addr Byte 1 Below Designates Addr MSB in a 10bit address transfer and
* the complete address in an 7bit address transfer.
*
* Condition								   | Return Values
* -----------------------------------------+------------------------------
* Library Uninitialized					   | EAPI_STATUS_NOT_INITIALIZED
* pBuffer==NULL							   | EAPI_STATUS_INVALID_PARAMETER
* BufLen==0								   | EAPI_STATUS_INVALID_PARAMETER
* ByteCnt==0							   | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							   | EAPI_STATUS_UNSUPPORTED
* ByteCnt>pMaxBlkLen					   | EAPI_STATUS_INVALID_BLOCK_LENGTH
* Bus Busy SDA/SDC low					   | EAPI_STATUS_BUSY_COLLISION
* Arbitration Error/Collision Error		   | EAPI_STATUS_BUSY_COLLISION
* On Write 1 write cycle				   |
* SDA Remains low						   |
* Time-out due to clock stretching		   | EAPI_STATUS_TIMEOUT
* start<Addr Byte 1><W>Nak				   | EAPI_STATUS_NOT_FOUND
* start<Addr Byte 1><R>Nak				   | EAPI_STATUS_NOT_FOUND
* start<Addr Byte 1><W>Ack<Addr Byte 2>Nak | EAPI_STATUS_WRITE_ERROR or
*										   | EAPI_STATUS_NOT_FOUND
* start<Addr Byte 1><W>Ack<CMD Byte 1>Nak  | EAPI_STATUS_WRITE_ERROR
* start<Addr Byte 1><W>Ack<Data Byte 1>Nak | EAPI_STATUS_WRITE_ERROR
* ByteCnt>BufLen						   | EAPI_STATUS_MORE_DATA
* Common Error							   | Common Error Code
* Else									   | EAPI_STATUS_SUCCESS
*/
/**
*	@param	Id:			I2C Bus Id: EAPI_ID_I2C_EXTERNAL
*	@param	Id:			I2C Bus Id: EAPI_ID_SMBUS_EXTERNAL
*	@param	Addr:		Encoded 7/10Bit I2C Device Address
*	@param	Cmd:		I2C Command/Offset
*	@retval	*pBuffer:	Transfer Data pBuffer
*	@param	BufLen		Data pBuffer Length: Byte=1; Word=2
*	@param  ByteCnt		Byte Count to read:	Byte=1; Word=2
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: pBuffer==NULL/BufLen==0/ByteCnt==0
*	@return EAPI_STATUS_UNSUPPORTED: unknown Id
*	@return EAPI_STATUS_INVALID_BLOCK_LENGTH: ByteCnt>pMaxBlkLen
*	@return EAPI_STATUS_BUSY_COLLISION: Bus Busy SDA/SDC low/Arbitration Error/Collision Error
*	@return EAPI_STATUS_TIMEOUT: Time-out due to clock stretching
*	@return EAPI_STATUS_NOT_FOUND: start<Addr Byte 1><R>Nak

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiI2CReadTransfer(
        __IN EApiId_t Id, /* I2C Bus Id */
        __IN uint32_t Addr, /* Encoded 7/10Bit I2C Device Address */
        __IN uint32_t Cmd, /* I2C Command/Offset */
        __OUT void *pBuffer, /* Transfer Data pBuffer */
        __IN uint32_t BufLen, /* Data pBuffer Length */
        __IN uint32_t ByteCnt /* Byte Count to read */
        );
/*
* EApiI2CWriteTransfer
* Addr Byte 1 Below Designates Addr MSB in a 10bit address transfer and
* the complete address in an 7bit address transfer.
*
* Condition								   | Return Values
* -----------------------------------------+------------------------------
* Library Uninitialized					   | EAPI_STATUS_NOT_INITIALIZED
* pBuffer==NULL							   | EAPI_STATUS_INVALID_PARAMETER
* ByteCnt==0							   | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							   | EAPI_STATUS_UNSUPPORTED
* ByteCnt+(overhead)>pMaxBlkLen			   | EAPI_STATUS_INVALID_BLOCK_LENGTH
* Bus Busy SDA/SDC low					   | EAPI_STATUS_BUSY_COLLISION
* Arbitration Error/Collision Error		   | EAPI_STATUS_BUSY_COLLISION
* On Write 1 write cycle				   |
* SDA Remains low						   |
* Time-out due to clock stretching		   | EAPI_STATUS_TIMEOUT
* start<Addr Byte 1><W>Nak				   | EAPI_STATUS_NOT_FOUND
* start<Addr Byte 1><R>Nak				   | EAPI_STATUS_NOT_FOUND
* start<Addr Byte 1><W>Ack<Addr Byte 2>Nak | EAPI_STATUS_WRITE_ERROR or
*										   | EAPI_STATUS_NOT_FOUND
* start<Addr Byte 1><W>Ack<CMD Byte 1>Nak  | EAPI_STATUS_WRITE_ERROR
* start<Addr Byte 1><W>Ack<Data Byte 1>Nak | EAPI_STATUS_WRITE_ERROR
* Common Error							   | Common Error Code
* Else									   | EAPI_STATUS_SUCCESS
*/
/**
*	@param	Id:			I2C Bus Id: EAPI_ID_I2C_EXTERNAL
*	@param	Id:			I2C Bus Id: EAPI_ID_SMBUS_EXTERNAL
*	@param	Addr:		Encoded 7/10Bit I2C Device Address
*	@param	Cmd:		I2C Command/Offset
*	@param	*pBuffer	Transfer Data pBuffer
*	@param  ByteCnt		Byte Count to write: Byte=1; Word=2
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: pBuffer==NULL/BufLen==0/ByteCnt==0
*	@return EAPI_STATUS_UNSUPPORTED: unknown Id
*	@return EAPI_STATUS_INVALID_BLOCK_LENGTH: ByteCnt+(overhead)>pMaxBlkLen
*	@return EAPI_STATUS_BUSY_COLLISION: Bus Busy SDA/SDC low/Arbitration Error/Collision Error
*	@return EAPI_STATUS_TIMEOUT: Time-out due to clock stretching
*	@return EAPI_STATUS_NOT_FOUND: start<Addr Byte 1><W>Nak
*	@return EAPI_STATUS_WRITE_ERROR: start<Addr Byte 1><W>Ack<Addr Byte 2>Nak/<CMD Byte 1>Nak/<Data Byte 1>Nak

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiI2CWriteTransfer(
        __IN EApiId_t Id, /* I2C Bus Id */
        __IN uint32_t Addr, /* Encoded 7/10Bit I2C Device Address */
        __IN uint32_t Cmd, /* I2C Command/Offset;  Seq&Page Mode-->ByteCnt*/
        __IN void *pBuffer, /* Transfer Data pBuffer */
        __IN uint32_t ByteCnt /* Byte Count to write */
        );
/*
* I2C Probe Types
*
* Probe Type 1:
* Address Format : 7Bit
* Start<Addr Byte><W>Ack Stop
*
* Probe Type 2:
* Address Format : 10Bit
* Start<Addr Byte MSB><W>Ack<Addr Byte LSB>Ack Stop
*
*/
/*
* EApiI2CProbeDevice
*
* Condition									   | Return Values
* ---------------------------------------------+------------------------------
* Library Uninitialized						   | EAPI_STATUS_NOT_INITIALIZED
* Bus Busy SDA/SDC low						   | EAPI_STATUS_BUSY_COLLISION
* Arbitration Error/Collision Error			   | EAPI_STATUS_BUSY_COLLISION
* On Write 1 write cycle					   |
* SDA Remains low							   |
* Time-out due to clock stretching			   | EAPI_STATUS_TIMEOUT
*											   |
* 7Bit Address								   |
* start<Addr Byte><W>Nak					   | EAPI_STATUS_NOT_FOUND
*											   |
* 10Bit Address								   |
* start<Addr Byte MSB><W>Nak				   | EAPI_STATUS_NOT_FOUND
* start<Addr Byte MSB><W>Ack<Addr Byte LSB>Nak | EAPI_STATUS_NOT_FOUND
*											   |
* Common Error								   | Common Error Code
* Else										   | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiI2CProbeDevice(
        __IN EApiId_t Id, /* I2C Bus Id */
        __IN uint32_t Addr /* Encoded 7/10Bit I2C Device Address */
        );

/*
*
* W A T C H D O G
*
*/
/*
*
* After EApiWDogStart
*
* |<- Delay ->|<- Event Timeout ->|<- Reset Timeout ->|
* A-----------B-------------------C-------------------D
*
* After EApiWDogTrigger
*
* |<- Event Timeout ->|<- Reset Timeout ->|
* E-------------------F-------------------G
*
* Stage A
* Watchdog is started.
*
* Stage B
* Initial Delay Period is exhausted.
*
* Stage C/F
* Event is triggered, NMI, IRQ, or PIN is Triggered.
* To Allow for possible Software Recovery.
*
* Stage D/G
* System is reset.
*
* Stage E
* Watchdog is Triggered.
*
* EApiWDogTrigger/EApiWDogStop Must be called before Stage C/F
* to prevent event from being generated.
*
* EApiWDogTrigger/EApiWDogStop Must be called before Stage D/G
* to prevent The system from being reset.
*
*/

/*
* EApiWDogGetCap
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* Unsupported							 | EAPI_STATUS_UNSUPPORTED
* pMaxDelay==NULL&&						 |
* pMaxResetTimeout==NULL&&				 |
* pMaxEventTimeout==NULL				 | EAPI_STATUS_INVALID_PARAMETER
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@retval	*pMaxDelay:				Maximum Supported Delay in milliseconds
*	@retval	*pMaxEventTimeout:		Maximum Supported Event Timeout in milliseconds 0 == Unsupported
*	@retval	*pMaxResetTimeout:		Maximum Supported Reset Timeout in milliseconds
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported
*	@return EAPI_STATUS_INVALID_PARAMETER: pMaxDelay==NULL && pMaxResetTimeout==NULL && pMaxEventTimeout==NULL

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiWDogGetCap(
        __OUTOPT uint32_t *pMaxDelay, /* Maximum Supported Delay in milliseconds*/
        __OUTOPT uint32_t *pMaxEventTimeout, /* Maximum Supported Event Timeout in
                                                                                        * milliseconds
                                                                                        * 0 == Unsupported
                                                                                        */
        __OUTOPT uint32_t *pMaxResetTimeout /* Maximum Supported
                                                                                * Reset Timeout in
                                                                                * milliseconds
                                                                                */
        );
/*
* EApiWDogStart
*
* Condition						   | Return Values
* ---------------------------------+------------------------------
* Library Uninitialized			   | EAPI_STATUS_NOT_INITIALIZED
* Unsupported					   | EAPI_STATUS_UNSUPPORTED
* Delay>pMaxDelay				   | EAPI_STATUS_INVALID_PARAMETER
* EventTimeout>pMaxEventTimeout	   | EAPI_STATUS_INVALID_PARAMETER
* ResetTimeout>pMaxResetTimeout	   | EAPI_STATUS_INVALID_PARAMETER
* Already Running				   | EAPI_STATUS_RUNNING
* Common Error					   | Common Error Code
* Else							   | EAPI_STATUS_SUCCESS
*/
/**
*	@param	Delay:				Delay in milliseconds
*	@param	EventTimeout:		Event Timeout in milliseconds
*	@param	ResetTimeout:		Reset Timeout in milliseconds
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported
*	@return EAPI_STATUS_INVALID_PARAMETER: Delay>pMaxDelay/EventTimeout>pMaxEventTimeout/ResetTimeout>pMaxResetTimeout
*	@return EAPI_STATUS_RUNNING: Already Running

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiWDogStart(
        __IN uint32_t Delay, /* Delay in milliseconds */
        __IN uint32_t Minute, /* Control Minute or Second */
        __IN uint32_t EventTimeout, /* Event Timeout in milliseconds */
        __IN uint32_t ResetTimeout /* Reset Timeout in milliseconds	*/
        );
/*
* EApiWDogTrigger
*
* Condition						   | Return Values
* ---------------------------------+------------------------------
* Library Uninitialized			   | EAPI_STATUS_NOT_INITIALIZED
* Unsupported					   | EAPI_STATUS_UNSUPPORTED
* Watchdog Not Started			   | EAPI_STATUS_ERROR
* Common Error					   | Common Error Code
* Else							   | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiWDogTrigger(void);
/*
* EApiWDogStop
*
* Condition						   | Return Values
* ---------------------------------+------------------------------
* Library Uninitialized			   | EAPI_STATUS_NOT_INITIALIZED
* Unsupported					   | EAPI_STATUS_UNSUPPORTED
* Common Error					   | Common Error Code
* Else							   | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Close Watchdog Instance
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported
*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiWDogStop(void);

/*
* EApiWDogReloadTimer
*
* Condition						   | Return Values
* ---------------------------------+------------------------------
* Library Uninitialized			   | EAPI_STATUS_NOT_INITIALIZED
* Unsupported					   | EAPI_STATUS_UNSUPPORTED
* Common Error					   | Common Error Code
* Else							   | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Reload the Timeout count
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported
*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiWDogReloadTimer(void);

/*
*
* G P I O
*
*/
/* IDs */
/*
* Individual ID Per GPIO Mapping
*/
#define EAPI_GPIO_GPIO_ID(GPIO_NUM)	EAPI_UINT32_C(GPIO_NUM)
#define EAPI_GPIO_GPIO_BITMASK		EAPI_UINT32_C(1)
#define EAPI_ID_GPIO_GPIO00			EAPI_GPIO_GPIO_ID(0) /* (Optional) */
#define EAPI_ID_GPIO_GPIO01			EAPI_GPIO_GPIO_ID(1) /* (Optional) */
#define EAPI_ID_GPIO_GPIO02			EAPI_GPIO_GPIO_ID(2) /* (Optional) */
#define EAPI_ID_GPIO_GPIO03			EAPI_GPIO_GPIO_ID(3) /* (Optional) */
#define EAPI_ID_GPIO_GPIO04			EAPI_GPIO_GPIO_ID(4) /* (Optional) */
#define EAPI_ID_GPIO_GPIO05			EAPI_GPIO_GPIO_ID(5) /* (Optional) */
#define EAPI_ID_GPIO_GPIO06			EAPI_GPIO_GPIO_ID(6) /* (Optional) */
#define EAPI_ID_GPIO_GPIO07			EAPI_GPIO_GPIO_ID(7) /* (Optional) */

/*
* Multiple GPIOs Per ID Mapping
*/
#define EAPI_GPIO_BANK_ID(GPIO_NUM)		EAPI_UINT32_C(0x10000|((GPIO_NUM)>>5))
#define EAPI_GPIO_BANK_MASK(GPIO_NUM)	EAPI_UINT32_C((1<<((GPIO_NUM)&0x1F))
#define EAPI_GPIO_BANK_TEST_STATE(GPIO_NUM, TState, TValue) \
                                                                (((TValue>>((GPIO_NUM)&0x1F))&1)==(TState))
#define EAPI_ID_GPIO_BANK00 EAPI_GPIO_BANK_ID( 0) /* GPIOs 0 - 31 (optional) */
#define EAPI_ID_GPIO_BANK01 EAPI_GPIO_BANK_ID(32) /* GPIOs 32 - 63 (optional) */
#define EAPI_ID_GPIO_BANK02 EAPI_GPIO_BANK_ID(64) /* GPIOs 64 - 95 (optional) */

/* Bit mask Bit States */
#define EAPI_GPIO_BITMASK_SELECT	1
#define EAPI_GPIO_BITMASK_NOSELECT	0
/* Levels */
#define EAPI_GPIO_LOW				0
#define EAPI_GPIO_HIGH				1
/* Directions */
#define EAPI_GPIO_INPUT				1
#define EAPI_GPIO_OUTPUT			0

/*
* EApiGPIOGetDirectionCaps
*
* Condition							 | Return Values
* -----------------------------------+------------------------------
* Library Uninitialized				 | EAPI_STATUS_NOT_INITIALIZED
* ((pOutputs==NULL)&&(pInputs==NULL))| EAPI_STATUS_INVALID_PARAMETER
* Unsupported Id					 | EAPI_STATUS_UNSUPPORTED
* Common Error						 | Common Error Code
* Else								 | EAPI_STATUS_SUCCESS
*/
/**
*	@brief	Not support this function
*/
EApiStatus_t
EAPI_CALLTYPE
EApiGPIOGetDirectionCaps(
        __IN EApiId_t Id, /* GPIO Id */
        __OUTOPT uint32_t *pInputs, /* Supported GPIO Input Bit Mask */
        __OUTOPT uint32_t *pOutputs /* Supported GPIO Output Bit Mask */
        );
/*
* EApiGPIOGetDirection
*
* Condition							 | Return Values
* -----------------------------------+------------------------------
* Library Uninitialized				 | EAPI_STATUS_NOT_INITIALIZED
* Bitmask==0						 | EAPI_STATUS_INVALID_PARAMETER
* Unsupported Id					 | EAPI_STATUS_UNSUPPORTED
* (Bitmask&~(pInputs|pOutputs))		 | EAPI_STATUS_INVALID_BITMASK
* Common Error						 | Common Error Code
* Else								 | EAPI_STATUS_SUCCESS
*/
/**
*	@param	Id:				GPIO Id: EAPI_GPIO_GPIO_ID(GPIO_NUM)
*	@param	Bitmask:		Bit mask of Affected Bits: 0xFFFFFFFF
*	@retval	*pDirection:	Current Direction
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: Bitmask==0
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported Id
*	@return EAPI_STATUS_INVALID_BITMASK: (Bitmask&~(pInputs|pOutputs))

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiGPIOGetDirection(
        __IN EApiId_t Id, /* GPIO Id */
        __IN uint32_t Bitmask, /* Bit mask of Affected Bits */
        __OUT uint32_t *pDirection /* Current Direction */
        );
/*
* EApiGPIOSetDirection
*
* Condition							 | Return Values
* -----------------------------------+------------------------------
* Library Uninitialized				 | EAPI_STATUS_NOT_INITIALIZED
* Bitmask==0						 | EAPI_STATUS_INVALID_PARAMETER
* Unsupported Id					 | EAPI_STATUS_UNSUPPORTED
* (Bitmask&~(pInputs|pOutputs))		 | EAPI_STATUS_INVALID_BITMASK
* (Bitmask&pDirection)&~pInputs		 | EAPI_STATUS_INVALID_DIRECTION
* (Bitmask&~pDirection)&~pOutputs	 | EAPI_STATUS_INVALID_DIRECTION
* Common Error						 | Common Error Code
* Else								 | EAPI_STATUS_SUCCESS
*/
/**
*	@param	Id:				GPIO Id: EAPI_GPIO_GPIO_ID(GPIO_NUM)
*	@param	Bitmask:		Bit mask of Affected Bits: 0xFFFFFFFF
*	@param	Direction:		Direction
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: Bitmask==0
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported Id
*	@return EAPI_STATUS_INVALID_BITMASK: (Bitmask&~(pInputs|pOutputs))
*	@return EAPI_STATUS_INVALID_DIRECTION: (Bitmask&pDirection)&~pInputs/(Bitmask&~pDirection)&~pOutputs

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiGPIOSetDirection(
        __IN EApiId_t Id, /* GPIO Id */
        __IN uint32_t Bitmask, /* Bit mask of Affected Bits */
        __IN uint32_t Direction /* Direction */
        );
/*
* EApiGPIOGetLevel
*
* Condition							 | Return Values
* -----------------------------------+------------------------------
* Library Uninitialized				 | EAPI_STATUS_NOT_INITIALIZED
* Bitmask==0						 | EAPI_STATUS_INVALID_PARAMETER
* Unsupported Id					 | EAPI_STATUS_UNSUPPORTED
* (Bitmask&~(pInputs|pOutputs))		 | EAPI_STATUS_INVALID_BITMASK
* Common Error						 | Common Error Code
* Else								 | EAPI_STATUS_SUCCESS
*/
/**
*	@param	Id:				GPIO Id: EAPI_GPIO_GPIO_ID(GPIO_NUM)
*	@param	Bitmask:		Bit mask of Affected Bits: 0xFFFFFFFF
*	@retval	*pLevel:		Current Level
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: Bitmask==0
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported Id
*	@return EAPI_STATUS_INVALID_BITMASK: (Bitmask&~(pInputs|pOutputs))

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiGPIOGetLevel(
        __IN EApiId_t Id, /* GPIO Id */
        __IN uint32_t Bitmask, /* Bit mask of Affected Bits */
        __OUT uint32_t *pLevel /* Current Level */
        );
/*
* EApiGPIOSetLevel
*
* Condition							 | Return Values
* -----------------------------------+------------------------------
* Library Uninitialized				 | EAPI_STATUS_NOT_INITIALIZED
* Bitmask==0						 | EAPI_STATUS_INVALID_PARAMETER
* Unsupported Id					 | EAPI_STATUS_UNSUPPORTED
* Common Error						 | Common Error Code
* Else								 | EAPI_STATUS_SUCCESS
*/
/**
*	@param	Id:				GPIO Id: EAPI_GPIO_GPIO_ID(GPIO_NUM)
*	@param	Bitmask:		Bit mask of Affected Bits: 0xFFFFFFFF
*	@param	Level:			Level
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: Bitmask==0
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported Id

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiGPIOSetLevel(
        __IN EApiId_t Id, /* GPIO Id */
        __IN uint32_t Bitmask, /* Bit mask of Affected Bits */
        __IN uint32_t Level /* Level */
        );

/*
*
* AAEON Customize Function
* Smart Fan
*
*/
#define EAPI_SFAN_SEL_ID(SFAN_NUM)	EAPI_UINT32_C(SFAN_NUM)
#define EAPI_ID_SFAN00			EAPI_SFAN_SEL_ID(0) /* (Optional) */
#define EAPI_ID_SFAN01			EAPI_SFAN_SEL_ID(1) /* (Optional) */
#define EAPI_ID_SFAN02			EAPI_SFAN_SEL_ID(2) /* (Optional) */

/*
* EApiSfanGetStatus
*
* Condition							 | Return Values
* -----------------------------------+------------------------------
* Library Uninitialized				 | EAPI_STATUS_NOT_INITIALIZED
* Unsupported Id					 | EAPI_STATUS_UNSUPPORTED
* Common Error						 | Common Error Code
* Else								 | EAPI_STATUS_SUCCESS
*/
/**
*	@param	Id:					Sfan Id: EAPI_ID_SFAN00
*	@param	Id:					Sfan Id: EAPI_ID_SFAN01
*	@param	Id:					Sfan Id: EAPI_ID_SFAN02
*	@retval	*pFanAutoMode:		Mode for Fan 1:AutoMode; 0: ManualMode
*	@retval	*pFullSpeedTemp:	Fan speed is full speed when exceeds the setting temperature 0-100
*	@retval	*pLowSpeedTemp:		Fan speed is low speed when less than the setting temperature 0-100
*	@retval	*pManualSpeed:		ManualSpeed 0-255
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported Id

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiSfanGetStatus(
        __IN EApiId_t Id, /* Sfan Id */
        __OUT uint32_t *pFanAutoMode,
        __OUT uint32_t *pFullSpeedTemp,
        __OUT uint32_t *pLowSpeedTemp,
        __OUT uint32_t *pManualSpeed
        );
/*
* EApiSfanSetStatus
*
* Condition							 | Return Values
* -----------------------------------+------------------------------
* Library Uninitialized				 | EAPI_STATUS_NOT_INITIALIZED
* Unsupported Id					 | EAPI_STATUS_UNSUPPORTED
* Common Error						 | Common Error Code
* Else								 | EAPI_STATUS_SUCCESS
*/
/**
*	@param	Id:					Sfan Id: EAPI_ID_SFAN00
*	@param	Id:					Sfan Id: EAPI_ID_SFAN01
*	@param	Id:					Sfan Id: EAPI_ID_SFAN02
*	@param	FanAutoMode:		Mode for Fan 1:AutoMode; 0: ManualMode
*	@param	FullSpeedTemp:		Fan speed is full speed when exceeds the setting temperature 0-100
*	@param	LowSpeedTemp:		Fan speed is low speed when less than the setting temperature 0-100
*	@param	ManualSpeed:		ManualSpeed 0-255
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported Id

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiSfanSetStatus(
        __IN EApiId_t Id, /* Sfan Id */
        __IN uint32_t FanAutoMode,
        __IN uint32_t FullSpeedTemp,
        __IN uint32_t LowSpeedTemp,
        __IN uint32_t ManualSpeed
        );

/**
*	@param	Id:				GPIO Id: EAPI_GPIO_GPIO_ID(GPIO_NUM)
*	@retval	*PinCount:		Pin Count
*	@retval	*pDioDisable:	Pin Enable or Disable 1:Disable; 0:Enable
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: Bitmask==0
*	@return EAPI_STATUS_UNSUPPORTED: Unsupported Id
*	@return EAPI_STATUS_INVALID_BITMASK: (Bitmask&~(pInputs|pOutputs))

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiGPIOGetCaps(
        __IN EApiId_t Id, /* GPIO Id */
        __OUTOPT uint32_t *PinCount, /* PinCount */
        __OUTOPT uint32_t *pDioDisable /* Pin Enable or Disable: 1:Disable; 0:Enable */
        );

/**
*	@param	Id:					Sfan Id: EAPI_ID_SFAN00
*	@param	Id:					Sfan Id: EAPI_ID_SFAN01
*	@param	Id:					Sfan Id: EAPI_ID_SFAN02
*	@retval	*pAutoModeCap:		Auto mode enable 1:Enable; 0:Disable
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiSfanGetCaps(
        __IN EApiId_t Id, /* Sfan Id */
        __OUT uint32_t *pAutoModeCap
        );

/**
*	@param  Id:			Value Id: EAPI_ID_HWMON_FAN_CPU
*	@param  Id:			Value Id: EAPI_ID_HWMON_FAN_CHIPSET
*	@param  Id:			Value Id: EAPI_ID_HWMON_FAN_SYSTEM

*	@param  Id:			Value Id: EAPI_ID_HWMON_CPU_TEMP
*	@param  Id:			Value Id: EAPI_ID_HWMON_CHIPSET_TEMP
*	@param  Id:			Value Id: EAPI_ID_HWMON_SYSTEM_TEMP

*	@param  Id:			Value Id: EAPI_ID_HWMON_VOLTAGE_VCORE
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_2V5
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_3V3
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_VBAT
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_5V
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_5VSB
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_12V
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_DIMM
*	@param  Id:			Value Id:EAPI_ID_HWMON_VOLTAGE_3VSB

*	@param  Id:			Value Id: EAPI_ID_BOARD_DRIVER_VERSION_VAL
*	@param  Id:			Value Id: EAPI_ID_AONCUS_HISAFE_FUCTION
*	@retval	*pFanEnable:	FanEnable 1:Enable; 0:Disable
*	@retval	*pTempEnable:	TempEnable 1:Enable; 0:Disable
*	@retval	*pVoltEnable:	VoltEnable 1:Enable; 0:Disable
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_UNSUPPORTED: unknown Id

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiHWMONGetCaps(
        __IN EApiId_t Id, /* HWMON Id */
        __OUT uint32_t *pFanEnable,
        __OUT uint32_t *pTempEnable,
        __OUT uint32_t *pVoltEnable
        );

/**
*	@retval	*pwdtMinute:		Get the mode of minute or second
*	@retval	*pwdtCountTime:		Get WDT time count
*	@retval	*pwdtReloadTime:	Get WDT ReloadTime
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiWDogGetStatus(
        __OUTOPT uint32_t *pwdtMinute,
        __OUTOPT uint32_t *pwdtCountTime,
        __OUTOPT uint32_t *pwdtReloadTime
        );

/**
*	@param	wdtMinute:		Set the mode of minute or second
*	@param	wdtCountTime:	Set WDT time count
*	@param	wdtReloadTime:	Set WDT ReloadTime
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiWDogSetStatus(
        __IN uint32_t wdtMinute,
        __IN uint32_t wdtCountTime,
        __IN uint32_t wdtReloadTime
        );

/**
*	@retval *pBLCDevCount:	Get the count of backlight control
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiBKLIGHTGetCaps(
        __OUT uint32_t *pBLCDevCount
        );

/*
*
* P W M
*
*/
/* IDS */
#define EAPI_ID_PWM_1 EAPI_UINT32_C(0)
#define EAPI_ID_PWM_2 EAPI_UINT32_C(1)
#define EAPI_ID_PWM_3 EAPI_UINT32_C(2)

/*
* EApiPwmGetValue
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* pPWMBaseUnitInt==NULL ||				 |
* pPWMBaseUnitFrac==NULL ||				 |
* pPWMDutyCycle==NULL					 | EAPI_STATUS_INVALID_PARAMETER
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@param  Id:		PWM Id: EAPI_ID_PWM_1
*	@param  Id:		PWM Id: EAPI_ID_PWM_2
*	@param  Id:		PWM Id: EAPI_ID_PWM_3
*	@retval *pPWMBaseUnitInt:	Get the integer portion of Base Unit
*	@retval *pPWMBaseUnitFrac:	Get the fractional portion of Base Unit
*	@retval *pPWMDutyCycle:	Get the duty cycle
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: pPWMBaseUnitInt==NULL or pPWMBaseUnitFrac==NULL or pPWMDutyCycle==NULL
*	@return EAPI_STATUS_UNSUPPORTED: unknown Id

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiPwmGetValue(
        __IN EApiId_t Id, /* PWM Id */
        __OUT uint32_t *pPWMBaseUnitInt, /* PWM Base Unit Integer */
        __OUT uint32_t *pPWMBaseUnitFrac, /* PWM Base Unit Integer */
        __OUT uint32_t *pPWMDutyCycle /* PWM Base Unit Integer */
);
/*
* EApiPwmSetValue
*
* Condition								 | Return Values
* ---------------------------------------+------------------------------
* Library Uninitialized					 | EAPI_STATUS_NOT_INITIALIZED
* unknown Id							 | EAPI_STATUS_UNSUPPORTED
* Common Error							 | Common Error Code
* Else									 | EAPI_STATUS_SUCCESS
*/
/**
*	@param  Id:		PWM Id: EAPI_ID_PWM_1
*	@param  Id:		PWM Id: EAPI_ID_PWM_2
*	@param  Id:		PWM Id: EAPI_ID_PWM_3
*	@retval PWMBaseUnitInt:	Set the integer portion of Base Unit
*	@retval PWMBaseUnitFrac: Set the fractional portion of Base Unit
*	@retval PWMDutyCycle: Set the duty cycle
*	@return EAPI_STATUS_NOT_INITIALIZED: Library Uninitialized
*	@return EAPI_STATUS_INVALID_PARAMETER: Bright>EAPI_BACKLIGHT_SET_BRIGHTEST
*	@return EAPI_STATUS_UNSUPPORTED: unknown Id

*	@return EAPI_STATUS_SUCCESS: Else
*/
EApiStatus_t
EAPI_CALLTYPE
EApiPwmSetValue(
        __IN EApiId_t Id, /* PWM Id */
        __IN uint32_t PWMBaseUnitInt, /* PWM Base Unit Integer */
        __IN uint32_t PWMBaseUnitFrac, /* PWM Base Unit Integer */
        __IN uint32_t PWMDutyCycle /* PWM Base Unit Integer */
);


/*
*
* S P I
*
*/
/* IDS */
#define EAPI_ID_SPI_1 EAPI_UINT32_C(0)
#define EAPI_ID_SPI_2 EAPI_UINT32_C(1)
#define EAPI_ID_SPI_3 EAPI_UINT32_C(2)

EApiStatus_t
EAPI_CALLTYPE
EApiSpiGetConfig(
        __IN EApiId_t Id, /* SPI Id */
        __OUT uint32_t *pSSPMode, /* SSP Frame Format */
        __OUT uint32_t *pSPIFrequency, /* SPI clock frequency */
        __OUT uint32_t *pSPIPolarity /* SPI clock polarity */
);

EApiStatus_t
EAPI_CALLTYPE
EApiSpiSetConfig(
        __IN EApiId_t Id, /* SPI Id */
        __IN uint32_t SSPMode, /* SSP Frame Format */
        __IN uint32_t SPIFrequency, /* SPI clock frequency */
        __IN uint32_t SPIPolarity /* SPI clock polarity */
);

EApiStatus_t
EAPI_CALLTYPE
EApiSpiGetValue(
        __IN EApiId_t Id, /* PWM Id */
        __IN uint32_t ByteCnt, /* Byte Count to read */
        __OUT uint32_t *pBuffer /* Data pBuffer */
);

EApiStatus_t
EAPI_CALLTYPE
EApiSpiSetValue(
        __IN EApiId_t Id, /* PWM Id */
        __IN uint32_t ByteCnt, /* Byte Count to read */
        __IN uint32_t Buffer /* Data pBuffer */
);
/****************************************************************************/
}
