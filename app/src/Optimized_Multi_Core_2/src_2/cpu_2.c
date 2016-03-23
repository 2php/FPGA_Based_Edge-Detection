#include "../offset.h"
#include "../processing.inc"

int main()
{
	unsigned char* Cpu2_SP = (unsigned char*)Cpu2_SF;
	unsigned char* Cpu2_EP = (unsigned char*)Cpu2_EF;
	printf("CPU-2 says wassup bro AJS");

	while(1)
	{
		if(*Cpu2_SP==1)
		{
			*Cpu2_SP=0;
			grayScaleAndInterpolation(temp,*temp,*(temp+1),2);
			*Cpu2_EP=1;
			//sobelAndAscii(temp,*temp,*(temp+1),2);//if Sobel is impemented via multi core -currently not called.
		}
	}/**/		
  	return 0;
}
