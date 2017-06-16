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

#include <glm/gtx/transform.hpp>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#pragma comment(lib,"glew32.lib") 
#ifdef _DEBUG
#pragma comment(lib,"opencv_world320d.lib") 
#pragma comment(lib,"freeglutd.lib") 
#else
#pragma comment(lib,"opencv_world320.lib") 
#pragma comment(lib,"freeglut.lib") 
#endif

#include "OpenVRGL.h"

class CBall
{
public:
	void BuildBall(float fSize, unsigned int uNumW, unsigned int uNumH)
	{
		// TODO: this is a trivial way, the top and bottom should be simplified
		unsigned int uSize = uNumW * uNumH;
		m_aVertexArray = new float[3 * uSize];
		m_aTextureArray = new float[2 * uSize];

		float fXRatio = 360.0 / (uNumW - 1);
		float fYRatio = 180.0 / (uNumH - 1);
		float fX;
		float fY;

		for (int i = 0; i<uNumH; i++)
		{
			for (int j = 0; j<uNumW; j++)
			{				
				m_aTextureArray[2*(i * uNumW + j) ] = j / (uNumW - 1.0);
				m_aTextureArray[2*(i * uNumW + j) + 1] = (i / (uNumH - 1.0));
				fX = -180.0 + j * fXRatio;
				fY = -90.0 + i * fYRatio;
				m_aVertexArray[3 * (i * uNumW + j)] = (fSize*cos(fY*M_PI / 180.0)*cos(fX*M_PI / 180.0));
				m_aVertexArray[3 * (i * uNumW + j) + 1] = (fSize*cos(fY*M_PI / 180.0)*sin(fX*M_PI / 180.0));
				m_aVertexArray[3 * (i * uNumW + j) + 2] = (fSize*sin(fY*M_PI / 180.0));
			}
		}

		m_uIndexNum = (2 * uNumW * (uNumH - 1)) + (uNumH - 1);
		m_aIndexArray = new unsigned int[m_uIndexNum];
		unsigned int uIndex = 0;
		for (int i = 0; i<uNumH - 1; i++)
		{
			for (int j = 0; j<uNumW; j++)
			{
				m_aIndexArray[uIndex] = (i*uNumW + j);
				uIndex++;
				m_aIndexArray[uIndex] = ((i + 1)*uNumW + j);
				uIndex++;
			}
			m_aIndexArray[uIndex] = (std::numeric_limits<unsigned int>::max());
			uIndex++;
		}

	}

	void Initial(const cv::Mat& rImg)
	{
		glGenTextures(1, &m_uTextureID);
		glBindTexture(GL_TEXTURE_2D, m_uTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rImg.cols, rImg.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, rImg.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glPrimitiveRestartIndex(std::numeric_limits<unsigned int>::max());
		glEnable(GL_PRIMITIVE_RESTART);
	}

	void Release()
	{
		glDeleteTextures(1, &m_uTextureID);
	}

	void Draw()
	{
		glBindTexture(GL_TEXTURE_2D, m_uTextureID);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, m_aVertexArray);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, m_aTextureArray);

//		glDrawElements(GL_TRIANGLES, m_uIndexNum, GL_UNSIGNED_INT, m_aIndexArray);
		glDrawElements(GL_TRIANGLE_STRIP, m_uIndexNum, GL_UNSIGNED_INT, m_aIndexArray);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	}

	void UpdateTexture(const cv::Mat& rImg )
	{
		glBindTexture(GL_TEXTURE_2D, m_uTextureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rImg.cols, rImg.rows, GL_BGR, GL_UNSIGNED_BYTE, rImg.data);

	}

protected:
	float*			m_aVertexArray = nullptr;
	float*			m_aTextureArray = nullptr;
	unsigned int*	m_aIndexArray = nullptr;
	unsigned int	m_uIndexNum = 0;
	GLuint			m_uTextureID = 0;
};

COpenVRGL*	g_pOpenVRGL = nullptr;
CBall		g_Ball;
glm::mat4	g_matBall;

bool			g_bVRVideoMode = true;

cv::VideoCapture	g_cvVideo;
cv::Mat				g_imgFrame;
unsigned int		g_uUpdateTimeInterval = 10;
std::thread			g_threadLoadFrame;
bool				g_bUpdated = false;
bool				g_bRunning = true;

void onExit()
{
	g_pOpenVRGL->Release();
	delete g_pOpenVRGL;

	g_bRunning = false;
	g_threadLoadFrame.join();
}

void timer(int iVal)
{
	glutPostRedisplay();

	if (g_bUpdated)
	{
		g_Ball.UpdateTexture(g_imgFrame);
		g_bUpdated = false;
	}

	auto* pController = g_pOpenVRGL->GetController(vr::TrackedControllerRole_Invalid);
	if (pController != nullptr)
	{
		if (pController->m_eState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad))
		{
			glm::mat4 matHMD = g_pOpenVRGL->GetHMDPose();
			glm::vec3 vecSide = COpenVRGL::GetCameraSide(matHMD);
			glm::vec3 vecUp = COpenVRGL::GetCameraUpper(matHMD);

			glm::vec2 vTP(pController->m_eState.rAxis[0].x, pController->m_eState.rAxis[0].y);

			g_matBall = glm::rotate(0.05f * vTP.x, vecUp) * glm::rotate(-0.05f * vTP.y, vecSide) * g_matBall;
		}
		if (pController->m_eState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip))
		{
			g_matBall = glm::mat4();
		}
	}

	glutTimerFunc(g_uUpdateTimeInterval, timer, iVal);
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
	glRotatef(90, 1, 0, 0);

	if(!g_bVRVideoMode)
		glTranslatef(0,1.2,0);

	glMultMatrixf(&g_matBall[0][0]);

	g_Ball.Draw();
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
		g_Ball.BuildBall(10, 255, 255);
	else
		g_Ball.BuildBall(0.5, 51, 51);
	g_Ball.Initial(g_imgFrame);

	// register glut callback functions
	glutDisplayFunc(display);
	//glutIdleFunc(idle);
	glutTimerFunc(g_uUpdateTimeInterval, timer, 1);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(specialKey);
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
