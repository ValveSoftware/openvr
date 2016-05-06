#pragma once

#pragma region Header Files
// STD Header
#include <array>

// OpenGL related header
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>

// OpenVR Header
#include <openvr.h>
#pragma comment(lib,"openvr_api.lib")
#pragma endregion

/**
 * A helper class for OpenVR with OpenGL
 * Most code is copy from https://github.com/ValveSoftware/openvr/blob/master/samples/hellovr_opengl/hellovr_opengl_main.cpp
 **/
class COpenVRGL
{
protected:
	/**
	 * Store the data for left and right eye
	 **/
	struct SEyeData
	{
		vr::Hmd_Eye m_eEye; // Eye iindex

		GLuint m_nRenderFramebufferId;	// Frame buffer id
		GLuint m_nDepthBufferId;		// depth buffer id of m_nRenderFramebufferId
		GLuint m_nRenderTextureId;		// Frame buffer texture of m_nRenderFramebufferId

		GLuint m_nResolveFramebufferId;	// Frame buffer for distortion
		GLuint m_nResolveTextureId;		// Frame buffer texture of m_nResolveFramebufferId

		glm::mat4x4	m_matEyePos;		// transform matrix of eye position related to head
		glm::mat4x4	m_matProjection;	// projection matrix of eye
	};

public:
	// Initialize.
	bool Initial(float fNear, float fFar);

	void Release();

	// Render frame
	template<typename RENDER_FUNC>
	void Render(RENDER_FUNC funcDraw)
	{
		// Update HMD data
		UpdateHeadPose();

		// draw left eye and right eye scene
		RenderToTarget(funcDraw, vr::Eye_Left);
		RenderToTarget(funcDraw, vr::Eye_Right);

		// 2nd pass render, compute distortion
		RenderDistortionAndSubmit();
	}

	// Draw the frame buffer of one eye to another frame buffer
	void DrawOnBuffer(vr::Hmd_Eye eEye, GLuint uBufferId = 0 );

protected:
	void InitialEyeData(SEyeData &mEyeData, float fNear, float fFar);
	void SetupDistortion();
	void CreateShader();
	void RenderDistortionAndSubmit();
	void UpdateHeadPose();

	template<typename RENDER_FUNC>
	void RenderToTarget(RENDER_FUNC funcDraw, vr::Hmd_Eye eEye)
	{
		SEyeData* pFBD = GetEyeData(eEye);

		glEnable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_FRAMEBUFFER, pFBD->m_nRenderFramebufferId);
		glViewport(0, 0, m_uWidth, m_uHeight);

		funcDraw(eEye, pFBD->m_matEyePos * m_matHMDPose, pFBD->m_matProjection);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, pFBD->m_nRenderFramebufferId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pFBD->m_nResolveFramebufferId);
		
		glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	SEyeData* GetEyeData(vr::Hmd_Eye eEye)
	{
		if (eEye == vr::Eye_Right)
			return &m_aEyeData[1];
		else
			return &m_aEyeData[0];
	}

protected:
	vr::IVRSystem*	m_pVRSystem;	// root object of OpenVR HMD
	glm::mat4		m_matHMDPose;	// transform matrix of HMD
	vr::TrackedDevicePose_t m_aTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];	// Matrix array to save transform matrix for all device

	uint32_t		m_uWidth;			// Width of frame buffer
	uint32_t		m_uHeight;			// Height of frame buffer
	std::array<SEyeData, 2> m_aEyeData;	// data for left and right eye (L/R)

	GLuint			m_uDistortionShaderProgramId;	// Shader program id for 2nd-pass render for distortion
	GLuint			m_unLensVAO;					// Vertex Array Object of the grid mesh of distortion
	GLuint			m_glIDVertBuffer;				// Vertex buffer of m_unLensVAO
	GLuint			m_glIDIndexBuffer;				// Index Buffer of m_unLensVAO
	unsigned int	m_uiIndexSize;					// length of m_glIDIndexBuffer
};
