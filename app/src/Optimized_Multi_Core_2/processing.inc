#ifndef PROCESSING_C_
#define PROCESSING_C_

void grayScaleAndInterpolation(unsigned char* temp,int size_x, int size_y,int callingCPU)
{
	//gray scale and interpolation combined for higer throughput and less memory footprint.	
	unsigned int col = (size_x<<1)+size_x;	//Total actual columns including RGB pixels.	
	int x,y,tred,tblue,tgreen;
	int pic = (size_y-22)>>1;

	int row = rowGI[callingCPU][size[pic]];	//Use pre calucalted position values of input pixels to caluclate further results.
	int rowNum=rowNumGI[callingCPU][size[pic]];
	int storeAdd=storeAddGI[callingCPU][size[pic]];
 	

	for(x =0; x < row;x+=2)// jumping through 2 rows at once.
	{
		for(y = 4; y < col+4;y+=6)// jumping through two * three columns at once.
		{
			//12 pixels related for interpolation are fetched and preped for further processing.
			tred = *(temp+rowNum+y)+*(temp+rowNum+y+3) +*(temp+rowNum+y+col) +*(temp+rowNum+y+col+3);
			tgreen=*(temp+rowNum+y+1)+*(temp+rowNum+y+4) +*(temp+rowNum+y+col+1) +*(temp+rowNum+y+col+4);
			tblue =*(temp+rowNum+y+2)+*(temp+rowNum+y+5) +*(temp+rowNum+y+col+2) +*(temp+rowNum+y+col+5);

			*(resAdd+storeAdd) =((((tred<<2)+ tred)>>4)+(((tgreen<<3)+ tgreen)>>4)+(tblue>>3))>>2;
			storeAdd+=1;//Gray scale and Interpolation achived in tandem	
	    	}
	rowNum+= (col<<1);
	}
}

//if Sobel is impemented via multi core -currently not called.
/*void sobelAndAscii(unsigned char* temp,int size_x, int size_y,int callingCPU)
{
	//gray scale and interpolation combined for higer throughput and less memory footprint.	
	unsigned int colS = (size_x>>1);
	unsigned int gx,gy,x,y;
	int pic = (size_y-22)>>1;

	int rowS = rowSobel[callingCPU][size[pic]];
	int rowNumS=rowNumSobel[callingCPU][size[pic]];
	int storeAddS=storeAddSobel[callingCPU][size[pic]];
 	

	for(x = 0; x < rowS;x++)
	{
		for(y = 4; y < colS+2;y++)
		{
			// Convolution simplified in to six simple additions or subtractions.
	
			gx   =   *(resAdd+rowNumS+y)- *(resAdd+rowNumS+y+2)
				+(*(resAdd+rowNumS+y+colS)<<1)-(*(resAdd+rowNumS+y+colS+2)<<1)
				+*(resAdd+rowNumS+y+(colS<<1))- *(resAdd+rowNumS+y+(colS<<1)+2);

			gy   =   *(resAdd+rowNumS+y)+(*(resAdd+rowNumS+y+1)<<1)+*(resAdd+rowNumS+y+2)-
				 *(resAdd+rowNumS+y+(colS<<1))-(*(resAdd+rowNumS+y+(colS<<1)+1)<<1)
				-*(resAdd+rowNumS+y+(colS<<1)+2);

			//(abs(gx)+abs(gy))>>6; // Sqaring and Sqaure root approximations by absolute values //From a research paper.
							 // Divided by 2^6 to get the value spread to 0-15.

			// Finding the right Ascii char using the 0-15 spread and 
			//corresponding pre decided Ascii char by darkness quotient.		
			switch((abs(gx)+abs(gy))>>6)	          
			{
				case 0: 
				case 1:
				*(AsciiAdd+storeAddS) =' ';
				break;
				case 2: *(AsciiAdd+storeAddS) ='.';
				break;
				case 3: *(AsciiAdd+storeAddS)='-';
				break;
				case 4: *(AsciiAdd+storeAddS) =':';
				break;
				case 5: 
				case 6: 
				case 7: 
				case 8: *(AsciiAdd+storeAddS) ='0';
				break;
				case 9: *(AsciiAdd+storeAddS)='y';
				break;
				case 10: *(AsciiAdd+storeAddS) = 'o';
				break;
				case 11: *(AsciiAdd+storeAddS)='A';
				break;
				case 12: *(AsciiAdd+storeAddS) ='#'; 
				break;
				case 13: *(AsciiAdd+storeAddS)='$';
				break;
				case 14: *(AsciiAdd+storeAddS) ='%';
				break;
				case 15: *(AsciiAdd+storeAddS)='@';
				break;
				case 16: 
				default:
				*(AsciiAdd+storeAddS) ='_';
				break;	
			}
		storeAddS+=1;
		}
	rowNumS+= colS;
	}
}/**/
#endif /* PROCESSING_C_ */
