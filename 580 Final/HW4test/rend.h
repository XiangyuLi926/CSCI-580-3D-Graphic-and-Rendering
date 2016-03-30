#ifndef GZ_RENDER
#define GZ_RENDER

#include "disp.h"

/* Camera defaults */
#define	DEFAULT_FOV		35.0
#define	DEFAULT_IM_Z	(-10.0)  /* world coords for image plane origin */
#define	DEFAULT_IM_Y	(5.0)    /* default look-at point = 0,0,0 */
#define	DEFAULT_IM_X	(-10.0)

#define	DEFAULT_AMBIENT  {0.1, 0.1, 0.1}
#define	DEFAULT_DIFFUSE  {0.7, 0.6, 0.5}
#define	DEFAULT_SPECULAR {0.2, 0.3, 0.4}
#define	DEFAULT_SPEC     32

#define	MATLEVELS	64		/* how many matrix pushes allowed */
#define	MAX_LIGHTS	10		/* how many lights allowed */

#define GzTexture	GzPointer

struct GzRender {			/* define a renderer */
	GzRenderClass	renderClass;
	GzDisplay		*display;
	short		    open;

	/*
	* Matrix transforms
	*/
	GzCamera		camera;
	short		    matlevel;
	GzMatrix		Xvert[MATLEVELS];
	GzMatrix		Xnorm[MATLEVELS];
	GzMatrix		Xsp;	        /* NDC to screen & Camera to screen */

	/*
	* Lighting shading
	*/
	GzColor		flatcolor;
	int			interp_mode;
	int			numlights;
	GzLight		lights[MAX_LIGHTS];
	GzLight		ambientlight;
	GzColor		Ka, Kd, Ks;
	float		spec;		/* specular power */

	/*
	* Texture part
	*/
	GzTexture		tex_fun;    /* tex_fun(float u, float v, GzColor color) */
};

// Function declaration
// HW2
int GzNewRender(GzRender **render, GzRenderClass renderClass, GzDisplay *display);
int GzFreeRender(GzRender *render);
int GzBeginRender(GzRender	*render);
int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, GzPointer *valueList);
int GzPutTriangle(GzRender *render, int	numParts, GzToken *nameList, GzPointer *valueList);

// HW3
int GzPutCamera(GzRender *render, GzCamera *camera);
int GzPushMatrix(GzRender *render, GzMatrix	matrix);
int GzPopMatrix(GzRender *render);

// HW4
int GzPushLight(GzRender *render, GzLight light);
int GzPopLight(GzRender *render);

// Object Translation
int GzRotXMat(float degree, GzMatrix mat);
int GzRotYMat(float degree, GzMatrix mat);
int GzRotZMat(float degree, GzMatrix mat);
int GzTrxMat(GzCoord translate, GzMatrix mat);
int GzScaleMat(GzCoord scale, GzMatrix mat);

#endif  /*GZ_RENDER included*/