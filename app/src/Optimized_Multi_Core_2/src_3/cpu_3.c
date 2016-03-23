#include "../offset.h"
#include "../processing.inc"

int main()
{
	unsigned char* Cpu3_SP = (unsigned char*)Cpu3_SF;
	unsigned char* Cpu3_EP = (unsigned char*)Cpu3_EF;
	printf("CPU-3 says wassup bro AJS");

	while(1)
	{
		if(*Cpu3_SP==1)
		{
			*Cpu3_SP=0;
			grayScaleAndInterpolation(temp,*temp,*(temp+1),3);
			*Cpu3_EP=1;
			//sobelAndAscii(temp,*temp,*(temp+1),3);//if Sobel is impemented via multi core -currently not called.
		}
	}/**/		
  	return 0;
}
