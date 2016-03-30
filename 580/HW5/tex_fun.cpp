/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"
#include	"math.h"

GzColor	*image;
int xs, ys;
int reset = 1;

/* Image texture function */
int	Find_Pixel(int x, int y);

int tex_fun(float u, float v, GzColor color)
{
	unsigned char		pixel[3];
	unsigned char     dummy;
	char  		foo[8];
	int   		i, j;
	FILE			*fd;

	if (reset) {          /* open and load texture file */
		fd = fopen("texture", "rb");
		if (fd == NULL) {
			fprintf(stderr, "texture file not found\n");
			exit(-1);
		}
		fscanf(fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
		image = (GzColor*)malloc(sizeof(GzColor)*(xs + 1)*(ys + 1));
		if (image == NULL) {
			fprintf(stderr, "malloc for texture image failed\n");
			exit(-1);
		}

		for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
			fread(pixel, sizeof(pixel), 1, fd);
			image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
			image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
			image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
		}

		reset = 0;          /* init is done */
		fclose(fd);
	}

	/* bounds-test u,v to make sure nothing will overflow image array bounds */
	/* determine texture cell corner values and perform bilinear interpolation */
	/* set color to interpolated GzColor value and return */

	/*
	u,v coords at vertices range over [0, 1]
	¨C per our definition
	*/

	if (u < 0)
		u = 0;
	if (u > 1)
		u = 1;
	if (v < 0)
		v = 0;
	if (v > 1)
		v = 1;
	/*
	U,V ¡ú u,v
	u * (Xsize-1)
	v * (Ysize-1)

	u,v ¡ú U,V
	*/
	float Image_Texture_Map_U = u * (xs - 1);
	float Image_Texture_Map_V = v * (ys - 1);

	/*
	  A----------B
	  |          |
	  D----------C
	  */
	int Ax = (int)floor(Image_Texture_Map_U);
	int Ay = (int)floor(Image_Texture_Map_V);
	int Bx = (int)ceil(Image_Texture_Map_U);
	int By = (int)floor(Image_Texture_Map_V);
	int Cx = (int)ceil(Image_Texture_Map_U);
	int Cy = (int)ceil(Image_Texture_Map_V);
	int Dx = (int)floor(Image_Texture_Map_U);
	int Dy = (int)ceil(Image_Texture_Map_V);

	/*
	u = s
	v = t
	x left to right increases
	y up to down increases

	Color(p) = s t C + (1-s) t D + s (1-t) B + (1-s) (1-t) A
	¨C s, t are fractional distances [0,1]
	¨C A, B, C, D are pixel colors at integer-coord pixels
	*/

	int A = Find_Pixel(Ax, Ay);
	int B = Find_Pixel(Bx, By);
	int C = Find_Pixel(Cx, Cy);
	int D = Find_Pixel(Dx, Dy);


	for (int i = RED; i <= BLUE; i++)
	{
		color[i] = u * v * image[C][i] + (1 - u) * v * image[D][i] + u * (1 - v) * image[B][i] + (1 - u) * (1 - v) * image[A][i];
	}

	return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	color[0] = 1 * sin(cos(500 * u * v));
	color[1] = 0;
	color[2] = 1 * sin(cos(500 * v * u));



	return GZ_SUCCESS;
}

int	Find_Pixel(int x, int y)
{
	return x + y * xs;
}

