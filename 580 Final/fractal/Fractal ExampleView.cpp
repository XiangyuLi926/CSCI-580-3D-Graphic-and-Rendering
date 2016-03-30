// Fractal ExampleView.cpp : implementation of the CFractalExampleView class
//

//
// Written by: Paul E. Martz
// 
// Copyright 1997 by Paul E. Martz, all right reserved
//
// Non-commercial use by individuals is permitted.
//


#include "stdafx.h"
#include <math.h>
#include "Fractal Example.h"

#include "Fractal ExampleDoc.h"
#include "Fractal ExampleView.h"
#include "FractalOptionsDlg.h"

#include "fractmod.h"

#define CLOUDMAP 10
#define TEXIMAGEMAP 20

/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFractalExampleView

IMPLEMENT_DYNCREATE(CFractalExampleView, COpenGLView)

BEGIN_MESSAGE_MAP(CFractalExampleView, COpenGLView)
	//{{AFX_MSG_MAP(CFractalExampleView)
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFractalExampleView construction/destruction

CFractalExampleView::CFractalExampleView()
{
	// TODO: add construction code here

}

CFractalExampleView::~CFractalExampleView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CFractalExampleView drawing


/////////////////////////////////////////////////////////////////////////////
// CFractalExampleView diagnostics

#ifdef _DEBUG
void CFractalExampleView::AssertValid() const
{
	CView::AssertValid();
}

void CFractalExampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFractalExampleDoc* CFractalExampleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFractalExampleDoc)));
	return (CFractalExampleDoc*)m_pDocument;
}
#endif //_DEBUG


///////////////////////////////////////////
// static routines used for rendering

// Scale value passed into fractal generation routines.
#define DEF_HEIGHT_SCALE 1.f

// Keep linker happy
#ifdef __cplusplus
extern "C" {
#endif

// These next few routines are declared external by fractmod.c
// They are referenced by fractmod.c's functions:
//   draw1DFractArrayAsLines
//   draw2DFractArrayAsLines
//   draw2DFractArrayAsTriangles
// If you don't use those functions from fractmod.c, these
// functions here could be left blank. However they would
// still need to be declared to resolve the symbols (unless
// you removed their references in fractmod.c).

void draw3DTriangle (float x1, float y1, float z1,
		     float x2, float y2, float z2,
		     float x3, float y3, float z3,
		     float nx, float ny, float nz)
{
    glBegin (GL_TRIANGLES);
    glNormal3f (nx, ny, nz);
    glVertex3f (x1, y1, z1);
    glVertex3f (x2, y2, z2);
    glVertex3f (x3, y3, z3);
    glEnd ();
}
void draw3DLine (float x1, float y1, float z1,
		 float x2, float y2, float z2)
{
    glBegin (GL_LINES);
    glVertex3f (x1, y1, z1);
    glVertex3f (x2, y2, z2);
    glEnd ();
}
void draw2DLine (float x1, float y1,
		 float x2, float y2)
{
    glBegin (GL_LINES);
    glVertex2f (x1, y1);
    glVertex2f (x2, y2);
    glEnd ();
}
#ifdef __cplusplus
}
#endif

// Sets up lighting in OpenGL but does not enable it.
// Lighting will be enabled in ::renderTeximageMap().
static void initLights ()
{
    GLfloat light_ambient[]  = {.5f, .5f, .5f, 1.f};
    GLfloat light_specular[] = {0.f, 0.f, 0.f, 1.f};
    GLfloat light_diffuse[]  = {1.f, 1.f, 1.f, 1.f};
    GLfloat light_position[] = {1.f, .5f, 1.f, 0.f}; /* w=0 ==> directional */

    glLightfv (GL_LIGHT1, GL_POSITION, light_position);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, light_ambient);
    glEnable (GL_LIGHT1);
    glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
}


/////////////////////////////////////////////////////////////////////////////
// CFractalExampleView message handlers

void CFractalExampleView::OnInitialUpdate() 
{
	COpenGLView::OnInitialUpdate();

	// Probably not the best place in the world to 
	// initialize all this stuff. But it works for me.
	// And after all, this is a hack, isn't it?

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//* Default values you see in the View Options dialog box.
	colorInvert = FALSE;
	iterations = 2;
	cloudIter = 5;
	teximageIter = 8;
	renderMode = twoDR_lines;
	arrayIs2D = TRUE;
	tile = 1;
	randomSeed = 0;
	aaLines = TRUE;
	textureLinear = TRUE;

	// Initial rotations, translations, and a toggle for 
	// the shift key status.
	shiftKeyStatus = FALSE;
	yRot = 0.f;
	yTrans = -.5f;
	zTrans = -3.f;

	// Ptrs to data and dirty flags. Initially, all 
	// are NULL and dirty.
	surfFA = NULL;
	surfFAdirty = TRUE;
	teximageFA = NULL;
	teximageFAdirty = TRUE;
	cloudFA = NULL;
	cloudFAdirty = TRUE;

	cloudH = surfaceH = teximageH = 0.7f;
}

// surfToSky
//
// Converts a 2D fract array into a nice blue and white
// cloudy sky texture map. First, it finds the average height
// value in the map. Then it clamps values to +/- SKY_LIMIT
// around the average value. Values in the array are then
// mapped from a medium blue to white. The resulting RGB 
// values are stored in skyTex.
//
// Note that while the fract array is assumed to have dimension
// (size+1)X(size+1), skyTex must have dimension sizeXsize.
// This facilitates its being used as an OpenGL texture map.
static void surfToSky (float *fa, int size, GLubyte *skyTex)
{
    int	i, j;
    const int subSize = size;
    unsigned char *skyptr;
    float	shadeFactor;
    float	skyAv=0.f;

    size++;
    for (i=0; i<size*size; i++)
		skyAv += fa[i];
	skyAv /= (float)(size*size);

#define SKY_LIMIT	0.2f
    skyptr = skyTex;
    for (i=0; i<subSize; i++) {
		for (j=0; j<subSize; j++) {
			if (fa[i*size+j] < (skyAv - SKY_LIMIT))
				shadeFactor = -SKY_LIMIT;
			else if (fa[i*size+j] > (skyAv + SKY_LIMIT))
				shadeFactor = SKY_LIMIT;
			else
				shadeFactor = fa[i*size+j] - skyAv;

			/* shadeFactor is now in range -SKY_LIMIT - SKY_LIMIT.
			   scale shadeFactor to range 0.0 - 0.5 */
			shadeFactor = ((shadeFactor + SKY_LIMIT) *
			   (.5f / (2.f * SKY_LIMIT)));

			/* Make red and green range from 0.41 - 0.9 */
			skyptr[0] = (GLubyte)((.4 + shadeFactor) * 255); /* red */
			skyptr[1] = (GLubyte)((.4 + shadeFactor) * 255); /* green */
			skyptr[2] = (GLubyte)(.9 * 255.); /* blue */

			skyptr += 3;
		}
    }
}

// renderCloudMap
//
// Render the cloud texture map. If the cloud fract array
// doesn't already exist, allocate it, tesselate it, and
// create the texture map using surfToSky. Then create a 
// display list with this texture map in the list.
//
// Once we have the texture map, we simply draw a quad as
// big as will fit in the window, with the cloud texture map
// applied to it.
BOOL CFractalExampleView::renderCloudMap ()
{
	CWaitCursor hourglass;
    int	size = 1 << cloudIter;

	if (cloudFA && cloudFAdirty) {
		freeFractArray (cloudFA);
		cloudFA = NULL;
	}
	if (!cloudFA) {
		cloudFA = alloc2DFractArray (size);
		if (cloudFA==NULL) return (FALSE);
		fill2DFractArray (cloudFA, size, randomSeed,
			DEF_HEIGHT_SCALE, cloudH);
	}

	glPushAttrib (0xfffffff);

	if (cloudFAdirty) {
		GLubyte *tmap;

		tmap = (GLubyte *) malloc (size*size*3);
		if (tmap==NULL) return (FALSE);

		surfToSky (cloudFA, size, tmap);

		glNewList (CLOUDMAP, GL_COMPILE);

		glTexImage2D (GL_TEXTURE_2D, 0, /* lod */
		      3, /* num components */
		      size, size, /* width, height */
		      0, /* border */
		      GL_RGB, GL_UNSIGNED_BYTE, /* format, type */
		      tmap);

		glEndList ();

		free (tmap);
	}
	glCallList (CLOUDMAP);

	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	if (currentWidth > currentHeight)
		glOrtho (-1.f, aspectRatio*2.f-1.f,
			-1.f, 1.f, 1., -1.);
	else 
 		glOrtho (-1.f, 1.f,
			-1.f, 2.f/aspectRatio-1.f, 1., -1.);
	glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

	glDisable (GL_DEPTH_TEST);
	glDisable (GL_FOG);
	glDisable (GL_LIGHTING);
	glDisable (GL_BLEND);
	glEnable (GL_TEXTURE_2D);
    
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 
		(GLfloat) GL_REPLACE);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		(GLfloat) GL_NEAREST);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		(GLfloat) GL_NEAREST);

	glClear (GL_COLOR_BUFFER_BIT);

	glBegin (GL_QUADS); {
		glTexCoord2f (0.f, 0.f); glVertex2f (-1.f, -1.f);
		glTexCoord2f (1.f, 0.f); glVertex2f (1.f, -1.f);
		glTexCoord2f (1.f, 1.f); glVertex2f (1.f, 1.f);
		glTexCoord2f (0.f, 1.f); glVertex2f (-1.f, 1.f);
	} glEnd ();
	glFlush ();

	glPopAttrib ();

	cloudFAdirty = FALSE;
	return (TRUE);
}

// setH1H2
//
// The h1 and h2 parameters are set to 1/3 and 2/3 of the
// height values stored in the 2D fract array. These values
// are then used in our rinky-dink-but-effective coloring,
// scheme wherein the lower 1/3 of terrain is colored dark
// green, the middle third is colored grey, and the upper
// third is white.
void setH1H2 (float *fa, int size, float *h1, float *h2)
{
    float min, max;
    int	i;

    size++;
    min = 2.f;
    max = -2.f;
    for (i=0; i<size*size; i++) {
	if (fa[i] > max)
	    max = fa[i];
	else if (fa[i] < min)
	    min = fa[i];
    }
    *h1 = (max - min) * .66f + min;
    *h2 = (max - min) * .33f + min;
}

// SET_COLOR -- Based on the height value, set the current 
// color to dark green, grey, or white.
#define SET_COLOR(h,h1,h2)\
{ \
    if (h > h1) \
		glColor3f (.95f, .95f, 1.f); \
    else if (h > h2) \
		glColor3f (.55f, .55f, .6f); \
    else \
		glColor3f (.2f, .4f, .2f); \
}

// TEXCOORD -- Based on the current values of X and Z passed
// in as U and V, Set the texture coordinate. Sinze X and Z
// could range from -1.0 to 1.0, this is simply a matter of
// adding 1 and dividing by 2.
#define TEXCOORD(u,v) \
    glTexCoord2f ((u+1.f)*.5f, -((v-1.f)*.5f))

// getNormal
//
// Given the three vertices of a triangle, return a surface
// normal to that triangle.
static void getNormal (float x1, float y1, float z1,
		       float x2, float y2, float z2,
		       float x3, float y3, float z3,
		       float *normal)
{
    float	len;
    float	v1x, v1y, v1z;
    float	v2x, v2y, v2z;


    v1x = x2 - x1;
    v1y = y2 - y1;
    v1z = z2 - z1;
    v2x = x3 - x1;
    v2y = y3 - y1;
    v2z = z3 - z1;

    normal[0] = v1y*v2z - v1z*v2y;
    normal[1] = v1z*v2x - v1x*v2z;
    normal[2] = v1x*v2y - v1y*v2x;

    len = (float) sqrt (normal[0]*normal[0] + normal[1]*normal[1] +
			normal[2]*normal[2]);

    normal[0] /= len;
    normal[1] /= len;
    normal[2] /= len;
}

// genNormal
//
// Use getNormal to get the Normal for the specified triangle.
// Then set the current normal in OpenGL.
static void genNormal (float x1, float y1, float z1,
		       float x2, float y2, float z2,
		       float x3, float y3, float z3)
{
    float	norm[3];

    getNormal (x1, y1, z1,
	       x2, y2, z2,
	       x3, y3, z3, norm);
    glNormal3f (norm[0], norm[1], norm[2]);
}

// renderAsTriangle
//
// As far as OpenGL efficiency goes, this routine will be better
// than fractmod.c's draw2DFractArrayAsTriangles because it uses
// the triangle strip primitive. If the "lights" param is non-zero,
// surface normals are specified. If the "dotex" param is non-zero,
// texture coordinates are sprcified.
static void renderAsTriangles (float *fa, int sz,
			       int lights, int dotex)
{
	float h1, h2;
    int	i, j;
    float	x, z, inc;
    const int	subSize = sz, superSize = sz+1;

	// h1 and h2 will be 1/3 and 2/3 of our height
	// values. The SET_COLOR macro uses this to determine
	// the syrface color for a given height.
 	setH1H2 (fa, subSize, &h1, &h2);

	inc = 2.f / subSize;
    z = -1.f;
    for (i=0; i<subSize; i++) {
		glBegin (GL_TRIANGLE_STRIP);
		x = -1.f, j=0;
		if (dotex)
			TEXCOORD (x, z);
		else {
			if (lights)
			genNormal (x, fa[i*superSize+j], z,
			   x, fa[(i+1)*superSize+j], z+inc,
			   x+inc, fa[i*superSize+j+1], z);
			SET_COLOR (fa[i*superSize+j], h1, h2);
		}
		glVertex3f (x, fa[i*superSize+j], z);
		if (dotex)
		    TEXCOORD (x, z+inc);
		else {
		    if (lights)
			genNormal (x, fa[(i+1)*superSize+j], z+inc,
			   x+inc, fa[(i+1)*superSize+j+1], z+inc,
			   x+inc, fa[i*superSize+j+1], z);
		    SET_COLOR (fa[(i+1)*superSize+j], h1, h2);
		}
		glVertex3f (x, fa[(i+1)*superSize+j], z+inc);
		for (; j<subSize; j++) {
			if (dotex)
				TEXCOORD (x+inc, z);
		    else {
				if (lights)
				genNormal (x, fa[(i*superSize)+j], z,
			       x, fa[(i+1)*superSize+j], z+inc,
			       x+inc, fa[i*superSize+j+1], z);
				SET_COLOR (fa[i*superSize+j+1], h1, h2);
		    }
		    glVertex3f (x+inc, fa[i*superSize+j+1], z);
			if (dotex)
				TEXCOORD (x+inc, z+inc);
		    else {
				if (lights)
				genNormal (x, fa[(i+1)*superSize+j], z+inc,
			       x+inc, fa[(i+1)*superSize+j+1], z+inc,
			       x+inc, fa[i*superSize+j+1], z);
				SET_COLOR (fa[(i+1)*superSize+j+1], h1, h2);
			}
		    glVertex3f (x+inc, fa[(i+1)*superSize+j+1], z+inc);
		    x += inc;
		}
		z += inc;
		glEnd ();
    }
}

// renderTeximageMap
//
// If the teximage map is dirty, allocate and tesselate a new
// one. Render it to the screen as lit triangles. Read the
// image back and set it as a texture map in a display list.
BOOL CFractalExampleView::renderTeximageMap ()
{
	CWaitCursor hourglass;
    int	size = 1 << teximageIter;
	UINT pmapDim, smallDim;

	if (teximageFA && teximageFAdirty) {
		freeFractArray (teximageFA);
		teximageFA = NULL;
	}
	if (!teximageFA) {
		teximageFA = alloc2DFractArray (size);
		if (teximageFA==NULL) return (FALSE);
		fill2DFractArray (teximageFA, size, randomSeed,
			DEF_HEIGHT_SCALE, teximageH);
	}

	// Find the biggest power of two that will fit
	// in the current window. This is where we will 
	// draw the image.
	smallDim = (currentWidth > currentHeight) ?
		currentHeight : currentWidth;
	pmapDim = 1 << (UINT) (log ((float) smallDim) / log (2.));

	glPushAttrib (0xfffffff);

	// Only draw into biggest power of 2 square.
	glViewport (0, 0, pmapDim, pmapDim);
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	glOrtho (-1.f, 1.f,
		-1.f, 1.f, 1., -1.);
	glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
	// The array is rendered into the XZ plane. We need to
	// rotate by 90 degrees so we will be looking at it
	// "top down".
	glRotatef (90.f, 1.f, 0.f, 0.f);
	// Init lights now. Light position must be specified
	// after the rotate, or else it won't be rotated.
	initLights ();

	glDisable (GL_DEPTH_TEST);
	glDisable (GL_FOG);
	glEnable (GL_LIGHTING);
	glDisable (GL_BLEND);
    
	glClear (GL_COLOR_BUFFER_BIT);

	renderAsTriangles (teximageFA, size, 1, 0);
	glFlush ();

	glPopAttrib ();


	if (teximageFAdirty) {
		GLubyte *pmap;

		pmap = (GLubyte *) malloc (pmapDim*pmapDim*3);
		if (pmap==NULL) return (FALSE);

		glReadPixels (0, 0, pmapDim, pmapDim,
		      GL_RGB, GL_UNSIGNED_BYTE, pmap);

		glNewList (TEXIMAGEMAP, GL_COMPILE);
		glTexImage2D (GL_TEXTURE_2D, 0, /* lod */
		      3, /* num components */
		      pmapDim, pmapDim, /* width, height */
		      0, /* border */
		      GL_RGB, GL_UNSIGNED_BYTE, /* format, type */
		      pmap);

		glEndList ();

		free (pmap);
	}

	teximageFAdirty = FALSE;
	return (TRUE);
}

// renderFullImage
//
// First we render the sky. If the sky fract array is dirty,
// we make a new one by calling renderCloudMap. The sky is
// rendering as an 8-sided pyramid.
// The apex is at (0,2,0), just above
// the origin. The base of the eight walls are positioned
// roughly in a circle of radius 15, in the XZ plane, around
// the point (0,-1,0). Fig is enabled for the sky with a fog
// range of (3,11). For speed, no Z buffering when rendering
// the sky.
// 
// Then, we render the surface fract array for as many times
// as it is tiled, and it is texture mapped with the teximage
// map. If the teximage map is dirty, we make a clean one by
// calling renderTeximageMap.
BOOL CFractalExampleView::renderFullImage ()
{
	CWaitCursor hourglass;
    int	size = 1 << iterations;
	GLfloat	fogColor[4] = {.84f, .87f, 1.f, 1.f};

	// Validate the surface texture map
	if (teximageFAdirty)
		renderTeximageMap ();
	// validate the cloud texture map
	if (cloudFAdirty)
		renderCloudMap ();

	// validate the surface
	if (surfFA && surfFAdirty) {
		freeFractArray (surfFA);
		surfFA = NULL;
	}
	if (!surfFA) {
		surfFA = alloc2DFractArray (size);
		if (surfFA==NULL) return (FALSE);
		fill2DFractArray (surfFA, size, randomSeed,
			DEF_HEIGHT_SCALE, surfaceH);
		surfFAdirty = FALSE;
	}

	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	gluPerspective (60., aspectRatio, .1, 15.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
	glTranslatef (0.f, yTrans, zTrans);
    glRotatef (yRot, 0.f, 1.f, 0.f);

	glPushAttrib (0xffffffff);

	// fog range will be 3 to 11 for sky, but will
	// be a little closer when we render the terrain.
	// This creates that nice kind of true atmospheric
	// effect when looking through more air (along
	// the horizon) as opposed to straight up.
	glFogf (GL_FOG_DENSITY, 1.f);
	glFogf (GL_FOG_START, 3.f);
	glFogf (GL_FOG_END, 11.f);
	glFogi (GL_FOG_MODE, GL_LINEAR);
	glFogfv (GL_FOG_COLOR, fogColor);
	glEnable (GL_FOG);

	glDisable (GL_DITHER);
	// invertColors doesn't apply here; clear to fog color.
	glClearColor (fogColor[0], fogColor[1], fogColor[2], 1.f);
	glClear (GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);

	glDisable (GL_LIGHTING);
	glDisable (GL_DEPTH_TEST);

	// Goes a little faster but looks like crud:
	//glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, 
		(GLfloat) GL_REPLACE);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		textureLinear ? (GLfloat) GL_LINEAR
		: (GLfloat) GL_NEAREST);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		textureLinear ? (GLfloat) GL_LINEAR
		: (GLfloat) GL_NEAREST);
	glEnable (GL_TEXTURE_2D);

	glCallList (CLOUDMAP);
	// TMMAX defines texture map wrapping.
#define TMMAX 3.f // wrap from -3 to 3, or six times across our pyramid
#define TMMAX45 (TMMAX * 0.707f) // roughly sine(TMMAX)
	// Kludgey, but it works:
	// This is an 8-sided pyramid covered with the cloud
	// texture, wrapped 4 times.
	glBegin (GL_TRIANGLE_FAN); {
		glTexCoord2f (0.f, 0.f); glVertex3f (0.f, 2.f, 0.f);
		glTexCoord2f (-TMMAX45, -TMMAX45); glVertex3f (-10.f, -1.f, 10.f);
		glTexCoord2f (0.f, -TMMAX); glVertex3f (0.f, -1.f, 15.f);
		glTexCoord2f (TMMAX45, -TMMAX45); glVertex3f (10.f, -1.f, 10.f);
		glTexCoord2f (TMMAX, 0.f); glVertex3f (15.f, -1.f, 0.f);
		glTexCoord2f (TMMAX45, TMMAX45); glVertex3f (10.f, -1.f, -10.f);
		glTexCoord2f (0.f, TMMAX); glVertex3f (0.f, -1.f, -15.f);
		glTexCoord2f (-TMMAX45, TMMAX45); glVertex3f (-10.f, -1.f, -10.f);
		glTexCoord2f (-TMMAX, 0.f); glVertex3f (-15.f, -1.f, 0.f);
		glTexCoord2f (-TMMAX45, -TMMAX45); glVertex3f (-10.f, -1.f, 10.f);
	} glEnd ();

	// Don't need depth test for sky but turn it on for
	// rendering the height map because I'm too lazy to do
	// painter's algorithm.
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	
	// Here's the tighter fog range for the surface.
	glFogf (GL_FOG_START, 2.f);
	glFogf (GL_FOG_END, 7.f);

	// surface texture map
	glCallList (TEXIMAGEMAP);

	float x, z;
	UINT i, j;

	z = -1.f * (float) (tile-1);
	for (i=0; i<tile; i++) {
		x = -1.f * (float) (tile-1);
		for (j=0; j<tile; j++) {
			glPushMatrix ();
			glTranslatef (x, 0.f, z);

			renderAsTriangles (surfFA, size, 0, 1);

			glPopMatrix ();
			x += 2.f;
		}
		z += 2.f;
	}

	glPopAttrib ();

	return (TRUE);
}

// RenderScene
//
// This function overrides the same function in COpenGLView.
// Here we handle rendering 1D midpoint displacement or 2D mesh
// / lines, both of which are line rendering modes. If our
// renderMode variable is set to anything else, we call
// another function to handle the rendering.
BOOL CFractalExampleView::RenderScene()
{
    int	size = 1 << iterations;
	GLfloat black[4] = {0.f, 0.f, 0.f, 1.f};
	GLfloat white[4] = {1.f, 1.f, 1.f, 1.f};

	// Handle inverted colors.
	if (colorInvert)
	    glClearColor (black[0], black[1], black[2], black[3]);
	else
	    glClearColor (white[0], white[1], white[2], white[3]);

	// Handle non line mode renderMode settings.
	if (renderMode == twoDR_clouds)
		return (renderCloudMap ());
	else if (renderMode == twoDR_teximage)
		return (renderTeximageMap ());
	else if (renderMode == twoDR_rendered)
		return (renderFullImage ());

	// Shouldn't take too long, unless drawing huge
	// AA scenes, but what the heck...
	CWaitCursor hourglass;

	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	if (arrayIs2D)
		// Needs to be identical to the same call in renderFullScene.
	    gluPerspective (60., aspectRatio, .1, 15.0);
	else
		glOrtho (-aspectRatio, aspectRatio, -1., 1., 1., -1.);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
	if (arrayIs2D) {
	    glTranslatef (0.f, yTrans, zTrans);
		glRotatef (yRot, 0.f, 1.f, 0.f);
	}

	glPushAttrib (0xffffffff);

	glDisable (GL_DEPTH_TEST);
	glDisable (GL_LIGHTING);
	if (colorInvert)
		glColor4fv (white);
	else
		glColor4fv (black);

	if (aaLines) {
		glEnable (GL_LINE_SMOOTH);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable (GL_LINE_SMOOTH);
		glDisable (GL_BLEND);
	}
    
	glClear (GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT);

	if (arrayIs2D) {
		float x, z;
		UINT i, j;

		// validate surface
		if (surfFA && surfFAdirty) {
			freeFractArray (surfFA);
			surfFA = NULL;
		}
		if (!surfFA) {
			surfFA = alloc2DFractArray (size);
			if (surfFA==NULL) return (FALSE);
			fill2DFractArray (surfFA, size, randomSeed,
				DEF_HEIGHT_SCALE, surfaceH);
			surfFAdirty = FALSE;
		}

		z = -1.f * (float) (tile-1);
		for (i=0; i<tile; i++) {
			x = -1.f * (float) (tile-1);
			for (j=0; j<tile; j++) {
				glPushMatrix ();
				glTranslatef (x, 0.f, z);

				draw2DFractArrayAsLines (surfFA, size);

				glPopMatrix ();
				x += 2.f;
			}
			z += 2.f;
		}
	}
	else {
		float x;
		UINT i;
	    float	*fa;

		// Always validate and free 1D array.
		// We won't be doing any realtime transformations on it.
		fa = alloc1DFractArray (size);
		if (fa==NULL) return (FALSE);
		fill1DFractArray (fa, size, randomSeed,
			DEF_HEIGHT_SCALE, surfaceH);

		x = -1.f * (float) (tile-1);
		for (i=0; i<tile; i++) {

			glPushMatrix ();
			glTranslatef (x, 0.f, 0.f);

			draw1DFractArrayAsLines (fa, size);

			glPopMatrix ();
			x += 2.f;
		}

		freeFractArray (fa);
	}

	glFlush();
	glPopAttrib ();
	
	return (TRUE);
}

// OnViewOptions
//
// Handles dialog box paramter changes and records them in the View.
void CFractalExampleView::OnViewOptions() 
{
	CFractalOptionsDlg dlg;
	CString oneD = "1D midpoint displacement";
	CString twoDLines = "2D mesh / lines";
	CString twoDClouds = "2D mesh / clouds";
	CString twoDTeximage = "2D mesh / teximage";
	CString twoDRendered = "2D mesh / rendered";

	dlg.m_invert = colorInvert;
	dlg.m_aaLines = aaLines;
	dlg.m_textureLinear = textureLinear;
	dlg.m_randomSeed = randomSeed;
	dlg.m_tile = tile;
	dlg.m_iterations = iterations;
	dlg.m_cloudIter = cloudIter;
	dlg.m_teximageIter = teximageIter;
	if (!arrayIs2D)
		dlg.m_renderType = oneD;
	else if (renderMode == twoDR_lines)
		dlg.m_renderType = twoDLines;
	else if (renderMode == twoDR_clouds)
		dlg.m_renderType = twoDClouds;
	else if (renderMode == twoDR_teximage)
		dlg.m_renderType = twoDTeximage;
	else if (renderMode == twoDR_rendered)
		dlg.m_renderType = twoDRendered;
	dlg.m_cloudH = cloudH;
	dlg.m_surfaceH = surfaceH;
	dlg.m_teximageH = teximageH;

	if (dlg.DoModal () == IDOK) {
		teximageFAdirty = (teximageFAdirty ||
			(teximageH != dlg.m_teximageH) ||
			(randomSeed != dlg.m_randomSeed) ||
			(teximageIter != dlg.m_teximageIter));
		surfFAdirty = (surfFAdirty ||
			(surfaceH != dlg.m_surfaceH) ||
			(randomSeed != dlg.m_randomSeed) ||
			(iterations != dlg.m_iterations));
		cloudFAdirty = (cloudFAdirty ||
			(cloudH != dlg.m_cloudH) ||
			(randomSeed != dlg.m_randomSeed) ||
			(cloudIter != dlg.m_cloudIter));

		colorInvert = dlg.m_invert;
		aaLines = dlg.m_aaLines;
		textureLinear = dlg.m_textureLinear;
		randomSeed = dlg.m_randomSeed;
		tile = dlg.m_tile;
		iterations = dlg.m_iterations;
		cloudIter = dlg.m_cloudIter;
		teximageIter = dlg.m_teximageIter;

		arrayIs2D = !(dlg.m_renderType == oneD);

		if (dlg.m_renderType == twoDClouds)
			renderMode = twoDR_clouds;
		else if (dlg.m_renderType == twoDTeximage)
			renderMode = twoDR_teximage;
		else if (dlg.m_renderType == twoDRendered)
			renderMode = twoDR_rendered;
		else // default: if (dlg.m_renderType == twoDLines)
			renderMode = twoDR_lines;

		cloudH = dlg.m_cloudH;
		surfaceH = dlg.m_surfaceH;
		teximageH = dlg.m_teximageH;


		OnDraw ( GetDC() );
	}
}

//OnSize
//
// Handles resize events. Records width, height, and 
//aspectRatio in the View.
void CFractalExampleView::OnSize (UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
		
	if (0 >= cx || 0 >= cy)
		return;

	currentWidth = cx;
	currentHeight = cy;
	glViewport(0, 0, cx, cy);

	// compute the aspect ratio
	// this will keep all dimension scales equal
	aspectRatio = (float)cx/(float)cy;
}

// OnKeyDown
//
// Translates keypress events to rotations or translations
// which are then recorded in the View object.
void CFractalExampleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BOOL needRedraw = FALSE;

	switch (nChar) {
	case VK_LEFT:
		yRot += 3.f;
		needRedraw = TRUE;
		break;
	case VK_UP:
		if (shiftKeyStatus)
			yTrans += .05f;
		else
			zTrans += .05f;
		needRedraw = TRUE;
		break;
	case VK_RIGHT:
		yRot -= 3.f;
		needRedraw = TRUE;
		break;
	case VK_DOWN:
		if (shiftKeyStatus)
			yTrans -= .05f;
		else
			zTrans -= .05f;
		needRedraw = TRUE;
		break;
	case VK_SHIFT:
		shiftKeyStatus = TRUE;
		return;
	}

	if (needRedraw) {
		OnDraw ( GetDC() );
		return;
	}
	else
		// Let parent class handle other keys.
		COpenGLView::OnKeyDown(nChar, nRepCnt, nFlags);
}

// OnKeyUp
//
// Tracks state of the shift key. There's got to be a better
// way to tell whether a key is shifted or not than by
// tracking press and release events on the shift key.
void CFractalExampleView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar) {
	case VK_SHIFT:
		shiftKeyStatus = FALSE;
		return;
	}
	COpenGLView::OnKeyUp(nChar, nRepCnt, nFlags);
}

// OnDestroy
//
// Free all that memory.
void CFractalExampleView::OnDestroy() 
{
	COpenGLView::OnDestroy();
	
	if (surfFA) 
		freeFractArray (surfFA);
	if (teximageFA) 
		freeFractArray (teximageFA);
	if (cloudFA)
		freeFractArray (cloudFA);
}
