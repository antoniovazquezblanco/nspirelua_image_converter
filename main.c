#include <stdio.h>
#include <unistd.h>
#include <png.h>
#include <byteswap.h>
#include <stdlib.h>
#include <math.h>

// Shows an usage help screen...
void usage();
// Prints a number in nspire lua format...
void print(void *ptr, unsigned char l);
// Debugging...
void debug(void *ptr, unsigned char l);

int main(int argc, char **argv)
{
	unsigned int tmp;
	unsigned int tmp2;
	FILE *file = NULL;
	char file_header[8];
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int width;
	unsigned int height;
	png_bytep * row_pointers;

	// Process arguments...
	if(argc < 2)
	{
		usage();
		return 1;
	}

	// Open input file...
	file = fopen(argv[1], "rb");
	if(!file)
	{
		fprintf(stderr, "[!] Could not open %s...\n", argv[1]);	
		return 1;
	}
	fread(file_header, 1, 8, file);
	if(png_sig_cmp(file_header, 0, 8))
	{
		fprintf(stderr, "[!] Input file is not a valid png...\n");
		fclose(file);
		return 1;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr)
	{
		fprintf(stderr, "[!] Could not allocate png struct...\n");
		return 1;
	}

	info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr)
	{
		fprintf(stderr, "[!] Could not allocate info struct...\n");
	}

	if(setjmp(png_jmpbuf(png_ptr)))
	{
		fprintf(stderr, "[!] Setjmp error...\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return 0;
	}

	png_init_io(png_ptr, file);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);

	if(png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
	{
		fprintf(stderr, "[!] Color should be RGBA...\n");
		return 1;
	}

	// Get image data...
	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (tmp = 0; tmp < height; tmp++)
                row_pointers[tmp] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
	png_read_image(png_ptr, row_pointers);

	// Print the header...
	print(&width, 4);
	print(&height, 4);
	tmp = 0;
	print(&tmp, 4);
	tmp = width*2;
	print(&tmp, 4);
	tmp = 16;
	print(&tmp, 2);
	tmp = 1;
	print(&tmp, 2);

	// Print image data...
	for(tmp = 0; tmp < height; tmp++)
	{
		png_byte* row = row_pointers[tmp];
		for(tmp2 = 0;  tmp2 < width; tmp2++)
		{
			png_byte* ptr = &(row[tmp2*4]);
			unsigned char r = (unsigned char)round(ptr[0]*31.0/255.0) & 0x1F;
			unsigned char g = (unsigned char)round(ptr[1]*31.0/255.0) & 0x1F;
			unsigned char b = (unsigned char)round(ptr[2]*31.0/255.0) & 0x1F;
			unsigned char a = (unsigned char)round(ptr[3]/255.0) & 0x01;
			unsigned short color = (b | (g << 5) | (r << 10) | (a << 15));
			print(&color, 2);
		}
	}
	printf("\n");

	// Clean up...
        for (tmp = 0; tmp < height; tmp++)
                free(row_pointers[tmp]);
        free(row_pointers);
	fclose(file);

	return 0;
}

void usage()
{
	printf("[?] Usage: imgnspire <file.png>\n");
	return;
}

void print(void *ptr, unsigned char l)
{
	unsigned char *p = (unsigned char *)ptr;
	int i;
	for(i = 0; i < l; i++)
		printf("\\%03d", *(p+i));
}
