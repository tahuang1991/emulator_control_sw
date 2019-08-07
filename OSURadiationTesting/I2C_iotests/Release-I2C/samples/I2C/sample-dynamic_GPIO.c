/*!
 * \file sample-dynamic.c
 *
 * \author FTDI
 * \date 20110512
 *
 * Copyright © 2011 Future Technology Devices International Limited
 * Company Confidential
 *
 * Project: libMPSSE
 * Module: I2C Sample Application - Interfacing 24LC024H I2C EEPROM
 *
 * Rivision History:
 * 0.1 - 20110513 - initial version
 * 0.2 - 20110801 - Changed LatencyTimer to 255
 * 				  Attempt to open channel only if available
 *				  Added & modified macros
 *                		  Change APIs I2C_GetChannelInfo & OpenChannel to start indexing from 0
 * 0.3 - 20111212 - Added comments
 */

/******************************************************************************/
/* 							 Include files										   */
/******************************************************************************/
/* Standard C libraries */
#include<stdio.h>
#include<stdlib.h>

/* OS specific libraries */
#ifdef _WIN32
#include<windows.h>
#endif

#ifdef __linux
#include<dlfcn.h>
#endif

/* Include D2XX header*/
#include "ftd2xx.h"

/* Include libMPSSE header */
#include "libMPSSE_i2c.h"

/******************************************************************************/
/*								Macro and type defines							   */
/******************************************************************************/
/* Helper macros */
#ifdef _WIN32
	#define GET_FUN_POINTER	GetProcAddress
	#define CHECK_ERROR(exp) {if(exp==NULL){printf("%s:%d:%s(): NULL expression\
		encountered \n",__FILE__, __LINE__, __FUNCTION__);exit(1);}else{;}};
#endif

#ifdef __linux
	#define GET_FUN_POINTER	dlsym
	#define CHECK_ERROR(exp) {if(dlerror() != NULL){printf("line %d: ERROR \
		dlsym\n",__LINE__);}}
#endif
#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \
	!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);exit(1);}else{;}};
#define CHECK_NULL(exp){if(exp==NULL){printf("%s:%d:%s():  NULL expression \
	encountered \n",__FILE__, __LINE__, __FUNCTION__);exit(1);}else{;}};

/* Application specific macro definations */
#define I2C_DEVICE_ADDRESS_EEPROM		0x57
#define I2C_DEVICE_BUFFER_SIZE		256
#define I2C_WRITE_COMPLETION_RETRY		10
#define START_ADDRESS_EEPROM 	0x00 /*read/write start address inside the EEPROM*/
#define END_ADDRESS_EEPROM		0x10
#define RETRY_COUNT_EEPROM		10	/* number of retries if read/write fails */
#define CHANNEL_TO_OPEN			1	/*0 for first available channel, 1 for next... */
#define DATA_OFFSET				2


/* Declaration of function pointers */
typedef FT_STATUS (*pfunc_I2C_GetNumChannels)(uint32 *numChannels);
pfunc_I2C_GetNumChannels p_I2C_GetNumChannels;
typedef FT_STATUS (*pfunc_I2C_GetChannelInfo)(uint32 index, FT_DEVICE_LIST_INFO_NODE *chanInfo);
pfunc_I2C_GetChannelInfo p_I2C_GetChannelInfo;
typedef FT_STATUS (*pfunc_I2C_OpenChannel)(uint32 index, FT_HANDLE *handle);
pfunc_I2C_OpenChannel p_I2C_OpenChannel;
typedef FT_STATUS (*pfunc_I2C_CloseChannel)(FT_HANDLE handle);
pfunc_I2C_CloseChannel p_I2C_CloseChannel;
typedef FT_STATUS (*pfunc_I2C_InitChannel)(FT_HANDLE handle, ChannelConfig *config);
pfunc_I2C_InitChannel p_I2C_InitChannel;
typedef FT_STATUS (*pfunc_I2C_DeviceRead)(FT_HANDLE handle,uint32 deviceAddress,uint32 sizeToTransfer, uint8 *buffer, uint32 *sizeTransfered, uint32 options);
pfunc_I2C_DeviceRead p_I2C_DeviceRead;
typedef FT_STATUS (*pfunc_I2C_DeviceWrite)(FT_HANDLE handle, uint32 deviceAddress,uint32 sizeToTransfer, uint8 *buffer, uint32 *sizeTransfered, uint32 options);
pfunc_I2C_DeviceWrite p_I2C_DeviceWrite;

typedef FT_STATUS (*pfunc_FT_WriteGPIO)(FT_HANDLE handle, uint8 dir, uint8 value);
pfunc_FT_WriteGPIO p_FT_WriteGPIO;
typedef FT_STATUS (*pfunc_FT_ReadGPIO)(FT_HANDLE handle, uint8 *value);
pfunc_FT_ReadGPIO p_FT_ReadGPIO;

/******************************************************************************/
/*								Global variables							  	    */
/******************************************************************************/
uint32 channels;
uint32 bytes_written;
FT_HANDLE ftHandle, ftHandle0, ftHandle1;
ChannelConfig channelConf;
FT_STATUS status;
uint8 buffer[I2C_DEVICE_BUFFER_SIZE];

/******************************************************************************/
/*						Public function definitions						  		   */
/******************************************************************************/
/*!
 */

/*!
 * \brief Main function / Entry point of the sample application
 *
 * This function is the entry point to the sample application. It opens the channel, writes to the
 * GPIO bits.
 *
 */
int main()
{
#ifdef _WIN32
	#ifdef _MSC_VER
		HMODULE h_libMPSSE;
	#else
		HANDLE h_libMPSSE;
	#endif
#endif

#ifdef __linux
	void *h_libMPSSE;
#endif

	FT_STATUS status;
	FT_DEVICE_LIST_INFO_NODE devList;
	uint8 address;
	uint8 data;
	int i,j;

	channelConf.ClockRate = I2C_CLOCK_FAST_MODE;/*i.e. 400000 KHz*/
	channelConf.LatencyTimer= 255;
	//channelConf.Options = I2C_DISABLE_3PHASE_CLOCKING;
	//channelConf.Options = I2C_ENABLE_DRIVE_ONLY_ZERO;

	/* load library */
#ifdef _WIN32
	#ifdef _MSC_VER
		h_libMPSSE = LoadLibrary(L"libMPSSE.dll");
	#else
		h_libMPSSE = LoadLibrary("libMPSSE.dll");
	#endif
	if(NULL == h_libMPSSE)
	{
		printf("Failed loading libMPSSE.dll. Please check if the file exists in\
 the working directory\n");
	}
#endif
	
#ifdef __linux
	h_libMPSSE = dlopen("libMPSSE.so",RTLD_LAZY);
	if(!h_libMPSSE)
	{
		printf("Failed loading libMPSSE.so\n");
	}
#endif

	/* init function pointers */
	p_I2C_GetNumChannels = (pfunc_I2C_GetNumChannels)GET_FUN_POINTER(h_libMPSSE, "I2C_GetNumChannels");
	CHECK_ERROR (p_I2C_GetNumChannels);
	p_I2C_GetChannelInfo = (pfunc_I2C_GetChannelInfo)GET_FUN_POINTER(h_libMPSSE, "I2C_GetChannelInfo");
	CHECK_ERROR(p_I2C_GetChannelInfo);
	p_I2C_OpenChannel = (pfunc_I2C_OpenChannel)GET_FUN_POINTER(h_libMPSSE, "I2C_OpenChannel");
	CHECK_ERROR(p_I2C_OpenChannel);
	p_I2C_CloseChannel = (pfunc_I2C_CloseChannel)GET_FUN_POINTER(h_libMPSSE,"I2C_CloseChannel");
	CHECK_ERROR(p_I2C_CloseChannel);
	p_I2C_InitChannel = (pfunc_I2C_InitChannel)GET_FUN_POINTER(h_libMPSSE, "I2C_InitChannel");
	CHECK_ERROR(p_I2C_InitChannel);
	p_I2C_DeviceRead = (pfunc_I2C_DeviceRead)GET_FUN_POINTER(h_libMPSSE, "I2C_DeviceRead");
	CHECK_ERROR(p_I2C_DeviceRead);
	p_I2C_DeviceWrite = (pfunc_I2C_DeviceWrite)GET_FUN_POINTER(h_libMPSSE, "I2C_DeviceWrite");
	CHECK_ERROR(p_I2C_DeviceWrite);

	p_FT_WriteGPIO = (pfunc_FT_WriteGPIO)GET_FUN_POINTER(h_libMPSSE, "FT_WriteGPIO");
	CHECK_ERROR(p_FT_WriteGPIO);
	p_FT_ReadGPIO = (pfunc_FT_ReadGPIO)GET_FUN_POINTER(h_libMPSSE, "FT_ReadGPIO");
	CHECK_ERROR(p_FT_ReadGPIO);

	status = p_I2C_GetNumChannels(&channels);
	APP_CHECK_STATUS(status);
	printf("Number of available I2C channels = %d\n",channels);
	if(channels>0)
	{
		for(i=0;i<channels;i++)
		{
			status = p_I2C_GetChannelInfo(i,&devList);
			APP_CHECK_STATUS(status);
			printf("Information on channel number %d:\n",i);
			/*print the dev info*/
			printf("		Flags=0x%x\n",devList.Flags); 
			printf("		Type=0x%x\n",devList.Type); 
			printf("		ID=0x%x\n",devList.ID); 
			printf("		LocId=0x%x\n",devList.LocId); 
			printf("		SerialNumber=%s\n",devList.SerialNumber); 
			printf("		Description=%s\n",devList.Description); 
			printf("		ftHandle=0x%x\n",devList.ftHandle);/* 0 if not open*/
		}

		/* Open the first available channel */
		status = p_I2C_OpenChannel(0,&ftHandle0);
		APP_CHECK_STATUS(status);
		printf("\nhandle0=0x%x status=%d\n",ftHandle0,status);
		/* Open the next available channel */
		status = p_I2C_OpenChannel(1,&ftHandle1);
		APP_CHECK_STATUS(status);
		printf("\nhandle1=0x%x status=%d\n",ftHandle1,status);
		status = p_I2C_InitChannel(ftHandle0,&channelConf);
		APP_CHECK_STATUS(status);
		status = p_I2C_InitChannel(ftHandle1,&channelConf);
		APP_CHECK_STATUS(status);

		for(i=0;i<channels;i++)
		{
			status = p_I2C_GetChannelInfo(i,&devList);
			APP_CHECK_STATUS(status);
			printf("Information on channel number %d:\n",i);
			/*print the dev info*/
			printf("		Flags=0x%x\n",devList.Flags); 
			printf("		Type=0x%x\n",devList.Type); 
			printf("		ID=0x%x\n",devList.ID); 
			printf("		LocId=0x%x\n",devList.LocId); 
			printf("		SerialNumber=%s\n",devList.SerialNumber); 
			printf("		Description=%s\n",devList.Description); 
			printf("		ftHandle=0x%x\n",devList.ftHandle);/* 0 if not open*/
		}
//		for(i=0;i<1000000;i++){
			status = p_FT_WriteGPIO(ftHandle0,0xFF,0xAA);
			status = p_FT_WriteGPIO(ftHandle1,0xFF,0x99);
//			status = p_FT_WriteGPIO(ftHandle0,0xFF,0x55);
//			status = p_FT_WriteGPIO(ftHandle1,0xFF,0x66);
//		}
		buffer[0] = 0x80; //MPSSE_CMD_SET_DATA_BITS_LOWBYTE;
		buffer[1] = 0xC3;
		buffer[2] = 0xFF;
		status = FT_Write(ftHandle0,buffer,3,&bytes_written);
		printf("status %d;  bytes written %d\n",status,bytes_written);
		status = FT_Write(ftHandle1,buffer,3,&bytes_written);
		buffer[1] = 0x23;
		printf("status %d;  bytes written %d\n",status,bytes_written);

		status = p_I2C_CloseChannel(ftHandle0);
		status = p_I2C_CloseChannel(ftHandle1);
	}
	return 0;
}
