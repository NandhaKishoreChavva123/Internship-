#include<stdio.h>
#include<stdlib.h>
#define BYTES_PER_PIXEL 1
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

char color_table[5][4] = {
    {0,   0,   0,   0},   // Black
    {0,   0, 255,   0},   // Red
    {0, 255,   0,   0},   // Green
    {255, 0,   0,   0},   // Blue
    {255, 255, 255, 0}    // White
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
	int color_index=0;
	struct bmp_header bmph={{'B','M'},{0},{0},{74,0,0,0}};
	struct bmp_info_header bih = { {40,0,0,0},{0},{0},{1,0},{8,0},{0},{0},{0x13,0x0B,0,0},{0x13,0x0B,0,0},{5,0,0,0},{0}};
	for (int i = 0; i < 4; i++)
	{
    		bih.image_width[i] = (WIDTH >> (8 * i)) & 0xFF;
   		bih.image_height[i] = (HEIGHT >> (8 * i)) & 0xFF;
	}
	FILE * fp = NULL;
	actual_size = WIDTH * BYTES_PER_PIXEL;
	padding = (4-(actual_size%4))% 4;
	row_size = actual_size + padding;
	pixel_data_size=row_size*HEIGHT;
	file_size=14+40+sizeof(color_table)+pixel_data_size;
	char pixel_array[pixel_data_size];

	for (int i=0; i<4; i++)
	       	bih.pixel_data_size[i]=((pixel_data_size>>(8*i))& 0xFF);
	for (int i=0; i<4; i++)
	       	bmph.filesize[i]=((file_size>>(8*i))& 0xFF);

	fp = fopen("image2.bmp","wb");
	if(fp == NULL)
	{
		printf("Failed to create the bmp file.\n");
		return 1;
	}

	//1. file header 
	if(fwrite(&bmph,sizeof(bmph),1,fp) != 1)
	{
		printf("Failed to write the bmp_header .\n");
		fclose(fp);
		return 1;
	}

	//2. information header
	if(fwrite(&bih,sizeof(bih),1,fp) !=1)
	{
		printf("Failed to write the bmp information header.\n");
		fclose(fp);
		return 1;
	}


	//3.color table
	if(fwrite(color_table,sizeof(color_table),1,fp) !=1)
	{
		printf("Failed to write the color table.\n");
		fclose(fp);
		return 1;
	}

      	//4.PIXEL data
	for(int y=HEIGHT-1; y>=0; y--)	
	{       
		for(int x=0; x<WIDTH; x++)
		{     
			if(x % 2 == 0)
			{
				color_index=1;
			}
			else if(x % 3 == 0)
			{
				color_index=2;
			}
			else if(x % 5 == 0)
			{
				color_index=3;
			}
		        else if(x % 7 == 0)
			{
				color_index=4;
			}
			else
			{
				color_index=0;
			}

			pixel_array[index++]=color_index;

		}
		for (int p = 0; p < padding; p++)
       	        {
           	        pixel_array[index++]=0;
       	        }
 	}
	if(fwrite(pixel_array,sizeof(pixel_array),1,fp) !=1)
	{
		printf("Failed to write the pixel data into the file.\n");
		fclose(fp);
		return 1;
	}
        fclose(fp);
	printf("8-bit BMP with color table created successfully\n");
	return 0;
}

	










