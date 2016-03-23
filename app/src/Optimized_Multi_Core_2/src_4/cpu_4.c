#include "../offset.h"
#include "../processing.inc"

int main()
{
	unsigned char* Cpu4_SP = (unsigned char*)Cpu4_SF;
	unsigned char* Cpu4_EP = (unsigned char*)Cpu4_EF;
	printf("CPU-4 says wassup bro AJS");

	while(1)
	{
		if(*Cpu4_SP==1)
		{
			*Cpu4_SP=0;
			grayScaleAndInterpolation(temp,*temp,*(temp+1),4);
			*Cpu4_EP=1;
			//sobelAndAscii(temp,*temp,*(temp+1),4); //if Sobel is impemented via multi core -currently not called.
		}
	}/**/		
  	return 0;
}
