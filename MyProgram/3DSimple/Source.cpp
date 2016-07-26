// STD Headers
#define _USE_MATH_DEFINES
#include <array>
#include <cmath>
#include <iostream>
#include <chrono>

// freeglut
#include <GL/glew.h>
#include <GL/freeglut.h>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

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
cv::VideoCapture	g_cvVideo;
cv::Mat				g_imgFrame;
unsigned int		g_uUpdateTimeInterval = 10;

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

void timer(int iVal)
{
	static std::chrono::system_clock::time_point tpNow, tpLUpdate = std::chrono::system_clock::now();
	tpNow = std::chrono::system_clock::now();
	if ((tpNow - tpLUpdate) >= std::chrono::milliseconds(40))
	{
		g_cvVideo >> g_imgFrame;

		glBindTexture(GL_TEXTURE_2D, g_uTextureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g_imgFrame.cols, g_imgFrame.rows, GL_BGR, GL_UNSIGNED_BYTE, g_imgFrame.data);

		tpLUpdate = tpNow;
	}

	glutPostRedisplay();
	glutTimerFunc(g_uUpdateTimeInterval, timer, 1);
}

void DrawOneEye(vr::Hmd_Eye eEye, glm::mat4 matModelView, glm::mat4 matProjection)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glClearColor(0.5f, 0.5f, 0.5f, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&(matProjection[0][0]));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&(glm::inverse(matModelView)[0][0]));

	//glm::vec3	vPos = COpenVRGL::GetCameraPos(matModelView);
	//glm::vec3	vCen = COpenVRGL::GetCameraDir( matModelView ) + vPos;
	//glm::vec3	vUp = COpenVRGL::GetCameraUpper( matModelView );
	//gluLookAt(
	//	vPos[0], vPos[1], vPos[2],
	//	vCen[0], vCen[1], vCen[2],
	//	vUp[0], vUp[1], vUp[2]);

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
	g_pOpenVRGL->DrawOnBuffer(vr::Eye_Left);

	// swap buffer
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	std::string sVideoFile;
	if (argc >= 2)
	{
		sVideoFile = argv[1];
	}
	else
	{
		std::cerr << "No video file" << std::endl;
		return -1;
	}

	#pragma region OpenGL Initialize
	// initial glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

	// create glut window
	glutInitWindowSize(640, 640);
	glutCreateWindow("OpenVR Ball");

	// initial glew
	glewInit();

	BuildBall(50, 51, 51);

	// register glut callback functions
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutTimerFunc(g_uUpdateTimeInterval,timer,1);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(specialKey);
	#pragma endregion

	#pragma region The texture of ball
	g_cvVideo.open(sVideoFile);
	g_cvVideo >> g_imgFrame;

	glGenTextures(1, &g_uTextureID);
	glBindTexture(GL_TEXTURE_2D, g_uTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_imgFrame.cols, g_imgFrame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, g_imgFrame.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	#pragma endregion

	g_pOpenVRGL = new COpenVRGL();
	g_pOpenVRGL->Initial(1, 1000);

	glutMainLoop();
}
