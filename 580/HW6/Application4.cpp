// Application4.cpp: implementation of the Application4 class.
//
//////////////////////////////////////////////////////////////////////

/*
 * application test code for homework assignment
 */

#include "stdafx.h"
#include "CS580HW.h"
#include "Application4.h"
#include "Gz.h"
#include "disp.h"
#include "rend.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define INFILE  "pot4.asc"
#define OUTFILE "output.ppm"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Application4::Application4()
{

}

Application4::~Application4()
{

}

int Application4::Initialize()
{
	/* to be filled in by the app if it sets camera params */

	GzCamera	camera;
	int		    xRes, yRes, dispClass;	/* display parameters */

	GzToken		nameListShader[9]; 	/* shader attribute names */
	GzPointer   valueListShader[9];		/* shader attribute pointers */
	GzToken     nameListLights[10];		/* light info */
	GzPointer   valueListLights[10];
	int			shaderType, interpStyle;
	float		specpower;
	int		    status;

	status = 0;

	/*
	 * Allocate memory for user input
	 */
	m_pUserInput = new GzInput;

	/*
	 * initialize the display and the renderer
	 */

	m_nWidth = 256;		// frame buffer and display width
	m_nHeight = 256;    // frame buffer and display height

	status |= GzNewFrameBuffer(&m_pFrameBuffer, m_nWidth, m_nHeight);

	status |= GzNewDisplay(&m_pDisplay, GZ_RGBAZ_DISPLAY, m_nWidth, m_nHeight);

	status |= GzGetDisplayParams(m_pDisplay, &xRes, &yRes, &dispClass);

	status |= GzInitDisplay(m_pDisplay);

	status |= GzNewRender(&m_pRender, GZ_Z_BUFFER_RENDER, m_pDisplay);

	for (int i = 0; i < AAKERNEL_SIZE; i++)
	{
		status |= GzNewDisplay(&Display_with_offset[i], GZ_RGBAZ_DISPLAY, m_nWidth, m_nHeight);
		status |= GzGetDisplayParams(Display_with_offset[i], &xRes, &yRes, &dispClass);
		status |= GzInitDisplay(Display_with_offset[i]);
		status |= GzNewRender(&Render_with_offset[i], GZ_Z_BUFFER_RENDER, Display_with_offset[i]);
	}

	/* Translation matrix */
	GzMatrix	scale =
	{
		3.25, 0.0, 0.0, 0.0,
		0.0, 3.25, 0.0, -3.25,
		0.0, 0.0, 3.25, 3.5,
		0.0, 0.0, 0.0, 1.0
	};

	GzMatrix	rotateX =
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, .7071, .7071, 0.0,
		0.0, -.7071, .7071, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	GzMatrix	rotateY =
	{
		.866, 0.0, -0.5, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.5, 0.0, .866, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

#if 1 	/* set up app-defined camera if desired, else use camera defaults */
	camera.position[X] = 13.2;
	camera.position[Y] = -8.7;
	camera.position[Z] = -14.8;

	camera.lookat[X] = 0.8;
	camera.lookat[Y] = 0.7;
	camera.lookat[Z] = 4.5;

	camera.worldup[X] = -0.2;
	camera.worldup[Y] = 1.0;
	camera.worldup[Z] = 0.0;

	camera.FOV = 53.7;              /* degrees */

	status |= GzPutCamera(m_pRender, &camera);
#endif 

	/* Start Renderer */
	status |= GzBeginRender(m_pRender);

	/* Light */
	GzLight	light1 = { { -0.7071, 0.7071, 0 }, { 0.5, 0.5, 0.9 } };
	GzLight	light2 = { { 0, -0.7071, -0.7071 }, { 0.9, 0.2, 0.3 } };
	GzLight	light3 = { { 0.7071, 0.0, -0.7071 }, { 0.2, 0.7, 0.3 } };
	GzLight	ambientlight = { { 0, 0, 0 }, { 0.3, 0.3, 0.3 } };

	/* Material property */
	GzColor specularCoefficient = { 0.3, 0.3, 0.3 };
	GzColor ambientCoefficient = { 0.1, 0.1, 0.1 };
	GzColor diffuseCoefficient = { 0.7, 0.7, 0.7 };

	/*
	  renderer is ready for frame --- define lights and shader at start of frame
	  */

	/*
	 * Tokens associated with light parameters
	 */
	nameListLights[0] = GZ_DIRECTIONAL_LIGHT;
	valueListLights[0] = (GzPointer)&light1;
	nameListLights[1] = GZ_DIRECTIONAL_LIGHT;
	valueListLights[1] = (GzPointer)&light2;
	nameListLights[2] = GZ_DIRECTIONAL_LIGHT;
	valueListLights[2] = (GzPointer)&light3;
	status |= GzPutAttribute(m_pRender, 3, nameListLights, valueListLights);

	nameListLights[0] = GZ_AMBIENT_LIGHT;
	valueListLights[0] = (GzPointer)&ambientlight;
	status |= GzPutAttribute(m_pRender, 1, nameListLights, valueListLights);

	/*
	 * Tokens associated with shading
	 */
	nameListShader[0] = GZ_DIFFUSE_COEFFICIENT;
	valueListShader[0] = (GzPointer)diffuseCoefficient;

	/*
	* Select either GZ_COLOR or GZ_NORMALS as interpolation mode
	*/
	nameListShader[1] = GZ_INTERPOLATE;
#if 0
	interpStyle = GZ_COLOR;         /* Gouraud shading */
#else 
	interpStyle = GZ_NORMALS;       /* Phong shading */
#endif

	valueListShader[1] = (GzPointer)&interpStyle;
	nameListShader[2] = GZ_AMBIENT_COEFFICIENT;
	valueListShader[2] = (GzPointer)ambientCoefficient;
	nameListShader[3] = GZ_SPECULAR_COEFFICIENT;
	valueListShader[3] = (GzPointer)specularCoefficient;
	nameListShader[4] = GZ_DISTRIBUTION_COEFFICIENT;
	specpower = 32;
	valueListShader[4] = (GzPointer)&specpower;

	status |= GzPutAttribute(m_pRender, 5, nameListShader, valueListShader);

	status |= GzPushMatrix(m_pRender, scale);
	status |= GzPushMatrix(m_pRender, rotateY);
	status |= GzPushMatrix(m_pRender, rotateX);

	if (status) exit(GZ_FAILURE);

	if (status)
		return(GZ_FAILURE);
	else
		return(GZ_SUCCESS);
}

int Application4::Render()
{
	GzToken		nameListTriangle[3]; 	/* vertex attribute names */
	GzPointer	valueListTriangle[3]; 	/* vertex attribute pointers */
	GzCoord		vertexList[3];	/* vertex position coordinates */
	GzCoord		normalList[3];	/* vertex normals */
	GzTextureIndex  	uvList[3];		/* vertex texture map indices */
	char		dummy[256];
	int			status;


	/* Antiaiasing filter matrix */
	float AAFilter[AAKERNEL_SIZE][3] = /* X, Y, coef */
	{
		-0.52, 0.38, 0.128,
		0.41, 0.56, 0.119,
		0.27, 0.08, 0.294,
		-0.17, -0.29, 0.249,
		0.58, -0.55, 0.104,
		-0.31, -0.71, 0.106
	};

	/* Initialize Display */
	status |= GzInitDisplay(m_pDisplay);

	for (int i = 0; i < AAKERNEL_SIZE; i++)
	{
		status |= GzInitDisplay(Display_with_offset[i]);
	}
	/*
	* Tokens associated with triangle vertex values
	*/
	nameListTriangle[0] = GZ_POSITION;
	nameListTriangle[1] = GZ_NORMAL;

	// I/O File open
	FILE *infile;
	if ((infile = fopen(INFILE, "r")) == NULL)
	{
		AfxMessageBox("The input file was not opened\n");
		return GZ_FAILURE;
	}

	FILE *outfile;
	if ((outfile = fopen(OUTFILE, "wb")) == NULL)
	{
		AfxMessageBox("The output file was not opened\n");
		return GZ_FAILURE;
	}

	/*
	* Walk through the list of triangles, set color
	* and render each triangle
	*/
	while (fscanf(infile, "%s", dummy) == 1) { 	/* read in tri word */
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[0][0]), &(vertexList[0][1]),
			&(vertexList[0][2]),
			&(normalList[0][0]), &(normalList[0][1]),
			&(normalList[0][2]),
			&(uvList[0][0]), &(uvList[0][1]));
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[1][0]), &(vertexList[1][1]),
			&(vertexList[1][2]),
			&(normalList[1][0]), &(normalList[1][1]),
			&(normalList[1][2]),
			&(uvList[1][0]), &(uvList[1][1]));
		fscanf(infile, "%f %f %f %f %f %f %f %f",
			&(vertexList[2][0]), &(vertexList[2][1]),
			&(vertexList[2][2]),
			&(normalList[2][0]), &(normalList[2][1]),
			&(normalList[2][2]),
			&(uvList[2][0]), &(uvList[2][1]));

		/*
		 * Set the value pointers to the first vertex of the
		 * triangle, then feed it to the renderer
		 * NOTE: this sequence matches the nameList token sequence
		 */
		valueListTriangle[0] = (GzPointer)vertexList;
		valueListTriangle[1] = (GzPointer)normalList;
		//GzPutTriangle(m_pRender, 2, nameListTriangle, valueListTriangle);

		//Here I discard the method to use 6 renderers because I should use 6 renderers to do shade and Xform
		for (int i = 0; i < AAKERNEL_SIZE; i++)
		{
			m_pRender->display = Display_with_offset[i];
			m_pRender->Offset_x = AAFilter[i][X];
			m_pRender->Offset_y = AAFilter[i][Y];
			GzPutTriangle(m_pRender, 2, nameListTriangle, valueListTriangle);
		}
	}

	GzIntensity Red, Green, Blue, alpha, Sum_R, Sum_G, Sum_B, Sum_a;
	GzDepth Temp_z, Sum_z;
	// Merge all displays to draw
	for (int i = 0; i < m_nWidth; i++)
	{
		for (int j = 0; j < m_nHeight; j++)
		{
			Sum_R = 0;
			Sum_G = 0;
			Sum_B = 0;
			Sum_a = 0;
			Sum_z = 0;
			for (int k = 0; k < AAKERNEL_SIZE; k++)
			{
				GzGetDisplay(Display_with_offset[k], i, j, &Red, &Green, &Blue, &alpha, &Temp_z);
				Sum_R += Red * AAFilter[k][2];
				Sum_G += Green * AAFilter[k][2];
				Sum_B += Blue * AAFilter[k][2];
				Sum_a += alpha * AAFilter[k][2];
				Sum_z += Temp_z * AAFilter[k][2];
			}
			GzPutDisplay(m_pDisplay, i, j, Sum_R, Sum_G, Sum_B, Sum_a, Sum_z);
		}
	}

	GzFlushDisplay2File(outfile, m_pDisplay); 	/* write out or update display to file*/
	GzFlushDisplay2FrameBuffer(m_pFrameBuffer, m_pDisplay);	// write out or update display to frame buffer

	/*
	 * Close file
	 */

	if (fclose(infile))
		AfxMessageBox("The input file was not closed\n");

	if (fclose(outfile))
		AfxMessageBox("The output file was not closed\n");

	if (status)
		return(GZ_FAILURE);
	else
		return(GZ_SUCCESS);
}

int Application4::Clean()
{
	/*
	 * Clean up and exit
	 */
	int	status = 0;

	status |= GzFreeRender(m_pRender);
	status |= GzFreeDisplay(m_pDisplay);

	for (int i = 0; i < AAKERNEL_SIZE; i++)
	{
		status |= GzFreeDisplay(Display_with_offset[i]);
		status |= GzFreeRender(Render_with_offset[i]);
	}

	if (status)
		return(GZ_FAILURE);
	else
		return(GZ_SUCCESS);
}



