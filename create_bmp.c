#include<stdio.h>
#include<stdlib.h>
#define BYTES_PER_PIXEL 3
struct bmp_header{
	char filetype[2];
	char filesize[4];
	char reserved[4];
	char data_offset[4];
};
struct bmp_info_header{
	char header_size[4];
	char image_width[4];
	char image_height[4];
	char color_planes[2];
	char bits_per_pixel[2];
	char compression[4];
	char pixel_data_size[4];
	char horizontal_resolution[4];
	char vertical_resolution[4];
	char colors_used[4];
	char important_colors[4];
};      
int main(int argc , char *argv[])
{        	
        if(argc != 3)
	{
		printf("Usage: %s <width> <Height>\n",argv[0]);
		return 1;
	}
	int WIDTH = atoi(argv[1]);
	int HEIGHT= atoi(argv[2]);
	int actual_size =0;
	int padding = 0;
	int row_size=0;
	int pixel_data_size=0;
	int file_size=0;
	int index=0;
	struct bmp_header bmph={{'B','M'},{0},{0},{54,0,0,0}};
	struct bmp_info_header bih = { {40,0,0,0},{WIDTH,0,0,0},{HEIGHT,0,0,0},{1,0},{24,0},{0},{0},{0x13,0x0B,0,0},{0x13,0x0B,0,0},{0},{0}};
	// Default colour: white
        char red   = 255;
        char green = 255;
        char blue  = 255;
	FILE * fp = NULL;
	actual_size = WIDTH * BYTES_PER_PIXEL;
	padding = (4-(actual_size%4))% 4;
	row_size = actual_size + padding;
	pixel_data_size=row_size*HEIGHT;
	file_size=54+pixel_data_size;
	char pixel_array[pixel_data_size];

	for (int i=0; i<4; i++)
	       	bih.pixel_data_size[i]=((pixel_data_size>>(8*i))& 0xFF);
	for (int i=0; i<4; i++)
	       	bmph.filesize[i]=((file_size>>(8*i))& 0xFF);

	fp = fopen("image.bmp","wb");
	if(fp == NULL)
	{
		printf("Failed to create the bmp file.\n");
		return 1;
	}

	//1. file header 
	if(fwrite(&bmph,sizeof(bmph),1,fp) != 1)
	{
		printf("Failed to write the bmp_header .\n");
		return 1;
	}

	//2. information header
	if(fwrite(&bih,sizeof(bih),1,fp) !=1)
	{
		printf("Failed to write the bmp information header.\n");
		return 1;
	}

      	//3.PIXEL data
	for(int y=HEIGHT-1; y>=0; y--)	
	{       

		for(int x=0; x<WIDTH; x++)
		{     
		       	red = 255;
		        green =255;
		        blue =255;
			if(x % 2 == 0)
			{
				red=255;
				green=0;
				blue=0;
			}
			else if(x % 3 == 0)
			{
				red =0;
				green =255;
				blue =0;
			}
			else if(x % 4 == 0)
			{
				red=0;
				green=0;
				blue=255;
			}
		
					
			pixel_array[index++]=blue;
			pixel_array[index++]=green;
			pixel_array[index++]=red;
		}
		

		for (int p = 0; p < padding; p++)
       	        {
           	        pixel_array[index++]=0;
       	        }
 	}
	if(fwrite(pixel_array,sizeof(pixel_array),1,fp) !=1)
	{
		printf("Failed to write the pixel data into the file.\n");
		return 1;
	}
        fclose(fp);
	printf("BMP file is created and image is stored successfully\n");
	return 0;
}

	









