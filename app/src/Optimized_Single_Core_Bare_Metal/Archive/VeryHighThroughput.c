#include <stdio.h>
#include "system.h"
#include "altera_avalon_performance_counter.h"
#include "io.h"
#include "images.h"
//#include "math.h"

/*ine SECTION_3	3221
#define SECTION_4	4831

#define PERF_PRINT	0*/

#define DEBUG 0
#define TRUE 1

#if DEBUG == 1
#define SIZE 11
#else
#define SIZE 3
#endif

/*#define FLAG_1 		SHARED_ONCHIP_BASE+1610
#define FLAG_2 		SHARED_ONCHIP_BASE+3220
#define FLAG_3 		SHARED_ONCHIP_BASE+4830
#define FLAG_4 		SHARED_ONCHIP_BASE+6440*/

//5000
//extern void delay (int millisec);

#if DEBUG == 1
unsigned char* pointer[] = {img1_24_24, img1_32_22, img1_32_32, img1_40_28, img1_40_40, img2_24_24, img2_32_22, img2_32_32, img2_40_28, img2_40_40, img3_32_32};
#else
unsigned char* pointer[] = {img1_32_32, img2_32_32, img3_32_32};
#endif

//unsigned char darkness[] = {' ','.','`','-',':','I','+','/','c','X','q','*','#','%','@','$'};

//unsigned int ascii[101];
//unsigned char ascii[403];
unsigned char* shared = (unsigned char*)SHARED_ONCHIP_BASE;
unsigned char* temp = (unsigned char*)SHARED_ONCHIP_BASE;

void sram2sm_p3(unsigned char* base)
{
	int x, y;
	//unsigned char* shared = (unsigned char*)SHARED_ONCHIP_BASE;
	//unsigned char* temp = (unsigned char*)SHARED_ONCHIP_BASE;
	//unsigned char SHARED_ONCHIP_BASE[8000];
	//unsigned char* shared;
	unsigned int* shared2;
	unsigned int* base2;
	


	//shared =  SHARED_ONCHIP_BASE;

	int size_x = *base++;
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
	*shared++  = *base++;

    base2 = base;
    shared2 = shared;
	//printf("The image is: %d x %d!! \n", *(shared-2), *(temp+2));

	for(x = 0; x < loop;x++)
	//for(y = 0; y < (size_y>>1);y++)
	{
		*shared2++ = *base2++;
		*shared2++ = *base2++;
		*shared2++ = *base2++;
		*shared2++ = *base2++;
	}

	//gry and interpolation
	int col = (size_x<<1)+size_x;
	int rowNum=0;
	int i = 4;
	

	int tred,tblue,tgreen;

	for(x = 4; x < size_y+4;x+=2)
	{
	for(y = 4; y < col+4;y+=6)
	{

        tred = *(temp+rowNum+y)+*(temp+rowNum+y+3) +*(temp+rowNum+y+col) +*(temp+rowNum+y+col+3);
        tgreen=*(temp+rowNum+y+1)+*(temp+rowNum+y+4) +*(temp+rowNum+y+col+1) +*(temp+rowNum+y+col+4);
        tblue =*(temp+rowNum+y+2)+*(temp+rowNum+y+5) +*(temp+rowNum+y+col+2) +*(temp+rowNum+y+col+5);

        *(temp+i) =((((tred<<2)+ tred)>>4)+(((tgreen<<3)+ tgreen)>>4)+(tblue>>3))>>2;
        i+=1;
    	}
	rowNum+= (col<<1);
	}

    	//sobel operator
    	int colS = (size_x>>1);
	int rowNumS=0;
	int gx,gy;
	unsigned int sobeltest[1000];
	//unsigned char testAsciiArt[]={' ', '.', '-', ':', '=', '+', '*', 'x', 'Y', 'o', 'A', '#', '$', '%', '@', '_'};
	char testAsciiArt;

    	for(x = 3; x < (size_y>>1)+1;x++)
	{
	for(y = 4; y < (size_x>>1)+1;y++)
	{
		/*int gx1,gx2,gx3,gx4,gx5,gx6;
		int gy1,gy2,gy3,gy4,gy5,gy6;  
		gx1= *(temp+rowNumS+y);
		gx2=*(temp+rowNumS+y+2);
		gx3=*(temp+rowNumS+y+colS);
		gx4=*(temp+rowNumS+y+colS+2);
		gx5=*(temp+rowNumS+y+(colS<<1));
		gx6=*(temp+rowNumS+y+(colS<<1)+2);
		   
		gy1= *(temp+rowNumS+y);
		gy2=*(temp+rowNumS+y+1);
		gy3=*(temp+rowNumS+y+2);
		gy4=*(temp+rowNumS+y+(colS<<1));
		gy5=*(temp+rowNumS+y+(colS<<1)+1);
		gy6=*(temp+rowNumS+y+(colS<<1)+2);
	    

		gx   =  gx1- gx2+(gx3<<1)-(gx4<<1)+gx5- gx6;

		gy   =  gy1+(gy2<<1)+gy3-gy4-(gy5<<1)-gy6;*/
		
		gx   =   *(temp+rowNumS+y)- *(temp+rowNumS+y+2)
                +(*(temp+rowNumS+y+colS)<<1)-(*(temp+rowNumS+y+colS+2)<<1)
                +*(temp+rowNumS+y+(colS<<1))- *(temp+rowNumS+y+(colS<<1)+2);

	       gy   =   *(temp+rowNumS+y)+(*(temp+rowNumS+y+1)<<1)+*(temp+rowNumS+y+2)-
		         *(temp+rowNumS+y+(colS<<1))-(*(temp+rowNumS+y+(colS<<1)+1)<<1)
		        -*(temp+rowNumS+y+(colS<<1)+2);

		sobeltest[i]=(abs(gx)+abs(gy));
		//int test = sqrt((gx*gx)+(gy*gy));
		sobeltest[i] = sobeltest[i]>>6;
		/*if (sobeltest[i]<17)
		{
		printf("%c ",testAsciiArt[sobeltest[i]-1]);
		}
		else
		{
		printf("%c ",testAsciiArt[15]);
		}
		i+=1;*/
		//printf(" %u,%d,%d ",temp,*(temp+rowNumS+y),temp+rowNumS+y);

		//int sob =(abs(gx)+abs(gy))>>6;
	//int test = sqrt((gx*gx)+(gy*gy));
	//test = test>>6;
        switch(sobeltest[i])
        {
        case 1: testAsciiArt=' ';
        break;
        case 2: testAsciiArt ='.';
        break;
        case 3: testAsciiArt='-';
        break;
        case 4: testAsciiArt =':';
        break;
        case 5: testAsciiArt='0';
        break;
        case 6: testAsciiArt ='+';
        break;
        case 7: testAsciiArt='0';
        break;
        case 8: testAsciiArt ='x';
        break;
        case 9: testAsciiArt='y';
        break;
        case 10: testAsciiArt = 'o';
        break;
        case 11: testAsciiArt='A';
        break;
        case 12: testAsciiArt ='#'; 
        break;
        case 13: testAsciiArt='$';
        break;
        case 14: testAsciiArt ='%';
        break;
        case 15: testAsciiArt='@';
        break;
        case 16: testAsciiArt ='_';
        break;
        default: testAsciiArt =' ';
        break;
        }
        i+=1;

        //sobeltest[i] = testAsciiArt;
	//sobeltest[i]>>6;
	//printf("%c ",testAsciiArt);/**/
	}
	rowNumS+= colS;
	//printf("\n");
	}
	
	//printf("\n\n");
}

int main()
{

    int counter = 0;
    //printf("Hello from cpu_0\n\n\n");
    //delay(2000);
    //printf("\n\n");

    PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
    PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);
    PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 1);
    while(counter < 420)
    {
        sram2sm_p3(pointer[counter%SIZE]);
        counter++;
	//printf("\n\n");
	//printf("\n\n");
	//delay(1000);

	}

   PERF_END(PERFORMANCE_COUNTER_0_BASE, 1);

   /* Print report */
   perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE,
                ALT_CPU_FREQ,        // defined in "system.h"
                1,                   // How many sections to print
                "Section 1"        // Display-name of section(s).
                );
   printf("Number of images processed: %d\n",counter);
   return 0;
}
