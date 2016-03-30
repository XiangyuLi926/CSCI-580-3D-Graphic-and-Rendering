/* CS580 Homework 4 */
#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define Pi 3.1415926

short ctoi(float color);
/*
Define functions written by myself
*/
//HW2
//void Gz_Make_CW(GzCoord v1, GzCoord v2, GzCoord v3);
void Gz_Swap_Vertex(GzCoord vertex1, GzCoord vertex2);
float Find_x_min(GzCoord v[3]);
float Find_x_max(GzCoord v[3]);

//HW3
void Gz_Normalize_Vector(GzCoord Vect);
int Gz_Change_Vertex_Value(GzCoord v_before[3], GzCoord v_after[3], GzMatrix Mat);

//HW4
int Gz_Shading_Eq(GzRender *render, GzColor Color, GzCoord Normal);
float Gz_Dot_Product(GzCoord Vect1, GzCoord Vect2);
int Gz_Push_Normal_Matrix(GzRender *render, GzMatrix matrix);
float Gz_In_Triangle_Area(GzCoord v0, GzCoord v1, GzCoord v2);

int Gz_Merge_Xsm_Xn(GzRender *render);//change in HW4
int Gz_Change_Vertex_Normal(GzCoord n_before[3], GzCoord n_after[3], GzMatrix Mat);
int Gz_LEE_Rasterization(GzRender *render, GzCoord vertex[3], GzCoord Normal[3]);//change in HW4
//----------------------------------------------------------
//Xform matrixes

int GzRotXMat(float degree, GzMatrix mat)
{
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	if (mat == 0)
		return GZ_FAILURE;

	float Rad = (float)degree * Pi / 180; // sin and cos receive the number in rad version! cannot use degree directly!
	/*
	mat has been initiated as
	1 0 0 0
	0 1 0 0
	0 0 1 0
	0 0 0 1
	*/
	mat[1][1] = cos(Rad);
	mat[1][2] = -sin(Rad);
	mat[2][1] = sin(Rad);
	mat[2][2] = cos(Rad);

	return GZ_SUCCESS;
}

int GzRotYMat(float degree, GzMatrix mat)
{
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	if (mat == 0)
		return GZ_FAILURE;

	float Rad = degree * Pi / 180;
	/*
	mat has been initiated as
	1 0 0 0
	0 1 0 0
	0 0 1 0
	0 0 0 1
	*/
	mat[0][0] = cos(Rad);
	mat[0][2] = sin(Rad);
	mat[2][0] = -sin(Rad);
	mat[2][2] = cos(Rad);

	return GZ_SUCCESS;
}

int GzRotZMat(float degree, GzMatrix mat)
{
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	if (mat == 0)
		return GZ_FAILURE;

	float Rad = degree * Pi / 180;
	/*
	mat has been initiated as
	1 0 0 0
	0 1 0 0
	0 0 1 0
	0 0 0 1
	*/
	mat[0][0] = cos(Rad);
	mat[0][1] = -sin(Rad);
	mat[1][0] = sin(Rad);
	mat[1][1] = cos(Rad);

	return GZ_SUCCESS;
}

int GzTrxMat(GzCoord translate, GzMatrix mat)
{
	// Create translation matrix
	// Pass back the matrix using mat value
	/*
	mat has been initiated as
	1 0 0 0
	0 1 0 0
	0 0 1 0
	0 0 0 1
	*/
	if (translate == 0 || mat == 0)
		return GZ_FAILURE;

	mat[0][3] = translate[0];
	mat[1][3] = translate[1];
	mat[2][3] = translate[2];
	return GZ_SUCCESS;
}

int GzScaleMat(GzCoord scale, GzMatrix mat)
{
	// Create scaling matrix
	// Pass back the matrix using mat value
	/*
	mat has been initiated as
	1 0 0 0
	0 1 0 0
	0 0 1 0
	0 0 0 1
	*/
	if (scale == 0 || mat == 0)
		return GZ_FAILURE;

	mat[0][0] = scale[0];
	mat[1][1] = scale[1];
	mat[2][2] = scale[2];
	return GZ_SUCCESS;
}

//----------------------------------------------------------
// Begin main functions
int GzNewRender(GzRender **render, GzRenderClass renderClass, GzDisplay *display)
{
	/*
	- malloc a renderer struct
	- keep closed until all inits are done
	- span interpolator needs pointer to display
	- check for legal class GZ_Z_BUFFER_RENDER

	- setup Xsp and anything only done once
	- init default camera
	*/
	*render = new GzRender;

	if (*render == 0 || renderClass != GZ_Z_BUFFER_RENDER || display == 0)
		return GZ_FAILURE;

	(*render)->renderClass = renderClass;
	(*render)->display = display;
	(*render)->open = 0; // closed

	//Added in HW3
	(*render)->camera.FOV = DEFAULT_FOV;
	(*render)->camera.position[X] = DEFAULT_IM_X;
	(*render)->camera.position[Y] = DEFAULT_IM_Y;
	(*render)->camera.position[Z] = DEFAULT_IM_Z;

	(*render)->camera.lookat[X] = 0.0;
	(*render)->camera.lookat[Y] = 0.0;
	(*render)->camera.lookat[Z] = 0.0;

	(*render)->camera.worldup[X] = 0.0;
	(*render)->camera.worldup[Y] = 1.0; // up vector in world space (unit-length vector)
	(*render)->camera.worldup[Z] = 0.0;

	(*render)->matlevel = -1; // empty

	/*
	Xsp= Xs/2 0     0      Xs/2
	0    -Ys/s 0      Ys/2
	0    0     Zmax/d 0
	0    0     0      1
	*/
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			(*render)->Xsp[i][j] = 0.0;
		}
	}

	(*render)->Xsp[0][0] = (*render)->display->xres / 2;
	(*render)->Xsp[0][3] = (*render)->display->xres / 2;
	(*render)->Xsp[1][1] = -(*render)->display->yres / 2;
	(*render)->Xsp[1][3] = (*render)->display->yres / 2;
	(*render)->Xsp[2][2] = (float)0x7fffffff / tan((*render)->camera.FOV * Pi / (2 * 180));
	(*render)->Xsp[3][3] = 1.0;

	//Added in HW4
	(*render)->interp_mode = GZ_RGB_COLOR;
	(*render)->flatcolor[0] = 0;
	(*render)->flatcolor[1] = 0;
	(*render)->flatcolor[2] = 0;
	(*render)->numlights = 0;
	
	GzColor Ka = DEFAULT_AMBIENT;
	GzColor Kd = DEFAULT_DIFFUSE;
	GzColor Ks = DEFAULT_SPECULAR;
	(*render)->ambientlight.color[RED] = 0.0;
	(*render)->ambientlight.color[GREEN] = 0.0;
	(*render)->ambientlight.color[BLUE] = 0.0;
	(*render)->Ka[RED] = Ka[RED];
	(*render)->Ka[GREEN] = Ka[GREEN];
	(*render)->Ka[BLUE] = Ka[BLUE];
	(*render)->Kd[RED] = Kd[RED];
	(*render)->Kd[GREEN] = Kd[GREEN];
	(*render)->Kd[BLUE] = Kd[BLUE];
	(*render)->Ks[RED] = Ks[RED];
	(*render)->Ks[GREEN] = Ks[GREEN];
	(*render)->Ks[BLUE] = Ks[BLUE];

	(*render)->Offset_x = 0;
	(*render)->Offset_y = 0;
	return GZ_SUCCESS;
}

int GzFreeRender(GzRender *render)
{
	/*
	- free all renderer resources
	*/
	if (render == 0)
		return GZ_FAILURE;

	GzFreeDisplay(render->display);
	delete[]render->Ximage;
	delete[]render->Xnorm;
	delete[]render->Xsp;
	delete[]render->flatcolor;
	delete[]render->lights;
	delete[]render->Ka;
	delete[]render->Kd;
	delete[]render->Ks;

	//delete camera
	delete[]render->camera.Xiw;
	delete[]render->camera.Xpi;
	delete[]render->camera.position;
	delete[]render->camera.lookat;
	delete[]render->camera.worldup;

	delete[]render->ambientlight.color;
	delete[]render->ambientlight.direction;
	//delete render
	delete render;
	return GZ_SUCCESS;
}

int GzBeginRender(GzRender	*render)
{
	/*
	- set up for start of each frame - clear frame buffer
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms if it want to.
	*/
	if (render == 0)
		return GZ_FAILURE;

	//clear frame buffer
	GzInitDisplay(render->display);

	/*
	- Compute Xiw
	- Z = cl / || cl ||
	- up' = up - (up Dot* Z)Z
	- Y = up' / || up'||
	- X = (Y x Z)
	- X = (Y x Z) = i (Yy Zz - Yz Zy) + j (Yz Zx - Yx Zz) + k (Yx Zy - Yy Zx) (LHC)
	*/
	GzCoord cl, Xiw_Z, Xiw_Y, Xiw_X, up, up_prime;

	cl[X] = render->camera.lookat[X] - render->camera.position[X];
	cl[Y] = render->camera.lookat[Y] - render->camera.position[Y];
	cl[Z] = render->camera.lookat[Z] - render->camera.position[Z];
	Xiw_Z[X] = cl[X];
	Xiw_Z[Y] = cl[Y];
	Xiw_Z[Z] = cl[Z];
	Gz_Normalize_Vector(Xiw_Z);

	up[X] = render->camera.worldup[X];
	up[Y] = render->camera.worldup[Y];
	up[Z] = render->camera.worldup[Z];
	float up_dot_Z = up[X] * Xiw_Z[X] + up[Y] * Xiw_Z[Y] + up[Z] * Xiw_Z[Z];
	up_prime[X] = up[X] - up_dot_Z * Xiw_Z[X];
	up_prime[Y] = up[Y] - up_dot_Z * Xiw_Z[Y];
	up_prime[Z] = up[Z] - up_dot_Z * Xiw_Z[Z];
	Xiw_Y[X] = up_prime[X];
	Xiw_Y[Y] = up_prime[Y];
	Xiw_Y[Z] = up_prime[Z];
	Gz_Normalize_Vector(Xiw_Y);

	Xiw_X[X] = Xiw_Y[Y] * Xiw_Z[Z] - Xiw_Y[Z] * Xiw_Z[Y];
	Xiw_X[Y] = Xiw_Y[Z] * Xiw_Z[X] - Xiw_Y[X] * Xiw_Z[Z];
	Xiw_X[Z] = Xiw_Y[X] * Xiw_Z[Y] - Xiw_Y[Y] * Xiw_Z[X];
	Gz_Normalize_Vector(Xiw_X);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			render->camera.Xiw[i][j] = 0.0;//initialize all the Xiw
		}
	}
	render->camera.Xiw[0][0] = Xiw_X[X];
	render->camera.Xiw[0][1] = Xiw_X[Y];
	render->camera.Xiw[0][2] = Xiw_X[Z];
	render->camera.Xiw[0][3] = -(Xiw_X[X] * render->camera.position[X] + Xiw_X[Y] * render->camera.position[Y] + Xiw_X[Z] * render->camera.position[Z]);

	render->camera.Xiw[1][0] = Xiw_Y[X];
	render->camera.Xiw[1][1] = Xiw_Y[Y];
	render->camera.Xiw[1][2] = Xiw_Y[Z];
	render->camera.Xiw[1][3] = -(Xiw_Y[X] * render->camera.position[X] + Xiw_Y[Y] * render->camera.position[Y] + Xiw_Y[Z] * render->camera.position[Z]);

	render->camera.Xiw[2][0] = Xiw_Z[X];
	render->camera.Xiw[2][1] = Xiw_Z[Y];
	render->camera.Xiw[2][2] = Xiw_Z[Z];
	render->camera.Xiw[2][3] = -(Xiw_Z[X] * render->camera.position[X] + Xiw_Z[Y] * render->camera.position[Y] + Xiw_Z[Z] * render->camera.position[Z]);

	render->camera.Xiw[3][3] = 1.0;


	/*
	- Compute Xpi
	*/
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			render->camera.Xpi[i][j] = 0.0;//initialize all the Xpi
			if (i == j)
				render->camera.Xpi[i][j] = 1.0;
		}
	}
	render->camera.Xpi[3][2] = tan(render->camera.FOV * Pi / (2 * 180));

	/*
	Initial Ximage
	*/
	GzMatrix Unit_Matrix =
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	render->matlevel = -1;
	GzPushMatrix(render, render->Xsp);
	Gz_Push_Normal_Matrix(render, Unit_Matrix);
	GzPushMatrix(render, render->camera.Xpi);
	Gz_Push_Normal_Matrix(render, Unit_Matrix);
	GzPushMatrix(render, render->camera.Xiw);
	/*
	Initial Xnorm
	*/




	render->open = 1;
	return GZ_SUCCESS;
}

int GzPutCamera(GzRender *render, GzCamera *camera)
{
	/*
	- overwrite renderer camera structure with new camera definition
	*/
	if (render == 0 || camera == 0)
		return GZ_FAILURE;
	//overwrite position lookat and worldup
	render->camera.position[X] = camera->position[X];
	render->camera.position[Y] = camera->position[Y];
	render->camera.position[Z] = camera->position[Z];
	render->camera.lookat[X] = camera->lookat[X];
	render->camera.lookat[Y] = camera->lookat[Y];
	render->camera.lookat[Z] = camera->lookat[Z];
	render->camera.worldup[X] = camera->worldup[X];
	render->camera.worldup[Y] = camera->worldup[Y];
	render->camera.worldup[Z] = camera->worldup[Z];
	render->camera.FOV = camera->FOV;

	Gz_Normalize_Vector(render->camera.worldup);

	//overwrite the Xsp [2][2]
	render->Xsp[2][2] = 2147483647 * tan((render)->camera.FOV * Pi / (2 * 180));

	return GZ_SUCCESS;
}

int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, GzPointer *valueList)
/* void** valuelist */
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
			render->flatcolor[0] = (*(GzColor*)valueList[i])[0];
			render->flatcolor[1] = (*(GzColor*)valueList[i])[1];
			render->flatcolor[2] = (*(GzColor*)valueList[i])[2];
			break;

		case GZ_INTERPOLATE:
			render->interp_mode = *(int *)valueList[i];
			break;

		case GZ_DIRECTIONAL_LIGHT:
			render->lights[render->numlights] = *(GzLight *)valueList[i];
			render->numlights++;
			break;

		case GZ_AMBIENT_LIGHT:
			render->ambientlight = *(GzLight *)valueList[i];
			break;

		case GZ_AMBIENT_COEFFICIENT:
			render->Ka[0] = (*(GzColor *)valueList[i])[0];
			render->Ka[1] = (*(GzColor *)valueList[i])[1];
			render->Ka[2] = (*(GzColor *)valueList[i])[2];
			break;

		case GZ_DIFFUSE_COEFFICIENT:
			render->Kd[0] = (*(GzColor *)valueList[i])[0];
			render->Kd[1] = (*(GzColor *)valueList[i])[1];
			render->Kd[2] = (*(GzColor *)valueList[i])[2];
			break;

		case GZ_SPECULAR_COEFFICIENT:
			render->Ks[0] = (*(GzColor *)valueList[i])[0];
			render->Ks[1] = (*(GzColor *)valueList[i])[1];
			render->Ks[2] = (*(GzColor *)valueList[i])[2];
			break;

		case GZ_DISTRIBUTION_COEFFICIENT:
			render->spec = *(float *)valueList[i];
			break;

		default:
			break;
		}
	}
	return GZ_SUCCESS;
}

int GzPushMatrix(GzRender *render, GzMatrix	matrix)
{
	/*
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/
	if (render == 0 || matrix == 0)
		return GZ_FAILURE;

	render->matlevel++;

	if (render->matlevel == MATLEVELS)
		return GZ_FAILURE;

	memcpy(render->Ximage[render->matlevel], matrix, sizeof(GzMatrix));

	Gz_Push_Normal_Matrix(render, matrix);
	return GZ_SUCCESS;
}

int GzPopMatrix(GzRender *render)
{
	/*
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/

	if (render == 0 || render->matlevel < 0)
		return GZ_FAILURE;

	render->matlevel--;

	return GZ_SUCCESS;
}

int GzPutTriangle(GzRender *render, int	numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
	/* HW2
	- pass in a triangle description with tokens and values corresponding to
	GZ_NULL_TOKEN:		do nothing - no values
	GZ_POSITION:		3 vert positions in model space
	- Invoke the scan converter and return an error code
	*/

	/* HW3
	- pass in a triangle description with tokens and values corresponding to
	GZ_POSITION:3 vert positions in model space
	- Xform positions of verts

	- Clip - just discard any triangle with verts behind view plane
	- test for triangles with all three verts off-screen
	- invoke triangle rasterizer
	*/

	if (render == 0 || nameList == 0 || valueList == 0)
		return GZ_FAILURE;


	if (render->matlevel > 0)
	{
		Gz_Merge_Xsm_Xn(render);
	}
	GzCoord *vertex_after = new GzCoord[3];
	GzCoord *Normal_after = new GzCoord[3];
	//Choose from options
	GzCoord* vertex = new GzCoord[3];
	GzCoord* Normal = new GzCoord[3];
	for (int i = 0; i < numParts; i++)
	{
		GzCoord *vertex = (GzCoord*)valueList[i];
		GzCoord *Normal = (GzCoord*)valueList[i];

		switch (nameList[i])
		{
		case GZ_NULL_TOKEN:
			break;
		case GZ_POSITION:
			if (render->matlevel >= 0)
				Gz_Change_Vertex_Value(vertex, vertex_after, render->Ximage[0]);
			break;
		case GZ_NORMAL:
			if (render->matlevel >= 0)
				Gz_Change_Vertex_Normal(Normal, Normal_after, render->Xnorm[0]);
			break;
		default:
			break;
		}
	}
	


	Gz_LEE_Rasterization(render, vertex_after, Normal_after);
	
	return GZ_SUCCESS;
}



/* NOT part of API - just for general assistance */
short ctoi(float color)		/* convert float color to GzIntensity short */
{
	return(short)((int)(color * ((1 << 12) - 1)));
}

//HW2

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

//HW3
void Gz_Normalize_Vector(GzCoord Vect)
{
	float Norm = sqrt((Vect[X] * Vect[X]) + (Vect[Y] * Vect[Y]) + (Vect[Z] * Vect[Z]));
	Vect[X] /= Norm;
	Vect[Y] /= Norm;
	Vect[Z] /= Norm;
}

int Gz_Change_Vertex_Value(GzCoord v_before[3], GzCoord v_after[3], GzMatrix Mat)
{
	float W;

	for (int i = 0; i < 3; i++)
	{
		v_after[i][X] = Mat[0][0] * v_before[i][X] + Mat[0][1] * v_before[i][Y] + Mat[0][2] * v_before[i][Z] + Mat[0][3] * 1.0;
		v_after[i][Y] = Mat[1][0] * v_before[i][X] + Mat[1][1] * v_before[i][Y] + Mat[1][2] * v_before[i][Z] + Mat[1][3] * 1.0;
		v_after[i][Z] = Mat[2][0] * v_before[i][X] + Mat[2][1] * v_before[i][Y] + Mat[2][2] * v_before[i][Z] + Mat[2][3] * 1.0;
		W = Mat[3][0] * v_before[i][X] + Mat[3][1] * v_before[i][Y] + Mat[3][2] * v_before[i][Z] + Mat[3][3] * 1.0;

		v_after[i][X] /= W;
		v_after[i][Y] /= W;
		v_after[i][Z] /= W;
	}
	return GZ_SUCCESS;
}

//HW4
int Gz_Shading_Eq(GzRender *render, GzColor Color, GzCoord N) {
	if (render == 0)
		return GZ_FAILURE;
	// Needs:C Ks Kd Ka La Le R E s N L
	Gz_Normalize_Vector(N);
	GzCoord E;
	E[X] = 0.0;
	E[Y] = 0.0;
	E[Z] = -1.0;
	Gz_Normalize_Vector(E);
	GzCoord * R = new GzCoord[render->numlights];
	int * flag = new int[render->numlights];//record the relation between N L E

	float NdotL, RdotE;
	float NdotE = Gz_Dot_Product(N, E);//use for relationship with NdotL

	//Compute R = 2(N dot product L)*N - L
	for (int i = 0; i < render->numlights; i++)
	{
		NdotL = Gz_Dot_Product(N, render->lights[i].direction);
		if (NdotE >= 0 && NdotL >= 0)
		{
			flag[i] = 1;
			R[i][X] = 2 * NdotL*N[X] - render->lights[i].direction[X];
			R[i][Y] = 2 * NdotL*N[Y] - render->lights[i].direction[Y];
			R[i][Z] = 2 * NdotL*N[Z] - render->lights[i].direction[Z];
			Gz_Normalize_Vector(R[i]);
		}
		else if (NdotE < 0 && NdotL < 0)
		{
			flag[i] = -1;
			R[i][X] = -2 * NdotL*N[X] - render->lights[i].direction[X];
			R[i][Y] = -2 * NdotL*N[Y] - render->lights[i].direction[Y];
			R[i][Z] = -2 * NdotL*N[Z] - render->lights[i].direction[Z];
			Gz_Normalize_Vector(R[i]);
		}
		else
		{
			flag[i] = 0;
			continue;
		}
	}

	//Compute Specular Color = Ks * sum[Le * pow(R dot E, s)]
	GzColor SpecularColor, SpecularColorTemp;
	for (int i = X; i <= Z; i++)
	{
		SpecularColor[i] = 0.0;
		SpecularColorTemp[i] = 0.0;
	}

	for (int i = 0; i <= render->numlights; i++)
	{
		if (flag[i] == 0)
			continue;//do nothing
		RdotE = Gz_Dot_Product(R[i], E);

		if (RdotE > 1)
			RdotE = 1;
		if (RdotE < 0)
			RdotE = 0;

		SpecularColorTemp[0] += render->lights[i].color[0] * pow(RdotE, render->spec);
		SpecularColorTemp[1] += render->lights[i].color[1] * pow(RdotE, render->spec);
		SpecularColorTemp[2] += render->lights[i].color[2] * pow(RdotE, render->spec);
	}
	for (int i = X; i <= Z; i++)
	{
		SpecularColor[i] = render->Ks[i] * SpecularColorTemp[i];
	}

	//Compute Diffuse Color = Kd * sum[le * N dot L]
	GzColor DiffuseColor, DiffuseColorTemp;
	for (int i = X; i <= Z; i++)
	{
		DiffuseColor[i] = 0.0;
		DiffuseColorTemp[i] = 0.0;
	}
	for (int i = 0; i <= render->numlights; i++)
	{
		if (flag[i] == 0)
			continue;//do nothing
		RdotE = Gz_Dot_Product(R[i], E);
		if (flag[i] == 1)
		{
			DiffuseColorTemp[0] += render->lights[i].color[0] * Gz_Dot_Product(N, render->lights[i].direction);
			DiffuseColorTemp[1] += render->lights[i].color[1] * Gz_Dot_Product(N, render->lights[i].direction);
			DiffuseColorTemp[2] += render->lights[i].color[2] * Gz_Dot_Product(N, render->lights[i].direction);
		}
		if (flag[i] == -1)
		{
			GzCoord NegtiveN = { -N[X], -N[Y], -N[Z] };
			DiffuseColorTemp[0] += render->lights[i].color[0] * Gz_Dot_Product(NegtiveN, render->lights[i].direction);
			DiffuseColorTemp[1] += render->lights[i].color[1] * Gz_Dot_Product(NegtiveN, render->lights[i].direction);
			DiffuseColorTemp[2] += render->lights[i].color[2] * Gz_Dot_Product(NegtiveN, render->lights[i].direction);
		}
	}
	for (int i = X; i <= Z; i++)
	{
		DiffuseColor[i] = render->Kd[i] * DiffuseColorTemp[i];
	}

	//Compute Ambient Color = la * Ka
	GzColor AmbientColor;
	for (int i = X; i <= Z; i++)
	{
		AmbientColor[i] = 0.0;
	}
	for (int i = X; i <= Z; i++)
	{
		AmbientColor[i] = render->Ka[i] * render->ambientlight.color[i];
	}

	//Compute Color C
	for (int i = X; i <= Z; i++)
	{
		Color[i] = SpecularColor[i] + DiffuseColor[i] + AmbientColor[i];
	}
	return GZ_SUCCESS;
}

float Gz_Dot_Product(GzCoord Vect1, GzCoord Vect2)
{
	return Vect1[X] * Vect2[X] + Vect1[Y] * Vect2[Y] + Vect1[Z] * Vect2[Z];
}

int Gz_Push_Normal_Matrix(GzRender *render, GzMatrix matrix)
{
	if (render == 0 || matrix == 0)
		return GZ_FAILURE;
	//Unitary Rotations for Normals
	/*
	use any row/col and compute scale factor
	K = 1 / (a2 + b2 + c2)1/2
	multiply all elements of 3x3 R : R' = K(R)
	R' is normalized (unitary) rotation matrix
	Push R' onto Xn
	*/
	float K = 1 / sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1] + matrix[0][2] * matrix[0][2]);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			matrix[i][j] *= K;
		}
	}
	memcpy(render->Xnorm[render->matlevel], matrix, sizeof(GzMatrix));

	return GZ_SUCCESS;
}

float Gz_In_Triangle_Area(GzCoord v1, GzCoord v2, GzCoord v3)
{
	return abs((v1[X] * v2[Y] + v1[Y] * v3[X] + v2[X] * v3[Y] - v2[Y] * v3[X] - v1[Y] * v2[X] - v1[X] * v3[Y]) / 2);
}

int Gz_Merge_Xsm_Xn(GzRender *render)
{
	GzMatrix Xsm, Xn, Mat_temp, Mat_temp_Xn, Mat_Iterate, Mat_Iterate_Xn;
	//initialize Mat
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Mat_temp[i][j] = 0.0;
			Mat_temp_Xn[i][j] = 0.0;
			if (i == j)
			{
				Mat_temp[i][j] = 1.0;
				Mat_temp_Xn[i][j] = 1.0;
			}
		}
	}
	//merge Xsm
	for (int i = 0; i <= render->matlevel; i++)
	{
		memcpy(Mat_Iterate, render->Ximage[i], sizeof(GzMatrix));
		memcpy(Mat_Iterate_Xn, render->Xnorm[i], sizeof(GzMatrix));
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				Xsm[k][j] = Mat_temp[k][0] * Mat_Iterate[0][j] + Mat_temp[k][1] * Mat_Iterate[1][j] +
					Mat_temp[k][2] * Mat_Iterate[2][j] + Mat_temp[k][3] * Mat_Iterate[3][j];
				Xn[k][j] = Mat_temp_Xn[k][0] * Mat_Iterate_Xn[0][j] + Mat_temp_Xn[k][1] * Mat_Iterate_Xn[1][j] +
					Mat_temp_Xn[k][2] * Mat_Iterate_Xn[2][j] + Mat_temp_Xn[k][3] * Mat_Iterate_Xn[3][j];
			}
		}
		memcpy(Mat_temp, Xsm, sizeof(GzMatrix));
		memcpy(Mat_temp_Xn, Xn, sizeof(GzMatrix));
	}

	//pop off all
	for (int i = render->matlevel; i >= 0; i--)
	{
		GzPopMatrix(render);
	}
	//push in Xsm
	GzPushMatrix(render, Xsm);
	Gz_Push_Normal_Matrix(render, Xn);
	return GZ_SUCCESS;
}

int Gz_Change_Vertex_Normal(GzCoord n_before[3], GzCoord n_after[3], GzMatrix Mat)
{
	for (int i = 0; i < 3; i++)
	{
		n_after[i][X] = Mat[0][0] * n_before[i][X] + Mat[0][1] * n_before[i][Y] + Mat[0][2] * n_before[i][Z];
		n_after[i][Y] = Mat[1][0] * n_before[i][X] + Mat[1][1] * n_before[i][Y] + Mat[1][2] * n_before[i][Z];
		n_after[i][Z] = Mat[2][0] * n_before[i][X] + Mat[2][1] * n_before[i][Y] + Mat[2][2] * n_before[i][Z];
	}
	for (int i = 0; i < 3; i++)
	{
		Gz_Normalize_Vector(n_after[i]);
	}
	return GZ_SUCCESS;
}

int Gz_LEE_Rasterization(GzRender *render, GzCoord vertex[3], GzCoord Normal[3])
{
	if (render == 0)
		return GZ_FAILURE;
	GzIntensity Red, Green, Blue, alpha;

	for (int i = 0; i < 3; i++)
	{
		vertex[i][X] += render->Offset_x;
		vertex[i][Y] += render->Offset_y;
	}

	//bubble sort
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (vertex[j][Y] > vertex[j + 1][Y])
			{
				Gz_Swap_Vertex(vertex[j], vertex[j + 1]);
				Gz_Swap_Vertex(Normal[j], Normal[j + 1]);
			}
		}
	}

	GzCoord Vector_1_to_2;
	GzCoord Vector_2_to_3;
	GzCoord Vector_3_to_1;
	GzColor Color_V1, Color_V2, Color_V3;
	Gz_Shading_Eq(render, Color_V1, Normal[0]);
	Gz_Shading_Eq(render, Color_V2, Normal[1]);
	Gz_Shading_Eq(render, Color_V3, Normal[2]);

	Vector_1_to_2[X] = vertex[1][X] - vertex[0][X];
	Vector_1_to_2[Y] = vertex[1][Y] - vertex[0][Y];
	Vector_1_to_2[Z] = vertex[1][Z] - vertex[0][Z];

	Vector_2_to_3[X] = vertex[2][X] - vertex[1][X];
	Vector_2_to_3[Y] = vertex[2][Y] - vertex[1][Y];
	Vector_2_to_3[Z] = vertex[2][Z] - vertex[1][Z];

	Vector_3_to_1[X] = vertex[0][X] - vertex[2][X];
	Vector_3_to_1[Y] = vertex[0][Y] - vertex[2][Y];
	Vector_3_to_1[Z] = vertex[0][Z] - vertex[2][Z];
	//We use + - area in this 3 vectors in common!
	//Because the coords may not be integer! But pixels are at integers! X grows right, Y grows down.
	int Xmin = (int)floor(Find_x_min(vertex));
	int Xmax = (int)ceil(Find_x_max(vertex));

	int Ymin = (int)floor(vertex[0][Y]);
	int Ymax = (int)ceil(vertex[2][Y]);

	//if some vertex locates on out of screen
	if (Xmin < 0)
		Xmin = 0;
	if (Xmax > render->display->xres)
		Xmax = render->display->xres;
	if (Ymin < 0)
		Ymin = 0;
	if (Ymax > render->display->yres)
		Ymax = render->display->yres;


	//Calculate the Ax+By+Cz+D=0 A B C D use vectors

	float A;
	float B;
	float C;
	float D;

	A = Vector_1_to_2[Y] * (-Vector_3_to_1[Z]) - (-Vector_3_to_1[Y]) * Vector_1_to_2[Z];
	B = Vector_1_to_2[Z] * (-Vector_3_to_1[X]) - (-Vector_3_to_1[Z]) * Vector_1_to_2[X];
	C = Vector_1_to_2[X] * (-Vector_3_to_1[Y]) - (-Vector_3_to_1[X]) * Vector_1_to_2[Y];
	D = -(A * vertex[0][X] + B * vertex[0][Y] + C * vertex[0][Z]);


	float Area = Gz_In_Triangle_Area(vertex[0], vertex[1], vertex[2]);

	// DRAW PIXELS
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

			GzCoord p = { i, j, 1.0};

			if ((Relationship_with_1_to_2 == 0 || Relationship_with_2_to_3 == 0 || Relationship_with_3_to_1 == 0) ||
				(Relationship_with_1_to_2 > 0 && Relationship_with_2_to_3 > 0 && Relationship_with_3_to_1 > 0) ||
				(Relationship_with_1_to_2 < 0 && Relationship_with_2_to_3 < 0 && Relationship_with_3_to_1 < 0)) 
			{
				GzGetDisplay(render->display, i, j, &Red, &Green, &Blue, &alpha, &tempz);

				if (Pixel_z < tempz && Pixel_z > render->camera.position[Z])
				{
					if (render->interp_mode == GZ_COLOR)
					{
						float V1 = Gz_In_Triangle_Area(vertex[1], p, vertex[2]);
						float V2 = Gz_In_Triangle_Area(vertex[0], p, vertex[2]);
						float V3 = Gz_In_Triangle_Area(vertex[0], p, vertex[1]);

						float tempRed = (V1*Color_V1[0] + V2*Color_V2[0] + V3*Color_V3[0]) / Area;
						float tempGreen = (V1*Color_V1[1] + V2*Color_V2[1] + V3*Color_V3[1]) / Area;
						float tempBlue = (V1*Color_V1[2] + V2*Color_V2[2] + V3*Color_V3[2]) / Area;

						if (tempRed > 1.0) 
							tempRed = 1.0;
						if (tempGreen > 1.0)
							tempGreen = 1.0;
						if (tempBlue > 1.0)
							tempBlue = 1.0;
						Red = ctoi(tempRed);
						Green = ctoi(tempGreen);
						Blue = ctoi(tempBlue);
					}
					else if (render->interp_mode == GZ_NORMAL)
					{
						float V1 = Gz_In_Triangle_Area(vertex[1], p, vertex[2]);
						float V2 = Gz_In_Triangle_Area(vertex[0], p, vertex[2]);
						float V3 = Gz_In_Triangle_Area(vertex[0], p, vertex[1]);

						GzCoord pN;
						pN[X] = (V1*Normal[0][X] + V2*Normal[1][X] + V3*Normal[2][X]) / Area;
						pN[Y] = (V1*Normal[0][Y] + V2*Normal[1][Y] + V3*Normal[2][Y]) / Area;
						pN[Z] = (V1*Normal[0][Z] + V2*Normal[1][Z] + V3*Normal[2][Z]) / Area;
						Gz_Normalize_Vector(pN);

						GzColor Color;
						Gz_Shading_Eq(render, Color, pN);

						Red = ctoi(Color[0]);
						Green = ctoi(Color[1]);
						Blue = ctoi(Color[2]);
					}
					else
					{
						Red = ctoi(render->flatcolor[0]);
						Green = ctoi(render->flatcolor[1]);
						Blue = ctoi(render->flatcolor[2]);
					}
					GzPutDisplay(render->display, i, j, Red, Green, Blue, alpha, Pixel_z);
				}
			}
		}
	}
	return 0;
}

//HW6