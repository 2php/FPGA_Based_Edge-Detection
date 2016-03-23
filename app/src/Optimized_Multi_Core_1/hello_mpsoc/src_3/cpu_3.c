#include <stdio.h>
#include "system.h"
#include "io.h"

#define TRUE 1
#define FLAG3 SHARED_ONCHIP_BASE+2           //used to indicate if data has been written or not
#define FLAG5 SHARED_ONCHIP_BASE+4           //used to indicate if data has been read or not
#define LOC1  SHARED_ONCHIP_BASE+5          //address where data is written

extern void delay (int millisec);

int main()
{
	//*********************filter kernel*********************
	char sobely[3][3]={-1, -2, -1, 0, 0, 0, 1, 2, 1};
	
	//*******************kernel dimensions*******************
	char kRows=sizeof(sobely[0]);             
	char kCols=sizeof(sobely)/kRows;
	
	//********************center of kernel*******************
	char kCenterX=kCols/2;
	char kCenterY=kRows/2;

	//*********************loop variables********************
	char i, j, k, l, ii, jj, mm, nn;
	
	//*****************input image dimensions****************
	unsigned char size_x=0, size_y=0;
	
	//pointer to shared memory segments
	unsigned char* shared1;             
	unsigned char* sharedx;
	short *shared2;

	//printf("Edge Detector (Sobely)\n");

	while (TRUE)
	{
		i=j=k=l=ii=jj=mm=nn=0;
		shared1=(unsigned char*) LOC1;
		shared1+=1603;
		sharedx=shared1+1206;
		shared2=sharedx;

		if(IORD_8DIRECT(FLAG3, 0)==1)     //wait for data to be written
		{
		//***************calc input image dimensions*************
		size_x=*(shared1++);
		size_y=*(shared1++);

		unsigned char img2small[size_x][size_y];          //input gray image
		short res_y[size_x][size_y];      //filter output

		//********************read input image*******************
		//printf("\n\n\nScaled Image: \n");           //***********for debugging
		for(i=0; i<size_x; i++)
		{
			//printf("[");           //***********for debugging
			for(j=0; j<size_y; j++)
			{
				img2small[i][j]=*shared1++;
				res_y[i][j]=0;
				//printf(" %d", img2small[i][j]/17);           //***********for debugging
			}
			//printf(" ]\n");           //***********for debugging
		}
		IOWR_8DIRECT(FLAG3, 0, 0);      //indicate data read
		}
		if(IORD_8DIRECT(FLAG5, 0)==0)     //wait for data to be read
		{
		//****************perform sobely filtering***************
		*(shared2++)=size_x;
		*(shared2++)=size_y;
		//printf("\nFilter Output(y): \n");           //***********for debugging
		for(i=0; i<size_x; i++)              //rows	
		{
			//printf("[");           //***********for debugging
			for(j=0; j<size_y; j++)          //columns
			{	
				for(k=0; k<kRows; k++)     //kernel rows
				{	
					mm=kRows-1-k;      //row index of flipped kernel
					for(l=0; l<kCols; l++) //kernel columns
					{
						nn=kCols-1-l;  //column index of flipped kernel

						//index of input signal, used for checking boundary; ignore pixels at the boundary
						ii=i+(k-kCenterY);
						jj=j+(l-kCenterX);

						//ignore input samples which are out of bound
						if(ii>=0 && ii<size_x && jj>=0 && jj<size_y)
						{
							res_y[i][j]+=img2small[ii][jj]*sobely[mm][nn];     //sobel along y
						}
					}
				}
				*shared2++=res_y[i][j];         //store in shared memory
				//printf(" %d", *(shared2-1));           //***********for debugging	
			}
			//printf(" ]\n");           //***********for debugging
		}
		IOWR_8DIRECT(FLAG5, 0, 1);      //indicate data write
		}
 	}

 	 return 0;
}
