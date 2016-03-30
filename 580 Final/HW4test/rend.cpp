#include <string.h>
#include <math.h>
#include "rend.h"

#define DEGREE2RADIAN   0.01745329252f

int GzRotXMat(float degree, GzMatrix mat)
{
	memset(mat, 0, sizeof(GzMatrix));
	mat[0][0] = 1.f;
	mat[1][1] = cosf(degree * DEGREE2RADIAN);
	mat[1][2] = -sinf(degree * DEGREE2RADIAN);
	mat[2][1] = -mat[1][2];
	mat[2][2] = mat[1][1];
	mat[3][3] = 1.f;
	return GZ_SUCCESS;
}


int GzRotYMat(float degree, GzMatrix mat)
{
	memset(mat, 0, sizeof(GzMatrix));
	mat[0][0] = cosf(degree * DEGREE2RADIAN);
	mat[0][2] = sinf(degree * DEGREE2RADIAN);
	mat[1][1] = 1.f;
	mat[2][0] = -mat[0][2];
	mat[2][2] = mat[0][0];
	mat[3][3] = 1.f;
	return GZ_SUCCESS;
}


int GzRotZMat(float degree, GzMatrix mat)
{
	memset(mat, 0, sizeof(GzMatrix));
	mat[0][0] = cosf(degree * DEGREE2RADIAN);
	mat[0][1] = -sinf(degree * DEGREE2RADIAN);
	mat[1][0] = -mat[0][1];
	mat[1][1] = mat[0][0];
	mat[2][2] = 1.f;
	mat[3][3] = 1.f;
	return GZ_SUCCESS;
}


int GzTrxMat(GzCoord translate, GzMatrix mat)
{
	memset(mat, 0, sizeof(GzMatrix));
	mat[0][0] = 1.f;
	mat[X][3] = translate[X];
	mat[1][1] = 1.f;
	mat[Y][3] = translate[Y];
	mat[2][2] = 1.f;
	mat[Z][3] = translate[Z];
	mat[3][3] = 1.f;
	return GZ_SUCCESS;
}


int GzScaleMat(GzCoord scale, GzMatrix mat)
{
	memset(mat, 0, sizeof(GzMatrix));
	mat[X][X] = scale[X];
	mat[Y][Y] = scale[Y];
	mat[Z][Z] = scale[Z];
	mat[3][3] = 1.f;
	return GZ_SUCCESS;
}


//----------------------------------------------------------
// Begin main functions

int GzNewRender(GzRender **render, GzRenderClass renderClass, GzDisplay	*display)
{
	GzRender *pThis = new GzRender();
	if (pThis == NULL)
	{
		*render = NULL;
		return GZ_FAILURE;
	}
	*render = pThis;
	pThis->renderClass = renderClass;
	pThis->display = display;
	pThis->open = 0;
	/* Matrix transforms */
	pThis->camera.position[X] = DEFAULT_IM_X;
	pThis->camera.position[Y] = DEFAULT_IM_Y;
	pThis->camera.position[Z] = DEFAULT_IM_Z;
	pThis->camera.lookat[X] = 0.f;
	pThis->camera.lookat[Y] = 0.f;
	pThis->camera.lookat[Z] = 0.f;
	pThis->camera.worldup[X] = 0.f;
	pThis->camera.worldup[Y] = 1.f;
	pThis->camera.worldup[Z] = 0.f;
	pThis->camera.FOV = DEFAULT_FOV;
	pThis->matlevel = -1;
	memset(pThis->Xsp, 0, sizeof(pThis->Xsp));
	pThis->Xsp[0][0] = display->xres * 0.5f;
	pThis->Xsp[0][3] = pThis->Xsp[0][0];
	pThis->Xsp[1][1] = -display->yres * 0.5f;
	pThis->Xsp[1][3] = -pThis->Xsp[1][1];
	pThis->Xsp[2][2] = 1.f;
	pThis->Xsp[3][3] = 1.f;
	/* Lighting & shading */
	GzColor tocopy[3] = { DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR };
	memcpy(pThis->Ka, tocopy[0], sizeof(GzColor));
	memcpy(pThis->Kd, tocopy[1], sizeof(GzColor));
	memcpy(pThis->Ks, tocopy[2], sizeof(GzColor));
	return GZ_SUCCESS;
}


int GzFreeRender(GzRender *render)
{
	if (render == NULL) return GZ_FAILURE;
	delete render;
	return GZ_SUCCESS;
}


static inline void
NormalNormal(GzCoord &This, bool precise = true)
{
	float sum = This[X] * This[X] + This[Y] * This[Y] + This[Z] * This[Z];
	if (precise || (sizeof(int) != sizeof(float))) sum = 1.f / sqrtf(sum);
	else
	{
		float half = sum * 0.5f;
		int i = *(int *)&sum;
		i = 0x5f3759df - (i >> 1);
		sum = *(float *)&i;
		sum *= (1.5f - (half * sum * sum));
		sum *= (1.5f - (half * sum * sum));
	}
	This[X] *= sum;
	This[Y] *= sum;
	This[Z] *= sum;
}

static inline float
NormalDotProduct(GzCoord &A, GzCoord &B)
{
	return (A[X] * B[X] + A[Y] * B[Y] + A[Z] * B[Z]);
}
static inline float
NormalDotProduct(GzMatrix &A, int row, GzCoord &B)
{
	return (A[row][X] * B[X] + A[row][Y] * B[Y] + A[row][Z] * B[Z]);
}


int GzBeginRender(GzRender *render)
{
    GzCoord test = {1.0e19,0,0};
    NormalNormal(test, false);
    
	if (render == NULL)
	{
		return GZ_FAILURE;
	}
	render->open = 1 & render->display->open;
	float recipd = tanf(render->camera.FOV * DEGREE2RADIAN * 0.5f);
	render->Xsp[2][2] = 0x7FFFFFFF * recipd;

	/* compute Xpi */
	memset(render->camera.Xpi, 0, sizeof(GzMatrix));
	render->camera.Xpi[0][0] = 1.f;
	render->camera.Xpi[1][1] = 1.f;
	render->camera.Xpi[2][2] = 1.f;
	render->camera.Xpi[3][2] = recipd;
	render->camera.Xpi[3][3] = 1.f;

	/* compute Xiw */
	GzCoord camZ = {
		render->camera.lookat[X] - render->camera.position[X],
		render->camera.lookat[Y] - render->camera.position[Y],
		render->camera.lookat[Z] - render->camera.position[Z]
	};
	NormalNormal(camZ);
	float dotYZ = NormalDotProduct(render->camera.worldup, camZ);
	GzCoord camY = {
		render->camera.worldup[X] - (dotYZ * camZ[X]),
		render->camera.worldup[Y] - (dotYZ * camZ[Y]),
		render->camera.worldup[Z] - (dotYZ * camZ[Z])
	};
	NormalNormal(camY);

	render->camera.Xiw[0][X] = camY[Y] * camZ[Z] - camY[Z] * camZ[Y];
	render->camera.Xiw[0][Y] = camY[Z] * camZ[X] - camY[X] * camZ[Z];
	render->camera.Xiw[0][Z] = camY[X] * camZ[Y] - camY[Y] * camZ[X];
	render->camera.Xiw[0][3] = -NormalDotProduct(render->camera.Xiw, 0, render->camera.position);
	memcpy(render->camera.Xiw[1], camY, sizeof(GzCoord));
	render->camera.Xiw[1][3] = -NormalDotProduct(render->camera.Xiw, 1, render->camera.position);
	memcpy(render->camera.Xiw[2], camZ, sizeof(GzCoord));
	render->camera.Xiw[2][3] = -NormalDotProduct(render->camera.Xiw, 2, render->camera.position);
	memset(render->camera.Xiw[3], 0, sizeof(GzCoord));
	render->camera.Xiw[3][3] = 1.f;

	render->matlevel = -1;
	GzPushMatrix(render, render->camera.Xpi);
	GzPushMatrix(render, render->camera.Xiw);

	render->numlights = -1;
	render->flatcolor[RED] = 1.f;
	render->flatcolor[GREEN] = 1.f;
	render->flatcolor[BLUE] = 1.f;

	return GZ_SUCCESS;
}

int GzPutCamera(GzRender *render, GzCamera *camera)
{
	render->open = 0;
	memcpy(&(render->camera), camera, sizeof(GzCamera));
	return GZ_SUCCESS;
}

int GzPushMatrix(GzRender *render, GzMatrix matrix)
{
	if ((render->matlevel + 1) == MATLEVELS)
	{
		return GZ_FAILURE;
	}
	render->matlevel++;
	GzMatrix &vert = render->Xvert[render->matlevel];
	if (render->matlevel > 0)
	{
		GzMatrix &last = render->Xvert[render->matlevel - 1];
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
			{
				vert[y][x] = last[y][0] * matrix[0][x] + last[y][1] * matrix[1][x] +
					last[y][2] * matrix[2][x] + last[y][3] * matrix[3][x];
			}
	}
	else
	{
		memcpy(vert, matrix, sizeof(GzMatrix));
	}
	/* Compute norm=inv(vert.') */
	GzMatrix &norm = render->Xnorm[render->matlevel];
	norm[0][0] = vert[1][1] * vert[2][2] - vert[1][2] * vert[2][1];
	norm[0][1] = vert[1][2] * vert[2][0] - vert[1][0] * vert[2][2];
	norm[0][2] = vert[1][0] * vert[2][1] - vert[1][1] * vert[2][0];
	norm[1][0] = vert[0][2] * vert[2][1] - vert[0][1] * vert[2][2];
	norm[1][1] = vert[0][0] * vert[2][2] - vert[0][2] * vert[2][0];
	norm[1][2] = vert[0][1] * vert[2][0] - vert[0][0] * vert[2][1];
	norm[2][0] = vert[0][1] * vert[1][2] - vert[0][2] * vert[1][1];
	norm[2][1] = vert[0][2] * vert[1][0] - vert[0][0] * vert[1][2];
	norm[2][2] = vert[0][0] * vert[1][1] - vert[0][1] * vert[1][0];
	return GZ_SUCCESS;
}

int GzPopMatrix(GzRender *render)
{
	if (render->matlevel < 0) return GZ_FAILURE;
	render->matlevel--;
	return GZ_SUCCESS;
}

int GzPushLight(GzRender *render, GzLight light)
{
	if ((render->numlights + 1) == MAX_LIGHTS)
	{
		return GZ_FAILURE;
	}
	render->numlights++;
	memcpy(&(render->lights[render->numlights]), &light, sizeof(GzLight));
	return GZ_SUCCESS;
}

int GzPopLight(GzRender *render)
{
	if (render->numlights < 0) return GZ_FAILURE;
	render->numlights--;
	return GZ_SUCCESS;
}


int GzPutAttribute(GzRender *render, int numAttributes, GzToken *nameList, GzPointer *valueList)
{
	if ((render == NULL) || (render->open == 0)) return GZ_FAILURE;
	int status = 0;
	while (numAttributes > 0)
	{
		numAttributes--;
		switch (nameList[numAttributes])
		{
		case GZ_RGB_COLOR:
		{
			GzColor *color = (GzColor *)valueList[numAttributes];
			render->flatcolor[RED] = (*color)[RED];
			render->flatcolor[GREEN] = (*color)[GREEN];
			render->flatcolor[BLUE] = (*color)[BLUE];
			break;
		}
		case GZ_DIRECTIONAL_LIGHT:
		{
			GzLight *light = (GzLight *)valueList[numAttributes];
			if ((render->numlights + 1) == MAX_LIGHTS)
			{
				status |= GZ_FAILURE;
			}
			else
			{
				render->numlights++;
				memcpy(&(render->lights[render->numlights]), light, sizeof(GzLight));
			}
			break;
		}
		case GZ_AMBIENT_LIGHT:
		{
			GzLight *light = (GzLight *)valueList[numAttributes];
			memcpy(&(render->ambientlight), light, sizeof(GzLight));
			break;
		}
		case GZ_AMBIENT_COEFFICIENT:
		{
			GzColor *color = (GzColor *)valueList[numAttributes];
			memcpy(&(render->Ka), color, sizeof(GzColor));
			break;
		}
		case GZ_DIFFUSE_COEFFICIENT:
		{
			GzColor *color = (GzColor *)valueList[numAttributes];
			memcpy(&(render->Kd), color, sizeof(GzColor));
			break;
		}
		case GZ_SPECULAR_COEFFICIENT:
		{
			GzColor *color = (GzColor *)valueList[numAttributes];
			memcpy(&(render->Ks), color, sizeof(GzColor));
			break;
		}
		case GZ_DISTRIBUTION_COEFFICIENT:
		{
			float *power = (float *)valueList[numAttributes];
			render->spec = *power;
			break;
		}
		case GZ_INTERPOLATE:
		{
			int *style = (int *)valueList[numAttributes];
			render->interp_mode = *style;
			break;
		}
		default:
		{
			status |= GZ_FAILURE;
			break;
		}
		}
	}
	return status;
}


/* Begin draw functions */
static inline void
GzCompute_Color(GzRender *render, float Vx, float Vy, GzCoord &Normal, GzColor &color)
{
	color[RED] = render->ambientlight.color[RED] * render->Ka[RED];
	color[GREEN] = render->ambientlight.color[GREEN] * render->Ka[GREEN];
	color[BLUE] = render->ambientlight.color[BLUE] * render->Ka[BLUE];
	/* compute E */
	GzCoord E =
	{
		-((Vx - render->Xsp[0][3]) / render->Xsp[0][0] * render->camera.Xpi[3][2]),
		-((Vy - render->Xsp[1][3]) / render->Xsp[1][1] * render->camera.Xpi[3][2]),
		-1
	};
	NormalNormal(E, false);
	for (int i = render->numlights; i >= 0; i--)
	{
		GzCoord &L = render->lights[i].direction;
		GzColor &Le = render->lights[i].color;
		float NL = NormalDotProduct(Normal, L);
		float NE = NormalDotProduct(Normal, E);
		bool flipnormal = (NL < 0.f) && (NE < 0.f);
		if (((NL > 0.f) && (NE > 0.f)) || flipnormal)
		{
			color[RED] += Le[RED] * render->Kd[RED] * (flipnormal ? -NL : NL);
			color[GREEN] += Le[GREEN] * render->Kd[GREEN] * (flipnormal ? -NL : NL);
			color[BLUE] += Le[BLUE] * render->Kd[BLUE] * (flipnormal ? -NL : NL);
			GzCoord R =
			{
				(NL + NL) * Normal[X] - L[X],
				(NL + NL) * Normal[Y] - L[Y],
				(NL + NL) * Normal[Z] - L[Z]
			};
			float RE = NormalDotProduct(R, E);
			if (RE > 0.f)
			{
				RE = powf(RE, render->spec);
				color[RED] += Le[RED] * render->Ks[RED] * RE;
				color[GREEN] += Le[GREEN] * render->Ks[GREEN] * RE;
				color[BLUE] += Le[BLUE] * render->Ks[BLUE] * RE;
			}
		}
	}
}


static inline int
GzDraw_Triangle_LEE(GzRender *render, GzCoord(&Vertex)[3], GzCoord(&Normal)[3])
{
	GzCoord NdcNorm[3];
	if (render->matlevel >= 0)
	{
		GzMatrix &vert = render->Xvert[render->matlevel];
		GzMatrix &norm = render->Xnorm[render->matlevel];
		for (int i = 0; i < 3; i++)
		{
			float cvert[4] =
			{
				NormalDotProduct(vert, 0, Vertex[i]) + vert[0][3],
				NormalDotProduct(vert, 1, Vertex[i]) + vert[1][3],
				NormalDotProduct(vert, 2, Vertex[i]) + vert[2][3],
				NormalDotProduct(vert, 3, Vertex[i]) + vert[3][3]
			};
			/* Xsp transform */
			Vertex[i][X] = (cvert[0] / cvert[3]) * render->Xsp[0][0] + render->Xsp[0][3];
			Vertex[i][Y] = (cvert[1] / cvert[3]) * render->Xsp[1][1] + render->Xsp[1][3];
			Vertex[i][Z] = (cvert[2] / cvert[3]) * render->Xsp[2][2];
			/* projected Z clip */
			if ((Vertex[i][Z] < 0) || (Vertex[i][Z] > (float)0x7FFFFFFF))
			{
				return GZ_SUCCESS;
			}
            if (Normal == NULL) continue;
			NdcNorm[i][0] = NormalDotProduct(norm, 0, Normal[i]);
			NdcNorm[i][1] = NormalDotProduct(norm, 1, Normal[i]);
			NdcNorm[i][2] = NormalDotProduct(norm, 2, Normal[i]);
			NormalNormal(NdcNorm[i]);
		}
	}
	/* Draw bounding check */
	int Xmin = render->display->xres, Xmax = -1;
	int Ymin = render->display->yres, Ymax = -1;
	for (int i = 0; i < 3; i++)
	{
		if (Xmin > Vertex[i][X]) Xmin = (int)Vertex[i][X];
		if (Xmax < Vertex[i][X]) Xmax = (int)Vertex[i][X];
		if (Ymin > Vertex[i][Y]) Ymin = (int)Vertex[i][Y];
		if (Ymax < Vertex[i][Y]) Ymax = (int)Vertex[i][Y];
	}
	Xmin = (Xmin < 0) ? 0 : Xmin;
	Xmax = (Xmax >= render->display->xres) ? (render->display->xres - 1) : Xmax;
	Ymin = (Ymin < 0) ? 0 : Ymin;
	Ymax = (Ymax >= render->display->yres) ? (render->display->yres - 1) : Ymax;
	if ((Xmin > Xmax) || (Ymin > Ymax)) return GZ_SUCCESS;

	/*
	* Line Equations	0-2: AB,AC,BC lines
	* Interpolations	3: Z	4: red or Nx	5: green or Ny	6: blue or Nz
	*/
#define XCEF_INTERPOLATE(Va, Vb, Vc)	(Va * Xcef[1] + Vb * Xcef[2] + Vc * Xcef[0])\
	/(Vertex[0][X] * Xcef[1] + Vertex[1][X] * Xcef[2] + Vertex[2][X] * Xcef[0])
#define YCEF_INTERPOLATE(Va, Vb, Vc)	(Va * Ycef[1] + Vb * Ycef[2] + Vc * Ycef[0])\
	/(Vertex[0][Y] * Ycef[1] + Vertex[1][Y] * Ycef[2] + Vertex[2][Y] * Ycef[0])
#define D_INTERPOLATE(Va, Vb, Vc, Xcef, Ycef)	(0.33333333333f * (Va + Vb + Vc\
	- Xcef * (Vertex[0][X] + Vertex[1][X] + Vertex[2][X])\
	- Ycef * (Vertex[0][Y] + Vertex[1][Y] + Vertex[2][Y])))
	float Xcef[7] =
	{
		Vertex[0][Y] - Vertex[1][Y],
		Vertex[1][Y] - Vertex[2][Y],
		Vertex[2][Y] - Vertex[0][Y],
		XCEF_INTERPOLATE(Vertex[0][Z], Vertex[1][Z], Vertex[2][Z])
	};
	float Ycef[7] =
	{
		Vertex[1][X] - Vertex[0][X],
		Vertex[2][X] - Vertex[1][X],
		Vertex[0][X] - Vertex[2][X],
		YCEF_INTERPOLATE(Vertex[0][Z], Vertex[1][Z], Vertex[2][Z])
	};
	float EvalStart[7] =
	{
		Xcef[0] * Xmin + Ycef[0] * Ymin + Vertex[0][X] * Vertex[1][Y] - Vertex[1][X] * Vertex[0][Y],
		Xcef[1] * Xmin + Ycef[1] * Ymin + Vertex[1][X] * Vertex[2][Y] - Vertex[2][X] * Vertex[1][Y],
		Xcef[2] * Xmin + Ycef[2] * Ymin + Vertex[2][X] * Vertex[0][Y] - Vertex[0][X] * Vertex[2][Y],
		Xcef[3] * Xmin + Ycef[3] * Ymin + D_INTERPOLATE(Vertex[0][Z], Vertex[1][Z], Vertex[2][Z], Xcef[3], Ycef[3])
	};
	switch (render->interp_mode)
	{
	case GZ_COLOR:
	{
		GzColor colors[3];
		GzCompute_Color(render, Vertex[0][X], Vertex[0][Y], NdcNorm[0], colors[0]);
		GzCompute_Color(render, Vertex[1][X], Vertex[1][Y], NdcNorm[1], colors[1]);
		GzCompute_Color(render, Vertex[2][X], Vertex[2][Y], NdcNorm[2], colors[2]);
		/* X interpolation of RGB */
		Xcef[4] = XCEF_INTERPOLATE(colors[0][RED], colors[1][RED], colors[2][RED]);
		Xcef[5] = XCEF_INTERPOLATE(colors[0][GREEN], colors[1][GREEN], colors[2][GREEN]);
		Xcef[6] = XCEF_INTERPOLATE(colors[0][BLUE], colors[1][BLUE], colors[2][BLUE]);
		/* Y interpolation of RGB  */
		Ycef[4] = YCEF_INTERPOLATE(colors[0][RED], colors[1][RED], colors[2][RED]);
		Ycef[5] = YCEF_INTERPOLATE(colors[0][GREEN], colors[1][GREEN], colors[2][GREEN]);
		Ycef[6] = YCEF_INTERPOLATE(colors[0][BLUE], colors[1][BLUE], colors[2][BLUE]);
		/* Initiate value of RGB interpolation */
		EvalStart[4] = Xcef[4] * Xmin + Ycef[4] * Ymin + D_INTERPOLATE\
			(colors[0][RED], colors[1][RED], colors[2][RED], Xcef[4], Ycef[4]);
		EvalStart[5] = Xcef[5] * Xmin + Ycef[5] * Ymin + D_INTERPOLATE\
			(colors[0][GREEN], colors[1][GREEN], colors[2][GREEN], Xcef[5], Ycef[5]);
		EvalStart[6] = Xcef[6] * Xmin + Ycef[6] * Ymin + D_INTERPOLATE\
			(colors[0][BLUE], colors[1][BLUE], colors[2][BLUE], Xcef[6], Ycef[6]);
		break;
	}
	case GZ_NORMALS:
	{
		/* X interpolation of NdcNorm */
		Xcef[4] = XCEF_INTERPOLATE(NdcNorm[0][X], NdcNorm[1][X], NdcNorm[2][X]);
		Xcef[5] = XCEF_INTERPOLATE(NdcNorm[0][Y], NdcNorm[1][Y], NdcNorm[2][Y]);
		Xcef[6] = XCEF_INTERPOLATE(NdcNorm[0][Z], NdcNorm[1][Z], NdcNorm[2][Z]);
		/* Y interpolation of NdcNorm */
		Ycef[4] = YCEF_INTERPOLATE(NdcNorm[0][X], NdcNorm[1][X], NdcNorm[2][X]);
		Ycef[5] = YCEF_INTERPOLATE(NdcNorm[0][Y], NdcNorm[1][Y], NdcNorm[2][Y]);
		Ycef[6] = YCEF_INTERPOLATE(NdcNorm[0][Z], NdcNorm[1][Z], NdcNorm[2][Z]);
		/* Initiate value of NdcNorm interpolation */
		EvalStart[4] = Xcef[4] * Xmin + Ycef[4] * Ymin + D_INTERPOLATE\
			(NdcNorm[0][X], NdcNorm[1][X], NdcNorm[2][X], Xcef[4], Ycef[4]);
		EvalStart[5] = Xcef[5] * Xmin + Ycef[5] * Ymin + D_INTERPOLATE\
			(NdcNorm[0][Y], NdcNorm[1][Y], NdcNorm[2][Y], Xcef[5], Ycef[5]);
		EvalStart[6] = Xcef[6] * Xmin + Ycef[6] * Ymin + D_INTERPOLATE\
			(NdcNorm[0][Z], NdcNorm[1][Z], NdcNorm[2][Z], Xcef[6], Ycef[6]);
		break;
	}
	default:
	{
		Xcef[4] = 0.f;
		Xcef[5] = 0.f;
		Xcef[6] = 0.f;
		Ycef[4] = 0.f;
		Ycef[5] = 0.f;
		Ycef[6] = 0.f;
		EvalStart[4] = 0.f;
		EvalStart[5] = 0.f;
		EvalStart[6] = 0.f;
		break;
	}
	}
	bool ntoprow = false;
	for (; Ymin <= Ymax; Ymin++)
	{
		float EvalNow[7];
		memcpy(EvalNow, EvalStart, sizeof(EvalNow));
		for (int Xnow = Xmin; Xnow <= Xmax; Xnow++)
		{
			bool Flag[3][2] =
			{
				{ EvalNow[0] <= 0.f, EvalNow[0] >= 0.f },
				{ EvalNow[1] <= 0.f, EvalNow[1] >= 0.f },
				{ EvalNow[2] <= 0.f, EvalNow[2] >= 0.f }
			};
			bool uniFlag[2] =
			{
				Flag[0][0] && Flag[1][0] && Flag[2][0],
				Flag[0][1] && Flag[1][1] && Flag[2][1]
			};
			if (uniFlag[0] || uniFlag[1])
			{
				if (uniFlag[0] && uniFlag[1] && ntoprow && (Xnow != Xmin))
				{
					/*Only draw top left part*/
					continue;
				}
				switch (render->interp_mode)
				{
				case GZ_COLOR:
				{
					GzPutDisplay(render->display,
						Xnow,
						Ymin,
						(GzIntensity)(EvalNow[4] * 4095.f),
						(GzIntensity)(EvalNow[5] * 4095.f),
						(GzIntensity)(EvalNow[6] * 4095.f),
						0,
						(GzDepth)EvalNow[3]);
					break;
				}
				case GZ_NORMALS:
				{
					GzColor Ncolor;
					GzCoord Nnow = { EvalNow[4], EvalNow[5], EvalNow[6] };
					NormalNormal(Nnow, false);
					GzCompute_Color(render, (float)Xnow, (float)Ymin, Nnow, Ncolor);
					GzPutDisplay(render->display,
						Xnow,
						Ymin,
						(GzIntensity)(Ncolor[RED] * 4095.f),
						(GzIntensity)(Ncolor[GREEN] * 4095.f),
						(GzIntensity)(Ncolor[BLUE] * 4095.f),
						0,
						(GzDepth)EvalNow[3]);
					break;
				}
				default:
				{
					GzPutDisplay(render->display,
						Xnow,
						Ymin,
						(GzIntensity)(render->flatcolor[RED] * 4095.f),
						(GzIntensity)(render->flatcolor[GREEN] * 4095.f),
						(GzIntensity)(render->flatcolor[BLUE] * 4095.f),
						0,
						(GzDepth)EvalNow[3]);
					break;
				}
				}
			}
			EvalNow[0] += Xcef[0];
			EvalNow[1] += Xcef[1];
			EvalNow[2] += Xcef[2];
			EvalNow[3] += Xcef[3];
			EvalNow[4] += Xcef[4];
			EvalNow[5] += Xcef[5];
			EvalNow[6] += Xcef[6];
		}
		EvalStart[0] += Ycef[0];
		EvalStart[1] += Ycef[1];
		EvalStart[2] += Ycef[2];
		EvalStart[3] += Ycef[3];
		EvalStart[4] += Ycef[4];
		EvalStart[5] += Ycef[5];
		EvalStart[6] += Ycef[6];
		ntoprow = true;
	}
	return GZ_SUCCESS;
}


int GzPutTriangle(GzRender *render, int numParts, GzToken *nameList, GzPointer *valueList)
{
	if ((render == NULL) || (render->open == 0)) return GZ_FAILURE;
	GzCoord(*Vertex)[3] = NULL;
	GzCoord(*Normal)[3] = NULL;
	int status = 0;
	while (numParts > 0)
	{
		numParts--;
		switch (nameList[numParts])
		{
		case GZ_POSITION:
		{
			Vertex = (GzCoord(*)[3])valueList[numParts];
			break;
		}
		case GZ_NORMAL:
		{
			Normal = (GzCoord(*)[3])valueList[numParts];
			break;
		}
		default:
		{
			status |= GZ_FAILURE;
			break;
		}
		}
	}
	if (Vertex != NULL)
	{
		GzDraw_Triangle_LEE(render, *Vertex, *Normal);
	}
	return status;
}