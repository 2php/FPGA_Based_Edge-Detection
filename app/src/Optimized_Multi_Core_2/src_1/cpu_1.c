#include "../offset.h"
#include "../processing.inc"

int main()
{
	unsigned char* Cpu1_SP = (unsigned char*)Cpu1_SF;
	unsigned char* Cpu1_EP = (unsigned char*)Cpu1_EF;
	printf("CPU-1 says wassup bro AJS");

	while(1)
	{
		if(*Cpu1_SP==1)
		{
			*Cpu1_SP=0;
			grayScaleAndInterpolation(temp,*temp,*(temp+1),1);
			*Cpu1_EP=1;
			//sobelAndAscii(temp,*temp,*(temp+1),1);//if Sobel is impemented via multi core -currently not called.
		}
	}		
  	return 0;
}
