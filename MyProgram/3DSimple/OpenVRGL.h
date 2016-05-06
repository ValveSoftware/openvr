#pragma once
#include <array>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>


#include <openvr.h>
#pragma comment(lib,"openvr_api.lib")

class COpenVRGL
{
protected:
	struct SEyeData
	{
		vr::Hmd_Eye m_eEye;

		GLuint m_nRenderFramebufferId;
		GLuint m_nDepthBufferId;
		GLuint m_nRenderTextureId;

		GLuint m_nResolveFramebufferId;
		GLuint m_nResolveTextureId;

		glm::mat4x4	m_matEyePos;
		glm::mat4x4	m_matProjection;
	};

public:
	uint32_t		m_uWidth;
	uint32_t		m_uHeight;
	vr::IVRSystem*	m_pVRSystem;
	std::array<SEyeData, 2> m_aEyeData;

	GLuint			m_uDistortionShaderProgramId;
	GLuint			m_unLensVAO;
	GLuint			m_glIDVertBuffer;
	GLuint			m_glIDIndexBuffer;
	unsigned int	m_uiIndexSize;

	glm::mat4		m_matHMDPose;
	vr::TrackedDevicePose_t m_aTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];

public:
	bool Initial(float fNear, float fFar);

	void Release()
	{
		vr::VR_Shutdown();
	}

	template<typename RENDER_FUNC>
	void Render(RENDER_FUNC funcDraw)
	{
		UpdateHeadPose();

		RenderToTarget(funcDraw, vr::Eye_Right);
		RenderToTarget(funcDraw, vr::Eye_Left);

		RenderDistortionAndSubmit();
	}

protected:
	void InitialEyeData(SEyeData &mEyeData, float fNear, float fFar);
	void SetupDistortion();
	void CreateShader();
	void RenderDistortionAndSubmit();
	void UpdateHeadPose();

	template<typename RENDER_FUNC>
	void RenderToTarget(RENDER_FUNC funcDraw, vr::Hmd_Eye eEye)
	{
		SEyeData* pFBD = nullptr;
		if (eEye == vr::Eye_Right)
			pFBD = &m_aEyeData[1];
		else
			pFBD = &m_aEyeData[0];

		glEnable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_FRAMEBUFFER, pFBD->m_nRenderFramebufferId);
		glViewport(0, 0, m_uWidth, m_uHeight);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(&(pFBD->m_matProjection[0][0]));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(&(pFBD->m_matEyePos * m_matHMDPose)[0][0]);

		funcDraw(eEye);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, pFBD->m_nRenderFramebufferId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pFBD->m_nResolveFramebufferId);
		
		glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
};
