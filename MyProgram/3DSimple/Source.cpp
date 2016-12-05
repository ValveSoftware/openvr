// STD Headers
#define _USE_MATH_DEFINES
#include <array>
#include <cmath>
#include <iostream>
#include <chrono>
#include <thread>

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

bool			g_bVRVideoMode = false;

float*			g_aVertexArray	= nullptr;
float*			g_aTextureArray	= nullptr;
unsigned int*	g_aIndexArray	= nullptr;
unsigned int	g_uIndexNum		= 0;
GLuint			g_uTextureID	= 0;
cv::VideoCapture	g_cvVideo;
cv::Mat				g_imgFrame;
unsigned int		g_uUpdateTimeInterval = 10;
std::thread			g_threadLoadFrame;
bool				g_bUpdated = false;
bool				g_bRunning = true;

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

void onExit()
{
	g_pOpenVRGL->Release();
	delete g_pOpenVRGL;

	g_bRunning = false;
	g_threadLoadFrame.join();
}

// glut idle function
void idle()
{
	glutPostRedisplay();
}

void timer(int iVal)
{
	if (g_bUpdated)
	{
		glBindTexture(GL_TEXTURE_2D, g_uTextureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g_imgFrame.cols, g_imgFrame.rows, GL_BGR, GL_UNSIGNED_BYTE, g_imgFrame.data);

		g_bUpdated = false;
	}
	glutPostRedisplay();
	glutTimerFunc(g_uUpdateTimeInterval, timer, iVal);
}

void timer_disp(int iVal)
{
	glutPostRedisplay();
	glutTimerFunc(g_uUpdateTimeInterval, timer_disp, iVal);
}

void DrawOneEye(vr::Hmd_Eye eEye, glm::mat4 matModelView, glm::mat4 matProjection)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glClearColor(0.05f, 0.05f, 0.05f, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&(matProjection[0][0]));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&(glm::inverse(matModelView)[0][0]));

	if(!g_bVRVideoMode)
		glTranslatef(0,1.2,0);

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
	static std::chrono::high_resolution_clock::time_point tpLast = std::chrono::high_resolution_clock::now(), tpNow;

	g_pOpenVRGL->ProcessEvent();
	g_pOpenVRGL->Render(DrawOneEye);
	g_pOpenVRGL->DrawOnBuffer(vr::Eye_Left);

	// swap buffer
	glutSwapBuffers();
	
	tpNow = std::chrono::high_resolution_clock::now();
	//std::cout << "FPS :" << 1000.0f / std::chrono::duration_cast<std::chrono::milliseconds>(tpNow - tpLast).count() << "\n";
	tpLast = tpNow;
}

int main(int argc, char** argv)
{
	std::string sSourceFile;
	if (argc >= 2)
	{
		sSourceFile = argv[1];
	}
	else
	{
		std::cerr << "No video file" << std::endl;
		return -1;
	}

	#pragma region Check Source file
	bool bVideo = true;
	std::string sExtName = sSourceFile.substr(sSourceFile.size() - 3);
	if (sExtName == "jpg" || sExtName == "png")
	{
		g_imgFrame = cv::imread(sSourceFile);
		bVideo = false;
	}
	else
	{
		g_cvVideo.open(sSourceFile);
		g_cvVideo >> g_imgFrame;
	}
	#pragma endregion

	#pragma region OpenGL Initialize
	// initial glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

	// create glut window
	glutInitWindowSize(640, 640);
	glutCreateWindow("OpenVR Ball");

	// initial glew
	glewInit();

	if(g_bVRVideoMode)
		BuildBall(10, 51, 51);
	else
		BuildBall(0.5, 51, 51);

	// register glut callback functions
	glutDisplayFunc(display);
	//glutIdleFunc(idle);
	if(bVideo)
		glutTimerFunc(g_uUpdateTimeInterval, timer, 1);
	else
		glutTimerFunc(g_uUpdateTimeInterval, timer_disp, 1);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(specialKey);
	#pragma endregion

	#pragma region The texture of ball
	glGenTextures(1, &g_uTextureID);
	glBindTexture(GL_TEXTURE_2D, g_uTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_imgFrame.cols, g_imgFrame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, g_imgFrame.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	#pragma endregion

	g_pOpenVRGL = new COpenVRGL();
	g_pOpenVRGL->Initial(0.1f, 30);

	if (bVideo == true)
	{
		g_threadLoadFrame = std::thread([]() {
			while (g_bRunning)
			{
				static std::chrono::system_clock::time_point tpNow, tpLUpdate = std::chrono::system_clock::now();
				tpNow = std::chrono::system_clock::now();
				if ((tpNow - tpLUpdate) >= std::chrono::milliseconds(40))
				{
					g_cvVideo >> g_imgFrame;
					g_bUpdated = true;
					tpLUpdate = tpNow;
				}
			}
		});
	}

	std::atexit(onExit);
	glutMainLoop();
}
