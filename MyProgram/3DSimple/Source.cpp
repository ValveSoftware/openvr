// STD Headers
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

// OpenVR
#include <openvr.h>
#pragma comment(lib,"openvr_api.lib") 

// freeglut
#include <GL/freeglut.h>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#ifdef _DEBUG
#pragma comment(lib,"opencv_world310d.lib") 
#else
#pragma comment(lib,"opencv_world310.lib") 
#endif

float*			g_aVertexArray	= nullptr;
float*			g_aTextureArray	= nullptr;
unsigned int*	g_aIndexArray	= nullptr;
unsigned int	g_uIndexNum		= 0;

void BuildBall( float fSize, unsigned int uNumW, unsigned int uNumH)
{
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

// glut display function(draw)
void display()
{
	// clear previous screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, g_aVertexArray);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, g_aTextureArray);
	
	glDrawElements(GL_TRIANGLES, g_uIndexNum, GL_UNSIGNED_INT, g_aIndexArray);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (false)
	{
		// Coordinate
		glLineWidth(10.0f);
		glBegin(GL_LINES);
		glColor3ub(255, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(50, 0, 0);

		glColor3ub(0, 255, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 50, 0);

		glColor3ub(0, 0, 255);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 50);
		glEnd();
		glLineWidth(1.0f);
	}

	// swap buffer
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	// Initial VR System
	//vr::EVRInitError eError = vr::VRInitError_None;
	//vr::IVRSystem* pVRSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);
	//if (eError != vr::VRInitError_None)
	//{
	//	std::cerr << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
	//	return 1;
	//}

	#pragma region OpenGL Initialize
	// initial glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

	// create glut window
	glutInitWindowSize(640, 640);
	glutCreateWindow("OpenVR Ball");

	// set OpenGL environment
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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


	cv::Mat imgImage = cv::imread("D:\\VR360A\\Out\\05.tif");

	GLuint uTextureID;
	glGenTextures(1, &uTextureID);
	glBindTexture(GL_TEXTURE_2D, uTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgImage.cols, imgImage.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, imgImage.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glutMainLoop();
}
