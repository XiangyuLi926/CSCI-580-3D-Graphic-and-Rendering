#include "disp.h"

/*
* Control using super-sampling
*/
#define SuperSampling

int GzNewFrameBuffer(char** framebuffer, int width, int height)
{
	*framebuffer = new char[width * height * 3]();
	return (*framebuffer == NULL) ? GZ_FAILURE : GZ_SUCCESS;
}

int GzNewDisplay(GzDisplay **display, GzDisplayClass dispClass, int xRes, int yRes)
{
	*display = new GzDisplay();
	if (*display == NULL)
	{
		return GZ_FAILURE;
	}
	GzDisplay &play = *(*display);
	if (xRes > MAXXRES) xRes = MAXXRES;
	if (yRes > MAXYRES) yRes = MAXYRES;
#ifdef SuperSampling
	xRes <<= 1;
	yRes <<= 1;
#endif
	play.fbuf = new GzPixel[xRes * yRes]();
	if (play.fbuf == NULL)
	{
		delete *display;
		*display = NULL;
		return GZ_FAILURE;
	}
	play.xres = (unsigned short)xRes;
	play.yres = (unsigned short)yRes;
	play.dispClass = dispClass;
	play.open = 0;
	return GZ_SUCCESS;
}

int GzFreeDisplay(GzDisplay *display)
{
	if (display == NULL) return GZ_FAILURE;
	delete[]display->fbuf;
	delete display;
	return GZ_SUCCESS;
}

int GzGetDisplayParams(GzDisplay *display, int *xRes, int *yRes, GzDisplayClass	*dispClass)
{
	if (display == NULL) return GZ_FAILURE;
#ifdef SuperSampling
	*xRes = display->xres >> 1;
	*yRes = display->yres >> 1;
#else
	*xRes = display->xres;
	*yRes = display->yres;
#endif
	*dispClass = display->dispClass;
	return GZ_SUCCESS;
}

int GzInitDisplay(GzDisplay *display)
{
	if (display == NULL) return GZ_FAILURE;
	GzPixel *pflt = display->fbuf;
	for (int i = display->xres * display->yres; i > 0; i--, pflt++)
	{
		pflt->red = 0;
		pflt->green = 0;
		pflt->blue = 0;
		pflt->alpha = 0;
		pflt->z = 0x7FFFFFFF;
	}
	display->open = 1;
	return GZ_SUCCESS;
}

int GzPutDisplay(GzDisplay *display, int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/*Rasterization Function assure boundary*/
	GzPixel *pflt = display->fbuf + ARRAY(i, j);
	if (pflt->z <= z) return GZ_SUCCESS;
	pflt->red = (r < 4095) ? r : 4095;
	pflt->green = (g < 4095) ? g : 4095;
	pflt->blue = (b < 4095) ? b : 4095;
	pflt->alpha = a;
	pflt->z = z;
	return GZ_SUCCESS;
}

int GzGetDisplay(GzDisplay *display, int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	if ((i < 0) || (i >= display->xres)) return GZ_FAILURE;
	if ((j < 0) || (j >= display->yres)) return GZ_FAILURE;
	GzPixel *pflt = display->fbuf + ARRAY(i, j);
	*r = pflt->red;
	*g = pflt->green;
	*b = pflt->blue;
	*a = pflt->alpha;
	*z = pflt->z;
	return GZ_SUCCESS;
}


int GzFlushDisplay2File(FILE* outfile, GzDisplay *display)
{
	if ((display == NULL) || (display->open == 0)) return GZ_FAILURE;
#ifdef SuperSampling
	fprintf(outfile, "P6 %d %d 255\r", display->xres >> 1, display->yres >> 1);
#else
	fprintf(outfile, "P6 %d %d 255\r", display->xres, display->yres);
#endif
	GzPixel *pflt = display->fbuf;
	unsigned int towrite = 0;
	char cache[4096];
#ifdef SuperSampling
	for (int y = display->yres >> 1; y > 0; y--, pflt += display->xres)
	{
		for (int x = display->xres >> 1; x > 0; x--, pflt += 2)
		{
			cache[towrite++] = (pflt[0].red + pflt[1].red + pflt[display->xres].red + pflt[display->xres + 1].red) >> 6;
			cache[towrite++] = (pflt[0].green + pflt[1].green + pflt[display->xres].green + pflt[display->xres + 1].green) >> 6;
			cache[towrite++] = (pflt[0].blue + pflt[1].blue + pflt[display->xres].blue + pflt[display->xres + 1].blue) >> 6;
#else
	for (int y = display->yres; y > 0; y--)
	{
		for (int x = display->xres; x > 0; x--, pflt++)
		{
			cache[towrite++] = (pflt[0].red) >> 4;
			cache[towrite++] = (pflt[0].green) >> 4;
			cache[towrite++] = (pflt[0].blue) >> 4;
#endif
			if (towrite >= 4093)
			{
				fwrite(cache, towrite, 1, outfile);
				towrite = 0;
			}
		}
	}
	fwrite(cache, towrite, 1, outfile);
	return GZ_SUCCESS;
}

int GzFlushDisplay2FrameBuffer(char* framebuffer, GzDisplay *display)
{
	if ((display == NULL) || (display->open == 0)) return GZ_FAILURE;
	GzPixel *pflt = display->fbuf;
#ifdef SuperSampling
	for (int y = display->yres >> 1; y > 0; y--, pflt += display->xres)
	{
		for (int x = display->xres >> 1; x > 0; x--, pflt += 2)
		{
			*framebuffer++ = (pflt[0].blue + pflt[1].blue + pflt[display->xres].blue + pflt[display->xres + 1].blue) >> 6;
			*framebuffer++ = (pflt[0].green + pflt[1].green + pflt[display->xres].green + pflt[display->xres + 1].green) >> 6;
			*framebuffer++ = (pflt[0].red + pflt[1].red + pflt[display->xres].red + pflt[display->xres + 1].red) >> 6;
#else
	for (int y = display->yres; y > 0; y--)
	{
		for (int x = display->xres; x > 0; x--, pflt++)
		{
			*framebuffer++ = (pflt[0].blue) >> 4;
			*framebuffer++ = (pflt[0].green) >> 4;
			*framebuffer++ = (pflt[0].red) >> 4;
#endif
		}
	}
	return GZ_SUCCESS;
}