#include "OpenVRGL.h"

#include <vector>
#include <thread>
#include <string>
#include <iostream>

#include <glm/matrix.hpp>

#pragma region Internal Functions
std::string GetTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char *pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}

GLuint CompileGLShader(const char * pchShaderName, const char * pchVertexShader, const char * pchFragmentShader)
{
	GLuint unProgramID = glCreateProgram();

	GLuint nSceneVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(nSceneVertexShader, 1, &pchVertexShader, NULL);
	glCompileShader(nSceneVertexShader);

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(nSceneVertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		printf("%s - Unable to compile vertex shader %d!\n", pchShaderName, nSceneVertexShader);
		glDeleteProgram(unProgramID);
		glDeleteShader(nSceneVertexShader);
		return 0;
	}
	glAttachShader(unProgramID, nSceneVertexShader);
	glDeleteShader(nSceneVertexShader); // the program hangs onto this once it's attached

	GLuint  nSceneFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(nSceneFragmentShader, 1, &pchFragmentShader, NULL);
	glCompileShader(nSceneFragmentShader);

	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(nSceneFragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		printf("%s - Unable to compile fragment shader %d!\n", pchShaderName, nSceneFragmentShader);
		glDeleteProgram(unProgramID);
		glDeleteShader(nSceneFragmentShader);
		return 0;
	}

	glAttachShader(unProgramID, nSceneFragmentShader);
	glDeleteShader(nSceneFragmentShader); // the program hangs onto this once it's attached

	glLinkProgram(unProgramID);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(unProgramID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("%s - Error linking program %d!\n", pchShaderName, unProgramID);
		glDeleteProgram(unProgramID);
		return 0;
	}

	glUseProgram(unProgramID);
	glUseProgram(0);

	return unProgramID;
}

glm::mat4 ConvertMatrix( const vr::HmdMatrix34_t& mat )
{
	return  glm::mat4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0,
		mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0,
		mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0,
		mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f
	);
}

glm::mat4 ConvertMatrix(const vr::HmdMatrix44_t& mat)
{
	return  glm::mat4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}
#pragma endregion

#pragma region Functions of COpenVRGL
bool COpenVRGL::Initial(float fNear, float fFar)
{
	// Initial VR System
	vr::EVRInitError eError = vr::VRInitError_None;
	vr::IVRSystem* pVRSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);
	if (eError != vr::VRInitError_None)
		return false;

	// Get render size
	m_pVRSystem = pVRSystem;

	m_pDisplayModule = new CDisplay(m_pVRSystem);
	m_pDisplayModule->Initial(fNear, fFar);

	m_pDeviceModel = new CDeviceModel(m_pVRSystem);
	m_pDeviceModel->Initial();

	InitializeDevice();
	return true;
}

void COpenVRGL::Release()
{
	vr::VR_Shutdown();
	m_pVRSystem = nullptr;

	if (m_pDisplayModule != nullptr)
	{
		m_pDisplayModule->Release();
		delete m_pDisplayModule;
		m_pDisplayModule = nullptr;
	}
}

void COpenVRGL::ProcessEvent()
{
	// Process SteamVR events
	vr::VREvent_t evtDevice;
	while (m_pVRSystem->PollNextEvent(&evtDevice, sizeof(evtDevice)))
	{
		switch (evtDevice.eventType)
		{
		case vr::VREvent_TrackedDeviceActivated:
			AddNewDevice(evtDevice.trackedDeviceIndex);
			break;

		case vr::VREvent_TrackedDeviceDeactivated:
			RemoveDevice(evtDevice.trackedDeviceIndex);
			break;

		case vr::VREvent_TrackedDeviceUpdated:
			break;

		default:
			break;
		}
	}

	for (auto& rController : m_vController)
	{
		rController.second.ProcessEvent(vr::TrackingUniverseStanding);
	}
}

COpenVRGL::CController* COpenVRGL::GetController(vr::ETrackedControllerRole eRole)
{
	if (m_vController.size() > 0)
	{
		if (eRole == vr::TrackedControllerRole_Invalid)
			return &(m_vController.begin()->second);

		for (auto it : m_vController)
		{
			if (it.second.m_eRole == eRole)
				return &(it.second);
		}
	}

	return nullptr;
}

void COpenVRGL::UpdateHeadPose()
{
	vr::VRCompositor()->WaitGetPoses(m_aTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	for (uint32_t uDeviceIndex = 0; uDeviceIndex < vr::k_unMaxTrackedDeviceCount; ++uDeviceIndex)
	{
		if (m_aTrackedDevicePose[uDeviceIndex].bPoseIsValid)
			m_aTrackedDeviceMatrix[uDeviceIndex] = ConvertMatrix(m_aTrackedDevicePose[uDeviceIndex].mDeviceToAbsoluteTracking);
	}

	if (m_aTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		m_pDisplayModule->SetHMDPose(m_aTrackedDeviceMatrix[vr::k_unTrackedDeviceIndex_Hmd]);
	}

}

void COpenVRGL::InitializeDevice()
{
	for (uint32_t uDeviceIndex = vr::k_unTrackedDeviceIndex_Hmd + 1; uDeviceIndex < vr::k_unMaxTrackedDeviceCount; ++uDeviceIndex)
	{
		if (m_pVRSystem->IsTrackedDeviceConnected(uDeviceIndex))
			AddNewDevice(uDeviceIndex);
	}
}

void COpenVRGL::AddNewDevice(vr::TrackedDeviceIndex_t uIdx)
{
	vr::ETrackedDeviceClass eDeviceType = m_pVRSystem->GetTrackedDeviceClass(uIdx);
	if (eDeviceType == vr::TrackedDeviceClass_Controller)
	{
		if (m_vController.find(uIdx) == m_vController.end())
		{
			m_vController.emplace(uIdx, CController(m_pVRSystem, uIdx));
		}
	}

	m_pDeviceModel->SetupRenderModelForTrackedDevice(uIdx);
}

void COpenVRGL::RemoveDevice(vr::TrackedDeviceIndex_t uIdx)
{
	vr::ETrackedDeviceClass eDeviceType = m_pVRSystem->GetTrackedDeviceClass(uIdx);
	if (eDeviceType == vr::TrackedDeviceClass_Controller)
	{
		if (m_vController.find(uIdx) != m_vController.end())
		{
			m_vController.erase(uIdx);
		}
	}
}
#pragma endregion

#pragma region Functions of COpenVRGL::CVRDisplay
struct VertexDataLens
{
	std::array<float, 2> position;
	std::array<float, 2> texCoord;

	VertexDataLens(const std::array<float, 2> & pos, const std::array<float, 2> tex) : position(pos), texCoord(tex) {}
};

void COpenVRGL::CDisplay::InitialEyeData(SEyeData & mEyeData, float fNear, float fFar)
{
	mEyeData.m_matEyePos = ConvertMatrix(m_pVRSystem->GetEyeToHeadTransform(mEyeData.m_eEye));
	mEyeData.m_matProjection = ConvertMatrix(m_pVRSystem->GetProjectionMatrix(mEyeData.m_eEye, fNear, fFar, vr::API_OpenGL));

	{
		// Frame Buffer
		glGenFramebuffers(1, &mEyeData.m_nRenderFramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, mEyeData.m_nRenderFramebufferId);

		// Depth Buffer
		glGenRenderbuffers(1, &mEyeData.m_nDepthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, mEyeData.m_nDepthBufferId);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, m_uWidth, m_uHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mEyeData.m_nDepthBufferId);

		// frame texture
		glGenTextures(1, &mEyeData.m_nRenderTextureId);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mEyeData.m_nRenderTextureId);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, m_uWidth, m_uHeight, true);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mEyeData.m_nRenderTextureId, 0);
	}

	{
		// Distortion frame buffer
		glGenFramebuffers(1, &mEyeData.m_nResolveFramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, mEyeData.m_nResolveFramebufferId);

		// Distortion frame texture
		glGenTextures(1, &mEyeData.m_nResolveTextureId);
		glBindTexture(GL_TEXTURE_2D, mEyeData.m_nResolveTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_uWidth, m_uHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mEyeData.m_nResolveTextureId, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void COpenVRGL::CDisplay::Release()
{
	for (auto& rEyeData : m_aEyeData)
	{
		glDeleteRenderbuffers(1, &rEyeData.m_nDepthBufferId);
		glDeleteTextures(1, &rEyeData.m_nRenderTextureId);
		glDeleteFramebuffers(1, &rEyeData.m_nRenderFramebufferId);
		glDeleteTextures(1, &rEyeData.m_nResolveTextureId);
		glDeleteFramebuffers(1, &rEyeData.m_nResolveFramebufferId);
	}
}

void COpenVRGL::CDisplay::Submit()
{
	// submit
	vr::Texture_t leftEyeTexture = { (void*)m_aEyeData[0].m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::Texture_t rightEyeTexture = { (void*)m_aEyeData[1].m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

	//glFinish();
}

void COpenVRGL::CDisplay::DrawOnBuffer(vr::Hmd_Eye eEye, GLuint uBufferId)
{
	// copy left eye frame to frame buffer for display
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GetEyeData(eEye)->m_nResolveFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, uBufferId);

	glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
#pragma endregion

#pragma region Functions of COpenVRGL::CDeviceModel::CGLRenderModel
COpenVRGL::CDeviceModel::CGLRenderModel::CGLRenderModel(const std::string& sModelName)
{
	m_sModelName = sModelName;
	m_glIndexBuffer = 0;
	m_glVertArray = 0;
	m_glVertBuffer = 0;
	m_glTexture = 0;
}

COpenVRGL::CDeviceModel::CGLRenderModel::~CGLRenderModel()
{
	Release();
}

bool COpenVRGL::CDeviceModel::CGLRenderModel::Initial(const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture)
{
	// create and bind a VAO to hold state for this model
	glGenVertexArrays(1, &m_glVertArray);
	glBindVertexArray(m_glVertArray);

	// Populate a vertex buffer
	glGenBuffers(1, &m_glVertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vr::RenderModel_Vertex_t) * vrModel.unVertexCount, vrModel.rVertexData, GL_STATIC_DRAW);

	// Identify the components in the vertex buffer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vr::RenderModel_Vertex_t), (void *)offsetof(vr::RenderModel_Vertex_t, vPosition));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vr::RenderModel_Vertex_t), (void *)offsetof(vr::RenderModel_Vertex_t, vNormal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vr::RenderModel_Vertex_t), (void *)offsetof(vr::RenderModel_Vertex_t, rfTextureCoord));

	// Create and populate the index buffer
	glGenBuffers(1, &m_glIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * vrModel.unTriangleCount * 3, vrModel.rIndexData, GL_STATIC_DRAW);

	glBindVertexArray(0);

	// create and populate the texture
	glGenTextures(1, &m_glTexture);
	glBindTexture(GL_TEXTURE_2D, m_glTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vrDiffuseTexture.unWidth, vrDiffuseTexture.unHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, vrDiffuseTexture.rubTextureMapData);

	// If this renders black ask McJohn what's wrong.
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_unVertexCount = vrModel.unTriangleCount * 3;

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}

void COpenVRGL::CDeviceModel::CGLRenderModel::Release()
{
	if (m_glVertBuffer)
	{
		glDeleteBuffers(1, &m_glIndexBuffer);
		glDeleteVertexArrays(1, &m_glVertArray);
		glDeleteBuffers(1, &m_glVertBuffer);
		m_glIndexBuffer = 0;
		m_glVertArray = 0;
		m_glVertBuffer = 0;
	}
}
#pragma endregion

#pragma region Functions of COpenVRGL::CDeviceModel
COpenVRGL::CDeviceModel::CDeviceModel(vr::IVRSystem * pVRSystem)
{
	m_pVRSystem = pVRSystem;

	for (auto& pModel : m_aDeviceModel)
		pModel = nullptr;
}

COpenVRGL::CDeviceModel::~CDeviceModel()
{
	Release();
}

void COpenVRGL::CDeviceModel::Initial()
{
	m_unRenderModelProgramID = CompileGLShader(
		"render model",

		// vertex shader
		"#version 410\n"
		"uniform mat4 matrix;\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec3 v3NormalIn;\n"
		"layout(location = 2) in vec2 v2TexCoordsIn;\n"
		"out vec2 v2TexCoord;\n"
		"void main()\n"
		"{\n"
		"	v2TexCoord = v2TexCoordsIn;\n"
		"	gl_Position = matrix * vec4(position.xyz, 1);\n"
		"}\n",

		//fragment shader
		"#version 410 core\n"
		"uniform sampler2D diffuse;\n"
		"in vec2 v2TexCoord;\n"
		"out vec4 outputColor;\n"
		"void main()\n"
		"{\n"
		"   outputColor = texture( diffuse, v2TexCoord);\n"
		"}\n"

	);
	m_nRenderModelMatrixLocation = glGetUniformLocation(m_unRenderModelProgramID, "matrix");
}

void COpenVRGL::CDeviceModel::Release()
{
	if (m_unRenderModelProgramID!=0)
		glDeleteProgram(m_unRenderModelProgramID);

	for (auto& pModel : m_mapRenderModel)
		delete pModel.second;
	m_mapRenderModel.clear();
}

void COpenVRGL::CDeviceModel::Draw(vr::Hmd_Eye eEye, const glm::mat4& matModelView, const glm::mat4& matProjection, const std::array<glm::mat4, vr::k_unMaxTrackedDeviceCount>& aMatrix)
{
	glUseProgram(m_unRenderModelProgramID);
	for (uint32_t uDeviceIndex = vr::k_unTrackedDeviceIndex_Hmd + 1; uDeviceIndex < vr::k_unMaxTrackedDeviceCount; ++uDeviceIndex)
	{
		if (m_aDeviceModel[uDeviceIndex] == nullptr || m_aShowDevice[uDeviceIndex] == false)
			continue;

		const glm::mat4& matDeviceToTracking = aMatrix[uDeviceIndex];

		auto matMVP = matProjection * glm::inverse(matModelView) * matDeviceToTracking;
		glUniformMatrix4fv(m_nRenderModelMatrixLocation, 1, GL_FALSE, &(matMVP[0][0]));

		m_aDeviceModel[uDeviceIndex]->Draw();
	}
	glUseProgram(0);
}

COpenVRGL::CDeviceModel::CGLRenderModel* COpenVRGL::CDeviceModel::GetRenderModel(const std::string& sModelName)
{
	// check if model already loaded
	auto itModel = m_mapRenderModel.find(sModelName);
	if (itModel != m_mapRenderModel.end())
	{
		return itModel->second;
	}

	// load the model if we didn't find one
	vr::RenderModel_t *pModel;
	vr::EVRRenderModelError error;
	vr::IVRRenderModels* pRM = vr::VRRenderModels();

	// Load Model
	while (true)
	{
		error = pRM->LoadRenderModel_Async(sModelName.c_str(), &pModel);
		if (error != vr::VRRenderModelError_Loading)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	if (error != vr::VRRenderModelError_None)
		return nullptr;

	// Load texture
	vr::RenderModel_TextureMap_t *pTexture;
	while (true)
	{
		error = pRM->LoadTexture_Async(pModel->diffuseTextureId, &pTexture);
		if (error != vr::VRRenderModelError_Loading)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	if (error != vr::VRRenderModelError_None)
	{
		pRM->FreeRenderModel(pModel);
		return nullptr;
	}

	// Initialize Model
	CGLRenderModel* pRenderModel = new CGLRenderModel(sModelName);
	if (!pRenderModel->Initial(*pModel, *pTexture))
	{
		delete pRenderModel;
		pRenderModel = nullptr;
	}
	else
	{
		m_mapRenderModel[sModelName] = pRenderModel;
	}

	pRM->FreeRenderModel(pModel);
	pRM->FreeTexture(pTexture);

	return pRenderModel;
}

void COpenVRGL::CDeviceModel::SetupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t uDeviceIndex)
{
	if (uDeviceIndex >= vr::k_unMaxTrackedDeviceCount)
		return;

	// try to find a model we've already set up
	std::string sRenderModelName = GetTrackedDeviceString(m_pVRSystem, uDeviceIndex, vr::Prop_RenderModelName_String);
	CGLRenderModel *pRenderModel = GetRenderModel(sRenderModelName);
	std::cout << " > Device << " << uDeviceIndex << " : " << sRenderModelName << std::endl;
	if (pRenderModel != nullptr)
	{
		m_aDeviceModel[uDeviceIndex] = pRenderModel;
		m_aShowDevice[uDeviceIndex] = true;
	}
}

#pragma endregion

COpenVRGL::CController::CController(vr::IVRSystem * pVRSystem, vr::TrackedDeviceIndex_t uIdx)
{
	m_pVRSystem = pVRSystem;
	m_uIdx = uIdx;
	m_eRole = m_pVRSystem->GetControllerRoleForTrackedDeviceIndex(uIdx);
}

bool COpenVRGL::CController::ProcessEvent(vr::ETrackingUniverseOrigin eOrigin)
{
	if (m_pVRSystem->GetControllerStateWithPose(eOrigin, m_uIdx, &m_eState, sizeof(m_eState), &m_Pose))
	{
		if (m_eState.ulButtonTouched != 0)
		{
			if (m_eState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu))
			{
				std::cout << "[App]";
			}
			if (m_eState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_Grip))
			{
				std::cout << "[Grip]";
			}
			if (m_eState.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger))
			{
				std::cout << "[Trigger:";
				if (m_eState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger))
					std::cout << "down";
				std::cout << "](" << m_eState.rAxis[1].x << ")";
			}
			if (m_eState.ulButtonTouched & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad))
			{
				std::cout << "[Touch:";
				if (m_eState.ulButtonPressed & vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad))
					std::cout << "down";
				std::cout << "](" << m_eState.rAxis[0].x << "/" << m_eState.rAxis[0].y << ")";
			}
			std::cout << std::endl;
		}
	}
	return false;
}
