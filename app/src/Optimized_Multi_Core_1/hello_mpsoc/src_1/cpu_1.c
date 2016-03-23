#include <stdio.h>
#include "system.h"
#include "io.h"

#define TRUE 1
#define FLAG1 SHARED_ONCHIP_BASE             //used to indicate if data has been written or not
#define FLAG2 SHARED_ONCHIP_BASE+1           //used to indicate if data has been read or not
#define FLAG3 SHARED_ONCHIP_BASE+2           //used to indicate if data has been read or not
#define LOC1  SHARED_ONCHIP_BASE+5           //address where data is written

extern void delay (int millisec);

int main()
{
	//*********************loop variables********************
	unsigned char i, j;
	
	//*****************input image dimensions****************
	unsigned char size_x=0, size_y=0, max_col=0;
	
	//pointer to shared memory segments
	unsigned char* shared1;             
	unsigned char* shared2;

	//printf("Image Scaler\n");

	while (TRUE)
	{
		shared1=(unsigned char*) LOC1;
		shared2=shared1+1603;
		if(IORD_8DIRECT(FLAG1, 0)==1);     //wait for data to be written
		{
			//***************calc input image dimensions*************
			size_x=*(shared1++);
			size_y=*(shared1++);
			max_col=*(shared1++);
	
			unsigned char img2gray[size_x][size_y];          //input gray image

			//********************read input image*******************
			//printf("\n\n\nGrayscale Image: \n");           //***********for debugging
			for(i=0; i<size_x; i++)
			{
				//printf("[");           //***********for debugging
				for(j=0; j<size_y; j++)
				{
					img2gray[i][j]=*shared1++;
					//printf(" %d", img2gray[i][j]/17);           //***********for debugging
				}
				//printf(" ]\n");           //***********for debugging
			}
			IOWR_8DIRECT(FLAG1, 0, 0);      //indicate data read
		}
		//wait for data to be read
		if((IORD_8DIRECT(FLAG2, 0)==0) && (IORD_8DIRECT(FLAG3, 0)==0))
		{
			//*********resize image & store is shared memory*********
			*(shared2++)=size_x/2;
		*(shared2++)=size_y/2;
		//printf("\nScaled Down Image: \n");           //***********for debugging
		for(i=0; i<size_x/2; i++)
		{
			//printf("[");           //***********for debugging
			for(j=0; j<size_y/2; j++)
			{
				*(shared2++)=(img2gray[2*i][2*j]+img2gray[(2*i)+1][2*j]+img2gray[2*i][(2*j)+1]+img2gray[(2*i)+1][(2*j)+1])>>2;
				//printf(" %d", *(shared2-1)/17);           //***********for debugging
			}
			//printf(" ]\n");           //***********for debugging
		}
		//indicate data write
		IOWR_8DIRECT(FLAG2, 0, 1);      
		IOWR_8DIRECT(FLAG3, 0, 1);
		}
 	}

 	 return 0;
}
