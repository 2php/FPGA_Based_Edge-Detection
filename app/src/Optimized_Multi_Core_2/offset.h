#ifndef __OFFSET_H_
#define __OFFSET_H_
#include "system.h"

#define DEBUG 0
#define TRUE 1
#define MaxWait 30

#if DEBUG == 1
#define SIZE 11
#else
#define SIZE 3
#endif


extern void delay (int millisec);
unsigned char* temp = (unsigned char*)SHARED_ONCHIP_BASE;

//Sizes of pictures coberted in to values 0-4 for easier calucaltions, using formula (actualSize-22)>>1.
int size[10]={		1,
			0,0,
			3,0,
			2,0,0,0,
			4};

// Number of rows each cpu should work on based on 5 unique picture sizes. row[cpu][size].
int rowGI[5][5]={	0,6,0,4,0,
			6,4,8,6,10,
			6,4,8,6,10,
			6,4,8,6,10,
			6,4,8,6,10};

// Row number each cpu should start on based on 5 unique picture sizes. rowNumber[cpu][size].
int rowNumGI[5][5]={	0,0,0,0,0,
			0,576,0,480,0,
			432,960,768,1200,1200,
			864,1344,1536,1920,2400,
			1296,1728,2304,2640,3600};

// Offset of storing address each cpu should start sting values in based on 5 unique picture sizes. storingAddress[cpu][size].	
int storeAddGI[5][5]={	0,0,0,0,0,
			0,48,0,40,0,
			36,80,64,100,100,
			72,112,128,160,200,
			108,144,192,220,300};

//if Sobel is impemented via multi core -currently not called. :Start
/*int rowSobel[5][5]={0,3,0,2,0,3,2,4,3,5,3,2,4,3,5,3,2,4,3,5,1,0,2,1,3};
int rowNumSobel[5][5]={0,0,0,0,0,0,48,0,40,0,36,80,64,100,100,72,112,128,160,200,108,144,192,220,300};	
int storeAddSobel[5][5]={0,4,0,4,0,4,46,4,40,4,34,74,60,94,94,64,102,116,148,184,94,130,172,202,274};/**/
//if Sobel is impemented via multi core -currently not called. :End

#define Cpu1_SF SHARED_ONCHIP_BASE+5000         //address where Cpu 1 Start Flag value is written
#define Cpu2_SF SHARED_ONCHIP_BASE+5400         //address where Cpu 2 Start Flag value is written
#define Cpu3_SF SHARED_ONCHIP_BASE+5600         //address where Cpu 3 Start Flag value is written
#define Cpu4_SF SHARED_ONCHIP_BASE+5800         //address where Cpu 4 Start Flag value is written
#define Cpu1_EF SHARED_ONCHIP_BASE+5001         //address where Cpu 1 End Flag value is written
#define Cpu2_EF SHARED_ONCHIP_BASE+5401         //address where Cpu 2 End Flag value is written
#define Cpu3_EF SHARED_ONCHIP_BASE+5601         //address where Cpu 3 End Flag value is written
#define Cpu4_EF SHARED_ONCHIP_BASE+5801         //address where Cpu 4 End Flag value is written
#define resultAdd SHARED_ONCHIP_BASE+6000       //address where intermediate results are written
#define Ascii_SA SHARED_ONCHIP_BASE+7000        //address where final Ascii values are written

unsigned char* resAdd = (unsigned char*)resultAdd;	//address where intermediate results are written
unsigned char* AsciiAdd = (unsigned char*)resultAdd;	//address where final Ascii results are written

#endif /* __OFFSET_H_ */


