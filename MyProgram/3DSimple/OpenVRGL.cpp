#include "OpenVRGL.h"

#include <vector>
#include <glm/matrix.hpp>

struct VertexDataLens
{
	std::array<float, 2> position;
	std::array<float, 2> texCoordRed;
	std::array<float, 2> texCoordGreen;
	std::array<float, 2> texCoordBlue;
};

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


bool COpenVRGL::Initial(float fNear, float fFar)
{
	// Initial VR System
	vr::EVRInitError eError = vr::VRInitError_None;
	vr::IVRSystem* pVRSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);
	if (eError != vr::VRInitError_None)
		return false;

	// Get render size
	m_pVRSystem = pVRSystem;

	m_pDisplayModule = new CVRDisplay(m_pVRSystem);
	m_pDisplayModule->Initial(fNear, fFar);

	return true;
}

void COpenVRGL::Release()
{
	vr::VR_Shutdown();

	delete m_pVRSystem;
	delete m_pDisplayModule;
}

void COpenVRGL::UpdateHeadPose()
{
	vr::VRCompositor()->WaitGetPoses(m_aTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
	if (m_aTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		vr::HmdMatrix34_t& mat = m_aTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking;
		m_pDisplayModule->SetHMDPose(glm::mat4(
			mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0,
			mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0,
			mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0,
			mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f
		));
	}
}

void COpenVRGL::CVRDisplay::InitialEyeData(SEyeData & mEyeData, float fNear, float fFar)
{
	{
		vr::HmdMatrix34_t mat = m_pVRSystem->GetEyeToHeadTransform(mEyeData.m_eEye);
		mEyeData.m_matEyePos = glm::mat4(
			mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0,
			mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0,
			mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0,
			mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f
		);
	}
	{
		vr::HmdMatrix44_t mat = m_pVRSystem->GetProjectionMatrix(mEyeData.m_eEye, fNear, fFar, vr::API_OpenGL);
		mEyeData.m_matProjection = glm::mat4(
			mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
			mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
			mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
			mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]);
	}

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

void COpenVRGL::CVRDisplay::SetupDistortion()
{
	GLushort m_iLensGridSegmentCountH = 43;
	GLushort m_iLensGridSegmentCountV = 43;

	float w = (float)(1.0 / float(m_iLensGridSegmentCountH - 1));
	float h = (float)(1.0 / float(m_iLensGridSegmentCountV - 1));

	float u, v = 0;

	std::vector<VertexDataLens> vVerts(0);
	VertexDataLens vert;

	//left eye distortion verts
	float Xoffset = -1;
	for (int y = 0; y<m_iLensGridSegmentCountV; y++)
	{
		for (int x = 0; x<m_iLensGridSegmentCountH; x++)
		{
			u = x*w; v = 1 - y*h;
			vert.position = { Xoffset + u, -1 + 2 * y*h };

			vr::DistortionCoordinates_t dc0;
			m_pVRSystem->ComputeDistortion(vr::Eye_Left, u, v, &dc0);

			vert.texCoordRed = { dc0.rfRed[0], 1 - dc0.rfRed[1] };
			vert.texCoordGreen = { dc0.rfGreen[0], 1 - dc0.rfGreen[1] };
			vert.texCoordBlue = { dc0.rfBlue[0], 1 - dc0.rfBlue[1] };

			vVerts.push_back(vert);
		}
	}

	//right eye distortion verts
	Xoffset = 0;
	for (int y = 0; y<m_iLensGridSegmentCountV; y++)
	{
		for (int x = 0; x<m_iLensGridSegmentCountH; x++)
		{
			u = x*w; v = 1 - y*h;
			vert.position = { Xoffset + u, -1 + 2 * y*h };

			vr::DistortionCoordinates_t dc0;
			m_pVRSystem->ComputeDistortion(vr::Eye_Right, u, v, &dc0);

			vert.texCoordRed = { dc0.rfRed[0], 1 - dc0.rfRed[1] };
			vert.texCoordGreen = { dc0.rfGreen[0], 1 - dc0.rfGreen[1] };
			vert.texCoordBlue = { dc0.rfBlue[0], 1 - dc0.rfBlue[1] };

			vVerts.push_back(vert);
		}
	}

	std::vector<GLushort> vIndices;
	GLushort a, b, c, d;

	GLushort offset = 0;
	for (GLushort y = 0; y<m_iLensGridSegmentCountV - 1; y++)
	{
		for (GLushort x = 0; x<m_iLensGridSegmentCountH - 1; x++)
		{
			a = m_iLensGridSegmentCountH*y + x + offset;
			b = m_iLensGridSegmentCountH*y + x + 1 + offset;
			c = (y + 1)*m_iLensGridSegmentCountH + x + 1 + offset;
			d = (y + 1)*m_iLensGridSegmentCountH + x + offset;
			vIndices.push_back(a);
			vIndices.push_back(b);
			vIndices.push_back(c);

			vIndices.push_back(a);
			vIndices.push_back(c);
			vIndices.push_back(d);
		}
	}

	offset = (m_iLensGridSegmentCountH)*(m_iLensGridSegmentCountV);
	for (GLushort y = 0; y<m_iLensGridSegmentCountV - 1; y++)
	{
		for (GLushort x = 0; x<m_iLensGridSegmentCountH - 1; x++)
		{
			a = m_iLensGridSegmentCountH*y + x + offset;
			b = m_iLensGridSegmentCountH*y + x + 1 + offset;
			c = (y + 1)*m_iLensGridSegmentCountH + x + 1 + offset;
			d = (y + 1)*m_iLensGridSegmentCountH + x + offset;
			vIndices.push_back(a);
			vIndices.push_back(b);
			vIndices.push_back(c);

			vIndices.push_back(a);
			vIndices.push_back(c);
			vIndices.push_back(d);
		}
	}
	m_uiIndexSize = vIndices.size();

	glGenVertexArrays(1, &m_unLensVAO);
	glBindVertexArray(m_unLensVAO);

	glGenBuffers(1, &m_glIDVertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_glIDVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, vVerts.size() * sizeof(VertexDataLens), &vVerts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_glIDIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIDIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size() * sizeof(GLushort), &vIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, texCoordRed));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, texCoordGreen));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataLens), (void *)offsetof(VertexDataLens, texCoordBlue));

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void COpenVRGL::CVRDisplay::CreateShader()
{
	m_uDistortionShaderProgramId = CompileGLShader(
		"Distortion",

		// vertex shader
		"#version 410 core\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec2 v2UVredIn;\n"
		"layout(location = 2) in vec2 v2UVGreenIn;\n"
		"layout(location = 3) in vec2 v2UVblueIn;\n"
		"noperspective  out vec2 v2UVred;\n"
		"noperspective  out vec2 v2UVgreen;\n"
		"noperspective  out vec2 v2UVblue;\n"
		"void main()\n"
		"{\n"
		"	v2UVred = v2UVredIn;\n"
		"	v2UVgreen = v2UVGreenIn;\n"
		"	v2UVblue = v2UVblueIn;\n"
		"	gl_Position = position;\n"
		"}\n",

		// fragment shader
		"#version 410 core\n"
		"uniform sampler2D mytexture;\n"

		"noperspective  in vec2 v2UVred;\n"
		"noperspective  in vec2 v2UVgreen;\n"
		"noperspective  in vec2 v2UVblue;\n"

		"out vec4 outputColor;\n"

		"void main()\n"
		"{\n"
		"	float fBoundsCheck = ( (dot( vec2( lessThan( v2UVgreen.xy, vec2(0.05, 0.05)) ), vec2(1.0, 1.0))+dot( vec2( greaterThan( v2UVgreen.xy, vec2( 0.95, 0.95)) ), vec2(1.0, 1.0))) );\n"
		"	if( fBoundsCheck > 1.0 )\n"
		"	{ outputColor = vec4( 0, 0, 0, 1.0 ); }\n"
		"	else\n"
		"	{\n"
		"		float red = texture(mytexture, v2UVred).x;\n"
		"		float green = texture(mytexture, v2UVgreen).y;\n"
		"		float blue = texture(mytexture, v2UVblue).z;\n"
		"		outputColor = vec4( red, green, blue, 1.0  ); }\n"
		"}\n"
	);
}

void COpenVRGL::CVRDisplay::RenderDistortionAndSubmit()
{
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, m_uWidth, m_uHeight);

	glBindVertexArray(m_unLensVAO);
	glUseProgram(m_uDistortionShaderProgramId);

	//render left lens (first half of index array )
	glBindTexture(GL_TEXTURE_2D, m_aEyeData[0].m_nResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDrawElements(GL_TRIANGLES, m_uiIndexSize / 2, GL_UNSIGNED_SHORT, 0);

	//render right lens (second half of index array )
	glBindTexture(GL_TEXTURE_2D, m_aEyeData[1].m_nResolveTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glDrawElements(GL_TRIANGLES, m_uiIndexSize / 2, GL_UNSIGNED_SHORT, (const void *)(m_uiIndexSize));

	glBindVertexArray(0);
	glUseProgram(0);

	// submit
	vr::Texture_t leftEyeTexture = { (void*)m_aEyeData[0].m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::Texture_t rightEyeTexture = { (void*)m_aEyeData[1].m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

	glFlush();
}

void COpenVRGL::CVRDisplay::DrawOnBuffer(vr::Hmd_Eye eEye, GLuint uBufferId)
{
	// copy left eye frame to frame buffer for display
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GetEyeData(eEye)->m_nResolveFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, uBufferId);

	glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
