/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"
#include	"disp.h"


int GzNewFrameBuffer(char** framebuffer, int width, int height)
{
	/* create a framebuffer:
	-- allocate memory for framebuffer : (sizeof)GzPixel x width x height
	-- pass back pointer
	*/
	*framebuffer = new char[sizeof(GzPixel)* width * height];

	if (*framebuffer == 0)
		return GZ_FAILURE;

	return GZ_SUCCESS;
}

int GzNewDisplay(GzDisplay	**display, GzDisplayClass dispClass, int xRes, int yRes)
{

	/* create a display:
	-- allocate memory for indicated class and resolution
	-- pass back pointer to GzDisplay object in display
	*/
	if (xRes <= 0 || yRes <= 0 || xRes > MAXXRES || yRes > MAXXRES)
		return GZ_FAILURE;

	*display = new GzDisplay;
	(*display)->xres = xRes;
	(*display)->yres = yRes;
	(*display)->dispClass = dispClass;
	(*display)->fbuf = new GzPixel[xRes * yRes];
	(*display)->open = 0;//closed
	if ((*display)->fbuf == 0)
		return GZ_FAILURE;

	return GZ_SUCCESS;
}

int GzFreeDisplay(GzDisplay	*display)
{
	/* clean up, free memory */
	if (display == 0)
		return GZ_FAILURE;
	delete[]display->fbuf;
	delete display;

	return GZ_SUCCESS;
}

int GzGetDisplayParams(GzDisplay *display, int *xRes, int *yRes, GzDisplayClass	*dispClass)
{
	/* pass back values for an open display */
	if (display == 0)
		return GZ_FAILURE;
	*xRes = display->xres;
	*yRes = display->yres;
	*dispClass = display->dispClass;
	return GZ_SUCCESS;
}

int GzInitDisplay(GzDisplay	*display)
{
	/* set everything to some default values - start a new frame */
	if (display == 0)
		return GZ_FAILURE;
	for (int i = 0; i < display->xres * display->yres; i++)
	{
		/*I use the Photoshop, the RGB is 128 112 96*/
		display->fbuf[i].red = 128 << 4;
		display->fbuf[i].green = 112 << 4;
		display->fbuf[i].blue = 96 << 4;
		display->fbuf[i].alpha = 1;
		display->fbuf[i].z = 0x7FFFFFFF;
	}
	display->open = 1;
	return GZ_SUCCESS;
}

int GzPutDisplay(GzDisplay *display, int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* write pixel values into the display */
	if (display == 0)
		return GZ_FAILURE;
	if (0 <= i && i < display->xres && 0 <= j && j < display->yres)
	{
		if (r > 4095)
			r = 4095;
		else if (r < 0)
			r = 0;
		if (g > 4095)
			g = 4095;
		else if (g < 0)
			g = 0;
		if (b > 4095)
			b = 4095;
		else if (b < 0)
			b = 0;
		display->fbuf[ARRAY(i, j)].red = r;
		display->fbuf[ARRAY(i, j)].green = g;
		display->fbuf[ARRAY(i, j)].blue = b;
		display->fbuf[ARRAY(i, j)].alpha = a;
		display->fbuf[ARRAY(i, j)].z = z;
		return GZ_SUCCESS;
	}
	else
		return GZ_FAILURE;
}

int GzGetDisplay(GzDisplay *display, int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	/* pass back pixel value in the display */
	/* check display class to see what vars are valid */
	if (display == 0 || r == 0 || g == 0 || b == 0 || a == 0 || z == 0)
		return GZ_FAILURE;
	if (0 > i || i >= display->xres || 0 > j || j >= display->yres)
	{
		return GZ_FAILURE;
	}
	*r = display->fbuf[ARRAY(i, j)].red;
	*g = display->fbuf[ARRAY(i, j)].green;
	*b = display->fbuf[ARRAY(i, j)].blue;
	*a = display->fbuf[ARRAY(i, j)].alpha;
	*z = display->fbuf[ARRAY(i, j)].z;
	return GZ_SUCCESS;
}

int GzFlushDisplay2File(FILE* outfile, GzDisplay *display)
{
	/* write pixels to ppm file based on display class -- "P6 %d %d 255\r" */
	if (display == 0)
		return GZ_FAILURE;
	fprintf(outfile, "P6 %d %d 255\r", display->xres, display->yres);
	GzIntensity R, G, B;
	for (int i = 0; i < display->xres * display->yres; i++)
	{
		R = display->fbuf[i].red >> 4;
		G = display->fbuf[i].green >> 4;
		B = display->fbuf[i].blue >> 4;
		fprintf(outfile, "%c%c%c", (char)R, (char)G, (char)B);
	}
	return GZ_SUCCESS;
}

int GzFlushDisplay2FrameBuffer(char* framebuffer, GzDisplay *display)
{
	/* write pixels to framebuffer:
	- Put the pixels into the frame buffer
	- Caution: store the pixel to the frame buffer as the order of blue, green, and red
	- Not red, green, and blue !!!
	*/
	if (framebuffer == 0 || display == 0)
		return GZ_FAILURE;

	for (int i = 0; i < display->xres * display->yres; i++)
	{
		framebuffer[3 * i] = (char)(display->fbuf[i].blue >> 4);
		framebuffer[3 * i + 1] = (char)(display->fbuf[i].green >> 4);
		framebuffer[3 * i + 2] = (char)(display->fbuf[i].red >> 4);
	}

	return GZ_SUCCESS;
}