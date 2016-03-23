#include <stdio.h>
#include <math.h>
#include "system.h"
#include "io.h"

#define TRUE 1
#define FLAG4 SHARED_ONCHIP_BASE+3           //used to indicate if data has been written or not
#define FLAG5 SHARED_ONCHIP_BASE+4           //used to indicate if data has been written or not
#define FLAG6 SHARED_ONCHIP_BASE+1004           //used to indicate if data has been read or not
#define LOC1  SHARED_ONCHIP_BASE+5           //address where data is written

extern void delay (int millisec);

int main()
{
	//*********************loop variables********************
	unsigned char i, j;
	
	//*****************input image dimensions****************
	unsigned char size_x=0, size_y=0;
	
	//pointer to shared memory segments             
	short *shared1;
	short *shared2;
	short *shared3;
	unsigned char *sharedx;

	unsigned short temp;        //gradient
	//printf("Gradient Calculator\n");

	while (TRUE)
	{
		sharedx=(unsigned char*) LOC1;
		sharedx+=2005;
		shared1=sharedx;
		sharedx+=804;
		shared2=sharedx;
		sharedx+=804;
		shared3=sharedx;
		
		if((IORD_8DIRECT(FLAG4, 0)==1) && (IORD_8DIRECT(FLAG5, 0)==1) && (IORD_8DIRECT(FLAG6, 0)==0))     //wait for data to be written
		{
		//***************calc input image dimensions*************
		size_x=*(shared1++);
		size_y=*(shared1++);
		shared2++;
		shared2++;
		*shared3++=size_x;
		*shared3++=size_y;

		//*******calc gradient and scale it for ASCII table******
		//printf("\n\nImage Gradient: \n");           //***********for debugging
		for(i=0; i<size_x; i++)
		{
			//printf("[");           //***********for debugging
			for(j=0; j<size_y; j++)
			{
				temp=(abs(*shared1)+abs(*shared2))>>6;
				if(temp>15)
				{
					*shared3++=15;
				}
				else
				{
					*shared3++=temp;
				}
				shared1++;
				shared2++;
				//printf(" %d", temp);                     //***********for debugging
				//printf(" %d", *(shared3-1));           //***********for debugging
			}
			//printf(" ]\n");           //***********for debugging
		}
		IOWR_8DIRECT(FLAG4, 0, 0);      //indicate data read
		IOWR_8DIRECT(FLAG5, 0, 0);      //indicate data read
		IOWR_8DIRECT(FLAG6, 0, 1);      //indicate data write
		}
 	}

 	 return 0;
}
