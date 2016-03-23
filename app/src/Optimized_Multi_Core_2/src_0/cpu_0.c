#include <stdio.h>
#include <stdlib.h>
#include "altera_avalon_performance_counter.h"
#include "images.h"
#include "../offset.h"
#include "../processing.inc"

#if DEBUG == 1
unsigned char* pointer[] = {img1_24_24, img1_32_22, img1_32_32, img1_40_28, img1_40_40, img2_24_24, img2_32_22, img2_32_32, img2_40_28, img2_40_40, img3_32_32};
#else
unsigned char* pointer[] = {img1_32_32, img2_32_32, img3_32_32};
#endif

	int size_x ;
	int size_y ;
	int max_col;
	unsigned char* shared ;
	unsigned char* temp ;
	
	unsigned int* shared2;
	unsigned int* base2;

//initialization of Flag Pointers :Start
unsigned char* Cpu1_SP = (unsigned char*)Cpu1_SF; 
unsigned char* Cpu2_SP = (unsigned char*)Cpu2_SF;
unsigned char* Cpu3_SP = (unsigned char*)Cpu3_SF;
unsigned char* Cpu4_SP = (unsigned char*)Cpu4_SF;
unsigned char* Cpu1_EP = (unsigned char*)Cpu1_EF;
unsigned char* Cpu2_EP = (unsigned char*)Cpu2_EF;
unsigned char* Cpu3_EP = (unsigned char*)Cpu3_EF;
unsigned char* Cpu4_EP = (unsigned char*)Cpu4_EF;
unsigned char* Ascii_SAP = (unsigned char*)Ascii_SA;
//initialization of Flag Pointers :End

void sram2sm_p3(unsigned char* base)
{
	
     
	int x, y;
	shared = (unsigned char*)SHARED_ONCHIP_BASE;
	temp = (unsigned char*)SHARED_ONCHIP_BASE;
	
	//unsigned int* shared2;
	//unsigned int* base2;

	size_x = *base++; //Char wise extraction of size values.
	size_y = *base++;
	max_col= *base++;
	int loop;
	if (size_y!=22)	
	{loop = (size_x>>2)*(((size_y<<1)+size_y)>>2);}
	else 
	{loop = 132;}

			
     	*shared++  = size_x;
	*shared++  = size_y;
	*shared++  = max_col;
	*shared++  = *base++; //dummmy correction

    	base2 = base;		//Moving to Int pointers to utilize full bandwitdth of the data bus.
    	shared2 = shared;	//This Archi. has 32 bit data bus, hence Int pointers.

	for(x = 0; x < loop;x++) //Transfer of data from Sram to Shared mem using min possible loops with 
	{			 //loop unrolling and loop merging.
		*shared2++ = *base2++;
		*shared2++ = *base2++;	//4bytes at a time.
		*shared2++ = *base2++;
		*shared2++ = *base2++;	//16bytes in a single loop. 16 divides all available images sizes.
	}        
}
void sobelAndPrint()
{
	  	
	//sobel filter
    	unsigned int colS = (size_x>>1);
	unsigned int rowNumS=0;
	//unsigned int gx,gy;
	unsigned char testAsciiArt[333]={};
	int i=0;
	int x,y;
	int sX=(size_y>>1)-2;
	int sY=(size_x>>1)-2;

    	for(x = 0; x < sX;x++)
	{
		for(y = 0; y < sY;y++)
		{
			// Convolution simplified in to six simple additions or subtractions.
	
			/*gx   =   *(resAdd+rowNumS+y)- *(resAdd+rowNumS+y+2)
				+(*(resAdd+rowNumS+y+colS)<<1)-(*(resAdd+rowNumS+y+colS+2)<<1)
				+*(resAdd+rowNumS+y+(colS<<1))- *(resAdd+rowNumS+y+(colS<<1)+2);

			gy   =   *(resAdd+rowNumS+y)+(*(resAdd+rowNumS+y+1)<<1)+*(resAdd+rowNumS+y+2)-
				 *(resAdd+rowNumS+y+(colS<<1))-(*(resAdd+rowNumS+y+(colS<<1)+1)<<1)
				-*(resAdd+rowNumS+y+(colS<<1)+2);/**/

			//(abs(gx)+abs(gy))>>6; // Sqaring and Sqaure root approximations by absolute values //From a research paper.
							 // Divided by 2^6 to get the value spread to 0-15.

			// Finding the right Ascii char using the 0-15 spread and 
			//corresponding pre decided Ascii char by darkness quotient.		
			//switch((abs(gx)+abs(gy))>>6)
			switch((abs(*(resAdd+rowNumS+y)- *(resAdd+rowNumS+y+2)
					+(*(resAdd+rowNumS+y+colS)<<1)-(*(resAdd+rowNumS+y+colS+2)<<1)
					+*(resAdd+rowNumS+y+(colS<<1))- *(resAdd+rowNumS+y+(colS<<1)+2))
				+abs(*(resAdd+rowNumS+y)+(*(resAdd+rowNumS+y+1)<<1)+*(resAdd+rowNumS+y+2)-
				 	*(resAdd+rowNumS+y+(colS<<1))-(*(resAdd+rowNumS+y+(colS<<1)+1)<<1)
					-*(resAdd+rowNumS+y+(colS<<1)+2)))>>6)	          
			{
				case 0: 
				case 1:
				testAsciiArt[i]=' ';
				break;
				case 2: testAsciiArt[i] ='.';
				break;
				case 3: testAsciiArt[i]='-';
				break;
				case 4: testAsciiArt[i] =':';
				break;
				case 5: 
				case 6: 
				case 7: 
				case 8: testAsciiArt[i] ='0';
				break;
				case 9: testAsciiArt[i]='y';
				break;
				case 10: testAsciiArt[i] = 'o';
				break;
				case 11: testAsciiArt[i]='A';
				break;
				case 12: testAsciiArt[i] ='#'; 
				break;
				case 13: testAsciiArt[i]='$';
				break;
				case 14: testAsciiArt[i] ='%';
				break;
				case 15: testAsciiArt[i]='@';
				break;
				case 16: 
				default:
				testAsciiArt[i] ='_';
				break;
			}
		#if DEBUG == 1
		printf("%c ",testAsciiArt[i]);
		#endif
		i+=1;
		}
	rowNumS+= colS;
	#if DEBUG == 1
	printf("\n");
	#endif
	}/**/

	//if Sobel is impemented via multi core -currently not called. :Start
	/*#if DEBUG == 1
	for(x = 0; x < (size_y>>1)-2;x++)
	//for(x = 4; x < SRow0+2;x++)
	{
		for(y = 0; y < (size_x>>1)-2;y++)
		{
			testAsciiArt[i-4] = *(AsciiAdd+i);			
			printf("%c ",*(AsciiAdd+i));
			i+=1;		
		}
	printf("\n");
	}
	#endif/**/
	//if Sobel is impemented via multi core -currently not called.:End
	*Cpu1_EP=0;*Cpu2_EP=0;*Cpu3_EP=0;*Cpu4_EP=0;
}

int main()
{   
    int counter = 0;      
    delay(3000);
    *Cpu1_SP=0;*Cpu2_SP=0;*Cpu3_SP=0;*Cpu4_SP=0;*Cpu1_EP=0;*Cpu2_EP=0;*Cpu3_EP=0;*Cpu4_EP=0;
    printf("CPU-0 says wassup bro AS\n");  
    int loop=420;
    unsigned int EPCheck;// =1;

    #if DEBUG == 1
    loop = 11;
    delay(1000);
    printf("\n\n");
    #endif

    PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
    PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);
    PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 1);			//perf monitor 1S
    while(counter < loop)
    {
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 2); 		//perf monitor 2S      
	sram2sm_p3(pointer[counter%SIZE]);
	PERF_END(PERFORMANCE_COUNTER_0_BASE, 2); 		//perf monitor 2E 
	
	*Cpu1_SP=1;*Cpu2_SP=1;*Cpu3_SP=1;*Cpu4_SP=1;
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 3);		//perf monitor 3S
	grayScaleAndInterpolation(temp,*temp,*(temp+1),0);	
	PERF_END(PERFORMANCE_COUNTER_0_BASE, 3);		//perf monitor 3E
	
	EPCheck=MaxWait;
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 4);		//perf monitor 4S
	//CPU sync, and designed to compensate the issues with pointer value refreshing in a while loop. 
	REFRESH:
	Cpu1_EP = Cpu1_EF;Cpu2_EP = Cpu2_EF;
	Cpu3_EP = Cpu3_EF;Cpu4_EP = Cpu4_EF;
	if((*(Cpu1_EP)+ *(Cpu2_EP)+ *(Cpu3_EP)+ *(Cpu4_EP)!=4) && EPCheck!=0)
	{	
		EPCheck--;		
	}
	if(EPCheck==0)
	{
		EPCheck=MaxWait;
		goto REFRESH;
	}/**/
	PERF_END(PERFORMANCE_COUNTER_0_BASE, 4); 		//perf monitor 4E
	
	
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 5);   		//perf monitor 5S	
	sobelAndPrint();
	PERF_END(PERFORMANCE_COUNTER_0_BASE, 5);		//perf monitor 5E 

        counter++;

	#if DEBUG == 1
	printf("\n\n");
	delay(400);
	#endif
     }
     PERF_END(PERFORMANCE_COUNTER_0_BASE, 1);			//perf monitor 1E

   /* Print report */
   perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE,
                ALT_CPU_FREQ,        // defined in "system.h"
                5,                   // How many sections to print
                "Total","Image Fetch","Gry&Int:C0","+G&I:C1-C4","SobelPrint"   // Display-name of section(s).
                );
   printf("Number of images processed: %d\n",counter);
   return 0;
}
