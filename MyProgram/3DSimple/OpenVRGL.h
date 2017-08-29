#pragma once

#pragma region Header Files
// STD Header
#include <array>
#include <map>
#include <vector>
#include <set>

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
	// Class for display content
	class CDisplay
	{
	protected:
		// Store the data for left and right eye
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
		CDisplay(vr::IVRSystem* pVRSystem)
		{
			m_pVRSystem = pVRSystem;
			pVRSystem->GetRecommendedRenderTargetSize(&m_uWidth, &m_uHeight);

			m_aEyeData[0].m_eEye = vr::Eye_Left;
			m_aEyeData[1].m_eEye = vr::Eye_Right;
		}

		~CDisplay()
		{
			Release();
		}

		// Initialize data for 2 eyes.
		// Must call after OpenGL initialized.
		void Initial( float fNear, float fFar)
		{
			InitialEyeData(m_aEyeData[0], fNear, fFar);
			InitialEyeData(m_aEyeData[1], fNear, fFar);
		}

		// Release related resource
		void Release();

		// Submit data to show in VR
		void Submit();

		// Render one-eye scene to texture
		// The draw call: void DrawOneEye(vr::Hmd_Eye eEye, glm::mat4 matModelView, glm::mat4 matProjection)
		template<typename RENDER_FUNC>
		void RenderToTarget(RENDER_FUNC funcDraw, vr::Hmd_Eye eEye)
		{
			SEyeData* pFBD = GetEyeData(eEye);

			glEnable(GL_MULTISAMPLE);

			glBindFramebuffer(GL_FRAMEBUFFER, pFBD->m_nRenderFramebufferId);
			glViewport(0, 0, m_uWidth, m_uHeight);

			funcDraw(eEye, m_matHMDPose * pFBD->m_matEyePos, pFBD->m_matProjection);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDisable(GL_MULTISAMPLE);

			glBindFramebuffer(GL_READ_FRAMEBUFFER, pFBD->m_nRenderFramebufferId);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pFBD->m_nResolveFramebufferId);

			glBlitFramebuffer(0, 0, m_uWidth, m_uHeight, 0, 0, m_uWidth, m_uHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		// Draw the frame buffer of one eye to another frame buffer
		void DrawOnBuffer(vr::Hmd_Eye eEye, GLuint uBufferId = 0);

		// Set HMD position
		void SetHMDPose(glm::mat4 matPose)
		{
			m_matHMDPose = matPose;
		}

	protected:
		void InitialEyeData(SEyeData &mEyeData, float fNear, float fFar);
		SEyeData* GetEyeData(vr::Hmd_Eye eEye)
		{
			if (eEye == vr::Eye_Right)
				return &m_aEyeData[1];
			else
				return &m_aEyeData[0];
		}

	protected:
		vr::IVRSystem*	m_pVRSystem;
		uint32_t		m_uWidth;			// Width of frame buffer
		uint32_t		m_uHeight;			// Height of frame buffer

		glm::mat4		m_matHMDPose;		// transform matrix of HMD
		std::array<SEyeData, 2> m_aEyeData;	// data for left and right eye (L/R)
	};

	// Controller of HTC Vive
	class CController
	{
	public:
		CController(vr::IVRSystem*	pVRSystem, vr::TrackedDeviceIndex_t	uIdx);

		// Process controller event
		bool ProcessEvent(vr::ETrackingUniverseOrigin eOrigin);

		vr::TrackedDeviceIndex_t id() const
		{
			return m_uIdx;
		}

	protected:
		vr::TrackedDeviceIndex_t	m_uIdx;
		vr::IVRSystem*				m_pVRSystem;

	public:
		vr::ETrackedControllerRole	m_eRole;
		vr::TrackedDevicePose_t		m_Pose;
		vr::VRControllerState_t		m_eState;
	};

	// class for device model
	class CDeviceModel
	{
	public:
		// The Device render model
		class CGLRenderModel
		{
		public:
			CGLRenderModel(const std::string& sModelName);
			~CGLRenderModel();

			// Initialize model to render
			bool Initial(const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture);

			// release resource
			void Release();

			// draw call
			void Draw()
			{
				glBindVertexArray(m_glVertArray);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_glTexture);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
				glDrawElements(GL_TRIANGLES, m_unVertexCount, GL_UNSIGNED_SHORT, 0);

				glBindVertexArray(0);
			}

		protected:
			GLuint		m_glVertBuffer;
			GLuint		m_glIndexBuffer;
			GLuint		m_glVertArray;
			GLuint		m_glTexture;
			GLsizei		m_unVertexCount;

			std::string	m_sModelName;
		};

	public:
		CDeviceModel(vr::IVRSystem* pVRSystem);
		~CDeviceModel();

		// Initialize
		void Initial();

		// Release resource
		void Release();

		// Draw all device models
		void Draw(vr::Hmd_Eye eEye, const glm::mat4& matModelView, const glm::mat4& matProjection, const std::array<glm::mat4, vr::k_unMaxTrackedDeviceCount>& aMatrix);

		// setup a device model
		void SetupRenderModelForTrackedDevice(vr::TrackedDeviceIndex_t uDeviceIndex);

		CGLRenderModel* GetRenderModel(const std::string& sModelName);

	protected:
		vr::IVRSystem*	m_pVRSystem;
		std::map<std::string, CGLRenderModel*>						m_mapRenderModel;
		std::array<CGLRenderModel*, vr::k_unMaxTrackedDeviceCount>	m_aDeviceModel;
		std::array<bool, vr::k_unMaxTrackedDeviceCount>				m_aShowDevice;

		GLuint	m_unRenderModelProgramID;
		GLint	m_nRenderModelMatrixLocation;
	};

public:
	COpenVRGL()
	{
		m_pVRSystem = nullptr;
		m_pDisplayModule = nullptr;
		m_pDeviceModel = nullptr;
		m_setShowDevice = {vr::TrackedDeviceClass_Controller};
	}

	~COpenVRGL()
	{
		Release();
	}

	// Initialize.
	bool Initial(float fNear, float fFar);

	// Release
	void Release();

	// Render frame
	template<typename RENDER_FUNC>
	void Render(RENDER_FUNC funcDraw)
	{
		COpenVRGL* pThis = this;
		auto funcExtDraw = [funcDraw, pThis](vr::Hmd_Eye eEye, glm::mat4 matModelView, glm::mat4 matProjection) {
			funcDraw(eEye, matModelView, matProjection);
			pThis->m_pDeviceModel->Draw(eEye, matModelView, matProjection, pThis->m_aTrackedDeviceMatrix);
		};

		// draw left eye and right eye scene
		m_pDisplayModule->RenderToTarget(funcExtDraw, vr::Eye_Left);
		m_pDisplayModule->RenderToTarget(funcExtDraw, vr::Eye_Right);

		// 2nd pass render, compute distortion
		m_pDisplayModule->Submit();

		// Update HMD data
		UpdateHeadPose();
	}

	// Draw the frame buffer of one eye to another frame buffer
	void DrawOnBuffer(vr::Hmd_Eye eEye, GLuint uBufferId = 0)
	{
		m_pDisplayModule->DrawOnBuffer(eEye, uBufferId);
	}

	// Process Event
	void ProcessEvent();

	CController* GetController(vr::ETrackedControllerRole eRole);

	glm::mat4 GetHMDPose() const
	{
		return m_aTrackedDeviceMatrix[vr::k_unTrackedDeviceIndex_Hmd];
	}

public:
	static glm::vec3 GetCameraPos(const glm::mat4& mMatrix)
	{
		return{ mMatrix[3][0],mMatrix[3][1], mMatrix[3][2] };
	}

	static glm::vec3 GetCameraUpper(const glm::mat4& mMatrix)
	{
		return{ mMatrix[1][0], mMatrix[1][1], mMatrix[1][2] };
	}

	static glm::vec3 GetCameraDir(const glm::mat4& mMatrix)
	{
		return{ -mMatrix[2][0], -mMatrix[2][1], -mMatrix[2][2] };
	}

	static glm::vec3 GetCameraSide(const glm::mat4& mMatrix)
	{
		return{ mMatrix[0][0], mMatrix[0][1], mMatrix[0][2] };
	}

protected:
	void UpdateHeadPose();
	void InitializeDevice();
	void AddNewDevice(vr::TrackedDeviceIndex_t uIdx);
	void RemoveDevice(vr::TrackedDeviceIndex_t uIdx);

public:
	std::set<vr::ETrackedDeviceClass> m_setShowDevice;

protected:
	vr::IVRSystem*	m_pVRSystem;	// root object of OpenVR HMD
	CDisplay*		m_pDisplayModule;
	CDeviceModel*	m_pDeviceModel;
	vr::TrackedDevicePose_t m_aTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];	// Matrix array to save transform matrix for all device
	std::array<glm::mat4, vr::k_unMaxTrackedDeviceCount>	m_aTrackedDeviceMatrix;
	std::map<vr::TrackedDeviceIndex_t,CController>	m_vController;
};
