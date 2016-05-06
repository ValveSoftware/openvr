// STD Headers
#define _USE_MATH_DEFINES
#include <array>
#include <cmath>
#include <iostream>

// freeglut
#include <GL/glew.h>
#include <GL/freeglut.h>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib,"glew32.lib") 
#ifdef _DEBUG
#pragma comment(lib,"opencv_world310d.lib") 
#pragma comment(lib,"freeglutd.lib") 
#else
#pragma comment(lib,"opencv_world310.lib") 
#pragma comment(lib,"freeglut.lib") 
#endif

#include "OpenVRGL.h"

COpenVRGL* g_pOpenVRGL = nullptr;

float*			g_aVertexArray	= nullptr;
float*			g_aTextureArray	= nullptr;
unsigned int*	g_aIndexArray	= nullptr;
unsigned int	g_uIndexNum		= 0;
uint32_t	g_uWidth		= 0;
uint32_t	g_uHeight		= 0;
GLuint		g_uTextureID	= 0;

void BuildBall( float fSize, unsigned int uNumW, unsigned int uNumH)
{
	// TODO: this is a trivial way, the top and bottom should be simplified
	unsigned int uSize = uNumW * uNumH;
	g_aVertexArray = new float[3 * uSize];
	g_aTextureArray = new float[2 * uSize];

	for (unsigned int iH = 0; iH < uNumH; ++iH)
	{
		float fY = float(iH) / (uNumH - 1);
		// pre-compute vertex position
		float fTmp	= fabs( fSize * sin(M_PI * fY) );
		float fVY	= fSize * cos(M_PI * fY);

		for (unsigned int iW = 0; iW < uNumW; ++iW)
		{
			float fX = float(iW) / (uNumW - 1);
			unsigned int iIdx = ( iH * uNumW + iW );

			// vertex
			g_aVertexArray[3 * iIdx]		= fTmp * cos( 2 * M_PI * fX);
			g_aVertexArray[3 * iIdx + 1]	= fVY;
			g_aVertexArray[3 * iIdx + 2]	= fTmp * sin(2 * M_PI * fX);

			// texture
			g_aTextureArray[2 * iIdx]		= fX;
			g_aTextureArray[2 * iIdx + 1]	= fY;
		}
	}

	g_uIndexNum = 6 * ( uNumW - 1 ) * ( uNumH - 1 );
	g_aIndexArray = new unsigned int[g_uIndexNum];
	for (unsigned int iH = 0; iH < uNumH - 1; ++iH)
	{
		for (unsigned int iW = 0; iW < uNumW - 1; ++iW)
		{
			unsigned int iVIdx = (iH * uNumW + iW);
			unsigned int iIIdx = 6 * (iH * ( uNumW - 1 ) + iW);

			g_aIndexArray[iIIdx]		= iVIdx;
			g_aIndexArray[iIIdx + 1]	= iVIdx + uNumW;
			g_aIndexArray[iIIdx + 2]	= iVIdx + uNumW + 1;
			g_aIndexArray[iIIdx + 3]	= iVIdx;
			g_aIndexArray[iIIdx + 4]	= iVIdx + uNumW + 1;
			g_aIndexArray[iIIdx + 5]	= iVIdx + 1;
		}
	}
}

// glut idle function
void idle()
{
	glutPostRedisplay();
}

void DrawOneEye(vr::Hmd_Eye eEye)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glClearColor(0.5f, 0.5f, 0.5f, 1);

	glBindTexture(GL_TEXTURE_2D, g_uTextureID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, g_aVertexArray);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, g_aTextureArray);

	glDrawElements(GL_TRIANGLES, g_uIndexNum, GL_UNSIGNED_INT, g_aIndexArray);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// glut display function(draw)
void display()
{
	g_pOpenVRGL->Render(DrawOneEye);

	//glBindFramebuffer(GL_READ_FRAMEBUFFER, g_pOpenVRGL->m_aEyeData[0].m_nRenderFramebufferId);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, g_pOpenVRGL->m_aEyeData[0].m_nResolveFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, g_pOpenVRGL->m_uWidth, g_pOpenVRGL->m_uHeight, 0, 0, g_pOpenVRGL->m_uWidth, g_pOpenVRGL->m_uHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glFinish();

	// swap buffer
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	#pragma region OpenGL Initialize
	// initial glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

	// create glut window
	glutInitWindowSize(640, 640);
	glutCreateWindow("OpenVR Ball");

	// initial glew
	glewInit();

	// OpenGL projection
	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0, 1.0, 100.0, 2000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	0, 0, 0, 
				0, 0, 1, 
				0, 1, 0);

	BuildBall(300, 31, 31);

	// register glut callback functions
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(specialKey);
	#pragma endregion

	#pragma region The texture of ball
	cv::Mat imgImage = cv::imread("D:\\VR360A\\Out\\05.tif");
	cv::cvtColor(imgImage, imgImage, CV_BGR2RGB);

	glGenTextures(1, &g_uTextureID);
	glBindTexture(GL_TEXTURE_2D, g_uTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgImage.cols, imgImage.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, imgImage.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	#pragma endregion

	g_pOpenVRGL = new COpenVRGL();
	g_pOpenVRGL->Initial(100, 5000);

	glutMainLoop();
}
