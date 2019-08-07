/*!
 * \file sample-static.c
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
 *                		  Change I2C_GetChannelInfo & OpenChannel to start indexing from 0
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

/* Include D2XX header*/
#include "ftd2xx.h"

/* Include libMPSSE header */
#include "libMPSSE_i2c.h"

/******************************************************************************/
/*								Macro and type defines							   */
/******************************************************************************/
/* Helper macros */

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
#define CHANNEL_TO_OPEN			0	/*0 for first available channel, 1 for next... */
#define DATA_OFFSET				1



/******************************************************************************/
/*								Global variables							  	    */
/******************************************************************************/
uint32 channels;
FT_HANDLE ftHandle, ftHandle0, ftHandle1,ftHandle2;
uint32 bytes_written;
ChannelConfig channelConf;
FT_STATUS status;
uint8 buffer[I2C_DEVICE_BUFFER_SIZE];




/******************************************************************************/
/*						Public function definitions						  		   */
/******************************************************************************/
/*!
 * \brief Writes to EEPROM
 *
 * This function writes a byte to a specified address within the 24LC024H EEPROM
 *
 * \param[in] slaveAddress Address of the I2C slave (EEPROM)
 * \param[in] registerAddress Address of the memory location inside the slave to where the byte
 *			is to be written
 * \param[in] data The byte that is to be written
 * \return Returns status code of type FT_STATUS(see D2XX Programmer's Guide)
 * \sa Datasheet of 24LC024H http://ww1.microchip.com/downloads/en/devicedoc/22102a.pdf
 * \note 
 * \warning
 */

FT_STATUS write_byte(uint8 slaveAddress, uint8 registerAddress, uint8 data)
{
	uint32 bytesToTransfer = 0;
	uint32 bytesTransfered;
	bool writeComplete=0;
	uint32 retry=0;
	
	bytesToTransfer=0;
	bytesTransfered=0;
	buffer[bytesToTransfer++]=registerAddress; /* Byte addressed inside EEPROM */
	buffer[bytesToTransfer++]=data;
	status = I2C_DeviceWrite(ftHandle, slaveAddress, bytesToTransfer, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES|I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT);


	/* poll to check completition */
	while((writeComplete==0) && (retry<I2C_WRITE_COMPLETION_RETRY))
	{
		bytesToTransfer=0;
		bytesTransfered=0;
		buffer[bytesToTransfer++]=registerAddress; /* Addressed inside EEPROM  */
		status = I2C_DeviceWrite(ftHandle, slaveAddress, bytesToTransfer,buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES|I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT|I2C_TRANSFER_OPTIONS_BREAK_ON_NACK);
		//		printf("bytesToTransfer %d, bytesTransferred %d\n",bytesToTransfer, bytesTransfered);
		if((FT_OK == status) && (bytesToTransfer == bytesTransfered))
		{
			writeComplete=1;
			printf("  ... Write done\n");
		}
		retry++;
	}
	return status;
}

/*!
 * \brief Reads from EEPROM
 *
 * This function reads a byte from a specified address within the 24LC024H EEPROM
 *
 * \param[in] slaveAddress Address of the I2C slave (EEPROM)
 * \param[in] registerAddress Address of the memory location inside the slave from where the 
 *			byte is to be read
 * \param[in] *data Address to where the byte is to be read
 * \return Returns status code of type FT_STATUS(see D2XX Programmer's Guide)
 * \sa Datasheet of 24LC024H http://ww1.microchip.com/downloads/en/devicedoc/22102a.pdf
 * \note 
 * \warning
 */
FT_STATUS read_byte(uint8 slaveAddress, uint8 registerAddress, uint8 *data)
{
	FT_STATUS status;
	uint32 bytesToTransfer = 0;
	uint32 bytesTransfered;
        int i;
        printf(" read address %02x\n",registerAddress);
        for(i=0;i<16;i++)buffer[i]=0xaa;

	bytesToTransfer=0;
	bytesTransfered=0;
	buffer[bytesToTransfer++]=registerAddress; /*Byte addressed inside EEPROM */
	status = I2C_DeviceWrite(ftHandle, slaveAddress, bytesToTransfer, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES|I2C_TRANSFER_OPTIONS_START_BIT);
	APP_CHECK_STATUS(status);
        usleep(5000000);
        printf(" bytesTransfered %d \n",bytesTransfered); 
        for(i=0;i<bytesTransfered;i++)printf("buffer  %d  %02x\n",i,buffer[i]);
	bytesToTransfer=2;
	bytesTransfered=0;
	status |= I2C_DeviceRead(ftHandle, slaveAddress, bytesToTransfer, buffer, &bytesTransfered,I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES | I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT); //|I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE);
	APP_CHECK_STATUS(status); //I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES
        printf(" bytesTransfered %d \n",bytesTransfered);
	*data = buffer[0];
	APP_CHECK_STATUS(status);
        printf(" bytesTransfered %d \n",bytesTransfered); 
        for(i=0;i<bytesTransfered;i++)printf("buffer  %d  %02x\n",i,buffer[i]);
	/*	bytesToTransfer=0;
	bytesTransfered=0;
	status |= I2C_DeviceRead(ftHandle, slaveAddress, bytesToTransfer, buffer, &bytesTransfered,I2C_TRANSFER_OPTIONS_NO_ADDRESS|I2C_TRANSFER_OPTIONS_STOP_BIT|I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE);
	APP_CHECK_STATUS(status);
        printf(" bytesTransfered %d \n",bytesTransfered);
	*data = buffer[0];
        for(i=0;i<bytesTransfered;i++)printf("buffer  %d  %02x\n",i,buffer[i]);
        for(i=0;i<bytesTransfered;i++)printf("buffer  %d  %02x\n",i,buffer[i]);
	*data = buffer[0]; */
	return status;
}

/*!
 * \brief Writes to EEPROM
 *
 * This function writes a byte to a specified address within the 24LC024H EEPROM
 *
 * \param[in] slaveAddress Address of the I2C slave (EEPROM)
 * \param[in] registerAddress Address of the memory location inside the slave to where the byte
 *			is to be written
 * \param[in] data The byte that is to be written
 * \return Returns status code of type FT_STATUS(see D2XX Programmer's Guide)
 * \sa Datasheet of 24LC024H http://ww1.microchip.com/downloads/en/devicedoc/22102a.pdf
 * \note 
 * \warning
 */

FT_STATUS write_IO_devs(uint16 pattern)
{
	FT_STATUS status;
	uint8 dev_address;
	uint8 data;
	uint8 bit9;
        uint32 wrt_options;
	uint32 nbytes = 0;
	uint32 bTrans;
	int i;

	data = pattern & 0xFF;
	bit9 = (pattern>>8) & 0x01;
	wrt_options = I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES|I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT;
	for(i=1;i<5;i++){
	  dev_address = 0x50|i;
	  printf("writing to device %02x\n",dev_address);
	  nbytes=0;
	  bTrans=0;
	  buffer[nbytes++]=0xF4; //Configuration register
	  buffer[nbytes++]=0x01; //disable writing to flash -- shadow memory only
	  status = I2C_DeviceWrite(ftHandle0, dev_address, nbytes, buffer, &bTrans, wrt_options);
	  printf("enable pullups and write data\n");
	  nbytes=0;
	  bTrans=0;
	  buffer[nbytes++]=0xF0; //Pullup enable 0
	  buffer[nbytes++]=0xFF; //enable pullups on I/O[7..0]
	  //		  buffer[nbytes++]=0xF1; //Pullup enable 1
	  buffer[nbytes++]=0x01; //enable pullups on I/O[8]
	  //		  buffer[nbytes++]=0xF2; //I/O Control 0
	  buffer[nbytes++]=data; //Data to I/O[7..0]
	  //		  buffer[nbytes++]=0xF3; //I/O Control 1
	  buffer[nbytes++]=bit9; //Data to I/O[8]
	  status = I2C_DeviceWrite(ftHandle0, dev_address, nbytes, buffer, &bTrans, wrt_options);
	  if(status != FT_OK)printf("error: status = %d\n",status);
	}
	return status;
}

/*!
 * \brief Main function / Entry point to the sample application
 *
 * This function is the entry point to the sample application. It opens the channel, writes to the
 * EEPROM and reads back.
 *
 * \param[in] none
 * \return Returns 0 for success
 * \sa
 * \note 
 * \warning
 */
int main()
{
	FT_STATUS status;
	FT_DEVICE_LIST_INFO_NODE devList;
	uint8 dev_address;
	uint8 data,dev1io,dev2io,dev3io,dev4io,bit8s,tmp;
        uint32 wrt_options,rd_options;
	uint32 nbytes = 0;
	uint32 bTrans;
	int i,j;

#ifdef _MSC_VER
	Init_libMPSSE();
#endif
	channelConf.ClockRate = I2C_CLOCK_FAST_MODE;/*i.e. 400000 KHz*/
	channelConf.LatencyTimer= 255;

	//channelConf.Options = I2C_DISABLE_3PHASE_CLOCKING;
	//channelConf.Options = I2C_ENABLE_DRIVE_ONLY_ZERO;

	status = I2C_GetNumChannels(&channels);
	APP_CHECK_STATUS(status);
	printf("Number of available I2C channels = %d\n",channels);
	
	if(channels>0)
	{

		/* Open the first available channel */
		status = I2C_OpenChannel(0,&ftHandle0);
		APP_CHECK_STATUS(status);
		printf("\nhandle0=0x%x status=%d\n",ftHandle0,status);
		/* Open the next available channel */
		status = I2C_OpenChannel(1,&ftHandle1);
		APP_CHECK_STATUS(status);
		printf("\nhandle1=0x%x status=%d\n",ftHandle1,status);
		/* Open the next available channel */
		//status = I2C_OpenChannel(2,&ftHandle1);
		//APP_CHECK_STATUS(status);

		for(i=0;i<channels;i++)
		{
			status = I2C_GetChannelInfo(i,&devList);
			APP_CHECK_STATUS(status);
			printf("Information on channel number %d:\n",i);
			/*print the dev info*/
			printf("		Flags=0x%x\n",devList.Flags); 
			printf("		Type=0x%x\n",devList.Type); 
			printf("		ID=0x%x\n",devList.ID); 
			printf("		LocId=0x%x\n",devList.LocId); 
			printf("		SerialNumber=%s\n",devList.SerialNumber); 
			printf("		Description=%s\n",devList.Description); 
			printf("		ftHandle=0x%x\n",devList.ftHandle);/*is 0 unless open*/
		}

		//printf("\nhandle1=0x%x status=%d\n",ftHandle1,status);
		status = I2C_InitChannel(ftHandle0,&channelConf);
		APP_CHECK_STATUS(status);	
		status = I2C_InitChannel(ftHandle1,&channelConf);
		APP_CHECK_STATUS(status);
                //system("insmod ftdi_sio");
                //usleep(10);
		//status = I2C_InitChannel(ftHandle2,&channelConf);
		//APP_CHECK_STATUS(status);

                char data[20];

                ftHandle=ftHandle0;
		/*
		dev_address = 0x51;
		printf("writing to device %02x\n",dev_address);
		write_byte(dev_address,0x00,0x34); //disable writing to flash -- shadow memory only
		*/
		/*
                for(i=1;i<5;i++){
		  dev_address = 0x50|i;
                  printf("writing to device %02x\n",dev_address);
		  write_byte(dev_address,0xf4,0x01); //disable writing to flash -- shadow memory only
                  printf("enable pullups\n");
		  write_byte(dev_address,0xF0,0xFF); //enable pullups on I/O[7..0]
		  write_byte(dev_address,0xF1,0x01); //enable pullups on I/O[8]
                  printf("Set data on I/Os\n");
		  write_byte(dev_address,0xF2,0x45+i); //data to I/O[7..0]
		  write_byte(dev_address,0xF3,0x01); //data to I/O[8]
		}
		*/
		/*
		wrt_options = I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES|I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT;
                for(i=1;i<5;i++){
		  dev_address = 0x50|i;
                  printf("writing to device %02x\n",dev_address);
		  nbytes=0;
		  bTrans=0;
		  buffer[nbytes++]=0xF4; //Configuration register
		  buffer[nbytes++]=0x01; //disable writing to flash -- shadow memory only
		  status = I2C_DeviceWrite(ftHandle0, dev_address, nbytes, buffer, &bTrans, wrt_options);
                  printf("enable pullups\n");
		  nbytes=0;
		  bTrans=0;
		  buffer[nbytes++]=0xF0; //Pullup enable 0
		  buffer[nbytes++]=0xFF; //enable pullups on I/O[7..0]
		  status = I2C_DeviceWrite(ftHandle0, dev_address, nbytes, buffer, &bTrans, wrt_options);
		  nbytes=0;
		  bTrans=0;
		  buffer[nbytes++]=0xF1; //Pullup enable 1
		  buffer[nbytes++]=0x01; //enable pullups on I/O[8]
		  status = I2C_DeviceWrite(ftHandle0, dev_address, nbytes, buffer, &bTrans, wrt_options);
                  printf("Set data on I/Os\n");
		  nbytes=0;
		  bTrans=0;
		  buffer[nbytes++]=0xF2; //I/O Control 0
		  buffer[nbytes++]=16*i+i; //Data to I/O[7..0]
		  status = I2C_DeviceWrite(ftHandle0, dev_address, nbytes, buffer, &bTrans, wrt_options);
		  nbytes=0;
		  bTrans=0;
		  buffer[nbytes++]=0xF3; //I/O Control 1
		  buffer[nbytes++]=i; //Data to I/O[8]
		  status = I2C_DeviceWrite(ftHandle0, dev_address, nbytes, buffer, &bTrans, wrt_options);
		}
		*/
		wrt_options = I2C_TRANSFER_OPTIONS_FAST_TRANSFER_BYTES|I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT;
                for(i=1;i<5;i++){
		  dev_address = 0x50|i;
                  printf("writing to device %02x\n",dev_address);
		  nbytes=0;
		  bTrans=0;
		  buffer[nbytes++]=0xF4; //Configuration register
		  buffer[nbytes++]=0x01; //disable writing to flash -- shadow memory only
		  status = I2C_DeviceWrite(ftHandle0, dev_address, nbytes, buffer, &bTrans, wrt_options);
                  printf("enable pullups and write data\n");
		  nbytes=0;
		  bTrans=0;
		  buffer[nbytes++]=0xF0; //Pullup enable 0
		  buffer[nbytes++]=0xFF; //enable pullups on I/O[7..0]
		  //		  buffer[nbytes++]=0xF1; //Pullup enable 1
		  buffer[nbytes++]=0x01; //enable pullups on I/O[8]
		  //		  buffer[nbytes++]=0xF2; //I/O Control 0
		  buffer[nbytes++]=16*(2+i)+6+i; //Data to I/O[7..0]
		  //		  buffer[nbytes++]=0xF3; //I/O Control 1
		  buffer[nbytes++]=i+1; //Data to I/O[8]
		  status = I2C_DeviceWrite(ftHandle0, dev_address, nbytes, buffer, &bTrans, wrt_options);
		  if(status != FT_OK)printf("error: status = %d\n",status);
		}


		//   char data[2];
		/*
                ftHandle=ftHandle0;
                read_byte(0x51,0x00,data);
                printf(" data %02x \n",data[0]);
		*/


		printf("write output enable and select dev 1 and 2\n");
		status = FT_WriteLowByteGPIO(ftHandle0,0xFB,0x00); //Port A Low Byte  Write output enable and select dev 1 & 2
		if(status != FT_OK)printf("error: status = %d\n",status);
		status = FT_WriteGPIO(ftHandle0,0x00,0x00);        //Port A High Byte Set directions to inputs
		if(status != FT_OK)printf("error: status = %d\n",status);
		printf("set directions of GPIO to inputs\n");
		status = FT_WriteLowByteGPIO(ftHandle1,0x00,0x00); //Port B Low Byte  Set directions to inputs
		if(status != FT_OK)printf("error: status = %d\n",status);
		status = FT_WriteGPIO(ftHandle1,0x00,0x00);        //Port B High Byte Set directions to inputs
		if(status != FT_OK)printf("error: status = %d\n",status);

		printf("read dev 1 and 2\n");
		status = FT_ReadGPIO(ftHandle0,&dev1io);        //Read Port A High Byte
		if(status != FT_OK)printf("error: status = %d\n",status);
		status = FT_ReadGPIO(ftHandle1,&dev2io);        //Read Port B High Byte
		if(status != FT_OK)printf("error: status = %d\n",status);
		status = FT_ReadLowByteGPIO(ftHandle1,&bit8s); //Read Port B Low Byte  
		if(status != FT_OK)printf("error: status = %d\n",status);

		printf("write output enable and select dev 3 and 4\n");
		status = FT_WriteLowByteGPIO(ftHandle0,0xFB,0x70); //Port A Low Byte  Write output enable and select dev 3 & 4
		if(status != FT_OK)printf("error: status = %d\n",status);

		printf("read dev 3 and 4\n");
		status = FT_ReadGPIO(ftHandle0,&dev3io);        //Read Port A High Byte
		if(status != FT_OK)printf("error: status = %d\n",status);
		status = FT_ReadGPIO(ftHandle1,&dev4io);        //Read Port B High Byte
		if(status != FT_OK)printf("error: status = %d\n",status);
		status = FT_ReadLowByteGPIO(ftHandle1,&bit8s); //Read Port B Low Byte  
		if(status != FT_OK)printf("error: status = %d\n",status);

		printf("tmp %02x\n",tmp&0xff);
		printf("readback(4321) %01x%02x %01x%02x %01x%02x %01x%02x\n",(bit8s>>7)&1,dev4io&0xff,(bit8s>>6)&1,dev3io&0xff,(bit8s>>5)&1,dev2io&0xff,(bit8s>>4)&1,dev1io&0xff);

		status = I2C_CloseChannel(ftHandle0);
		status = I2C_CloseChannel(ftHandle1);
	}

	
#ifdef _MSC_VER
	Cleanup_libMPSSE();
#endif

	return 0;
}
