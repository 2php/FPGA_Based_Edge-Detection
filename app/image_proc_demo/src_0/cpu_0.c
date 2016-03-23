#include <stdio.h>
#include <stdlib.h>
#include "altera_avalon_performance_counter.h"
#include "includes.h"
#include "system.h"
#include "io.h"
#include "images.h"

#define DEBUG 0
#define TRUE 1

#if DEBUG == 1
#define SIZE 11
#else
#define SIZE 3
#endif

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       4096
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    StartTask_Stack[TASK_STACKSIZE]; 

/* Definition of Task Priorities */

#define STARTTASK_PRIO      1
#define TASK1_PRIORITY      10
#define SECTION_1 1

#if DEBUG == 1
unsigned char* pointer[] = {img1_24_24, img1_32_22, img1_32_32, img1_40_28, img1_40_40, img2_24_24, img2_32_22, img2_32_32, img2_40_28, img2_40_40, img3_32_32};
#else
unsigned char* pointer[] = {img1_32_32, img2_32_32, img3_32_32};
#endif

void sram2sm_p3(unsigned char* base)
{
	int x, y;
	unsigned char* shared = (unsigned char*)SHARED_ONCHIP_BASE;
	unsigned char* temp = (unsigned char*)SHARED_ONCHIP_BASE;
	unsigned int* shared2;
	unsigned int* base2;

	int size_x = *base++; //Char wise extraction of size values.
	int size_y = *base++;
	int max_col= *base++;
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

	//gray scale and interpolation combined for higer throughput and less memory footprint.
	int col = (size_x<<1)+size_x;
	int rowNum=0;
	int i = 4;
	

	int tred,tblue,tgreen;
	for(x = 4; x < size_y+4;x+=2)// jumping through 2 rows at once.
	{
	for(y = 4; y < col+4;y+=6)// jumping through two * three columns at once.
	{
		//12 pixels related for interpolation are fetched and preped for further processing.
		tred = *(temp+rowNum+y)+*(temp+rowNum+y+3) +*(temp+rowNum+y+col) +*(temp+rowNum+y+col+3);
		tgreen=*(temp+rowNum+y+1)+*(temp+rowNum+y+4) +*(temp+rowNum+y+col+1) +*(temp+rowNum+y+col+4);
		tblue =*(temp+rowNum+y+2)+*(temp+rowNum+y+5) +*(temp+rowNum+y+col+2) +*(temp+rowNum+y+col+5);

		*(temp+i) =((((tred<<2)+ tred)>>4)+(((tgreen<<3)+ tgreen)>>4)+(tblue>>3))>>2;
		i+=1;//Gray scale and Interpolation achived in tandem
    	}
	rowNum+= (col<<1);
	}

    	//sobel operator
    	unsigned int colS = (size_x>>1);
	unsigned int rowNumS=0;
	unsigned int sobeltest,gx,gy;
	unsigned char testAsciiArt[333]={};
	i=0;
	//unsigned char testAsciiArt;

    	for(x = 4; x < (size_y>>1)+2;x++)
	{
	for(y = 4; y < (size_x>>1)+2;y++)
	{
		// Convolution simplified in to six simple additions or subtractions.
	
		gx   =   *(temp+rowNumS+y)- *(temp+rowNumS+y+2)
			+(*(temp+rowNumS+y+colS)<<1)-(*(temp+rowNumS+y+colS+2)<<1)
			+*(temp+rowNumS+y+(colS<<1))- *(temp+rowNumS+y+(colS<<1)+2);

		gy   =   *(temp+rowNumS+y)+(*(temp+rowNumS+y+1)<<1)+*(temp+rowNumS+y+2)-
			 *(temp+rowNumS+y+(colS<<1))-(*(temp+rowNumS+y+(colS<<1)+1)<<1)
			-*(temp+rowNumS+y+(colS<<1)+2);

		sobeltest =(abs(gx)+abs(gy))>>6; // Sqaring and Sqaure root approximations by absolute values //From a research paper.
						 // Divided by 2^6 to get the value spread to 0-15.
		/*if (sobeltest[i]<17)
		{
			printf("%c ",testAsciiArt[sobeltest[i]-1]);
		}
		else
		{
			printf("%c ",testAsciiArt[15]);
		}
		i+=1;*/
	
		// Finding the right scii char using the 0-15 spread and 
		//corresponding pre decided Ascii char by darkness quotient.		
	switch(sobeltest)	          
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
		case 5: testAsciiArt[i]='0';
		break;
		case 6: testAsciiArt[i] ='+';
		break;
		case 7: testAsciiArt[i]='0';
		break;
		case 8: testAsciiArt[i] ='x';
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
	printf("%c ",testAsciiArt[i]);/**/
	#endif
	i+=1;
	}
	rowNumS+= colS;
	#if DEBUG == 1
	printf("\n");
	#endif
	}
	
	//printf("\n\n");
}

void task1(void* pdata)
{
    INT8U err;
    INT8U value=0;
	
    int counter = 0;
    printf("cpu_0 says wassup bro AJ!!! \n\n\n");
    #if DEBUG == 1
    delay(1000);
    printf("\n\n");
    #endif

    PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
    PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);
    PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 1);
    while(counter < 420)
    {
        sram2sm_p3(pointer[counter%SIZE]);
        counter++;
	#if DEBUG == 1
	printf("\n\n");
	printf("\n\n");
	delay(400);
	#endif
     }

   PERF_END(PERFORMANCE_COUNTER_0_BASE, 1);

   /* Print report */
   perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE,
                ALT_CPU_FREQ,        // defined in "system.h"
                1,                   // How many sections to print
                "Section 1"        // Display-name of section(s).
                );
   printf("Number of images processed: %d\n",counter);
}

void StartTask(void* pdata)
{
  INT8U err;
  void* context;  

  /*
   * Create statistics task
   */

  OSStatInit();

  /* 
   * Creating Tasks in the system 
   */

  err=OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  if (DEBUG) {
     if (err == OS_ERR_NONE) { //if start successful
      printf("Task1 created\n");
    }
   }  

  printf("All Tasks and Kernel Objects generated!\n");

  /* Task deletes itself */

  OSTaskDel(OS_PRIO_SELF);
}


int main(void) {

  printf("MicroC/OS-II-Vesion: %1.2f\n", (double) OSVersion()/100.0);
     
  OSTaskCreateExt(
	 StartTask, // Pointer to task code
         NULL,      // Pointer to argument that is
                    // passed to task
         (void *)&StartTask_Stack[TASK_STACKSIZE-1], // Pointer to top
						     // of task stack 
         STARTTASK_PRIO,
         STARTTASK_PRIO,
         (void *)&StartTask_Stack[0],
         TASK_STACKSIZE,
         (void *) 0,  
         OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
         
  OSStart();
  
  return 0;
}

//sqrt() implementation
float sqrt(float x)  
{
	union
  	{
    	int i;
    	float x;
  	}u;

  	u.x=x;
  	u.i=(1<<29)+(u.i>>1)-(1<<22); 
  	return u.x;
}
