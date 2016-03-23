#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_performance_counter.h"
#include "io.h"
#include "images.h"

#define TRUE 1
#define DEBUG 0
#define SECTION_1 1
#define FLAG1 SHARED_ONCHIP_BASE             //used to indicate if data has been read or not
#define FLAG6 SHARED_ONCHIP_BASE+1004             //used to indicate if data has been written or not
#define LOC1  SHARED_ONCHIP_BASE+5           //address where data is written

void sram2sm_p3(unsigned char* base);     //SRAM to Shared On-Chip tranfer function
extern void delay (int millisec);         //delay milliseconds function

unsigned char *out;         //pointer to PGM output image

int main()
{
	#if DEBUG == 1
	/* Sequence of images for testing if the system functions properly */
	char number_of_images=10;
	unsigned char* img_array[10] = {img1_24_24, img1_32_22, img1_32_32, img1_40_28, img1_40_40, 
			img2_24_24, img2_32_22, img2_32_32, img2_40_28, img2_40_40 };
	#else
	/* Sequence of images for measuring performance */
	char number_of_images=3;
	unsigned char* img_array[3] = {img1_32_32, img2_32_32, img3_32_32};
	#endif
	
	//*********************loop variables********************
	unsigned char i, j, count;

	//*********************ASCII symbols*********************
	unsigned char symbols[]={' ', '.', '-', ':', '=', '+', '*', 'x', 'Y', 'o', 'A', '#', '$', '%', '@', '_'};

	//*****************ASCII image dimensions****************
	unsigned char size_x=0, size_y=0;

	//**********pointer to currently accessed image**********
	char current_image=0;
	
	//pointer to shared memory segments
	short* ascii_tab; unsigned char* sharedx;

	printf("Image Processing (Multi-Processor)\n");
	#if DEBUG == 0
	printf("\n\n\nInput Image Dimensions: \n");
	printf("Rows: 32\n");
	printf("Columns: 32\n\n\n");
	#endif

	unsigned char k;
	
	delay(1000);            //to compensate for the delay in writing to the other CPUs

	while (TRUE) 
	{	
		//***************start performance counter***************
		PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
		PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);
		PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, SECTION_1);
		
			sharedx=(unsigned char*) LOC1;
			sharedx+=3613;
			ascii_tab=sharedx;
			printf("%d\n", count);
			//************transfer data to shared memory*************
			if(IORD_8DIRECT(FLAG1, 0)==0);     //wait for data to be read
			{
			//printf("Transferring Image From SRAM to Shared Memory\n");           //***********for debugging
			sram2sm_p3(img_array[current_image]);    //transfer image to shared memory
			IOWR_8DIRECT(FLAG1, 0, 1);      //indicate data write	
			}		
		
			//**********************store ASCII**********************
			if(IORD_8DIRECT(FLAG6, 0)==1);     //wait for data to be written
			{
				#if DEBUG == 1
				printf("\nASCII Output: \n\n"); 
				#endif
				k=0;           //to keep track of pointer's orignal value
				size_x=*ascii_tab;
				*out=(*ascii_tab++)-1;    //x-dimension
				out++;
				size_y=*ascii_tab;
				*out=(*ascii_tab++)-1;    //y-dimension
				out++;
				*out=15;
				out++;
				k+=3;
				unsigned char res[size_x][size_y];

				//get image from memory
				for(i=0; i<size_x; i++)
				{
					for(j=0; j<size_y; j++)
					{
						res[i][j]=*ascii_tab++; 
						//printf("%d ", res[i][j]);
					}
					//printf("\n");
				}
				IOWR_8DIRECT(FLAG6, 0, 0);      //indicate data write
				
				#if DEBUG == 1
				printf("\n\n\nInput Image Dimensions: \n");
				printf("Rows: %d\n", size_x<<1);
				printf("Columns: %d\n", size_y<<1);
				#endif

				//get ASCII image and store in SRAM
				for(i=1; i<size_x-1; i++)
				{
					for(j=1; j<size_y-1; j++)
					{
						*out=symbols[res[i][j]];   
	
						#if DEBUG == 1
						printf("%c ", *out);
						#endif

						out++;
						k++;
					}
					#if DEBUG == 1
					printf("\n");
					#endif
				}
			}
			current_image=(current_image+1)%number_of_images;           //Increment the image pointer 
			out-=k;                //get output image pointer back to initial value
			//delay(5000);
		
		//***************stop performance counter****************
		PERF_END(PERFORMANCE_COUNTER_0_BASE, SECTION_1);  

		//Print report
		perf_print_formatted_report
		(PERFORMANCE_COUNTER_0_BASE,            
		ALT_CPU_FREQ,        // defined in "system.h"
		1,                   // How many sections to print
		"Section 1"        // Display-name of section(s).
		);
		delay(1000);
	}
	return 0;
}

//SRAM to Shared-on-chip transfer function
void sram2sm_p3(unsigned char* base)
{
	int x, y;
	unsigned char* shared;
	unsigned char r, g, b;

	shared = (unsigned char*) LOC1;

	int size__x = *base++;
	int size__y = *base++;
	int max_col= *base;
	*shared++  = size__x;
	*shared++  = size__y;
	*shared++  = max_col;
	//printf("Grayscale Image: \n");           //***********for debugging
	for(x = 0; x < size__x; x++)
	{
		//printf("[");           //***********for debugging
		for(y = 0; y < size__y; y++)
		{
			r=*(++base);
			g=*(++base);
			b=*(++base);
			*shared++=((((r<<2)+ r)>>4)+(((g<<3)+ g)>>4)+(b>>3))>>2;
			//printf(" %d", (*(shared-1))/17);           //***********for debugging
		}
		//printf(" ]\n");           //***********for debugging
	}
}
