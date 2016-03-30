#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"


/*
Define functions written by myself
*/
int Gz_LEE_Rasterization(GzRender *render, GzCoord Vertex[3]);
void Gz_Swap_Vertex(GzCoord vertex1, GzCoord vertex2);
void Gz_Make_CW(GzCoord v1, GzCoord v2, GzCoord v3);
float Find_x_min(GzCoord v[3]);
float Find_x_max(GzCoord v[3]);


int GzNewRender(GzRender **render, GzRenderClass renderClass, GzDisplay *display)
{
	/*
	- malloc a renderer struct
	- keep closed until BeginRender inits are done
	- span interpolator needs pointer to display for pixel writes
	- check for legal class GZ_Z_BUFFER_RENDER
	*/
	*render = new GzRender;

	if (*render == 0 || renderClass != GZ_Z_BUFFER_RENDER || display == 0)
		return GZ_FAILURE;

	(*render)->renderClass = renderClass;
	(*render)->display = display;
	(*render)->open = 0; // closed
	return GZ_SUCCESS;
}


int GzFreeRender(GzRender *render)
{
	/*
	-free all renderer resources
	*/
	if (render == 0)
		return GZ_FAILURE;

	delete render;
	return GZ_SUCCESS;
}


int GzBeginRender(GzRender	*render)
{
	/*
	- set up for start of each frame - init frame buffer
	*/
	if (render == 0)
		return GZ_FAILURE;

	render->open = 1;//open
	return GZ_SUCCESS;
}


int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList,
	GzPointer *valueList) /* void** valuelist */
{
	/*
	- set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	- later set shaders, interpolaters, texture maps, and lights
	*/
	if (render == 0 || nameList == 0 || valueList == 0)
		return GZ_FAILURE;

	for (int i = 0; i < numAttributes; i++)
	{
		switch (nameList[i])
		{
		case GZ_RGB_COLOR:
			render->flatcolor[0] = ((float*)valueList[i])[0];
			render->flatcolor[1] = ((float*)valueList[i])[1];
			render->flatcolor[2] = ((float*)valueList[0])[2];
			break;
			//Other cases for following homeworks
		default:
			break;
		}
	}

	return GZ_SUCCESS;
}


int GzPutTriangle(GzRender *render, int	numParts, GzToken *nameList,
	GzPointer *valueList)
	/* numParts - how many names and values */
{
	/*
	- pass in a triangle description with tokens and values corresponding to
	GZ_NULL_TOKEN:		do nothing - no values
	GZ_POSITION:		3 vert positions in model space
	- Invoke the scan converter and return an error code
	*/

	if (render == 0 || nameList == 0 || valueList == 0)
		return GZ_FAILURE;
	for (int i = 0; i < numParts; i++)
	{
		GzCoord *vertex;
		switch (nameList[i])
		{
		case GZ_NULL_TOKEN:
			break;

		case GZ_POSITION:
			vertex = (GzCoord*)valueList[i];
			Gz_LEE_Rasterization(render, vertex);
			break;

		default:
			break;
		}
	}

	return GZ_SUCCESS;
}

/* NOT part of API - just for general assistance */
short	ctoi(float color)		/* convert float color to GzIntensity short */
{
	return(short)((int)(color * ((1 << 12) - 1)));
}

int Gz_LEE_Rasterization(GzRender *render, GzCoord vertex[3])
{
	GzIntensity Red, Green, Blue, alpha;

	//bubble sort

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (vertex[j][Y] > vertex[j + 1][Y])
			{
				Gz_Swap_Vertex(vertex[j], vertex[j + 1]);
			}
		}
	}
	//I think there is no need to do clockwise, because I can use +/- common area 
	//Gz_Make_CW(vertex[0], vertex[1], vertex[2]);

	//After the CW, vectors and a rectangle enclousing the triangle in 2D map

	GzCoord Vector_1_to_2;
	GzCoord Vector_2_to_3;
	GzCoord Vector_3_to_1;

	Vector_1_to_2[X] = vertex[1][X] - vertex[0][X];
	Vector_1_to_2[Y] = vertex[1][Y] - vertex[0][Y];
	Vector_1_to_2[Z] = vertex[1][Z] - vertex[0][Z];

	Vector_2_to_3[X] = vertex[2][X] - vertex[1][X];
	Vector_2_to_3[Y] = vertex[2][Y] - vertex[1][Y];
	Vector_2_to_3[Z] = vertex[2][Z] - vertex[1][Z];

	Vector_3_to_1[X] = vertex[0][X] - vertex[2][X];
	Vector_3_to_1[Y] = vertex[0][Y] - vertex[2][Y];
	Vector_3_to_1[Z] = vertex[0][Z] - vertex[2][Z];
	//We use + area in this 3 vectors!
	//Because the coords may not be integer! But pixels are at integers! X grows right, Y grows down.
	int Xmin = floor(Find_x_min(vertex));
	int Xmax = ceil(Find_x_max(vertex));

	int Ymin = floor(vertex[0][Y]);
	int Ymax = ceil(vertex[2][Y]);

	//Calculate the Ax+By+Cz+D=0 A B C D use vectors

	float A;
	float B;
	float C;
	float D;

	A = Vector_1_to_2[Y] * (-Vector_3_to_1[Z]) - (-Vector_3_to_1[Y]) * Vector_1_to_2[Z];
	B = Vector_1_to_2[Z] * (-Vector_3_to_1[X]) - (-Vector_3_to_1[Z]) * Vector_1_to_2[X];
	C = Vector_1_to_2[X] * (-Vector_3_to_1[Y]) - (-Vector_3_to_1[X]) * Vector_1_to_2[Y];
	D = -(A * vertex[0][X] + B * vertex[0][Y] + C * vertex[0][Z]);

	//Scan the pixels and draw!
	float Pixel_z;
	for (int i = Xmin; i <= Xmax; i++)
	{
		if (i < 0 || i > render->display->xres)
			continue;
		for (int j = Ymin; j <= Ymax; j++)
		{
			if (j < 0 || j > render->display->yres)
				continue;

			Pixel_z = -(A * i + B * j + D) / C;
			GzDepth tempz = 0;  //get z from getdisplay
			/*
			Put point into the line fuctions: E1(x,y) E2(x,y) E3(x,y) the relationship with vectors in 2D map
			*/
			float Relationship_with_1_to_2 = Vector_1_to_2[Y] * (i - vertex[0][X]) - Vector_1_to_2[X] * (j - vertex[0][Y]);
			float Relationship_with_2_to_3 = Vector_2_to_3[Y] * (i - vertex[1][X]) - Vector_2_to_3[X] * (j - vertex[1][Y]);
			float Relationship_with_3_to_1 = Vector_3_to_1[Y] * (i - vertex[2][X]) - Vector_3_to_1[X] * (j - vertex[2][Y]);

			if (Relationship_with_1_to_2 == 0 || Relationship_with_2_to_3 == 0 || Relationship_with_3_to_1 == 0)
			{
				//one the line!
				GzGetDisplay(render->display, i, j, &Red, &Green, &Blue, &alpha, &tempz);
				if (Pixel_z < tempz)
				{
					Red = ctoi(render->flatcolor[0]);
					Green = ctoi(render->flatcolor[1]);
					Blue = ctoi(render->flatcolor[2]);
					GzPutDisplay(render->display, i, j, Red, Green, Blue, alpha, Pixel_z);
				}
			}
			else if ((Relationship_with_1_to_2 > 0 && Relationship_with_2_to_3 > 0 && Relationship_with_3_to_1 > 0) || (Relationship_with_1_to_2 < 0 && Relationship_with_2_to_3 < 0 && Relationship_with_3_to_1 < 0))
			{

				GzGetDisplay(render->display, i, j, &Red, &Green, &Blue, &alpha, &tempz);

				if (Pixel_z < tempz)
				{
					Red = ctoi(render->flatcolor[0]);
					Green = ctoi(render->flatcolor[1]);
					Blue = ctoi(render->flatcolor[2]);
					GzPutDisplay(render->display, i, j, Red, Green, Blue, alpha, Pixel_z);
				}
			}
		}
	}
	return GZ_SUCCESS;
}

void Gz_Swap_Vertex(GzCoord v1, GzCoord v2)
{
	GzCoord temp;
	temp[X] = v2[X];
	temp[Y] = v2[Y];
	temp[Z] = v2[Z];
	v2[X] = v1[X];
	v2[Y] = v1[Y];
	v2[Z] = v1[Z];
	v1[X] = temp[X];
	v1[Y] = temp[Y];
	v1[Z] = temp[Z];
}

float Find_x_min(GzCoord v[3])
{
	float min = v[0][X];
	for (int i = 1; i < 3; i++)
	{
		if (v[i][X] < min)
		{
			min = v[i][X];
		}
	}
	return min;
}

float Find_x_max(GzCoord v[3])
{
	float max = v[0][X];
	for (int i = 1; i < 3; i++)
	{
		if (v[i][X] > max)
		{
			max = v[i][X];
		}
	}
	return max;
}