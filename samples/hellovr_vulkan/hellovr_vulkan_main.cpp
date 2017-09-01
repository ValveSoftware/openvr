//========= Copyright Valve Corporation ============//

#if defined( _WIN32 )
	#define VK_USE_PLATFORM_WIN32_KHR
#else
	#define SDL_VIDEO_DRIVER_X11
	#define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <vulkan/vulkan.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <inttypes.h>
#include <openvr.h>
#include <deque>

#include "shared/lodepng.h"
#include "shared/Matrices.h"
#include "shared/pathtools.h"

#if defined(POSIX)
#include "unistd.h"
#endif

#ifndef _countof
#define _countof(x) (sizeof(x)/sizeof((x)[0]))
#endif

void ThreadSleep( unsigned long nMilliseconds )
{
#if defined(_WIN32)
	::Sleep( nMilliseconds );
#elif defined(POSIX)
	usleep( nMilliseconds * 1000 );
#endif
}

// Pipeline state objects
enum PipelineStateObjectEnum_t
{
	PSO_SCENE = 0,
	PSO_AXES,
	PSO_RENDERMODEL,
	PSO_COMPANION,
	PSO_COUNT
};

// Indices of descriptor sets for rendering
enum DescriptorSetIndex_t
{
	DESCRIPTOR_SET_LEFT_EYE_SCENE = 0,
	DESCRIPTOR_SET_RIGHT_EYE_SCENE,
	DESCRIPTOR_SET_COMPANION_LEFT_TEXTURE,
	DESCRIPTOR_SET_COMPANION_RIGHT_TEXTURE,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL0,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL1,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL2,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL3,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL4,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL5,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL6,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL7,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL8,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL9,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL10,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL11,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL12,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL13,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL14,
	DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL15,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL0,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL1,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL2,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL3,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL4,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL5,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL6,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL7,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL8,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL9,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL10,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL11,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL12,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL13,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL14,
	DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL15,
	NUM_DESCRIPTOR_SETS
};

class VulkanRenderModel
{
public:
	VulkanRenderModel( const std::string & sRenderModelName );
	~VulkanRenderModel();

	bool BInit( VkDevice pDevice, const VkPhysicalDeviceMemoryProperties &memoryProperties, VkCommandBuffer pCommandBuffer, vr::TrackedDeviceIndex_t unTrackedDeviceIndex, VkDescriptorSet pDescriptorSets[ 2 ], const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture );
	void Cleanup();
	void Draw( vr::EVREye nEye, VkCommandBuffer pCommandBuffer, VkPipelineLayout pPipelineLayout, const Matrix4 &matMVP );
	const std::string & GetName() const { return m_sModelName; }

private:
	VkDevice m_pDevice;
	VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;
	VkBuffer m_pVertexBuffer;
	VkDeviceMemory m_pVertexBufferMemory;
	VkBuffer m_pIndexBuffer;
	VkDeviceMemory m_pIndexBufferMemory;
	VkImage m_pImage;
	VkDeviceMemory m_pImageMemory;
	VkImageView m_pImageView;
	VkBuffer m_pImageStagingBuffer;
	VkDeviceMemory m_pImageStagingBufferMemory;
	VkBuffer m_pConstantBuffer[ 2 ];
	VkDeviceMemory m_pConstantBufferMemory[ 2 ];
	void *m_pConstantBufferData[ 2 ];
	VkDescriptorSet m_pDescriptorSets[ 2 ];
	VkSampler m_pSampler;

	size_t m_unVertexCount;
	vr::TrackedDeviceIndex_t m_unTrackedDeviceIndex;
	std::string m_sModelName;
};

static bool g_bPrintf = true;

// Vulkan extension entrypoints
static PFN_vkCreateDebugReportCallbackEXT g_pVkCreateDebugReportCallbackEXT = nullptr;
static PFN_vkDestroyDebugReportCallbackEXT g_pVkDestroyDebugReportCallbackEXT = nullptr;

//-----------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------------
class CMainApplication
{
public:
	CMainApplication( int argc, char *argv[] );
	virtual ~CMainApplication();

	bool BInit();
	bool BInitVulkan();
	bool BInitVulkanInstance();
	bool BInitVulkanDevice();
	bool BInitVulkanSwapchain();
	bool BInitCompositor();
	bool GetVulkanInstanceExtensionsRequired( std::vector< std::string > &outInstanceExtensionList );
	bool GetVulkanDeviceExtensionsRequired( VkPhysicalDevice pPhysicalDevice, std::vector< std::string > &outDeviceExtensionList );

	void SetupRenderModels();

	void Shutdown();

	void RunMainLoop();
	bool HandleInput();
	void ProcessVREvent( const vr::VREvent_t & event );
	void RenderFrame();

	bool SetupTexturemaps();
	static void GenMipMapRGBA( const uint8_t *pSrc, uint8_t *ppDst, int nSrcWidth, int nSrcHeight, int *pDstWidthOut, int *pDstHeightOut );

	void SetupScene();
	void AddCubeToScene( Matrix4 mat, std::vector<float> &vertdata );
	void AddCubeVertex( float fl0, float fl1, float fl2, float fl3, float fl4, std::vector<float> &vertdata );

	void UpdateControllerAxes();

	bool SetupStereoRenderTargets();
	void SetupCompanionWindow();
	void SetupCameras();

	void RenderStereoTargets();
	void RenderCompanionWindow();
	void RenderScene( vr::Hmd_Eye nEye );

	Matrix4 GetHMDMatrixProjectionEye( vr::Hmd_Eye nEye );
	Matrix4 GetHMDMatrixPoseEye( vr::Hmd_Eye nEye );
	Matrix4 GetCurrentViewProjectionMatrix( vr::Hmd_Eye nEye );
	void UpdateHMDMatrixPose();

	Matrix4 ConvertSteamVRMatrixToMatrix4( const vr::HmdMatrix34_t &matPose );

	bool CreateAllShaders();
	void CreateAllDescriptorSets();

	void SetupRenderModelForTrackedDevice( vr::TrackedDeviceIndex_t unTrackedDeviceIndex );
	VulkanRenderModel *FindOrLoadRenderModel( vr::TrackedDeviceIndex_t unTrackedDeviceIndex, const char *pchRenderModelName );

private: 
	bool m_bDebugVulkan;
	bool m_bVerbose;
	bool m_bPerf;
	bool m_bVblank;
	int m_nMSAASampleCount;
	// Optional scaling factor to render with supersampling (defaults off, use -scale)
	float m_flSuperSampleScale;
	
	vr::IVRSystem *m_pHMD;
	vr::IVRRenderModels *m_pRenderModels;
	std::string m_strDriver;
	std::string m_strDisplay;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[ vr::k_unMaxTrackedDeviceCount ];
	Matrix4 m_rmat4DevicePose[ vr::k_unMaxTrackedDeviceCount ];
	bool m_rbShowTrackedDevice[ vr::k_unMaxTrackedDeviceCount ];

private: // SDL bookkeeping
	SDL_Window *m_pCompanionWindow;
	uint32_t m_nCompanionWindowWidth;
	uint32_t m_nCompanionWindowHeight;

private:
	int m_iTrackedControllerCount;
	int m_iTrackedControllerCount_Last;
	int m_iValidPoseCount;
	int m_iValidPoseCount_Last;
	bool m_bShowCubes;

	std::string m_strPoseClasses;                            // what classes we saw poses for this frame
	char m_rDevClassChar[ vr::k_unMaxTrackedDeviceCount ];   // for each device, a character representing its class

	int m_iSceneVolumeWidth;
	int m_iSceneVolumeHeight;
	int m_iSceneVolumeDepth;
	float m_fScaleSpacing;
	float m_fScale;
	
	int m_iSceneVolumeInit;                                  // if you want something other than the default 20x20x20
	
	float m_fNearClip;
	float m_fFarClip;

	unsigned int m_uiVertcount;
	unsigned int m_uiCompanionWindowIndexSize;

	VkInstance m_pInstance;
	VkDevice m_pDevice;
	VkPhysicalDevice m_pPhysicalDevice;
	VkQueue m_pQueue;
	VkSurfaceKHR m_pSurface;
	VkSwapchainKHR m_pSwapchain;
	VkPhysicalDeviceProperties m_physicalDeviceProperties;
	VkPhysicalDeviceMemoryProperties m_physicalDeviceMemoryProperties;
	VkPhysicalDeviceFeatures m_physicalDeviceFeatures;
	uint32_t m_nQueueFamilyIndex;
	VkDebugReportCallbackEXT m_pDebugReportCallback;
	uint32_t m_nSwapQueueImageCount;
	uint32_t m_nFrameIndex;
	uint32_t m_nCurrentSwapchainImage;
	std::vector< VkImage > m_swapchainImages;
	std::vector< VkImageView > m_pSwapchainImageViews;
	std::vector< VkFramebuffer > m_pSwapchainFramebuffers;
	std::vector< VkSemaphore > m_pSwapchainSemaphores;
	VkRenderPass m_pSwapchainRenderPass;
	

	VkCommandPool m_pCommandPool;
	VkDescriptorPool m_pDescriptorPool;
	VkDescriptorSet m_pDescriptorSets[ NUM_DESCRIPTOR_SETS ];

	struct VulkanCommandBuffer_t
	{
		VkCommandBuffer m_pCommandBuffer;
		VkFence m_pFence;
	};
	std::deque< VulkanCommandBuffer_t > m_commandBuffers;
	VulkanCommandBuffer_t m_currentCommandBuffer;
	
	VulkanCommandBuffer_t GetCommandBuffer();

	// Scene resources
	VkBuffer m_pSceneVertexBuffer;
	VkDeviceMemory m_pSceneVertexBufferMemory;
	VkBufferView m_pSceneVertexBufferView;
	VkBuffer m_pSceneConstantBuffer[ 2 ];
	VkDeviceMemory m_pSceneConstantBufferMemory[ 2 ];
	void *m_pSceneConstantBufferData[ 2 ];
	VkImage m_pSceneImage;
	VkDeviceMemory m_pSceneImageMemory;
	VkImageView m_pSceneImageView;
	VkBuffer m_pSceneStagingBuffer;
	VkDeviceMemory m_pSceneStagingBufferMemory;
	VkSampler m_pSceneSampler;

	// Storage for VS and PS for each PSO
	VkShaderModule m_pShaderModules[ PSO_COUNT * 2 ];
	VkPipeline m_pPipelines[ PSO_COUNT ];
	VkDescriptorSetLayout m_pDescriptorSetLayout;
	VkPipelineLayout m_pPipelineLayout;
	VkPipelineCache m_pPipelineCache;

	// Companion window resources
	VkBuffer m_pCompanionWindowVertexBuffer;
	VkDeviceMemory m_pCompanionWindowVertexBufferMemory;
	VkBuffer m_pCompanionWindowIndexBuffer;
	VkDeviceMemory m_pCompanionWindowIndexBufferMemory;

	// Controller axes resources
	VkBuffer m_pControllerAxesVertexBuffer;
	VkDeviceMemory m_pControllerAxesVertexBufferMemory;

	unsigned int m_uiControllerVertcount;

	Matrix4 m_mat4HMDPose;
	Matrix4 m_mat4eyePosLeft;
	Matrix4 m_mat4eyePosRight;

	Matrix4 m_mat4ProjectionCenter;
	Matrix4 m_mat4ProjectionLeft;
	Matrix4 m_mat4ProjectionRight;

	struct VertexDataScene
	{
		Vector3 position;
		Vector2 texCoord;
	};

	struct VertexDataWindow
	{
		Vector2 position;
		Vector2 texCoord;

		VertexDataWindow( const Vector2 & pos, const Vector2 tex ) :  position(pos), texCoord(tex) {	}
	};

	struct FramebufferDesc
	{
		VkImage m_pImage;
		VkImageLayout m_nImageLayout;
		VkDeviceMemory m_pDeviceMemory;
		VkImageView m_pImageView;
		VkImage m_pDepthStencilImage;
		VkImageLayout m_nDepthStencilImageLayout;
		VkDeviceMemory m_pDepthStencilDeviceMemory;
		VkImageView m_pDepthStencilImageView;
		VkRenderPass m_pRenderPass;
		VkFramebuffer m_pFramebuffer;
	};
	FramebufferDesc m_leftEyeDesc;
	FramebufferDesc m_rightEyeDesc;

	bool CreateFrameBuffer( int nWidth, int nHeight, FramebufferDesc &framebufferDesc );
	
	uint32_t m_nRenderWidth;
	uint32_t m_nRenderHeight;

	std::vector< VulkanRenderModel * > m_vecRenderModels;
	VulkanRenderModel *m_rTrackedDeviceToRenderModel[ vr::k_unMaxTrackedDeviceCount ];
};

//-----------------------------------------------------------------------------
// Purpose: Outputs a set of optional arguments to debugging output, using
//          the printf format setting specified in fmt*.
//-----------------------------------------------------------------------------
void dprintf( const char *fmt, ... )
{
	va_list args;
	char buffer[ 2048 ];

	va_start( args, fmt );
	vsprintf_s( buffer, fmt, args );
	va_end( args );

	if ( g_bPrintf )
		printf( "%s", buffer );

	OutputDebugStringA( buffer );
}

//-----------------------------------------------------------------------------
// Purpose: VK_EXT_debug_report callback
//-----------------------------------------------------------------------------
static VkBool32 VKAPI_PTR VKDebugMessageCallback( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object,
    size_t location, int32_t messageCode, const char* pLayerPrefix, const char *pMessage, void *pUserData )
{
	char buf[4096] = { 0 };
	switch ( flags )
	{
	case VK_DEBUG_REPORT_ERROR_BIT_EXT:
		sprintf( buf, "VK ERROR %s %" PRIu64 ":%d: %s\n", pLayerPrefix, uint64_t( location ), messageCode, pMessage );
		break;
	case VK_DEBUG_REPORT_WARNING_BIT_EXT:
		sprintf( buf, "VK WARNING %s %" PRIu64 ":%d: %s\n", pLayerPrefix, uint64_t( location ), messageCode, pMessage );
		break;
	case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
		sprintf( buf, "VK PERF %s %" PRIu64 ":%d: %s\n", pLayerPrefix, uint64_t( location ), messageCode, pMessage );
		break;
	case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
		sprintf( buf, "VK INFO %s %" PRIu64 ":%d: %s\n", pLayerPrefix, uint64_t( location ), messageCode, pMessage );
		break;
	case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
		sprintf( buf, "VK DEBUG %s %" PRIu64 ":%d: %s\n", pLayerPrefix, uint64_t( location ), messageCode, pMessage );
		break;
	default:
		break;
	}

	dprintf( "%s\n", buf );

	return VK_FALSE;
}

//-----------------------------------------------------------------------------
// Purpose: Determine the memory type index from the memory requirements
// and type bits
//-----------------------------------------------------------------------------
static bool MemoryTypeFromProperties( const VkPhysicalDeviceMemoryProperties &memoryProperties, uint32_t nMemoryTypeBits, VkMemoryPropertyFlags nMemoryProperties, uint32_t *pTypeIndexOut )
{
	for ( uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++ ) 
	{
		if ( ( nMemoryTypeBits & 1 ) == 1) 
		{
			// Type is available, does it match user properties?
			if ( ( memoryProperties.memoryTypes[i].propertyFlags & nMemoryProperties ) == nMemoryProperties ) 
			{
				*pTypeIndexOut = i;
				return true;
			}
		}
		nMemoryTypeBits >>= 1;
	}

	// No memory types matched, return failure
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Helper function to create Vulkan static VB/IBs
//-----------------------------------------------------------------------------
static bool CreateVulkanBuffer( VkDevice pDevice, const VkPhysicalDeviceMemoryProperties &memoryProperties, const void *pBufferData, VkDeviceSize nSize, VkBufferUsageFlags nUsage, VkBuffer *ppBufferOut, VkDeviceMemory *ppDeviceMemoryOut )
{
	// Create the vertex buffer and fill with data
	VkBufferCreateInfo bufferCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferCreateInfo.size = nSize;
	bufferCreateInfo.usage = nUsage;
	VkResult nResult = vkCreateBuffer( pDevice, &bufferCreateInfo, nullptr, ppBufferOut );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "%s - vkCreateBuffer failed with error %d\n", __FUNCTION__, nResult );
		return false;
	}

	VkMemoryRequirements memoryRequirements = {};
	vkGetBufferMemoryRequirements( pDevice, *ppBufferOut, &memoryRequirements );

	VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
	if ( !MemoryTypeFromProperties( memoryProperties, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &allocInfo.memoryTypeIndex ) )
	{
		dprintf( "%s - failed to find matching memoryTypeIndex for buffer\n", __FUNCTION__ );
		return false;
	}
	allocInfo.allocationSize = memoryRequirements.size;

	nResult = vkAllocateMemory( pDevice, &allocInfo, nullptr, ppDeviceMemoryOut );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "%s - vkCreateBuffer failed with error %d\n", __FUNCTION__, nResult );
		return false;
	}

	nResult = vkBindBufferMemory( pDevice, *ppBufferOut, *ppDeviceMemoryOut, 0 );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "%s vkBindBufferMemory failed with error %d\n", __FUNCTION__, nResult );
		return false;
	}

	if ( pBufferData != nullptr )
	{
		void *pData;
		nResult = vkMapMemory( pDevice, *ppDeviceMemoryOut, 0, VK_WHOLE_SIZE, 0, &pData );
		if ( nResult != VK_SUCCESS )
		{
			dprintf( "%s - vkMapMemory returned error %d\n", __FUNCTION__, nResult );
			return false;
		}
		memcpy( pData, pBufferData, nSize );
		vkUnmapMemory( pDevice, *ppDeviceMemoryOut );

		VkMappedMemoryRange memoryRange = { VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE };
		memoryRange.memory = *ppDeviceMemoryOut;
		memoryRange.size = VK_WHOLE_SIZE;
		vkFlushMappedMemoryRanges( pDevice, 1, &memoryRange );

	}
	return true;
}


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMainApplication::CMainApplication( int argc, char *argv[] )
	: m_pCompanionWindow(NULL)
	, m_nCompanionWindowWidth( 640 )
	, m_nCompanionWindowHeight( 320 )
	, m_pHMD( NULL )
	, m_pRenderModels( NULL )
	, m_bDebugVulkan( false )
	, m_bVerbose( false )
	, m_bPerf( false )
	, m_bVblank( false )
	, m_nMSAASampleCount( 4 )
	, m_flSuperSampleScale( 1.0f )
	, m_iTrackedControllerCount( 0 )
	, m_iTrackedControllerCount_Last( -1 )
	, m_iValidPoseCount( 0 )
	, m_iValidPoseCount_Last( -1 )
	, m_iSceneVolumeInit( 20 )
	, m_strPoseClasses("")
	, m_bShowCubes( true )
	, m_pInstance( VK_NULL_HANDLE )
	, m_pDevice( VK_NULL_HANDLE )
	, m_pPhysicalDevice( VK_NULL_HANDLE )
	, m_pQueue( VK_NULL_HANDLE )
	, m_pSurface( VK_NULL_HANDLE )
	, m_pSwapchain( VK_NULL_HANDLE )
	, m_pDebugReportCallback( VK_NULL_HANDLE )
	, m_pCommandPool( VK_NULL_HANDLE )
	, m_pDescriptorPool( VK_NULL_HANDLE )
	, m_nSwapQueueImageCount( 0 )
	, m_nFrameIndex( 0 )
	, m_nCurrentSwapchainImage( 0 )
	, m_pSceneVertexBuffer( VK_NULL_HANDLE )
	, m_pSceneVertexBufferMemory( VK_NULL_HANDLE )
	, m_pSceneVertexBufferView( VK_NULL_HANDLE )
	, m_pSceneImage( VK_NULL_HANDLE )
	, m_pSceneImageMemory( VK_NULL_HANDLE )
	, m_pSceneImageView( VK_NULL_HANDLE )
	, m_pSceneStagingBuffer( VK_NULL_HANDLE )
	, m_pSceneStagingBufferMemory( VK_NULL_HANDLE )
	, m_pSceneSampler( VK_NULL_HANDLE )
	, m_pDescriptorSetLayout( VK_NULL_HANDLE )
	, m_pPipelineLayout( VK_NULL_HANDLE )
	, m_pPipelineCache( VK_NULL_HANDLE )
	, m_pCompanionWindowVertexBuffer( VK_NULL_HANDLE )
	, m_pCompanionWindowVertexBufferMemory( VK_NULL_HANDLE )
	, m_pCompanionWindowIndexBuffer( VK_NULL_HANDLE )
	, m_pCompanionWindowIndexBufferMemory( VK_NULL_HANDLE )
	, m_pControllerAxesVertexBuffer( VK_NULL_HANDLE )
	, m_pControllerAxesVertexBufferMemory( VK_NULL_HANDLE )
{
	memset( &m_leftEyeDesc, 0, sizeof( m_leftEyeDesc ) );
	memset( &m_rightEyeDesc, 0, sizeof( m_rightEyeDesc ) );
	memset( &m_pShaderModules[ 0 ], 0, sizeof( m_pShaderModules ) );
	memset( &m_pPipelines[ 0 ], 0, sizeof( m_pPipelines ) );
	memset( m_pSceneConstantBufferData, 0, sizeof( m_pSceneConstantBufferData ) );
	memset( m_pDescriptorSets, 0, sizeof( m_pDescriptorSets ) );

	for( int i = 1; i < argc; i++ )
	{
		if( !stricmp( argv[i], "-vulkandebug" ) )
		{
			m_bDebugVulkan = true;
		}
		else if( !stricmp( argv[i], "-verbose" ) )
		{
			m_bVerbose = true;
		}
		else if( !stricmp( argv[i], "-novblank" ) )
		{
			m_bVblank = false;
		}
		else if ( !stricmp( argv[i], "-msaa" ) && ( argc > i + 1 ) && ( *argv[ i + 1 ] != '-' ) )
		{
			m_nMSAASampleCount = atoi( argv[ i + 1 ] );
			i++;
		}
		else if ( !stricmp( argv[i], "-supersample" ) && ( argc > i + 1 ) && ( *argv[ i + 1 ] != '-' ) )
		{
			m_flSuperSampleScale = ( float )atof( argv[ i + 1 ] );
			i++;
		}
		else if( !stricmp( argv[i], "-noprintf" ) )
		{
			g_bPrintf = false;
		}
		else if ( !stricmp( argv[i], "-cubevolume" ) && ( argc > i + 1 ) && ( *argv[ i + 1 ] != '-' ) )
		{
			m_iSceneVolumeInit = atoi( argv[ i + 1 ] );
			i++;
		}
	}
	// other initialization tasks are done in BInit
	memset( m_rDevClassChar, 0, sizeof( m_rDevClassChar ) );
};

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CMainApplication::~CMainApplication()
{
	// work is done in Shutdown
	dprintf( "Shutdown" );
}

//-----------------------------------------------------------------------------
// Purpose: Helper to get a string from a tracked device property and turn it
//			into a std::string
//-----------------------------------------------------------------------------
std::string GetTrackedDeviceString( vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL )
{
	uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty( unDevice, prop, NULL, 0, peError );
	if( unRequiredBufferLen == 0 )
		return "";

	char *pchBuffer = new char[ unRequiredBufferLen ];
	unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty( unDevice, prop, pchBuffer, unRequiredBufferLen, peError );
	std::string sResult = pchBuffer;
	delete [] pchBuffer;
	return sResult;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CMainApplication::BInit()
{
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
	{
		dprintf("%s - SDL could not initialize! SDL Error: %s\n", __FUNCTION__, SDL_GetError());
		return false;
	}

	// Loading the SteamVR Runtime
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init( &eError, vr::VRApplication_Scene );

	if ( eError != vr::VRInitError_None )
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s( buf, sizeof( buf ), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription( eError ) );
		SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "VR_Init Failed", buf, NULL );
		return false;
	}

	m_pRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface( vr::IVRRenderModels_Version, &eError );
	if( !m_pRenderModels )
	{
		m_pHMD = NULL;
		vr::VR_Shutdown();

		char buf[1024];
		sprintf_s( buf, sizeof( buf ), "Unable to get render model interface: %s", vr::VR_GetVRInitErrorAsEnglishDescription( eError ) );
		SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "VR_Init Failed", buf, NULL );
		return false;
	}

	int nWindowPosX = 700;
	int nWindowPosY = 100;
	Uint32 unWindowFlags = SDL_WINDOW_SHOWN;

	m_pCompanionWindow = SDL_CreateWindow( "hellovr [Vulkan]", nWindowPosX, nWindowPosY, m_nCompanionWindowWidth, m_nCompanionWindowHeight, unWindowFlags );
	if (m_pCompanionWindow == NULL)
	{
		dprintf( "%s - Window could not be created! SDL Error: %s\n", __FUNCTION__, SDL_GetError() );
		return false;
	}

	m_strDriver = "No Driver";
	m_strDisplay = "No Display";

	m_strDriver = GetTrackedDeviceString( m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String );
	m_strDisplay = GetTrackedDeviceString( m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String );

	std::string strWindowTitle = "hellovr [Vulkan] - " + m_strDriver + " " + m_strDisplay;
	SDL_SetWindowTitle( m_pCompanionWindow, strWindowTitle.c_str() );
	
	// cube array
	m_iSceneVolumeWidth = m_iSceneVolumeInit;
	m_iSceneVolumeHeight = m_iSceneVolumeInit;
	m_iSceneVolumeDepth = m_iSceneVolumeInit;

	m_fScale = 0.3f;
	m_fScaleSpacing = 4.0f;
 
	m_fNearClip = 0.1f;
	m_fFarClip = 30.0f;

	m_uiVertcount = 0;
	m_uiCompanionWindowIndexSize = 0;

	if ( !BInitVulkan() )
	{
		dprintf( "%s - Unable to initialize Vulkan!\n", __FUNCTION__ );
		return false;
	}

	if ( !BInitCompositor() )
	{
		dprintf( "%s - Failed to initialize VR Compositor!\n", __FUNCTION__ );
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Ask OpenVR for the list of instance extensions required
//--------------------------------------------------------------------------------------
bool CMainApplication::GetVulkanInstanceExtensionsRequired( std::vector< std::string > &outInstanceExtensionList )
{
	if ( !vr::VRCompositor() )
	{
		return false;
	}

	outInstanceExtensionList.clear();
	uint32_t nBufferSize = vr::VRCompositor()->GetVulkanInstanceExtensionsRequired( nullptr, 0 );
	if ( nBufferSize > 0 )
	{
		// Allocate memory for the space separated list and query for it
		char *pExtensionStr = new char[ nBufferSize ];
		pExtensionStr[0] = 0;
		vr::VRCompositor()->GetVulkanInstanceExtensionsRequired( pExtensionStr, nBufferSize );

		// Break up the space separated list into entries on the CUtlStringList
		std::string curExtStr;
		uint32_t nIndex = 0;
		while ( pExtensionStr[ nIndex ] != 0 && ( nIndex < nBufferSize ) )
		{
			if ( pExtensionStr[ nIndex ] == ' ' )
			{
				outInstanceExtensionList.push_back( curExtStr );
				curExtStr.clear();
			}
			else
			{
				curExtStr += pExtensionStr[ nIndex ];
			}
			nIndex++;
		}
		if ( curExtStr.size() > 0 )
		{
			outInstanceExtensionList.push_back( curExtStr );
		}

		delete [] pExtensionStr;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// Ask OpenVR for the list of device extensions required
//--------------------------------------------------------------------------------------
bool CMainApplication::GetVulkanDeviceExtensionsRequired( VkPhysicalDevice pPhysicalDevice, std::vector< std::string > &outDeviceExtensionList )
{
	if ( !vr::VRCompositor() )
	{
		return false;
	}

	outDeviceExtensionList.clear();
	uint32_t nBufferSize = vr::VRCompositor()->GetVulkanDeviceExtensionsRequired( ( VkPhysicalDevice_T * ) pPhysicalDevice, nullptr, 0 );
	if ( nBufferSize > 0 )
	{
		// Allocate memory for the space separated list and query for it
		char *pExtensionStr = new char[ nBufferSize ];
		pExtensionStr[0] = 0;
		vr::VRCompositor()->GetVulkanDeviceExtensionsRequired( ( VkPhysicalDevice_T * ) pPhysicalDevice, pExtensionStr, nBufferSize );

		// Break up the space separated list into entries on the CUtlStringList
		std::string curExtStr;
		uint32_t nIndex = 0;
		while ( pExtensionStr[ nIndex ] != 0 && ( nIndex < nBufferSize ) )
		{
			if ( pExtensionStr[ nIndex ] == ' ' )
			{
				outDeviceExtensionList.push_back( curExtStr );
				curExtStr.clear();
			}
			else
			{
				curExtStr += pExtensionStr[ nIndex ];
			}
			nIndex++;
		}
		if ( curExtStr.size() > 0 )
		{
			outDeviceExtensionList.push_back( curExtStr );
		}

		delete [] pExtensionStr;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Initialize Vulkan VkInstance
//-----------------------------------------------------------------------------
bool CMainApplication::BInitVulkanInstance()
{
	VkResult nResult;

	//----------------------//
	// VkInstance creation  //
	//----------------------//
	// Query OpenVR to determine which instance extensions need to be enabled before creating the instance
	std::vector< std::string > requiredInstanceExtensions;
	if ( !GetVulkanInstanceExtensionsRequired( requiredInstanceExtensions ) )
	{
		dprintf( "Could not determine OpenVR Vulkan instance extensions.\n" );
		return false;
	}

	// Additional required instance extensions
	requiredInstanceExtensions.push_back( VK_KHR_SURFACE_EXTENSION_NAME );
#if defined ( _WIN32 )
	requiredInstanceExtensions.push_back( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );
#else
	requiredInstanceExtensions.push_back( VK_KHR_XLIB_SURFACE_EXTENSION_NAME );
#endif

	uint32_t nEnabledLayerCount = 0;
	VkLayerProperties *pLayerProperties = nullptr;
	char **ppEnabledLayerNames = nullptr;

	// Enable validation layers
	if ( m_bDebugVulkan )
	{
		// OpenVR: no unique_objects when using validation with SteamVR
		char const *pInstanceValidationLayers[] = 
		{
			"VK_LAYER_GOOGLE_threading",
			"VK_LAYER_LUNARG_parameter_validation",
			"VK_LAYER_LUNARG_object_tracker",
			"VK_LAYER_LUNARG_image",
			"VK_LAYER_LUNARG_core_validation",
			"VK_LAYER_LUNARG_swapchain"
		};
	
		uint32_t nInstanceLayerCount = 0;
		VkResult nResult = vkEnumerateInstanceLayerProperties( &nInstanceLayerCount, nullptr );
		if ( nResult == VK_SUCCESS && nInstanceLayerCount > 0 )
		{
			pLayerProperties = new VkLayerProperties[ nInstanceLayerCount ];
			ppEnabledLayerNames = new char*[ nInstanceLayerCount ];
			nResult = vkEnumerateInstanceLayerProperties( &nInstanceLayerCount, pLayerProperties );
			if ( nResult != VK_SUCCESS )
			{
				dprintf( "Error vkEnumerateInstanceLayerProperties in %d\n", nResult );
				return false;
			}

			uint32_t nLayerIndex = 0;
			for ( nLayerIndex = 0; nLayerIndex < nInstanceLayerCount; nLayerIndex++ )
			{
				for ( uint32_t nLayer = 0; nLayer < _countof( pInstanceValidationLayers ); nLayer++ )
				{
					if ( strstr( pLayerProperties[ nLayerIndex ].layerName, pInstanceValidationLayers[ nLayer ] ) != NULL )
					{
						ppEnabledLayerNames[ nEnabledLayerCount++ ] = pLayerProperties[ nLayerIndex ].layerName;
					}
				}
			}
			requiredInstanceExtensions.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
		}
	}

	uint32_t nInstanceExtensionCount = 0;
	nResult = vkEnumerateInstanceExtensionProperties( NULL, &nInstanceExtensionCount, NULL );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkEnumerateInstanceExtensionProperties failed with error %d\n", nResult );
		return false;
	}

	// Allocate enough ExtensionProperties to support all extensions being enabled
	char** ppEnableInstanceExtensionNames =  new char*[ requiredInstanceExtensions.size() ];
	int32_t nEnableInstanceExtensionNamesCount = 0;
	VkExtensionProperties *pExtensionProperties = new VkExtensionProperties[ nInstanceExtensionCount ];
	if ( nInstanceExtensionCount > 0 )
	{
		nResult = vkEnumerateInstanceExtensionProperties( NULL, &nInstanceExtensionCount, pExtensionProperties );
		if ( nResult != VK_SUCCESS )
		{
			dprintf( "vkEnumerateInstanceExtensionProperties failed with error %d\n", nResult );
			return false;
		}

		for ( size_t nExt = 0; nExt < requiredInstanceExtensions.size(); nExt++ )
		{
			bool bFound = false;
			uint32_t nExtIndex = 0;
			for ( nExtIndex = 0; nExtIndex < nInstanceExtensionCount; nExtIndex++ )
			{
				if ( strcmp( requiredInstanceExtensions[ nExt ].c_str(), pExtensionProperties[ nExtIndex ].extensionName ) == 0 )
				{
					bFound = true;
					ppEnableInstanceExtensionNames[ nEnableInstanceExtensionNamesCount++ ] = pExtensionProperties[ nExtIndex ].extensionName;
					break;
				}
			}

			if ( !bFound )
			{
				dprintf( "Vulkan missing requested extension '%s'.\n", requiredInstanceExtensions[ nExt ] );
			}
		}

		if ( nEnableInstanceExtensionNamesCount != requiredInstanceExtensions.size() )
		{
			return false;
		}
	}

	VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	appInfo.pApplicationName = "hellovr_vulkan";
	appInfo.applicationVersion = 1;
	appInfo.pEngineName = nullptr;
	appInfo.engineVersion = 1;
	appInfo.apiVersion = VK_MAKE_VERSION( 1, 0, 0 );

	// Create the instance
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = NULL;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledExtensionCount = nEnableInstanceExtensionNamesCount;
	instanceCreateInfo.ppEnabledExtensionNames = ppEnableInstanceExtensionNames;
	instanceCreateInfo.enabledLayerCount = nEnabledLayerCount;
	instanceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames;

	nResult = vkCreateInstance( &instanceCreateInfo, nullptr, &m_pInstance );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateInstance failed with error %d\n", nResult );
		return false;
	}

	// Enable debug report extension
	if ( m_bDebugVulkan )
	{
		g_pVkCreateDebugReportCallbackEXT = ( PFN_vkCreateDebugReportCallbackEXT ) vkGetInstanceProcAddr( m_pInstance, "vkCreateDebugReportCallbackEXT" );
		g_pVkDestroyDebugReportCallbackEXT = ( PFN_vkDestroyDebugReportCallbackEXT ) vkGetInstanceProcAddr( m_pInstance, "vkDestroyDebugReportCallbackEXT" );

		VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo = {};
		debugReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		debugReportCreateInfo.pNext = NULL;
		debugReportCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT;
		debugReportCreateInfo.pfnCallback = VKDebugMessageCallback;
		debugReportCreateInfo.pUserData = NULL;
		g_pVkCreateDebugReportCallbackEXT( m_pInstance, &debugReportCreateInfo, NULL, &m_pDebugReportCallback );
	}

	delete [] ppEnableInstanceExtensionNames;
	delete [] ppEnabledLayerNames;
	delete [] pLayerProperties;
	delete [] pExtensionProperties;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Initialize Vulkan VkDevice
//-----------------------------------------------------------------------------
bool CMainApplication::BInitVulkanDevice()
{
	uint32_t nDeviceCount = 0;
	VkResult nResult = vkEnumeratePhysicalDevices( m_pInstance, &nDeviceCount, NULL );
	if ( nResult != VK_SUCCESS || nDeviceCount == 0 )
	{
		dprintf( "vkEnumeratePhysicalDevices failed, unable to init and enumerate GPUs with Vulkan.\n" );
		return false;
	}

	VkPhysicalDevice *pPhysicalDevices = new VkPhysicalDevice[ nDeviceCount ];
	nResult = vkEnumeratePhysicalDevices( m_pInstance, &nDeviceCount, pPhysicalDevices );
	if ( nResult != VK_SUCCESS || nDeviceCount == 0 )
	{
		dprintf( "vkEnumeratePhysicalDevices failed, unable to init and enumerate GPUs with Vulkan.\n" );
		return false;
	}

	// Query OpenVR for the physical device to use
	uint64_t pHMDPhysicalDevice = 0;
	m_pHMD->GetOutputDevice( &pHMDPhysicalDevice, vr::TextureType_Vulkan, ( VkInstance_T * ) m_pInstance );

	// Select the HMD physical device
	m_pPhysicalDevice = VK_NULL_HANDLE;
	for ( uint32_t nPhysicalDevice = 0; nPhysicalDevice < nDeviceCount; nPhysicalDevice++ )
	{
		if ( ( ( VkPhysicalDevice ) pHMDPhysicalDevice ) == pPhysicalDevices[ nPhysicalDevice ] )
		{
			m_pPhysicalDevice = ( VkPhysicalDevice ) pHMDPhysicalDevice;
			break;
		}
	}
	if ( m_pPhysicalDevice == VK_NULL_HANDLE )
	{
		// Fallback: Grab the first physical device
		dprintf( "Failed to find GetOutputDevice VkPhysicalDevice, falling back to choosing first device.\n" );
		m_pPhysicalDevice = pPhysicalDevices[ 0 ];
	}
	delete [] pPhysicalDevices;

	vkGetPhysicalDeviceProperties( m_pPhysicalDevice, &m_physicalDeviceProperties );
	vkGetPhysicalDeviceMemoryProperties( m_pPhysicalDevice, &m_physicalDeviceMemoryProperties );
	vkGetPhysicalDeviceFeatures( m_pPhysicalDevice, &m_physicalDeviceFeatures );

	//--------------------//
	// VkDevice creation  //
	//--------------------//
	// Query OpenVR for the required device extensions for this physical device
	std::vector< std::string > requiredDeviceExtensions;
	GetVulkanDeviceExtensionsRequired( m_pPhysicalDevice, requiredDeviceExtensions );
	// Add additional required extensions
	requiredDeviceExtensions.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

	// Find the first graphics queue
	uint32_t nQueueCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties( m_pPhysicalDevice, &nQueueCount, 0 );
	VkQueueFamilyProperties *pQueueFamilyProperties = new VkQueueFamilyProperties[ nQueueCount ];
	vkGetPhysicalDeviceQueueFamilyProperties( m_pPhysicalDevice, &nQueueCount, pQueueFamilyProperties );
	if ( nQueueCount == 0 )
	{
		dprintf( "Failed to get queue properties.\n" );
		return false;
	}
	uint32_t nGraphicsQueueIndex = 0;
	for ( nGraphicsQueueIndex = 0; nGraphicsQueueIndex < nQueueCount; nGraphicsQueueIndex++ )
	{
		if ( pQueueFamilyProperties[ nGraphicsQueueIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT )
		{
			break;
		}
	}
	if ( nGraphicsQueueIndex >= nQueueCount )
	{
		dprintf( "No graphics queue found\n" );
		return false;
	}
	m_nQueueFamilyIndex = nGraphicsQueueIndex;
	delete [] pQueueFamilyProperties;

	uint32_t nDeviceExtensionCount = 0;
	nResult = vkEnumerateDeviceExtensionProperties( m_pPhysicalDevice, NULL, &nDeviceExtensionCount, NULL );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkEnumerateDeviceExtensionProperties failed with error %d\n", nResult );
		return false;
	}

	// Allocate enough ExtensionProperties to support all extensions being enabled
	const char** ppDeviceExtensionNames = new const char* [ nDeviceExtensionCount ];
	uint32_t nEnabledDeviceExtensionCount = 0;

	// Enable required device extensions
	VkExtensionProperties *pDeviceExtProperties = new VkExtensionProperties[ nDeviceExtensionCount ];
	memset( pDeviceExtProperties, 0, sizeof( VkExtensionProperties ) * nDeviceExtensionCount );
	if ( nDeviceExtensionCount > 0 )
	{
		nResult = vkEnumerateDeviceExtensionProperties( m_pPhysicalDevice, NULL, &nDeviceExtensionCount, pDeviceExtProperties );
		if ( nResult != VK_SUCCESS )
		{
			dprintf( "vkEnumerateDeviceExtensionProperties failed with error %d\n", nResult );
			return false;
		}

		for ( size_t nRequiredDeviceExt = 0; nRequiredDeviceExt < requiredDeviceExtensions.size(); nRequiredDeviceExt++ )
		{
			bool bExtFound = false;
			for ( uint32_t nDeviceExt = 0; nDeviceExt < nDeviceExtensionCount; nDeviceExt++ )
			{
				if ( stricmp( requiredDeviceExtensions[ nRequiredDeviceExt ].c_str(), pDeviceExtProperties[ nDeviceExt ].extensionName ) == 0 )
				{
					bExtFound = true;
					break;
				}
			}

			if ( bExtFound )
			{
				ppDeviceExtensionNames[ nEnabledDeviceExtensionCount ] = requiredDeviceExtensions[ nRequiredDeviceExt ].c_str();
				nEnabledDeviceExtensionCount++;
			}
		}
	}

	// Create the device
	VkDeviceQueueCreateInfo deviceQueueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
	deviceQueueCreateInfo.queueFamilyIndex = m_nQueueFamilyIndex;
	deviceQueueCreateInfo.queueCount = 1;
	float fQueuePriority = 1.0f;
	deviceQueueCreateInfo.pQueuePriorities = &fQueuePriority;

	VkDeviceCreateInfo deviceCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	deviceCreateInfo.enabledExtensionCount = nEnabledDeviceExtensionCount;
	deviceCreateInfo.ppEnabledExtensionNames = ppDeviceExtensionNames;
	deviceCreateInfo.pEnabledFeatures = &m_physicalDeviceFeatures;

	nResult = vkCreateDevice( m_pPhysicalDevice, &deviceCreateInfo, nullptr, &m_pDevice );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateDevice failed with error %d\n", nResult );
		return false;
	}

	// Get the device queue
	vkGetDeviceQueue( m_pDevice, m_nQueueFamilyIndex, 0, &m_pQueue );
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Initialize Vulkan swapchain and associated images
//-----------------------------------------------------------------------------
bool CMainApplication::BInitVulkanSwapchain()
{
	//----------------------//
	// Swapchain creation   //
	//----------------------//
	SDL_SysWMinfo wmInfo;
	SDL_VERSION( &wmInfo.version ); 
	SDL_GetWindowWMInfo( m_pCompanionWindow, &wmInfo );
	VkResult nResult;
#ifdef VK_USE_PLATFORM_WIN32_KHR
	VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo = {};
	win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	win32SurfaceCreateInfo.pNext = NULL;
	win32SurfaceCreateInfo.flags = 0;
	win32SurfaceCreateInfo.hinstance = GetModuleHandle( NULL );
	win32SurfaceCreateInfo.hwnd = ( HWND ) wmInfo.info.win.window;
	nResult = vkCreateWin32SurfaceKHR( m_pInstance, &win32SurfaceCreateInfo, nullptr, &m_pSurface );
#else
	VkXlibSurfaceCreateInfoKHR xlibSurfaceCreateInfo = { VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR };
	xlibSurfaceCreateInfo.flags = 0;
	xlibSurfaceCreateInfo.dpy = wmInfo.info.x11.display;
	xlibSurfaceCreateInfo.window = wmInfo.info.x11.window;
	nResult = vkCreateXlibSurfaceKHR( m_pInstance, &xlibSurfaceCreateInfo, nullptr, &m_pSurface );
#endif
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "Failed to create VkSurfaceKHR error %d.\n", nResult );
		return false;
	}

	VkBool32 bSupportsPresent = VK_FALSE;
	nResult = vkGetPhysicalDeviceSurfaceSupportKHR( m_pPhysicalDevice, m_nQueueFamilyIndex, m_pSurface, &bSupportsPresent );
	if ( nResult != VK_SUCCESS || bSupportsPresent == VK_FALSE )
	{
		dprintf( "vkGetPhysicalDeviceSurfaceSupportKHR present not supported.\n" );
		return false;
	}

	// Query supported swapchain formats
	VkFormat nSwapChainFormat;
	uint32_t nFormatIndex = 0;
	uint32_t nNumSupportedSwapChainFormats = 0;
	VkColorSpaceKHR nColorSpace;
	if ( vkGetPhysicalDeviceSurfaceFormatsKHR( m_pPhysicalDevice, m_pSurface, &nNumSupportedSwapChainFormats, NULL ) != VK_SUCCESS )
	{
		dprintf( "Unable to query size of supported swapchain formats.\n" );
		return false;
	}
	VkSurfaceFormatKHR *pSupportedSurfaceFormats = new VkSurfaceFormatKHR[ nNumSupportedSwapChainFormats ];
	if ( vkGetPhysicalDeviceSurfaceFormatsKHR( m_pPhysicalDevice, m_pSurface, &nNumSupportedSwapChainFormats, pSupportedSurfaceFormats ) != VK_SUCCESS )
	{
		dprintf( "Unable to query supported swapchain formats.\n" );
		return false;
	}
	if ( nNumSupportedSwapChainFormats == 1 && pSupportedSurfaceFormats[0].format == VK_FORMAT_UNDEFINED )
	{
		nSwapChainFormat = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else 
	{
		// Favor sRGB if it's available
		for ( nFormatIndex = 0; nFormatIndex < nNumSupportedSwapChainFormats; nFormatIndex++ )
		{
			if ( pSupportedSurfaceFormats[ nFormatIndex ].format == VK_FORMAT_B8G8R8A8_SRGB || 
				 pSupportedSurfaceFormats[ nFormatIndex ].format == VK_FORMAT_R8G8B8A8_SRGB )
			{
				break;
			}
		}
		if ( nFormatIndex == nNumSupportedSwapChainFormats )
		{
			// Default to the first one if no sRGB
			nFormatIndex = 0;
		}
		nSwapChainFormat = pSupportedSurfaceFormats[ nFormatIndex ].format;
	}
	nColorSpace = pSupportedSurfaceFormats[ nFormatIndex ].colorSpace;
	delete [] pSupportedSurfaceFormats;

	// Check the surface properties and formats
	VkSurfaceCapabilitiesKHR surfaceCaps = {};
	nResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR( m_pPhysicalDevice, m_pSurface, &surfaceCaps );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed with error %d\n", nResult );
		return false;
	}

	uint32_t nPresentModeCount = 0;
	nResult = vkGetPhysicalDeviceSurfacePresentModesKHR( m_pPhysicalDevice, m_pSurface, &nPresentModeCount, NULL );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkGetPhysicalDeviceSurfacePresentModesKHR failed with error %d\n", nResult );
		return false;
	}
	VkPresentModeKHR *pPresentModes = new VkPresentModeKHR[ nPresentModeCount ];
	nResult = vkGetPhysicalDeviceSurfacePresentModesKHR( m_pPhysicalDevice, m_pSurface, &nPresentModeCount, pPresentModes );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkGetPhysicalDeviceSurfacePresentModesKHR failed with error %d\n", nResult );
		return false;
	}

	// width and height are either both -1, or both not -1.
	VkExtent2D swapChainExtent;
	if ( surfaceCaps.currentExtent.width == -1 )
	{
		// If the surface size is undefined, the size is set to the size of the images requested.
		swapChainExtent.width = m_nCompanionWindowWidth;
		swapChainExtent.height = m_nCompanionWindowHeight;
	}
	else
	{
		// If the surface size is defined, the swap chain size must match
		swapChainExtent = surfaceCaps.currentExtent;
	}

	// VK_PRESENT_MODE_FIFO_KHR - equivalent of eglSwapInterval(1).  The presentation engine waits for the next vertical blanking period to update 
	// the current image. Tearing cannot be observed. This mode must be supported by all implementations.
	VkPresentModeKHR swapChainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	for ( uint32_t i = 0; i < nPresentModeCount; i++ )
	{
		// Order of preference for no vsync:
		// 1. VK_PRESENT_MODE_IMMEDIATE_KHR - The presentation engine does not wait for a vertical blanking period to update the current image,
		//                                    meaning this mode may result in visible tearing
		// 2. VK_PRESENT_MODE_MAILBOX_KHR - The presentation engine waits for the next vertical blanking period to update the current image. Tearing cannot be observed. 
		//                                  An internal single-entry queue is used to hold pending presentation requests.
		// 3. VK_PRESENT_MODE_FIFO_RELAXED_KHR - equivalent of eglSwapInterval(-1).
		if ( pPresentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR )
		{
			// The presentation engine does not wait for a vertical blanking period to update the 
			// current image, meaning this mode may result in visible tearing
			swapChainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			break;
		}
		else if ( pPresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR )
		{
			swapChainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		}
		else if ( ( swapChainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR ) &&
			( pPresentModes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR ) )
		{
			// VK_PRESENT_MODE_FIFO_RELAXED_KHR - equivalent of eglSwapInterval(-1)
			swapChainPresentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		}
	}

	// Have a swap queue depth of at least three frames
	m_nSwapQueueImageCount = surfaceCaps.minImageCount;
	if ( m_nSwapQueueImageCount < 2 )
	{
		m_nSwapQueueImageCount = 2;
	}
	if ( ( surfaceCaps.maxImageCount > 0 ) && ( m_nSwapQueueImageCount > surfaceCaps.maxImageCount ) )
	{
		// Application must settle for fewer images than desired:
		m_nSwapQueueImageCount = surfaceCaps.maxImageCount;
	}

	VkSurfaceTransformFlagsKHR preTransform;
	if ( surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR )
	{
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		preTransform = surfaceCaps.currentTransform;
	}

	VkImageUsageFlags nImageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if ( ( surfaceCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT ) )
	{
		nImageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}
	else
	{
		dprintf( "Vulkan swapchain does not support VK_IMAGE_USAGE_TRANSFER_DST_BIT. Some operations may not be supported.\n" );
	}

	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.pNext = NULL;
	swapChainCreateInfo.surface = m_pSurface;
	swapChainCreateInfo.minImageCount = m_nSwapQueueImageCount;
	swapChainCreateInfo.imageFormat = nSwapChainFormat;
	swapChainCreateInfo.imageColorSpace = nColorSpace;
	swapChainCreateInfo.imageExtent = swapChainExtent;
	swapChainCreateInfo.imageUsage = nImageUsageFlags;
	swapChainCreateInfo.preTransform = ( VkSurfaceTransformFlagBitsKHR ) preTransform;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapChainCreateInfo.queueFamilyIndexCount = 0;
	swapChainCreateInfo.pQueueFamilyIndices = NULL;
	swapChainCreateInfo.presentMode = swapChainPresentMode;
	swapChainCreateInfo.clipped = VK_TRUE;
	if ( ( surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR ) != 0 )
	{
		swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	}
	else if ( ( surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR ) != 0 )
	{
		swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
	}
	else
	{
		dprintf( "Unexpected value for VkSurfaceCapabilitiesKHR.compositeAlpha: %x\n", surfaceCaps.supportedCompositeAlpha );
	}

	nResult =  vkCreateSwapchainKHR( m_pDevice, &swapChainCreateInfo, NULL, &m_pSwapchain );
	if( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateSwapchainKHR returned an error %d.\n", nResult );
		return false;
	}

	nResult = vkGetSwapchainImagesKHR( m_pDevice, m_pSwapchain, &m_nSwapQueueImageCount, NULL );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkGetSwapchainImagesKHR failed with error %d\n", nResult );
		return false;
	}
	m_swapchainImages.resize( m_nSwapQueueImageCount );
	vkGetSwapchainImagesKHR( m_pDevice, m_pSwapchain, &m_nSwapQueueImageCount, &m_swapchainImages[ 0 ] );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkGetSwapchainImagesKHR failed with error %d\n", nResult );
		return false;
	}

	// Create a renderpass
	uint32_t nTotalAttachments = 1;
	VkAttachmentDescription attachmentDesc;
	VkAttachmentReference attachmentReference;
	attachmentReference.attachment = 0;
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	
	attachmentDesc.format = nSwapChainFormat;
	attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDesc.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentDesc.flags = 0;

	VkSubpassDescription subPassCreateInfo = { };
	subPassCreateInfo.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPassCreateInfo.flags = 0;
	subPassCreateInfo.inputAttachmentCount = 0;
	subPassCreateInfo.pInputAttachments = NULL;
	subPassCreateInfo.colorAttachmentCount = 1;
	subPassCreateInfo.pColorAttachments = &attachmentReference;
	subPassCreateInfo.pResolveAttachments = NULL;
	subPassCreateInfo.pDepthStencilAttachment = NULL;
	subPassCreateInfo.preserveAttachmentCount = 0;
	subPassCreateInfo.pPreserveAttachments = NULL;

	VkRenderPassCreateInfo renderPassCreateInfo = { };
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &attachmentDesc;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subPassCreateInfo;
	renderPassCreateInfo.dependencyCount = 0;
	renderPassCreateInfo.pDependencies = NULL;

	nResult = vkCreateRenderPass( m_pDevice, &renderPassCreateInfo, NULL, &m_pSwapchainRenderPass );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateRenderPass failed with error %d\n", nResult );
		return false;
	}

	// Create image views and framebuffers for each swapchain image
	for ( size_t nImage = 0; nImage < m_swapchainImages.size(); nImage++ )
	{
		VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		imageViewCreateInfo.flags = 0;
		imageViewCreateInfo.image = m_swapchainImages[ nImage ];
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = nSwapChainFormat;
		imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		VkImageView pImageView = VK_NULL_HANDLE;
		vkCreateImageView( m_pDevice, &imageViewCreateInfo, nullptr, &pImageView );
		m_pSwapchainImageViews.push_back( pImageView );

		VkImageView attachments[ 1 ] = { pImageView };
		VkFramebufferCreateInfo framebufferCreateInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
		framebufferCreateInfo.renderPass = m_pSwapchainRenderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &attachments[ 0 ];
		framebufferCreateInfo.width = m_nCompanionWindowWidth;
		framebufferCreateInfo.height = m_nCompanionWindowHeight;
		framebufferCreateInfo.layers = 1;
		VkFramebuffer pFramebuffer;
		nResult = vkCreateFramebuffer( m_pDevice, &framebufferCreateInfo, NULL, &pFramebuffer );
		if ( nResult != VK_SUCCESS )
		{
			dprintf( "vkCreateFramebuffer failed with error %d.\n", nResult );
			return false;
		}
		m_pSwapchainFramebuffers.push_back( pFramebuffer );

		VkSemaphoreCreateInfo semaphoreCreateInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		VkSemaphore pSemaphore = VK_NULL_HANDLE;
		vkCreateSemaphore( m_pDevice, &semaphoreCreateInfo, nullptr, &pSemaphore );
		m_pSwapchainSemaphores.push_back( pSemaphore );
	}

	delete [] pPresentModes;
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Initialize Vulkan. Returns true if Vulkan has been successfully
//          initialized, false if shaders could not be created.
//          If failure occurred in a module other than shaders, the function
//          may return true or throw an error. 
//-----------------------------------------------------------------------------
bool CMainApplication::BInitVulkan()
{
	if ( !BInitVulkanInstance() )
		return false;

	if ( !BInitVulkanDevice() )
		return false;

	if ( !BInitVulkanSwapchain() )
		return false;

	VkResult nResult;

	// Create the command pool
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		commandPoolCreateInfo.queueFamilyIndex = m_nQueueFamilyIndex;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		nResult = vkCreateCommandPool( m_pDevice, &commandPoolCreateInfo, nullptr, &m_pCommandPool );
		if ( nResult != VK_SUCCESS )
		{
			dprintf( "vkCreateCommandPool returned error %d.", nResult );
			return false;
		}
	}

	// Command buffer used during resource loading
	m_currentCommandBuffer = GetCommandBuffer();
	VkCommandBufferBeginInfo commandBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer( m_currentCommandBuffer.m_pCommandBuffer, &commandBufferBeginInfo );

	SetupTexturemaps();
	SetupScene();
	SetupCameras();
	SetupStereoRenderTargets();
	SetupCompanionWindow();
	
	if( !CreateAllShaders() )
		return false;

	CreateAllDescriptorSets();
	SetupRenderModels();

	// Submit the command buffer used during loading
	vkEndCommandBuffer( m_currentCommandBuffer.m_pCommandBuffer );
	VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_currentCommandBuffer.m_pCommandBuffer;
	vkQueueSubmit( m_pQueue, 1, &submitInfo, m_currentCommandBuffer.m_pFence );
	m_commandBuffers.push_front( m_currentCommandBuffer );

	// Wait for the GPU before proceeding
	vkQueueWaitIdle( m_pQueue );

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Initialize Compositor. Returns true if the compositor was
//          successfully initialized, false otherwise.
//-----------------------------------------------------------------------------
bool CMainApplication::BInitCompositor()
{
	vr::EVRInitError peError = vr::VRInitError_None;

	if ( !vr::VRCompositor() )
	{
		dprintf( "Compositor initialization failed. See log file for details\n" );
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::Shutdown()
{
	if ( m_pDevice != VK_NULL_HANDLE )
	{
		// Idle the device to make sure no work is outstanding
		vkDeviceWaitIdle( m_pDevice );
	}

	if( m_pHMD )
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}

	for( std::vector< VulkanRenderModel * >::iterator i = m_vecRenderModels.begin(); i != m_vecRenderModels.end(); i++ )
	{
		delete (*i);
	}
	m_vecRenderModels.clear();

	if ( m_pDevice != VK_NULL_HANDLE )
	{
		for( std::deque< VulkanCommandBuffer_t >::iterator i = m_commandBuffers.begin(); i != m_commandBuffers.end(); i++ )
		{
			vkFreeCommandBuffers( m_pDevice, m_pCommandPool, 1, &i->m_pCommandBuffer );
			vkDestroyFence( m_pDevice, i->m_pFence, nullptr );
		}

		vkDestroyCommandPool( m_pDevice, m_pCommandPool, nullptr );
		vkDestroyDescriptorPool( m_pDevice, m_pDescriptorPool, nullptr );

		FramebufferDesc *pFramebufferDescs[2] = { &m_leftEyeDesc, &m_rightEyeDesc };
		for ( int32_t nFramebuffer = 0; nFramebuffer < 2; nFramebuffer++ )
		{
			if ( pFramebufferDescs[ nFramebuffer ]->m_pImageView != VK_NULL_HANDLE )
			{
				vkDestroyImageView( m_pDevice, pFramebufferDescs[ nFramebuffer ]->m_pImageView, nullptr );
				vkDestroyImage( m_pDevice, pFramebufferDescs[ nFramebuffer ]->m_pImage, nullptr );
				vkFreeMemory( m_pDevice, pFramebufferDescs[ nFramebuffer ]->m_pDeviceMemory, nullptr );
				vkDestroyImageView( m_pDevice, pFramebufferDescs[ nFramebuffer ]->m_pDepthStencilImageView, nullptr );
				vkDestroyImage( m_pDevice, pFramebufferDescs[ nFramebuffer ]->m_pDepthStencilImage, nullptr );
				vkFreeMemory( m_pDevice, pFramebufferDescs[ nFramebuffer ]->m_pDepthStencilDeviceMemory, nullptr );
				vkDestroyRenderPass( m_pDevice, pFramebufferDescs[ nFramebuffer ]->m_pRenderPass, nullptr );
				vkDestroyFramebuffer( m_pDevice, pFramebufferDescs[ nFramebuffer ]->m_pFramebuffer, nullptr );
			}
		}

		vkDestroyImageView( m_pDevice, m_pSceneImageView, nullptr );
		vkDestroyImage( m_pDevice, m_pSceneImage, nullptr );
		vkFreeMemory( m_pDevice, m_pSceneImageMemory, nullptr );
		vkDestroyBuffer( m_pDevice, m_pSceneStagingBuffer, nullptr );
		vkFreeMemory( m_pDevice, m_pSceneStagingBufferMemory, nullptr );
		vkDestroySampler( m_pDevice, m_pSceneSampler, nullptr );
		vkDestroyBuffer( m_pDevice, m_pSceneVertexBuffer, nullptr );
		vkFreeMemory( m_pDevice, m_pSceneVertexBufferMemory, nullptr );
		for ( uint32_t nEye = 0; nEye < _countof( m_pSceneConstantBuffer); nEye++ )
		{
			vkDestroyBuffer( m_pDevice, m_pSceneConstantBuffer[ nEye ], nullptr );
			vkFreeMemory( m_pDevice, m_pSceneConstantBufferMemory[ nEye ], nullptr );
		}
	
		vkDestroyBuffer( m_pDevice, m_pCompanionWindowVertexBuffer, nullptr );
		vkFreeMemory( m_pDevice, m_pCompanionWindowVertexBufferMemory, nullptr );
		vkDestroyBuffer( m_pDevice, m_pCompanionWindowIndexBuffer, nullptr );
		vkFreeMemory( m_pDevice, m_pCompanionWindowIndexBufferMemory, nullptr );

		vkDestroyBuffer( m_pDevice, m_pControllerAxesVertexBuffer, nullptr );
		vkFreeMemory( m_pDevice, m_pControllerAxesVertexBufferMemory, nullptr );

		vkDestroyPipelineLayout( m_pDevice, m_pPipelineLayout, nullptr );
		vkDestroyDescriptorSetLayout( m_pDevice, m_pDescriptorSetLayout, nullptr );
		for ( uint32_t nPSO = 0; nPSO < PSO_COUNT; nPSO++ )
		{
			vkDestroyPipeline( m_pDevice, m_pPipelines[ nPSO ], nullptr );
		}
		for ( uint32_t nShader = 0; nShader < _countof( m_pShaderModules); nShader++ )
		{
			vkDestroyShaderModule( m_pDevice, m_pShaderModules[ nShader ], nullptr );
		}
		vkDestroyPipelineCache( m_pDevice, m_pPipelineCache, nullptr );

		if ( m_pDebugReportCallback != VK_NULL_HANDLE )
		{
			g_pVkDestroyDebugReportCallbackEXT( m_pInstance, m_pDebugReportCallback, nullptr );
		}

		for ( size_t nSwapchainIndex = 0; nSwapchainIndex < m_pSwapchainFramebuffers.size(); nSwapchainIndex++ )
		{
			vkDestroyFramebuffer( m_pDevice, m_pSwapchainFramebuffers[ nSwapchainIndex ], nullptr );
			vkDestroyImageView( m_pDevice, m_pSwapchainImageViews[ nSwapchainIndex ], nullptr );
			vkDestroySemaphore( m_pDevice, m_pSwapchainSemaphores[ nSwapchainIndex ], nullptr );
		}
		vkDestroyRenderPass( m_pDevice, m_pSwapchainRenderPass, nullptr );

		vkDestroySwapchainKHR( m_pDevice, m_pSwapchain, nullptr );
		vkDestroySurfaceKHR( m_pInstance, m_pSurface, nullptr );
		vkDestroyDevice( m_pDevice, nullptr );
		vkDestroyInstance( m_pInstance, nullptr );
	}
	if( m_pCompanionWindow )
	{
		SDL_DestroyWindow(m_pCompanionWindow);
	}

	SDL_Quit();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CMainApplication::HandleInput()
{
	SDL_Event sdlEvent;
	bool bRet = false;

	while ( SDL_PollEvent( &sdlEvent ) != 0 )
	{
		if ( sdlEvent.type == SDL_QUIT )
		{
			bRet = true;
		}
		else if ( sdlEvent.type == SDL_KEYDOWN )
		{
			if ( sdlEvent.key.keysym.sym == SDLK_ESCAPE 
			     || sdlEvent.key.keysym.sym == SDLK_q )
			{
				bRet = true;
			}
			if( sdlEvent.key.keysym.sym == SDLK_c )
			{
				m_bShowCubes = !m_bShowCubes;
			}
		}
	}

	// Process SteamVR events
	vr::VREvent_t event;
	while( m_pHMD->PollNextEvent( &event, sizeof( event ) ) )
	{
		ProcessVREvent( event );
	}

	// Process SteamVR controller state
	for( vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++ )
	{
		vr::VRControllerState_t state;
		if( m_pHMD->GetControllerState( unDevice, &state, sizeof(state) ) )
		{
			m_rbShowTrackedDevice[ unDevice ] = state.ulButtonPressed == 0;
		}
	}

	return bRet;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RunMainLoop()
{
	bool bQuit = false;

	SDL_StartTextInput();
	SDL_ShowCursor( SDL_DISABLE );

	while ( !bQuit )
	{
		bQuit = HandleInput();

		RenderFrame();
	}

	SDL_StopTextInput();
}

//-----------------------------------------------------------------------------
// Purpose: Processes a single VR event
//-----------------------------------------------------------------------------
void CMainApplication::ProcessVREvent( const vr::VREvent_t & event )
{
	switch( event.eventType )
	{
	case vr::VREvent_TrackedDeviceActivated:
		{
			SetupRenderModelForTrackedDevice( event.trackedDeviceIndex );
			dprintf( "Device %u attached. Setting up render model.\n", event.trackedDeviceIndex );
		}
		break;
	case vr::VREvent_TrackedDeviceDeactivated:
		{
			dprintf( "Device %u detached.\n", event.trackedDeviceIndex );
		}
		break;
	case vr::VREvent_TrackedDeviceUpdated:
		{
			dprintf( "Device %u updated.\n", event.trackedDeviceIndex );
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RenderFrame()
{
	if ( m_pHMD )
	{
		m_currentCommandBuffer = GetCommandBuffer();

		// Start the command buffer
		VkCommandBufferBeginInfo commandBufferBeginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer( m_currentCommandBuffer.m_pCommandBuffer, &commandBufferBeginInfo );

		UpdateControllerAxes();
		RenderStereoTargets();
		RenderCompanionWindow();

		// End the command buffer
		vkEndCommandBuffer( m_currentCommandBuffer.m_pCommandBuffer );

		// Submit the command buffer
		VkPipelineStageFlags nWaitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_currentCommandBuffer.m_pCommandBuffer;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_pSwapchainSemaphores[ m_nFrameIndex ];
		submitInfo.pWaitDstStageMask = &nWaitDstStageMask;
		vkQueueSubmit( m_pQueue, 1, &submitInfo, m_currentCommandBuffer.m_pFence );

		// Add the command buffer back for later recycling
		m_commandBuffers.push_front( m_currentCommandBuffer );

		// Submit to SteamVR
		vr::VRTextureBounds_t bounds;
		bounds.uMin = 0.0f;
		bounds.uMax = 1.0f;
		bounds.vMin = 0.0f;
		bounds.vMax = 1.0f;

		vr::VRVulkanTextureData_t vulkanData;
		vulkanData.m_nImage = ( uint64_t ) m_leftEyeDesc.m_pImage;
		vulkanData.m_pDevice = ( VkDevice_T * ) m_pDevice;
		vulkanData.m_pPhysicalDevice = ( VkPhysicalDevice_T * ) m_pPhysicalDevice;
		vulkanData.m_pInstance = ( VkInstance_T *) m_pInstance;
		vulkanData.m_pQueue = ( VkQueue_T * ) m_pQueue;
		vulkanData.m_nQueueFamilyIndex = m_nQueueFamilyIndex;

		vulkanData.m_nWidth = m_nRenderWidth;
		vulkanData.m_nHeight = m_nRenderHeight;
		vulkanData.m_nFormat = VK_FORMAT_R8G8B8A8_SRGB;
		vulkanData.m_nSampleCount = m_nMSAASampleCount;

		vr::Texture_t texture = { &vulkanData, vr::TextureType_Vulkan, vr::ColorSpace_Auto };
		vr::VRCompositor()->Submit( vr::Eye_Left, &texture, &bounds );
		
		vulkanData.m_nImage = ( uint64_t ) m_rightEyeDesc.m_pImage;
		vr::VRCompositor()->Submit( vr::Eye_Right, &texture, &bounds );
	}

	VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = NULL;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &m_pSwapchain;
	presentInfo.pImageIndices = &m_nCurrentSwapchainImage;
	vkQueuePresentKHR( m_pQueue, &presentInfo );

	// Spew out the controller and pose count whenever they change.
	if ( m_iTrackedControllerCount != m_iTrackedControllerCount_Last || m_iValidPoseCount != m_iValidPoseCount_Last )
	{
		m_iValidPoseCount_Last = m_iValidPoseCount;
		m_iTrackedControllerCount_Last = m_iTrackedControllerCount;
		
		dprintf( "PoseCount:%d(%s) Controllers:%d\n", m_iValidPoseCount, m_strPoseClasses.c_str(), m_iTrackedControllerCount );
	}

	UpdateHMDMatrixPose();

	m_nFrameIndex = ( m_nFrameIndex + 1 ) % m_swapchainImages.size();
}

//-----------------------------------------------------------------------------
// Purpose: Creates all the shaders used by HelloVR Vulkan
//-----------------------------------------------------------------------------
bool CMainApplication::CreateAllShaders()
{
	VkResult nResult;
	std::string sExecutableDirectory = Path_StripFilename( Path_GetExecutablePath() );
	
	const char *pShaderNames[ PSO_COUNT ] =
	{
		"scene",
		"axes",
		"rendermodel",
		"companion"
	};
	const char *pStageNames[ 2 ] =
	{
		"vs",
		"ps"
	};

	// Load the SPIR-V into shader modules
	for ( int32_t nShader = 0; nShader < PSO_COUNT; nShader++ )
	{
		for ( int32_t nStage = 0; nStage <= 1; nStage++ )
		{
			char shaderFileName[ 1024 ];
			sprintf( shaderFileName, "../shaders/%s_%s.spv", pShaderNames[ nShader ], pStageNames[ nStage ] );
			std::string shaderPath =  Path_MakeAbsolute( shaderFileName, sExecutableDirectory );

			FILE *fp = fopen( shaderPath.c_str(), "rb" );
			if ( fp == NULL )
			{
				dprintf( "Error opening SPIR-V file: %s\n", shaderPath.c_str() );
				return false;
			}
			fseek( fp, 0, SEEK_END );
			size_t nSize = ftell( fp );
			fseek( fp, 0, SEEK_SET );
			
			char *pBuffer = new char[ nSize ];
			if ( fread( pBuffer, 1, nSize, fp ) != nSize )
			{
				dprintf( "Error reading SPIR-V file: %s\n", shaderPath.c_str() );
				return false;
			}
			fclose( fp );

			// Create the shader module
			VkShaderModuleCreateInfo shaderModuleCreateInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
			shaderModuleCreateInfo.codeSize = nSize;
			shaderModuleCreateInfo.pCode = ( const uint32_t *) pBuffer;
			nResult = vkCreateShaderModule( m_pDevice, &shaderModuleCreateInfo, nullptr, &m_pShaderModules[ nShader * 2 + nStage ] );
			if ( nResult != VK_SUCCESS )
			{
				dprintf( "Error creating shader module for %s, error %d\n", shaderPath.c_str(), nResult );
				return false;
			}

			delete [] pBuffer;
		}
	}

	// Create a descriptor set layout/pipeline layout compatible with all of our shaders.  See bin/shaders/build_vulkan_shaders.bat for
	// how the HLSL is compiled with glslangValidator and binding numbers are generated
	VkDescriptorSetLayoutBinding layoutBindings[3] = {};
	layoutBindings[0].binding = 0;
	layoutBindings[0].descriptorCount = 1;
	layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	layoutBindings[1].binding = 1;
	layoutBindings[1].descriptorCount = 1;
	layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	layoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	layoutBindings[2].binding = 2;
	layoutBindings[2].descriptorCount = 1;
	layoutBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
	layoutBindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
	descriptorSetLayoutCreateInfo.bindingCount = 3;
	descriptorSetLayoutCreateInfo.pBindings = &layoutBindings[ 0 ];
	nResult = vkCreateDescriptorSetLayout( m_pDevice, &descriptorSetLayoutCreateInfo, nullptr, &m_pDescriptorSetLayout );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateDescriptorSetLayout failed with error %d\n", nResult );
		return false;
	}

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
	pipelineLayoutCreateInfo.pNext = NULL;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &m_pDescriptorSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = NULL;
	nResult = vkCreatePipelineLayout( m_pDevice, &pipelineLayoutCreateInfo, nullptr, &m_pPipelineLayout );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreatePipelineLayout failed with error %d\n", nResult );
		return false;
	}

	// Create pipeline cache
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo = { VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO };
	vkCreatePipelineCache( m_pDevice, &pipelineCacheCreateInfo, NULL, &m_pPipelineCache );
	
	// Renderpass for each PSO that is compatible with what it will render to
	VkRenderPass pRenderPasses[ PSO_COUNT ] =
	{
		m_leftEyeDesc.m_pRenderPass,
		m_leftEyeDesc.m_pRenderPass,
		m_leftEyeDesc.m_pRenderPass,
		m_pSwapchainRenderPass
	};

	size_t nStrides[ PSO_COUNT ] =
	{
		sizeof( VertexDataScene ),			// PSO_SCENE
		sizeof( float ) * 6,				// PSO_AXES
		sizeof( vr::RenderModel_Vertex_t ),	// PSO_RENDERMODEL
		sizeof( VertexDataWindow )			// PSO_COMPANION
	};

	VkVertexInputAttributeDescription attributeDescriptions[ PSO_COUNT * 3 ]
	{
		// PSO_SCENE
		{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,	0 },
		{ 1, 0, VK_FORMAT_R32G32_SFLOAT,	offsetof( VertexDataScene, texCoord ) },
		{ 0, 0, VK_FORMAT_UNDEFINED,		0 },
		// PSO_AXES
		{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,	0 },
		{ 1, 0, VK_FORMAT_R32G32B32_SFLOAT,	sizeof( float ) * 3 },
		{ 0, 0, VK_FORMAT_UNDEFINED,		0 },
		// PSO_RENDERMODEL
		{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT,	0 },
		{ 1, 0, VK_FORMAT_R32G32B32_SFLOAT,	offsetof( vr::RenderModel_Vertex_t, vNormal ) },
		{ 2, 0, VK_FORMAT_R32G32_SFLOAT,	offsetof( vr::RenderModel_Vertex_t, rfTextureCoord ) },
		// PSO_COMPANION
		{ 0, 0, VK_FORMAT_R32G32_SFLOAT,	0 },
		{ 1, 0, VK_FORMAT_R32G32_SFLOAT,	sizeof( float ) * 2 },
		{ 0, 0, VK_FORMAT_UNDEFINED,		0 },
	};

	// Create the PSOs
	for ( uint32_t nPSO = 0; nPSO < PSO_COUNT; nPSO++ )
	{
		VkGraphicsPipelineCreateInfo pipelineCreateInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
		
		// VkPipelineVertexInputStateCreateInfo
		VkVertexInputBindingDescription bindingDescription;
		bindingDescription.binding = 0;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		bindingDescription.stride = nStrides[ nPSO ];
		
		VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
		for ( uint32_t nAttr = 0; nAttr < 3; nAttr++ )
		{
			if ( attributeDescriptions[ nPSO * 3 + nAttr ].format != VK_FORMAT_UNDEFINED )
			{
				vertexInputCreateInfo.vertexAttributeDescriptionCount++;
			}
		}
		vertexInputCreateInfo.pVertexAttributeDescriptions = &attributeDescriptions[ nPSO * 3 ];
		vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
		vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;

		// VkPipelineDepthStencilStateCreateInfo
		VkPipelineDepthStencilStateCreateInfo dsState = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
		dsState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		dsState.depthTestEnable = ( nPSO != PSO_COMPANION ) ? VK_TRUE : VK_FALSE;
		dsState.depthWriteEnable = ( nPSO != PSO_COMPANION ) ? VK_TRUE : VK_FALSE;
		dsState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		dsState.depthBoundsTestEnable = VK_FALSE;
		dsState.stencilTestEnable = VK_FALSE;
		dsState.minDepthBounds = 0.0f;
		dsState.maxDepthBounds = 0.0f;

		// VkPipelineColorBlendStateCreateInfo
		VkPipelineColorBlendStateCreateInfo cbState = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
		cbState.logicOpEnable = VK_FALSE;
		cbState.logicOp = VK_LOGIC_OP_COPY;
		VkPipelineColorBlendAttachmentState cbAttachmentState = {};
		cbAttachmentState.blendEnable = VK_FALSE;
		cbAttachmentState.colorWriteMask = 0xf;
		cbState.attachmentCount = 1;
		cbState.pAttachments = &cbAttachmentState;

		// VkPipelineColorBlendStateCreateInfo
		VkPipelineRasterizationStateCreateInfo rsState = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
		rsState.polygonMode = VK_POLYGON_MODE_FILL;
		rsState.cullMode = VK_CULL_MODE_BACK_BIT;
		rsState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rsState.lineWidth = 1.0f;

		// VkPipelineInputAssemblyStateCreateInfo
		VkPipelineInputAssemblyStateCreateInfo iaState = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
		iaState.topology = ( nPSO == PSO_AXES ) ? VK_PRIMITIVE_TOPOLOGY_LINE_LIST : VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		iaState.primitiveRestartEnable = VK_FALSE;

		// VkPipelineMultisampleStateCreateInfo
		VkPipelineMultisampleStateCreateInfo msState = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
		msState.rasterizationSamples = ( nPSO == PSO_COMPANION ) ? VK_SAMPLE_COUNT_1_BIT : ( VkSampleCountFlagBits ) m_nMSAASampleCount;
		msState.minSampleShading = 0.0f;
		uint32_t nSampleMask = 0xFFFFFFFF;
		msState.pSampleMask = &nSampleMask;

		// VkPipelineViewportStateCreateInfo
		VkPipelineViewportStateCreateInfo vpState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
		vpState.viewportCount = 1;
		vpState.scissorCount = 1;

		VkPipelineShaderStageCreateInfo shaderStages[ 2 ] = { };
		shaderStages[ 0 ].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[ 0 ].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[ 0 ].module = m_pShaderModules[ nPSO * 2 + 0 ];
		shaderStages[ 0 ].pName = "VSMain";
		
		shaderStages[ 1 ].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[ 1 ].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[ 1 ].module = m_pShaderModules[ nPSO * 2 + 1 ];
		shaderStages[ 1 ].pName = "PSMain";

		pipelineCreateInfo.layout = m_pPipelineLayout;

		// Set pipeline states
		pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
		pipelineCreateInfo.pInputAssemblyState = &iaState;
		pipelineCreateInfo.pViewportState = &vpState;
		pipelineCreateInfo.pRasterizationState = &rsState;
		pipelineCreateInfo.pMultisampleState = &msState;
		pipelineCreateInfo.pDepthStencilState = &dsState;
		pipelineCreateInfo.pColorBlendState = &cbState;
		pipelineCreateInfo.stageCount = 2;
		pipelineCreateInfo.pStages = &shaderStages[ 0 ];
		pipelineCreateInfo.renderPass = pRenderPasses[ nPSO ];

		static VkDynamicState dynamicStates[] =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
		};

		static VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
		dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateCreateInfo.pNext = NULL;
		dynamicStateCreateInfo.dynamicStateCount = _countof( dynamicStates );
		dynamicStateCreateInfo.pDynamicStates = &dynamicStates[ 0 ];
		pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;


		// Create the pipeline
		nResult = vkCreateGraphicsPipelines( m_pDevice, m_pPipelineCache, 1, &pipelineCreateInfo, NULL, &m_pPipelines[ nPSO ] );
		if ( nResult != VK_SUCCESS )
		{
			dprintf( "vkCreateGraphicsPipelines failed with error %d\n", nResult );
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Creates all the descriptor sets
//-----------------------------------------------------------------------------
void CMainApplication::CreateAllDescriptorSets()
{
	VkDescriptorPoolSize poolSizes[ 3 ];
	poolSizes[ 0 ].descriptorCount = NUM_DESCRIPTOR_SETS;
	poolSizes[ 0 ].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[ 1 ].descriptorCount = NUM_DESCRIPTOR_SETS;
	poolSizes[ 1 ].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	poolSizes[ 2 ].descriptorCount = NUM_DESCRIPTOR_SETS;
	poolSizes[ 2 ].type = VK_DESCRIPTOR_TYPE_SAMPLER;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
	descriptorPoolCreateInfo.flags = 0;
	descriptorPoolCreateInfo.maxSets = NUM_DESCRIPTOR_SETS;
	descriptorPoolCreateInfo.poolSizeCount = _countof( poolSizes );
	descriptorPoolCreateInfo.pPoolSizes = &poolSizes[ 0 ];

	vkCreateDescriptorPool( m_pDevice, &descriptorPoolCreateInfo, nullptr, &m_pDescriptorPool );

	for ( int nDescriptorSet = 0; nDescriptorSet < NUM_DESCRIPTOR_SETS; nDescriptorSet++ )
	{
		VkDescriptorSetAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
		allocInfo.descriptorPool = m_pDescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_pDescriptorSetLayout;
		vkAllocateDescriptorSets( m_pDevice, &allocInfo, &m_pDescriptorSets[ nDescriptorSet ] );
	}

	// Scene descriptor sets
	for ( uint32_t nEye = 0; nEye < 2; nEye++ )
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = m_pSceneConstantBuffer[ nEye ];
		bufferInfo.offset = 0;
		bufferInfo.range = VK_WHOLE_SIZE;
		
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageView = m_pSceneImageView;
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		
		VkDescriptorImageInfo samplerInfo = {};
		samplerInfo.sampler = m_pSceneSampler;

		VkWriteDescriptorSet writeDescriptorSets[ 3 ] = { };
		writeDescriptorSets[ 0 ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[ 0 ].dstSet = m_pDescriptorSets[ DESCRIPTOR_SET_LEFT_EYE_SCENE + nEye ];
		writeDescriptorSets[ 0 ].dstBinding = 0;
		writeDescriptorSets[ 0 ].descriptorCount = 1;
		writeDescriptorSets[ 0 ].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSets[ 0 ].pBufferInfo = &bufferInfo;
		writeDescriptorSets[ 1 ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[ 1 ].dstSet = m_pDescriptorSets[ DESCRIPTOR_SET_LEFT_EYE_SCENE + nEye ];
		writeDescriptorSets[ 1 ].dstBinding = 1;
		writeDescriptorSets[ 1 ].descriptorCount = 1;
		writeDescriptorSets[ 1 ].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		writeDescriptorSets[ 1 ].pImageInfo = &imageInfo;
		writeDescriptorSets[ 2 ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[ 2 ].dstSet = m_pDescriptorSets[ DESCRIPTOR_SET_LEFT_EYE_SCENE + nEye ];
		writeDescriptorSets[ 2 ].dstBinding = 2;
		writeDescriptorSets[ 2 ].descriptorCount = 1;
		writeDescriptorSets[ 2 ].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		writeDescriptorSets[ 2 ].pImageInfo = &samplerInfo;
		
		vkUpdateDescriptorSets( m_pDevice, _countof( writeDescriptorSets ), writeDescriptorSets, 0, nullptr );
	}

	// Companion window descriptor sets
	{
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageView = m_leftEyeDesc.m_pImageView;
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkWriteDescriptorSet writeDescriptorSets[ 1 ] = { };
		writeDescriptorSets[ 0 ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[ 0 ].dstSet = m_pDescriptorSets[ DESCRIPTOR_SET_COMPANION_LEFT_TEXTURE ];
		writeDescriptorSets[ 0 ].dstBinding = 1;
		writeDescriptorSets[ 0 ].descriptorCount = 1;
		writeDescriptorSets[ 0 ].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		writeDescriptorSets[ 0 ].pImageInfo = &imageInfo;
		vkUpdateDescriptorSets( m_pDevice, _countof( writeDescriptorSets ), writeDescriptorSets, 0, nullptr );

		imageInfo.imageView = m_rightEyeDesc.m_pImageView;
		writeDescriptorSets[ 0 ].dstSet = m_pDescriptorSets[ DESCRIPTOR_SET_COMPANION_RIGHT_TEXTURE ];
		vkUpdateDescriptorSets( m_pDevice, _countof( writeDescriptorSets ), writeDescriptorSets, 0, nullptr );
	}
	
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CMainApplication::SetupTexturemaps()
{
	std::string sExecutableDirectory = Path_StripFilename( Path_GetExecutablePath() );
	std::string strFullPath = Path_MakeAbsolute( "../cube_texture.png", sExecutableDirectory );
	
	std::vector< unsigned char > imageRGBA;
	unsigned nImageWidth, nImageHeight;
	unsigned nError = lodepng::decode( imageRGBA, nImageWidth, nImageHeight, strFullPath.c_str() );
	
	if ( nError != 0 )
		return false;
	
	// Copy the base level to a buffer, reserve space for mips (overreserve by a bit to avoid having to calc mipchain size ahead of time)
	VkDeviceSize nBufferSize = 0;
	uint8_t *pBuffer = new uint8_t[ nImageWidth * nImageHeight * 4 * 2 ];
	uint8_t *pPrevBuffer = pBuffer;
	uint8_t *pCurBuffer = pBuffer;
	memcpy( pCurBuffer, &imageRGBA[0], sizeof( uint8_t ) * nImageWidth * nImageHeight * 4 );
	pCurBuffer += sizeof( uint8_t ) * nImageWidth * nImageHeight * 4;

	std::vector< VkBufferImageCopy > bufferImageCopies;
	VkBufferImageCopy bufferImageCopy = {};
	bufferImageCopy.bufferOffset = 0;
	bufferImageCopy.bufferRowLength = 0;
	bufferImageCopy.bufferImageHeight = 0;
	bufferImageCopy.imageSubresource.baseArrayLayer = 0;
	bufferImageCopy.imageSubresource.layerCount = 1;
	bufferImageCopy.imageSubresource.mipLevel = 0;
	bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	bufferImageCopy.imageOffset.x = 0;
	bufferImageCopy.imageOffset.y = 0;
	bufferImageCopy.imageOffset.z = 0;
	bufferImageCopy.imageExtent.width = nImageWidth;
	bufferImageCopy.imageExtent.height = nImageHeight;
	bufferImageCopy.imageExtent.depth = 1;
	bufferImageCopies.push_back( bufferImageCopy );

	int nMipWidth = nImageWidth;
	int nMipHeight = nImageHeight;

	while( nMipWidth > 1 && nMipHeight > 1 )
	{
		GenMipMapRGBA( pPrevBuffer, pCurBuffer, nMipWidth, nMipHeight, &nMipWidth, &nMipHeight );
		bufferImageCopy.bufferOffset = pCurBuffer - pBuffer;
		bufferImageCopy.imageSubresource.mipLevel++;
		bufferImageCopy.imageExtent.width = nMipWidth;
		bufferImageCopy.imageExtent.height = nMipHeight;
		bufferImageCopies.push_back( bufferImageCopy );
		pPrevBuffer = pCurBuffer;
		pCurBuffer += ( nMipWidth * nMipHeight * 4 * sizeof( uint8_t ) );
	}
	nBufferSize = pCurBuffer - pBuffer;

	// Create the image
	VkImageCreateInfo imageCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.extent.width = nImageWidth;
	imageCreateInfo.extent.height = nImageHeight;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = ( uint32_t ) bufferImageCopies.size();
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	imageCreateInfo.flags = 0;
	vkCreateImage( m_pDevice, &imageCreateInfo, nullptr, &m_pSceneImage );

	VkMemoryRequirements memoryRequirements = {};
	vkGetImageMemoryRequirements( m_pDevice, m_pSceneImage, &memoryRequirements );

	VkMemoryAllocateInfo memoryAllocateInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	MemoryTypeFromProperties( m_physicalDeviceMemoryProperties, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memoryAllocateInfo.memoryTypeIndex );
	vkAllocateMemory( m_pDevice, &memoryAllocateInfo, nullptr, &m_pSceneImageMemory );
	vkBindImageMemory( m_pDevice, m_pSceneImage, m_pSceneImageMemory, 0 );

	VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = m_pSceneImage;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = imageCreateInfo.format;
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = imageCreateInfo.mipLevels;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;
	vkCreateImageView( m_pDevice, &imageViewCreateInfo, nullptr, &m_pSceneImageView );

	// Create a staging buffer
	if ( !CreateVulkanBuffer( m_pDevice, m_physicalDeviceMemoryProperties, pBuffer, nBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &m_pSceneStagingBuffer, &m_pSceneStagingBufferMemory ) )
	{
		return false;
	}

	// Transition the image to TRANSFER_DST to receive image
	VkImageMemoryBarrier imageMemoryBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
	imageMemoryBarrier.srcAccessMask = 0;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imageMemoryBarrier.image = m_pSceneImage;
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = imageCreateInfo.mipLevels;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	imageMemoryBarrier.subresourceRange.layerCount = 1;
	imageMemoryBarrier.srcQueueFamilyIndex = m_nQueueFamilyIndex;
	imageMemoryBarrier.dstQueueFamilyIndex = m_nQueueFamilyIndex;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );

	// Issue the copy to fill the image data
	vkCmdCopyBufferToImage( m_currentCommandBuffer.m_pCommandBuffer, m_pSceneStagingBuffer, m_pSceneImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ( uint32_t ) bufferImageCopies.size(), &bufferImageCopies[ 0 ] );

	// Transition the image to SHADER_READ_OPTIMAL for reading
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
	
	// Create the sampler
	VkSamplerCreateInfo samplerCreateInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.anisotropyEnable = VK_TRUE;
	samplerCreateInfo.maxAnisotropy = 16.0f;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = ( float ) imageCreateInfo.mipLevels;
	vkCreateSampler( m_pDevice, &samplerCreateInfo, nullptr, &m_pSceneSampler );

	delete [] pBuffer;
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: generate next level mipmap for an RGBA image
//-----------------------------------------------------------------------------
void CMainApplication::GenMipMapRGBA( const uint8_t *pSrc, uint8_t *pDst, int nSrcWidth, int nSrcHeight, int *pDstWidthOut, int *pDstHeightOut )
{
	*pDstWidthOut = nSrcWidth / 2;
	if ( *pDstWidthOut <= 0 )
	{
		*pDstWidthOut = 1;
	}
	*pDstHeightOut = nSrcHeight / 2;
	if ( *pDstHeightOut <= 0 )
	{
		*pDstHeightOut = 1;
	}

	for ( int y = 0; y < *pDstHeightOut; y++ )
	{
		for ( int x = 0; x < *pDstWidthOut; x++ )
		{
			int nSrcIndex[4];
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			float a = 0.0f;

			nSrcIndex[0] = ( ( ( y * 2 ) * nSrcWidth ) + ( x * 2 ) ) * 4;
			nSrcIndex[1] = ( ( ( y * 2 ) * nSrcWidth ) + ( x * 2 + 1 ) ) * 4;
			nSrcIndex[2] = ( ( ( ( y * 2 ) + 1 ) * nSrcWidth ) + ( x * 2 ) ) * 4;
			nSrcIndex[3] = ( ( ( ( y * 2 ) + 1 ) * nSrcWidth ) + ( x * 2 + 1 ) ) * 4;

			// Sum all pixels
			for ( int nSample = 0; nSample < 4; nSample++ )
			{
				r += pSrc[ nSrcIndex[ nSample ] ];
				g += pSrc[ nSrcIndex[ nSample ] + 1 ];
				b += pSrc[ nSrcIndex[ nSample ] + 2 ];
				a += pSrc[ nSrcIndex[ nSample ] + 3 ];
			}

			// Average results
			r /= 4.0;
			g /= 4.0;
			b /= 4.0;
			a /= 4.0;

			// Store resulting pixels
			pDst[ ( y * ( *pDstWidthOut ) + x ) * 4 ] = ( uint8_t ) ( r );
			pDst[ ( y * ( *pDstWidthOut ) + x ) * 4 + 1] = ( uint8_t ) ( g );
			pDst[ ( y * ( *pDstWidthOut ) + x ) * 4 + 2] = ( uint8_t ) ( b );
			pDst[ ( y * ( *pDstWidthOut ) + x ) * 4 + 3] = ( uint8_t ) ( a );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: create a sea of cubes
//-----------------------------------------------------------------------------
void CMainApplication::SetupScene()
{
	if ( !m_pHMD )
		return;

	std::vector<float> vertdataarray;

	Matrix4 matScale;
	matScale.scale( m_fScale, m_fScale, m_fScale );
	Matrix4 matTransform;
	matTransform.translate(
		-( (float)m_iSceneVolumeWidth * m_fScaleSpacing ) / 2.f,
		-( (float)m_iSceneVolumeHeight * m_fScaleSpacing ) / 2.f,
		-( (float)m_iSceneVolumeDepth * m_fScaleSpacing ) / 2.f);
	
	Matrix4 mat = matScale * matTransform;

	for( int z = 0; z< m_iSceneVolumeDepth; z++ )
	{
		for( int y = 0; y< m_iSceneVolumeHeight; y++ )
		{
			for( int x = 0; x< m_iSceneVolumeWidth; x++ )
			{
				AddCubeToScene( mat, vertdataarray );
				mat = mat * Matrix4().translate( m_fScaleSpacing, 0, 0 );
			}
			mat = mat * Matrix4().translate( -((float)m_iSceneVolumeWidth) * m_fScaleSpacing, m_fScaleSpacing, 0 );
		}
		mat = mat * Matrix4().translate( 0, -((float)m_iSceneVolumeHeight) * m_fScaleSpacing, m_fScaleSpacing );
	}
	m_uiVertcount = vertdataarray.size()/5;
	
	// Create the vertex buffer and fill with data
	if ( !CreateVulkanBuffer( m_pDevice, m_physicalDeviceMemoryProperties, &vertdataarray[ 0 ], vertdataarray.size() * sizeof( float ), 
							  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &m_pSceneVertexBuffer, &m_pSceneVertexBufferMemory ) )
	{
		return;
	}
	
	// Create constant buffer to hold the per-eye CB data
	for ( uint32_t nEye = 0; nEye < 2; nEye++ )
	{
		VkBufferCreateInfo bufferCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		bufferCreateInfo.size = sizeof( Matrix4 );
		bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		vkCreateBuffer( m_pDevice, &bufferCreateInfo, nullptr, &m_pSceneConstantBuffer[ nEye ] );

		VkMemoryRequirements memoryRequirements = { };
		vkGetBufferMemoryRequirements( m_pDevice, m_pSceneConstantBuffer[ nEye ], &memoryRequirements );

		VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		MemoryTypeFromProperties( m_physicalDeviceMemoryProperties, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, &allocInfo.memoryTypeIndex );
		allocInfo.allocationSize = memoryRequirements.size;

		vkAllocateMemory( m_pDevice, &allocInfo, nullptr, &m_pSceneConstantBufferMemory[ nEye ] );
		vkBindBufferMemory( m_pDevice, m_pSceneConstantBuffer[ nEye ], m_pSceneConstantBufferMemory[ nEye ], 0 );

		// Map and keep mapped persistently
		vkMapMemory( m_pDevice, m_pSceneConstantBufferMemory[ nEye ], 0, VK_WHOLE_SIZE, 0, &m_pSceneConstantBufferData[ nEye ] );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::AddCubeVertex( float fl0, float fl1, float fl2, float fl3, float fl4, std::vector<float> &vertdata )
{
	vertdata.push_back( fl0 );
	vertdata.push_back( fl1 );
	vertdata.push_back( fl2 );
	vertdata.push_back( fl3 );
	vertdata.push_back( fl4 );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::AddCubeToScene( Matrix4 mat, std::vector<float> &vertdata )
{
	// Matrix4 mat( outermat.data() );

	Vector4 A = mat * Vector4( 0, 0, 0, 1 );
	Vector4 B = mat * Vector4( 1, 0, 0, 1 );
	Vector4 C = mat * Vector4( 1, 1, 0, 1 );
	Vector4 D = mat * Vector4( 0, 1, 0, 1 );
	Vector4 E = mat * Vector4( 0, 0, 1, 1 );
	Vector4 F = mat * Vector4( 1, 0, 1, 1 );
	Vector4 G = mat * Vector4( 1, 1, 1, 1 );
	Vector4 H = mat * Vector4( 0, 1, 1, 1 );

	// triangles instead of quads
	AddCubeVertex( E.x, E.y, E.z, 0, 1, vertdata ); //Front
	AddCubeVertex( F.x, F.y, F.z, 1, 1, vertdata );
	AddCubeVertex( G.x, G.y, G.z, 1, 0, vertdata );
	AddCubeVertex( G.x, G.y, G.z, 1, 0, vertdata );
	AddCubeVertex( H.x, H.y, H.z, 0, 0, vertdata );
	AddCubeVertex( E.x, E.y, E.z, 0, 1, vertdata );
					 
	AddCubeVertex( B.x, B.y, B.z, 0, 1, vertdata ); //Back
	AddCubeVertex( A.x, A.y, A.z, 1, 1, vertdata );
	AddCubeVertex( D.x, D.y, D.z, 1, 0, vertdata );
	AddCubeVertex( D.x, D.y, D.z, 1, 0, vertdata );
	AddCubeVertex( C.x, C.y, C.z, 0, 0, vertdata );
	AddCubeVertex( B.x, B.y, B.z, 0, 1, vertdata );
					
	AddCubeVertex( H.x, H.y, H.z, 0, 1, vertdata ); //Top
	AddCubeVertex( G.x, G.y, G.z, 1, 1, vertdata );
	AddCubeVertex( C.x, C.y, C.z, 1, 0, vertdata );
	AddCubeVertex( C.x, C.y, C.z, 1, 0, vertdata );
	AddCubeVertex( D.x, D.y, D.z, 0, 0, vertdata );
	AddCubeVertex( H.x, H.y, H.z, 0, 1, vertdata );
				
	AddCubeVertex( A.x, A.y, A.z, 0, 1, vertdata ); //Bottom
	AddCubeVertex( B.x, B.y, B.z, 1, 1, vertdata );
	AddCubeVertex( F.x, F.y, F.z, 1, 0, vertdata );
	AddCubeVertex( F.x, F.y, F.z, 1, 0, vertdata );
	AddCubeVertex( E.x, E.y, E.z, 0, 0, vertdata );
	AddCubeVertex( A.x, A.y, A.z, 0, 1, vertdata );
					
	AddCubeVertex( A.x, A.y, A.z, 0, 1, vertdata ); //Left
	AddCubeVertex( E.x, E.y, E.z, 1, 1, vertdata );
	AddCubeVertex( H.x, H.y, H.z, 1, 0, vertdata );
	AddCubeVertex( H.x, H.y, H.z, 1, 0, vertdata );
	AddCubeVertex( D.x, D.y, D.z, 0, 0, vertdata );
	AddCubeVertex( A.x, A.y, A.z, 0, 1, vertdata );

	AddCubeVertex( F.x, F.y, F.z, 0, 1, vertdata ); //Right
	AddCubeVertex( B.x, B.y, B.z, 1, 1, vertdata );
	AddCubeVertex( C.x, C.y, C.z, 1, 0, vertdata );
	AddCubeVertex( C.x, C.y, C.z, 1, 0, vertdata );
	AddCubeVertex( G.x, G.y, G.z, 0, 0, vertdata );
	AddCubeVertex( F.x, F.y, F.z, 0, 1, vertdata );
}

//-----------------------------------------------------------------------------
// Purpose: Update the vertex data for the controllers as X/Y/Z lines
//-----------------------------------------------------------------------------
void CMainApplication::UpdateControllerAxes()
{
	// don't draw controllers if somebody else has input focus
	if( m_pHMD->IsInputFocusCapturedByAnotherProcess() )
		return;

	std::vector<float> vertdataarray;

	m_uiControllerVertcount = 0;
	m_iTrackedControllerCount = 0;

	for ( vr::TrackedDeviceIndex_t unTrackedDevice = vr::k_unTrackedDeviceIndex_Hmd + 1; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; ++unTrackedDevice )
	{
		if ( !m_pHMD->IsTrackedDeviceConnected( unTrackedDevice ) )
			continue;

		if( m_pHMD->GetTrackedDeviceClass( unTrackedDevice ) != vr::TrackedDeviceClass_Controller )
			continue;

		m_iTrackedControllerCount += 1;

		if( !m_rTrackedDevicePose[ unTrackedDevice ].bPoseIsValid )
			continue;

		const Matrix4 & mat = m_rmat4DevicePose[unTrackedDevice];

		Vector4 center = mat * Vector4( 0, 0, 0, 1 );

		for ( int i = 0; i < 3; ++i )
		{
			Vector3 color( 0, 0, 0 );
			Vector4 point( 0, 0, 0, 1 );
			point[i] += 0.05f;  // offset in X, Y, Z
			color[i] = 1.0;  // R, G, B
			point = mat * point;
			vertdataarray.push_back( center.x );
			vertdataarray.push_back( center.y );
			vertdataarray.push_back( center.z );

			vertdataarray.push_back( color.x );
			vertdataarray.push_back( color.y );
			vertdataarray.push_back( color.z );
		
			vertdataarray.push_back( point.x );
			vertdataarray.push_back( point.y );
			vertdataarray.push_back( point.z );
		
			vertdataarray.push_back( color.x );
			vertdataarray.push_back( color.y );
			vertdataarray.push_back( color.z );
		
			m_uiControllerVertcount += 2;
		}

		Vector4 start = mat * Vector4( 0, 0, -0.02f, 1 );
		Vector4 end = mat * Vector4( 0, 0, -39.f, 1 );
		Vector3 color( .92f, .92f, .71f );

		vertdataarray.push_back( start.x );vertdataarray.push_back( start.y );vertdataarray.push_back( start.z );
		vertdataarray.push_back( color.x );vertdataarray.push_back( color.y );vertdataarray.push_back( color.z );

		vertdataarray.push_back( end.x );vertdataarray.push_back( end.y );vertdataarray.push_back( end.z );
		vertdataarray.push_back( color.x );vertdataarray.push_back( color.y );vertdataarray.push_back( color.z );
		m_uiControllerVertcount += 2;
	}

	// Setup the VB the first time through.
	if ( m_pControllerAxesVertexBuffer == VK_NULL_HANDLE && vertdataarray.size() > 0 )
	{
		// Make big enough to hold up to the max number
		VkDeviceSize nSize = sizeof( float ) * vertdataarray.size();
		nSize *= vr::k_unMaxTrackedDeviceCount;

		if ( !CreateVulkanBuffer( m_pDevice, m_physicalDeviceMemoryProperties, nullptr, nSize,
								  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &m_pControllerAxesVertexBuffer, &m_pControllerAxesVertexBufferMemory ) )
		{
			return;
		}
	}

	// Update the VB data
	if ( m_pControllerAxesVertexBuffer != VK_NULL_HANDLE && vertdataarray.size() > 0 )
	{
		void *pData;
		VkResult nResult = vkMapMemory( m_pDevice, m_pControllerAxesVertexBufferMemory, 0, VK_WHOLE_SIZE, 0, &pData );
		if ( nResult != VK_SUCCESS )
		{
			dprintf( "vkMapMemory returned error %d\n", nResult );
			return;
		}
		memcpy( pData, &vertdataarray[ 0 ], vertdataarray.size() * sizeof( float ) );
		vkUnmapMemory( m_pDevice, m_pControllerAxesVertexBufferMemory );

		VkMappedMemoryRange memoryRange = { VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE };
		memoryRange.memory = m_pControllerAxesVertexBufferMemory;
		memoryRange.size = VK_WHOLE_SIZE;
		vkFlushMappedMemoryRanges( m_pDevice, 1, &memoryRange );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::SetupCameras()
{
	m_mat4ProjectionLeft = GetHMDMatrixProjectionEye( vr::Eye_Left );
	m_mat4ProjectionRight = GetHMDMatrixProjectionEye( vr::Eye_Right );
	m_mat4eyePosLeft = GetHMDMatrixPoseEye( vr::Eye_Left );
	m_mat4eyePosRight = GetHMDMatrixPoseEye( vr::Eye_Right );
}

//-----------------------------------------------------------------------------
// Purpose: Creates a frame buffer. Returns true if the buffer was set up.
//          Returns false if the setup failed.
//-----------------------------------------------------------------------------
bool CMainApplication::CreateFrameBuffer( int nWidth, int nHeight, FramebufferDesc &framebufferDesc )
{
	//---------------------------//
	//    Create color target    //
	//---------------------------//
	VkImageCreateInfo imageCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.extent.width = nWidth;
	imageCreateInfo.extent.height = nHeight;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.samples = ( VkSampleCountFlagBits ) m_nMSAASampleCount;
	imageCreateInfo.usage = ( VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT );
	imageCreateInfo.flags = 0;

	VkResult nResult;
	nResult = vkCreateImage( m_pDevice, &imageCreateInfo, nullptr, &framebufferDesc.m_pImage );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateImage failed for eye image with error %d\n", nResult );
		return false;
	}
	VkMemoryRequirements memoryRequirements = {};
	vkGetImageMemoryRequirements( m_pDevice, framebufferDesc.m_pImage, &memoryRequirements );

	VkMemoryAllocateInfo memoryAllocateInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	if ( !MemoryTypeFromProperties( m_physicalDeviceMemoryProperties, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memoryAllocateInfo.memoryTypeIndex ) )
	{
		dprintf( "Failed to find memory type matching requirements.\n" );
		return false;
	}

	nResult = vkAllocateMemory( m_pDevice, &memoryAllocateInfo, nullptr, &framebufferDesc.m_pDeviceMemory );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "Failed to find memory for image.\n" );
		return false;
	}

	nResult = vkBindImageMemory( m_pDevice, framebufferDesc.m_pImage, framebufferDesc.m_pDeviceMemory, 0 );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "Failed to bind memory for image.\n" );
		return false;
	}

	VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = framebufferDesc.m_pImage;
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = imageCreateInfo.format;
	imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;
	nResult = vkCreateImageView( m_pDevice, &imageViewCreateInfo, nullptr, &framebufferDesc.m_pImageView );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateImageView failed with error %d\n", nResult );
		return false;
	}

	//-----------------------------------//
	//    Create depth/stencil target    //
	//-----------------------------------//
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = VK_FORMAT_D32_SFLOAT;
	imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	nResult = vkCreateImage( m_pDevice, &imageCreateInfo, nullptr, &framebufferDesc.m_pDepthStencilImage );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateImage failed for eye depth buffer with error %d\n", nResult );
		return false;
	}
	vkGetImageMemoryRequirements( m_pDevice, framebufferDesc.m_pDepthStencilImage, &memoryRequirements );

	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	if ( !MemoryTypeFromProperties( m_physicalDeviceMemoryProperties, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memoryAllocateInfo.memoryTypeIndex ) )
	{
		dprintf( "Failed to find memory type matching requirements.\n" );
		return false;
	}

	nResult = vkAllocateMemory( m_pDevice, &memoryAllocateInfo, nullptr, &framebufferDesc.m_pDepthStencilDeviceMemory );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "Failed to find memory for image.\n" );
		return false;
	}

	nResult = vkBindImageMemory( m_pDevice, framebufferDesc.m_pDepthStencilImage, framebufferDesc.m_pDepthStencilDeviceMemory, 0 );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "Failed to bind memory for image.\n" );
		return false;
	}
	
	imageViewCreateInfo.image = framebufferDesc.m_pDepthStencilImage;
	imageViewCreateInfo.format = imageCreateInfo.format;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	nResult = vkCreateImageView( m_pDevice, &imageViewCreateInfo, nullptr, &framebufferDesc.m_pDepthStencilImageView );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateImageView failed with error %d\n", nResult );
		return false;
	}

	// Create a renderpass
	uint32_t nTotalAttachments = 2;
	VkAttachmentDescription attachmentDescs[ 2 ];
	VkAttachmentReference attachmentReferences[ 2 ];
	attachmentReferences[ 0 ].attachment = 0;
	attachmentReferences[ 0 ].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentReferences[ 1 ].attachment = 1;
	attachmentReferences[ 1 ].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	attachmentDescs[ 0 ].format = VK_FORMAT_R8G8B8A8_SRGB;
	attachmentDescs[ 0 ].samples = imageCreateInfo.samples;
	attachmentDescs[ 0 ].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescs[ 0 ].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescs[ 0 ].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescs[ 0 ].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescs[ 0 ].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentDescs[ 0 ].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentDescs[ 0 ].flags = 0;

	attachmentDescs[ 1 ].format = VK_FORMAT_D32_SFLOAT;
	attachmentDescs[ 1 ].samples = imageCreateInfo.samples;
	attachmentDescs[ 1 ].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescs[ 1 ].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescs[ 1 ].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescs[ 1 ].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescs[ 1 ].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachmentDescs[ 1 ].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachmentDescs[ 1 ].flags = 0;

	VkSubpassDescription subPassCreateInfo = { };
	subPassCreateInfo.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subPassCreateInfo.flags = 0;
	subPassCreateInfo.inputAttachmentCount = 0;
	subPassCreateInfo.pInputAttachments = NULL;
	subPassCreateInfo.colorAttachmentCount = 1;
	subPassCreateInfo.pColorAttachments = &attachmentReferences[ 0 ];
	subPassCreateInfo.pResolveAttachments = NULL;
	subPassCreateInfo.pDepthStencilAttachment = &attachmentReferences[ 1 ];
	subPassCreateInfo.preserveAttachmentCount = 0;
	subPassCreateInfo.pPreserveAttachments = NULL;

	VkRenderPassCreateInfo renderPassCreateInfo = { };
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.attachmentCount = 2;
	renderPassCreateInfo.pAttachments = &attachmentDescs[ 0 ];
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subPassCreateInfo;
	renderPassCreateInfo.dependencyCount = 0;
	renderPassCreateInfo.pDependencies = NULL;

	nResult = vkCreateRenderPass( m_pDevice, &renderPassCreateInfo, NULL, &framebufferDesc.m_pRenderPass );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateRenderPass failed with error %d.\n", nResult );
		return false;
	}

	// Create the framebuffer
	VkImageView attachments[ 2 ] = { framebufferDesc.m_pImageView, framebufferDesc.m_pDepthStencilImageView };
	VkFramebufferCreateInfo framebufferCreateInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
	framebufferCreateInfo.renderPass = framebufferDesc.m_pRenderPass;
	framebufferCreateInfo.attachmentCount = 2;
	framebufferCreateInfo.pAttachments = &attachments[ 0 ];
	framebufferCreateInfo.width = nWidth;
	framebufferCreateInfo.height = nHeight;
	framebufferCreateInfo.layers = 1;
	nResult = vkCreateFramebuffer( m_pDevice, &framebufferCreateInfo, NULL, &framebufferDesc.m_pFramebuffer );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "vkCreateFramebuffer failed with error %d.\n", nResult );
		return false;
	}

	framebufferDesc.m_nImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	framebufferDesc.m_nDepthStencilImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Get an available command buffer or create a new one if 
// none available.  Associate a fence with the command buffer.
//-----------------------------------------------------------------------------
CMainApplication::VulkanCommandBuffer_t CMainApplication::GetCommandBuffer()
{
	VulkanCommandBuffer_t commandBuffer;
	if ( m_commandBuffers.size() > 0 )
	{
		// If the fence associated with the command buffer has finished, reset it and return it
		if ( vkGetFenceStatus( m_pDevice, m_commandBuffers.back().m_pFence ) == VK_SUCCESS )
		{
			VulkanCommandBuffer_t *pCmdBuffer = &m_commandBuffers.back();
			commandBuffer.m_pCommandBuffer = pCmdBuffer->m_pCommandBuffer;
			commandBuffer.m_pFence = pCmdBuffer->m_pFence;

			vkResetCommandBuffer( commandBuffer.m_pCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT );
			vkResetFences( m_pDevice, 1, &commandBuffer.m_pFence );
			m_commandBuffers.pop_back();
			return commandBuffer;
		}
	}

	// Create a new command buffer and associated fence
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	commandBufferAllocateInfo.commandBufferCount = 1;
	commandBufferAllocateInfo.commandPool = m_pCommandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	vkAllocateCommandBuffers( m_pDevice, &commandBufferAllocateInfo, &commandBuffer.m_pCommandBuffer );

	VkFenceCreateInfo fenceCreateInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
	vkCreateFence( m_pDevice, &fenceCreateInfo, nullptr, &commandBuffer.m_pFence );
	return commandBuffer;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CMainApplication::SetupStereoRenderTargets()
{
	if ( !m_pHMD )
		return false;

	m_pHMD->GetRecommendedRenderTargetSize( &m_nRenderWidth, &m_nRenderHeight );
	m_nRenderWidth = ( uint32_t )( m_flSuperSampleScale * ( float ) m_nRenderWidth );
	m_nRenderHeight = ( uint32_t )( m_flSuperSampleScale * ( float ) m_nRenderHeight );

	CreateFrameBuffer( m_nRenderWidth, m_nRenderHeight, m_leftEyeDesc );
	CreateFrameBuffer( m_nRenderWidth, m_nRenderHeight, m_rightEyeDesc );
	return true;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::SetupCompanionWindow()
{
	if ( !m_pHMD )
		return;

	std::vector<VertexDataWindow> vVerts;

	// left eye verts
	vVerts.push_back( VertexDataWindow( Vector2(-1, -1), Vector2(0, 1)) );
	vVerts.push_back( VertexDataWindow( Vector2(0, -1), Vector2(1, 1)) );
	vVerts.push_back( VertexDataWindow( Vector2(-1, 1), Vector2(0, 0)) );
	vVerts.push_back( VertexDataWindow( Vector2(0, 1), Vector2(1, 0)) );

	// right eye verts
	vVerts.push_back( VertexDataWindow( Vector2(0, -1), Vector2(0, 1)) );
	vVerts.push_back( VertexDataWindow( Vector2(1, -1), Vector2(1, 1)) );
	vVerts.push_back( VertexDataWindow( Vector2(0, 1), Vector2(0, 0)) );
	vVerts.push_back( VertexDataWindow( Vector2(1, 1), Vector2(1, 0)) );

	// Create the vertex buffer and fill with data
	if ( !CreateVulkanBuffer( m_pDevice, m_physicalDeviceMemoryProperties, &vVerts[ 0 ], sizeof( VertexDataWindow ) * vVerts.size(),
							  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &m_pCompanionWindowVertexBuffer, &m_pCompanionWindowVertexBufferMemory ) )
	{
		return;
	}

	// Create index buffer
	uint16_t vIndices[] = { 0, 1, 3,   0, 3, 2,   4, 5, 7,   4, 7, 6};
	m_uiCompanionWindowIndexSize = _countof( vIndices );
	if ( !CreateVulkanBuffer( m_pDevice, m_physicalDeviceMemoryProperties, &vIndices[ 0 ], sizeof( vIndices ),
							  VK_BUFFER_USAGE_INDEX_BUFFER_BIT, &m_pCompanionWindowIndexBuffer, &m_pCompanionWindowIndexBufferMemory ) )
	{
		return;
	}

	// Transition all of the swapchain images to PRESENT_SRC so they are ready for presentation
	for ( size_t nSwapchainImage = 0; nSwapchainImage < m_swapchainImages.size(); nSwapchainImage++ )
	{
		VkImageMemoryBarrier imageMemoryBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
		imageMemoryBarrier.srcAccessMask = 0;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		imageMemoryBarrier.image = m_swapchainImages[ nSwapchainImage ];
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
		imageMemoryBarrier.subresourceRange.levelCount = 1;
		imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
		imageMemoryBarrier.subresourceRange.layerCount = 1;
		imageMemoryBarrier.srcQueueFamilyIndex = m_nQueueFamilyIndex;
		imageMemoryBarrier.dstQueueFamilyIndex = m_nQueueFamilyIndex;
		vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RenderStereoTargets()
{

	// Set viewport and scissor
	VkViewport viewport = { 0.0f, 0.0f, (float ) m_nRenderWidth, ( float ) m_nRenderHeight, 0.0f, 1.0f };
	vkCmdSetViewport( m_currentCommandBuffer.m_pCommandBuffer, 0, 1, &viewport );
	VkRect2D scissor = { 0, 0, m_nRenderWidth, m_nRenderHeight };
	vkCmdSetScissor( m_currentCommandBuffer.m_pCommandBuffer, 0, 1, &scissor );

	//----------//
	// Left Eye //
	//----------//
	// Transition eye image to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	VkImageMemoryBarrier imageMemoryBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_TRANSFER_READ_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.oldLayout = m_leftEyeDesc.m_nImageLayout;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	imageMemoryBarrier.image = m_leftEyeDesc.m_pImage;
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = 1;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	imageMemoryBarrier.subresourceRange.layerCount = 1;
	imageMemoryBarrier.srcQueueFamilyIndex = m_nQueueFamilyIndex;
	imageMemoryBarrier.dstQueueFamilyIndex = m_nQueueFamilyIndex;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
	m_leftEyeDesc.m_nImageLayout = imageMemoryBarrier.newLayout;

	// Transition the depth buffer to VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL on first use
	if ( m_leftEyeDesc.m_nDepthStencilImageLayout == VK_IMAGE_LAYOUT_UNDEFINED )
	{
		imageMemoryBarrier.image = m_leftEyeDesc.m_pDepthStencilImage;
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		imageMemoryBarrier.srcAccessMask = 0;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		imageMemoryBarrier.oldLayout = m_leftEyeDesc.m_nDepthStencilImageLayout;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
		m_leftEyeDesc.m_nDepthStencilImageLayout = imageMemoryBarrier.newLayout;
	}

	// Start the renderpass
	VkRenderPassBeginInfo renderPassBeginInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
	renderPassBeginInfo.renderPass = m_leftEyeDesc.m_pRenderPass;
	renderPassBeginInfo.framebuffer = m_leftEyeDesc.m_pFramebuffer;
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = m_nRenderWidth;
	renderPassBeginInfo.renderArea.extent.height = m_nRenderHeight;
	renderPassBeginInfo.clearValueCount = 2;
	VkClearValue clearValues[ 2 ];
	clearValues[ 0 ].color.float32[ 0 ] = 0.0f;
	clearValues[ 0 ].color.float32[ 1 ] = 0.0f;
	clearValues[ 0 ].color.float32[ 2 ] = 0.0f;
	clearValues[ 0 ].color.float32[ 3 ] = 1.0f;
	clearValues[ 1 ].depthStencil.depth = 1.0f;
	clearValues[ 1 ].depthStencil.stencil = 0;
	renderPassBeginInfo.pClearValues = &clearValues[ 0 ];
	vkCmdBeginRenderPass( m_currentCommandBuffer.m_pCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE );

	RenderScene( vr::Eye_Left );

	vkCmdEndRenderPass( m_currentCommandBuffer.m_pCommandBuffer );
	
	// Transition eye image to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL for display on the companion window
	imageMemoryBarrier.image = m_leftEyeDesc.m_pImage;
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	imageMemoryBarrier.oldLayout = m_leftEyeDesc.m_nImageLayout;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
	m_leftEyeDesc.m_nImageLayout = imageMemoryBarrier.newLayout;

	//-----------//
	// Right Eye //
	//-----------//
	// Transition to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	imageMemoryBarrier.image = m_rightEyeDesc.m_pImage;
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_TRANSFER_READ_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.oldLayout = m_rightEyeDesc.m_nImageLayout;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
	m_rightEyeDesc.m_nImageLayout = imageMemoryBarrier.newLayout;

	// Transition the depth buffer to VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL on first use
	if ( m_rightEyeDesc.m_nDepthStencilImageLayout == VK_IMAGE_LAYOUT_UNDEFINED )
	{
		imageMemoryBarrier.image = m_rightEyeDesc.m_pDepthStencilImage;
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		imageMemoryBarrier.srcAccessMask = 0;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		imageMemoryBarrier.oldLayout = m_rightEyeDesc.m_nDepthStencilImageLayout;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
		m_rightEyeDesc.m_nDepthStencilImageLayout = imageMemoryBarrier.newLayout;
	}

	// Start the renderpass
	renderPassBeginInfo.renderPass = m_rightEyeDesc.m_pRenderPass;
	renderPassBeginInfo.framebuffer = m_rightEyeDesc.m_pFramebuffer;
	renderPassBeginInfo.pClearValues = &clearValues[ 0 ];
	vkCmdBeginRenderPass( m_currentCommandBuffer.m_pCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE );

	RenderScene( vr::Eye_Right );

	vkCmdEndRenderPass( m_currentCommandBuffer.m_pCommandBuffer );
	
	// Transition eye image to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL for display on the companion window
	imageMemoryBarrier.image = m_rightEyeDesc.m_pImage;
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	imageMemoryBarrier.oldLayout = m_rightEyeDesc.m_nImageLayout;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
	m_rightEyeDesc.m_nImageLayout = imageMemoryBarrier.newLayout;
}

//-----------------------------------------------------------------------------
// Purpose: Renders a scene with respect to nEye.
//-----------------------------------------------------------------------------
void CMainApplication::RenderScene( vr::Hmd_Eye nEye )
{
	if( m_bShowCubes )
	{
		vkCmdBindPipeline( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipelines[ PSO_SCENE ] );
		
		// Update the persistently mapped pointer to the CB data with the latest matrix
		memcpy( m_pSceneConstantBufferData[ nEye ], GetCurrentViewProjectionMatrix( nEye ).get(), sizeof( Matrix4 ) );

		vkCmdBindDescriptorSets( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipelineLayout, 0, 1, &m_pDescriptorSets[ DESCRIPTOR_SET_LEFT_EYE_SCENE + nEye ], 0, nullptr );

		// Draw
		VkDeviceSize nOffsets[ 1 ] = { 0 };
		vkCmdBindVertexBuffers( m_currentCommandBuffer.m_pCommandBuffer, 0, 1, &m_pSceneVertexBuffer, &nOffsets[ 0 ] );
		vkCmdDraw( m_currentCommandBuffer.m_pCommandBuffer, m_uiVertcount, 1, 0, 0 );
	}

	bool bIsInputCapturedByAnotherProcess = m_pHMD->IsInputFocusCapturedByAnotherProcess();
	if( !bIsInputCapturedByAnotherProcess && m_pControllerAxesVertexBuffer != VK_NULL_HANDLE )
	{
		// draw the controller axis lines
		vkCmdBindPipeline( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipelines[ PSO_AXES ] );

		VkDeviceSize nOffsets[ 1 ] = { 0 };
		vkCmdBindVertexBuffers( m_currentCommandBuffer.m_pCommandBuffer, 0, 1, &m_pControllerAxesVertexBuffer, &nOffsets[ 0 ] );
		vkCmdDraw( m_currentCommandBuffer.m_pCommandBuffer, m_uiControllerVertcount, 1, 0, 0 );
	}

	// ----- Render Model rendering -----
	vkCmdBindPipeline( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipelines[ PSO_RENDERMODEL ] );
	for( uint32_t unTrackedDevice = 0; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++ )
	{
		if( !m_rTrackedDeviceToRenderModel[ unTrackedDevice ] || !m_rbShowTrackedDevice[ unTrackedDevice ] )
			continue;

		const vr::TrackedDevicePose_t & pose = m_rTrackedDevicePose[ unTrackedDevice ];
		if( !pose.bPoseIsValid )
			continue;

		if( bIsInputCapturedByAnotherProcess && m_pHMD->GetTrackedDeviceClass( unTrackedDevice ) == vr::TrackedDeviceClass_Controller )
			continue;

		const Matrix4 & matDeviceToTracking = m_rmat4DevicePose[ unTrackedDevice ];
		Matrix4 matMVP = GetCurrentViewProjectionMatrix( nEye ) * matDeviceToTracking;
		
		m_rTrackedDeviceToRenderModel[ unTrackedDevice ]->Draw( nEye, m_currentCommandBuffer.m_pCommandBuffer, m_pPipelineLayout, matMVP );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RenderCompanionWindow()
{
	// Get the next swapchain image
	VkResult nResult = vkAcquireNextImageKHR( m_pDevice, m_pSwapchain, UINT64_MAX, m_pSwapchainSemaphores[ m_nFrameIndex ], VK_NULL_HANDLE, &m_nCurrentSwapchainImage );
	if ( nResult != VK_SUCCESS )
	{
		dprintf( "Skipping companion window rendering, vkAcquireNextImageKHR returned %d\n", nResult );
		return;
	}

	// Transition the swapchain image to COLOR_ATTACHMENT_OPTIMAL for rendering
	VkImageMemoryBarrier imageMemoryBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	imageMemoryBarrier.image = m_swapchainImages[ m_nCurrentSwapchainImage ];
	imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
	imageMemoryBarrier.subresourceRange.levelCount = 1;
	imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
	imageMemoryBarrier.subresourceRange.layerCount = 1;
	imageMemoryBarrier.srcQueueFamilyIndex = m_nQueueFamilyIndex;
	imageMemoryBarrier.dstQueueFamilyIndex = m_nQueueFamilyIndex;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );

	// Start the renderpass
	VkRenderPassBeginInfo renderPassBeginInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
	renderPassBeginInfo.renderPass = m_pSwapchainRenderPass;
	renderPassBeginInfo.framebuffer = m_pSwapchainFramebuffers[ m_nCurrentSwapchainImage ];
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = m_nCompanionWindowWidth;
	renderPassBeginInfo.renderArea.extent.height = m_nCompanionWindowHeight;
	VkClearValue clearValues[ 1 ];
	clearValues[ 0 ].color.float32[ 0 ] = 0.0f;
	clearValues[ 0 ].color.float32[ 1 ] = 0.0f;
	clearValues[ 0 ].color.float32[ 2 ] = 0.0f;
	clearValues[ 0 ].color.float32[ 3 ] = 1.0f;
	renderPassBeginInfo.clearValueCount = _countof( clearValues );
	renderPassBeginInfo.pClearValues = &clearValues[ 0 ];
	vkCmdBeginRenderPass( m_currentCommandBuffer.m_pCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE );

	// Set viewport/scissor
	VkViewport viewport = { 0.0f, 0.0f, (float ) m_nCompanionWindowWidth, ( float ) m_nCompanionWindowHeight, 0.0f, 1.0f };
	vkCmdSetViewport( m_currentCommandBuffer.m_pCommandBuffer, 0, 1, &viewport );
	VkRect2D scissor = { 0, 0, m_nCompanionWindowWidth, m_nCompanionWindowHeight };
	vkCmdSetScissor( m_currentCommandBuffer.m_pCommandBuffer, 0, 1, &scissor );

	// Bind the pipeline and descriptor set
	vkCmdBindPipeline( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipelines[ PSO_COMPANION ] );
	vkCmdBindDescriptorSets( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipelineLayout, 0, 1, &m_pDescriptorSets[ DESCRIPTOR_SET_COMPANION_LEFT_TEXTURE ], 0, nullptr );

	// Draw left eye texture to companion window
	VkDeviceSize nOffsets[ 1 ] = { 0 };
	vkCmdBindVertexBuffers( m_currentCommandBuffer.m_pCommandBuffer, 0, 1, &m_pCompanionWindowVertexBuffer, &nOffsets[ 0 ] );
	vkCmdBindIndexBuffer( m_currentCommandBuffer.m_pCommandBuffer, m_pCompanionWindowIndexBuffer, 0, VK_INDEX_TYPE_UINT16 );
	vkCmdDrawIndexed( m_currentCommandBuffer.m_pCommandBuffer, m_uiCompanionWindowIndexSize / 2, 1, 0, 0, 0 );

	// Draw right eye texture to companion window
	vkCmdBindDescriptorSets( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipelineLayout, 0, 1, &m_pDescriptorSets[ DESCRIPTOR_SET_COMPANION_RIGHT_TEXTURE ], 0, nullptr );
	vkCmdDrawIndexed( m_currentCommandBuffer.m_pCommandBuffer, m_uiCompanionWindowIndexSize / 2, 1, ( m_uiCompanionWindowIndexSize / 2 ), 0, 0 );

	// End the renderpass
	vkCmdEndRenderPass( m_currentCommandBuffer.m_pCommandBuffer );

	// Transition the swapchain image to PRESENT_SRC for presentation
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );

	// Transition both of the eye textures to VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL for SteamVR which requires this layout for submit
	imageMemoryBarrier.image = m_leftEyeDesc.m_pImage;
	imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	imageMemoryBarrier.oldLayout = m_leftEyeDesc.m_nImageLayout;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
	m_leftEyeDesc.m_nImageLayout = imageMemoryBarrier.newLayout;

	imageMemoryBarrier.image = m_rightEyeDesc.m_pImage;
	vkCmdPipelineBarrier( m_currentCommandBuffer.m_pCommandBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );
	m_rightEyeDesc.m_nImageLayout = imageMemoryBarrier.newLayout;
}

//-----------------------------------------------------------------------------
// Purpose: Gets a Matrix Projection Eye with respect to nEye.
//-----------------------------------------------------------------------------
Matrix4 CMainApplication::GetHMDMatrixProjectionEye( vr::Hmd_Eye nEye )
{
	if ( !m_pHMD )
		return Matrix4();

	vr::HmdMatrix44_t mat = m_pHMD->GetProjectionMatrix( nEye, m_fNearClip, m_fFarClip );

	return Matrix4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1], 
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2], 
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}


//-----------------------------------------------------------------------------
// Purpose: Gets an HMDMatrixPoseEye with respect to nEye.
//-----------------------------------------------------------------------------
Matrix4 CMainApplication::GetHMDMatrixPoseEye( vr::Hmd_Eye nEye )
{
	if ( !m_pHMD )
		return Matrix4();

	vr::HmdMatrix34_t matEyeRight = m_pHMD->GetEyeToHeadTransform( nEye );
	Matrix4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0, 
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
		);

	return matrixObj.invert();
}

//-----------------------------------------------------------------------------
// Purpose: Gets a Current View Projection Matrix with respect to nEye,
//          which may be an Eye_Left or an Eye_Right.
//-----------------------------------------------------------------------------
Matrix4 CMainApplication::GetCurrentViewProjectionMatrix( vr::Hmd_Eye nEye )
{
	Matrix4 matMVP;
	if( nEye == vr::Eye_Left )
	{
		matMVP = m_mat4ProjectionLeft * m_mat4eyePosLeft * m_mat4HMDPose;
	}
	else if( nEye == vr::Eye_Right )
	{
		matMVP = m_mat4ProjectionRight * m_mat4eyePosRight *  m_mat4HMDPose;
	}

	return matMVP;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::UpdateHMDMatrixPose()
{
	if ( !m_pHMD )
		return;

	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0 );

	m_iValidPoseCount = 0;
	m_strPoseClasses = "";
	for ( int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice )
	{
		if ( m_rTrackedDevicePose[nDevice].bPoseIsValid )
		{
			m_iValidPoseCount++;
			m_rmat4DevicePose[nDevice] = ConvertSteamVRMatrixToMatrix4( m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking );
			if (m_rDevClassChar[nDevice]==0)
			{
				switch (m_pHMD->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
				case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
				default:                                       m_rDevClassChar[nDevice] = '?'; break;
				}
			}
			m_strPoseClasses += m_rDevClassChar[nDevice];
		}
	}

	if ( m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid )
	{
		m_mat4HMDPose = m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		m_mat4HMDPose.invert();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Finds a render model we've already loaded or loads a new one
//-----------------------------------------------------------------------------
VulkanRenderModel *CMainApplication::FindOrLoadRenderModel( vr::TrackedDeviceIndex_t unTrackedDeviceIndex, const char *pchRenderModelName )
{
	VulkanRenderModel *pRenderModel = NULL;
	// To simplify the Vulkan rendering code, create an instance of the model for each model name.  This is less efficient
	// memory wise, but simplifies the rendering code so we can store the transform in a constant buffer associated with
	// the model itself.  You would not want to do this in a production application.
	//for( std::vector< VulkanRenderModel * >::iterator i = m_vecRenderModels.begin(); i != m_vecRenderModels.end(); i++ )
	//{
		//if( !stricmp( (*i)->GetName().c_str(), pchRenderModelName ) )
		//{
			//pRenderModel = *i;
			//break;
		//}
	//}

	// load the model if we didn't find one
	if( !pRenderModel )
	{
		vr::RenderModel_t *pModel;
		vr::EVRRenderModelError error;
		while ( 1 )
		{
			error = vr::VRRenderModels()->LoadRenderModel_Async( pchRenderModelName, &pModel );
			if ( error != vr::VRRenderModelError_Loading )
				break;

			ThreadSleep( 1 );
		}

		if ( error != vr::VRRenderModelError_None )
		{
			dprintf( "Unable to load render model %s - %s\n", pchRenderModelName, vr::VRRenderModels()->GetRenderModelErrorNameFromEnum( error ) );
			return NULL; // move on to the next tracked device
		}

		vr::RenderModel_TextureMap_t *pTexture;
		while ( 1 )
		{
			error = vr::VRRenderModels()->LoadTexture_Async( pModel->diffuseTextureId, &pTexture );
			if ( error != vr::VRRenderModelError_Loading )
				break;

			ThreadSleep( 1 );
		}

		if ( error != vr::VRRenderModelError_None )
		{
			dprintf( "Unable to load render texture id:%d for render model %s\n", pModel->diffuseTextureId, pchRenderModelName );
			vr::VRRenderModels()->FreeRenderModel( pModel );
			return NULL; // move on to the next tracked device
		}

		pRenderModel = new VulkanRenderModel( pchRenderModelName );
		VkDescriptorSet pDescriptorSets[ 2 ] =
		{
			m_pDescriptorSets[ DESCRIPTOR_SET_LEFT_EYE_RENDER_MODEL0 + unTrackedDeviceIndex ],
			m_pDescriptorSets[ DESCRIPTOR_SET_RIGHT_EYE_RENDER_MODEL0 + unTrackedDeviceIndex ],
		};
		if ( !pRenderModel->BInit( m_pDevice, m_physicalDeviceMemoryProperties, m_currentCommandBuffer.m_pCommandBuffer, unTrackedDeviceIndex, pDescriptorSets, *pModel, *pTexture ) )
		{
			dprintf( "Unable to create Vulkan model from render model %s\n", pchRenderModelName );
			delete pRenderModel;
			pRenderModel = NULL;
		}
		else
		{
			m_vecRenderModels.push_back( pRenderModel );
		}
		vr::VRRenderModels()->FreeRenderModel( pModel );
		vr::VRRenderModels()->FreeTexture( pTexture );
	}

	return pRenderModel;
}

//-----------------------------------------------------------------------------
// Purpose: Create/destroy Vulkan a Render Model for a single tracked device
//-----------------------------------------------------------------------------
void CMainApplication::SetupRenderModelForTrackedDevice( vr::TrackedDeviceIndex_t unTrackedDeviceIndex )
{
	if( unTrackedDeviceIndex >= vr::k_unMaxTrackedDeviceCount )
		return;

	// try to find a model we've already set up
	std::string sRenderModelName = GetTrackedDeviceString( m_pHMD, unTrackedDeviceIndex, vr::Prop_RenderModelName_String );
	VulkanRenderModel *pRenderModel = FindOrLoadRenderModel( unTrackedDeviceIndex, sRenderModelName.c_str() );
	if( !pRenderModel )
	{
		std::string sTrackingSystemName = GetTrackedDeviceString( m_pHMD, unTrackedDeviceIndex, vr::Prop_TrackingSystemName_String );
		dprintf( "Unable to load render model for tracked device %d (%s.%s)", unTrackedDeviceIndex, sTrackingSystemName.c_str(), sRenderModelName.c_str() );
	}
	else
	{
		m_rTrackedDeviceToRenderModel[ unTrackedDeviceIndex ] = pRenderModel;
		m_rbShowTrackedDevice[ unTrackedDeviceIndex ] = true;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Create/destroy Vulkan Render Models
//-----------------------------------------------------------------------------
void CMainApplication::SetupRenderModels()
{
	memset( m_rTrackedDeviceToRenderModel, 0, sizeof( m_rTrackedDeviceToRenderModel ) );

	if( !m_pHMD )
		return;

	for( uint32_t unTrackedDevice = vr::k_unTrackedDeviceIndex_Hmd + 1; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++ )
	{
		if( !m_pHMD->IsTrackedDeviceConnected( unTrackedDevice ) )
			continue;

		SetupRenderModelForTrackedDevice( unTrackedDevice );
	}

}

//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to our local matrix class
//-----------------------------------------------------------------------------
Matrix4 CMainApplication::ConvertSteamVRMatrixToMatrix4( const vr::HmdMatrix34_t &matPose )
{
	Matrix4 matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
		);
	return matrixObj;
}

//-----------------------------------------------------------------------------
// Purpose: Create/destroy Vulkan Render Models
//-----------------------------------------------------------------------------
VulkanRenderModel::VulkanRenderModel( const std::string & sRenderModelName )
	: m_sModelName( sRenderModelName )
	, m_pDevice( VK_NULL_HANDLE )
	, m_pVertexBuffer( VK_NULL_HANDLE )
	, m_pVertexBufferMemory( VK_NULL_HANDLE )
	, m_pIndexBuffer( VK_NULL_HANDLE )
	, m_pIndexBufferMemory( VK_NULL_HANDLE )
	, m_pImage( VK_NULL_HANDLE )
	, m_pImageMemory( VK_NULL_HANDLE )
	, m_pImageView( VK_NULL_HANDLE )
	, m_pImageStagingBuffer( VK_NULL_HANDLE )
	, m_pImageStagingBufferMemory( VK_NULL_HANDLE )
	, m_pSampler( VK_NULL_HANDLE )
{
	memset( m_pConstantBuffer, 0, sizeof( m_pConstantBuffer ) );
	memset( m_pConstantBufferMemory, 0, sizeof( m_pConstantBufferMemory ) );
	memset( m_pConstantBufferData, 0, sizeof( m_pConstantBufferData ) );
	memset( m_pDescriptorSets, 0, sizeof( m_pDescriptorSets ) );
}

VulkanRenderModel::~VulkanRenderModel()
{
	Cleanup();
}

//-----------------------------------------------------------------------------
// Purpose: Allocates and populates the Vulkan resources for a render model
//-----------------------------------------------------------------------------
bool VulkanRenderModel::BInit( VkDevice pDevice, const VkPhysicalDeviceMemoryProperties &memoryProperties, VkCommandBuffer pCommandBuffer, vr::TrackedDeviceIndex_t unTrackedDeviceIndex, VkDescriptorSet pDescriptorSets[ 2 ], const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture )
{
	m_pDevice = pDevice;
	m_physicalDeviceMemoryProperties = memoryProperties;
	m_unTrackedDeviceIndex = unTrackedDeviceIndex;
	m_pDescriptorSets[ 0 ] = pDescriptorSets[ 0 ];
	m_pDescriptorSets[ 1 ] = pDescriptorSets[ 1 ];

	// Create and populate the vertex buffer
	{
		if ( !CreateVulkanBuffer( m_pDevice, m_physicalDeviceMemoryProperties, vrModel.rVertexData, sizeof( vr::RenderModel_Vertex_t ) * vrModel.unVertexCount, 
								  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &m_pVertexBuffer, &m_pVertexBufferMemory ) )
		{
			return false;
		}
	}

	// Create and populate the index buffer
	{
		if ( !CreateVulkanBuffer( m_pDevice, m_physicalDeviceMemoryProperties, vrModel.rIndexData, sizeof( uint16_t ) * vrModel.unTriangleCount * 3, 
								  VK_BUFFER_USAGE_INDEX_BUFFER_BIT, &m_pIndexBuffer, &m_pIndexBufferMemory ) )
		{
			return false;
		}
	}

	// create and populate the texture
	{
		int nImageWidth = vrDiffuseTexture.unWidth;
		int nImageHeight = vrDiffuseTexture.unHeight;
		
		// Copy the base level to a buffer, reserve space for mips (overreserve by a bit to avoid having to calc mipchain size ahead of time)
		VkDeviceSize nBufferSize = 0;
		uint8_t *pBuffer = new uint8_t[ nImageWidth * nImageHeight * 4 * 2 ];
		uint8_t *pPrevBuffer = pBuffer;
		uint8_t *pCurBuffer = pBuffer;
		memcpy( pCurBuffer, vrDiffuseTexture.rubTextureMapData, sizeof( uint8_t ) * nImageWidth * nImageHeight * 4 );
		pCurBuffer += sizeof( uint8_t ) * nImageWidth * nImageHeight * 4;

		std::vector< VkBufferImageCopy > bufferImageCopies;
		VkBufferImageCopy bufferImageCopy = {};
		bufferImageCopy.bufferOffset = 0;
		bufferImageCopy.bufferRowLength = 0;
		bufferImageCopy.bufferImageHeight = 0;
		bufferImageCopy.imageSubresource.baseArrayLayer = 0;
		bufferImageCopy.imageSubresource.layerCount = 1;
		bufferImageCopy.imageSubresource.mipLevel = 0;
		bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferImageCopy.imageOffset.x = 0;
		bufferImageCopy.imageOffset.y = 0;
		bufferImageCopy.imageOffset.z = 0;
		bufferImageCopy.imageExtent.width = nImageWidth;
		bufferImageCopy.imageExtent.height = nImageHeight;
		bufferImageCopy.imageExtent.depth = 1;
		bufferImageCopies.push_back( bufferImageCopy );

		int nMipWidth = nImageWidth;
		int nMipHeight = nImageHeight;

		while( nMipWidth > 1 && nMipHeight > 1 )
		{
			CMainApplication::GenMipMapRGBA( pPrevBuffer, pCurBuffer, nMipWidth, nMipHeight, &nMipWidth, &nMipHeight );
			bufferImageCopy.bufferOffset = pCurBuffer - pBuffer;
			bufferImageCopy.imageSubresource.mipLevel++;
			bufferImageCopy.imageExtent.width = nMipWidth;
			bufferImageCopy.imageExtent.height = nMipHeight;
			bufferImageCopies.push_back( bufferImageCopy );
			pPrevBuffer = pCurBuffer;
			pCurBuffer += ( nMipWidth * nMipHeight * 4 * sizeof( uint8_t ) );
		}
		nBufferSize = pCurBuffer - pBuffer;

		// Create the image
		VkImageCreateInfo imageCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.extent.width = nImageWidth;
		imageCreateInfo.extent.height = nImageHeight;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = ( uint32_t ) bufferImageCopies.size();
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		imageCreateInfo.flags = 0;
		vkCreateImage( m_pDevice, &imageCreateInfo, nullptr, &m_pImage );

		VkMemoryRequirements memoryRequirements = {};
		vkGetImageMemoryRequirements( m_pDevice, m_pImage, &memoryRequirements );

		VkMemoryAllocateInfo memoryAllocateInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		MemoryTypeFromProperties( m_physicalDeviceMemoryProperties, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memoryAllocateInfo.memoryTypeIndex );
		vkAllocateMemory( m_pDevice, &memoryAllocateInfo, nullptr, &m_pImageMemory );
		vkBindImageMemory( m_pDevice, m_pImage, m_pImageMemory, 0 );

		VkImageViewCreateInfo imageViewCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		imageViewCreateInfo.flags = 0;
		imageViewCreateInfo.image = m_pImage;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = imageCreateInfo.format;
		imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = imageCreateInfo.mipLevels;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		vkCreateImageView( m_pDevice, &imageViewCreateInfo, nullptr, &m_pImageView );

		// Create a staging buffer
		VkBufferCreateInfo bufferCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		bufferCreateInfo.size = nBufferSize;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		vkCreateBuffer( m_pDevice, &bufferCreateInfo, nullptr, &m_pImageStagingBuffer );
		vkGetBufferMemoryRequirements( m_pDevice, m_pImageStagingBuffer, &memoryRequirements );

		VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		MemoryTypeFromProperties( m_physicalDeviceMemoryProperties, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &allocInfo.memoryTypeIndex );
		allocInfo.allocationSize = memoryRequirements.size;

		vkAllocateMemory( m_pDevice, &allocInfo, nullptr, &m_pImageStagingBufferMemory );
		vkBindBufferMemory( m_pDevice, m_pImageStagingBuffer, m_pImageStagingBufferMemory, 0 );
	
		// Copy memory to the staging buffer
		void *pData;
		VkResult nResult = vkMapMemory( m_pDevice, m_pImageStagingBufferMemory, 0, VK_WHOLE_SIZE, 0, &pData );
		if ( nResult != VK_SUCCESS )
		{
			dprintf( "vkMapMemory returned error %d\n", nResult );
			return false;
		}
		memcpy( pData, pBuffer, nBufferSize );
		vkUnmapMemory( m_pDevice, m_pImageStagingBufferMemory );

		VkMappedMemoryRange memoryRange = { VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE };
		memoryRange.memory = m_pImageStagingBufferMemory;
		memoryRange.size = VK_WHOLE_SIZE;
		vkFlushMappedMemoryRanges( m_pDevice, 1, &memoryRange );

		// Transition the image to TRANSFER_DST to receive image
		VkImageMemoryBarrier imageMemoryBarrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
		imageMemoryBarrier.srcAccessMask = 0;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imageMemoryBarrier.image = m_pImage;
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
		imageMemoryBarrier.subresourceRange.levelCount = imageCreateInfo.mipLevels;
		imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
		imageMemoryBarrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier( pCommandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );

		// Issue the copy to fill the image data
		vkCmdCopyBufferToImage( pCommandBuffer, m_pImageStagingBuffer, m_pImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ( uint32_t ) bufferImageCopies.size(), &bufferImageCopies[ 0 ] );

		// Transition the image to SHADER_READ_OPTIMAL for reading
		imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		vkCmdPipelineBarrier( pCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &imageMemoryBarrier );

		// Create a sampler
		VkSamplerCreateInfo samplerCreateInfo = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.anisotropyEnable = VK_TRUE;
		samplerCreateInfo.maxAnisotropy = 16.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = ( float ) imageCreateInfo.mipLevels;
		vkCreateSampler( m_pDevice, &samplerCreateInfo, nullptr, &m_pSampler );
	}

	// Create a constant buffer to hold the transform (one for each eye)
	for ( uint32_t nEye = 0; nEye < 2; nEye++ )
	{
		VkBufferCreateInfo bufferCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		bufferCreateInfo.size = sizeof( Matrix4 );
		bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		vkCreateBuffer( m_pDevice, &bufferCreateInfo, nullptr, &m_pConstantBuffer[ nEye ] );
		
		VkMemoryRequirements memoryRequirements = {};
		vkGetBufferMemoryRequirements( m_pDevice, m_pConstantBuffer[ nEye ], &memoryRequirements );
		VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		MemoryTypeFromProperties( m_physicalDeviceMemoryProperties, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, &allocInfo.memoryTypeIndex );
		allocInfo.allocationSize = memoryRequirements.size;

		vkAllocateMemory( m_pDevice, &allocInfo, nullptr, &m_pConstantBufferMemory[ nEye ] );
		vkBindBufferMemory( m_pDevice, m_pConstantBuffer[ nEye ], m_pConstantBufferMemory[ nEye ], 0 );

		// Map and keep mapped persistently
		vkMapMemory( m_pDevice, m_pConstantBufferMemory[ nEye ], 0, VK_WHOLE_SIZE, 0, &m_pConstantBufferData[ nEye ] );

		// Bake the descriptor set
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = m_pConstantBuffer[ nEye ];
		bufferInfo.offset = 0;
		bufferInfo.range = VK_WHOLE_SIZE;

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageView = m_pImageView;
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		
		VkDescriptorImageInfo samplerInfo = {};
		samplerInfo.sampler = m_pSampler;

		VkWriteDescriptorSet writeDescriptorSets[ 3 ] = { };
		writeDescriptorSets[ 0 ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[ 0 ].dstSet = m_pDescriptorSets[ nEye ];
		writeDescriptorSets[ 0 ].dstBinding = 0;
		writeDescriptorSets[ 0 ].descriptorCount = 1;
		writeDescriptorSets[ 0 ].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptorSets[ 0 ].pBufferInfo = &bufferInfo;
		writeDescriptorSets[ 1 ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[ 1 ].dstSet = m_pDescriptorSets[ nEye ];
		writeDescriptorSets[ 1 ].dstBinding = 1;
		writeDescriptorSets[ 1 ].descriptorCount = 1;
		writeDescriptorSets[ 1 ].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		writeDescriptorSets[ 1 ].pImageInfo = &imageInfo;
		writeDescriptorSets[ 2 ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSets[ 2 ].dstSet = m_pDescriptorSets[ nEye ];
		writeDescriptorSets[ 2 ].dstBinding = 2;
		writeDescriptorSets[ 2 ].descriptorCount = 1;
		writeDescriptorSets[ 2 ].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		writeDescriptorSets[ 2 ].pImageInfo = &samplerInfo;
		
		vkUpdateDescriptorSets( m_pDevice, _countof( writeDescriptorSets ), writeDescriptorSets, 0, nullptr );
	}

	m_unVertexCount = vrModel.unTriangleCount * 3;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Frees the Vulkan resources for a render model
//-----------------------------------------------------------------------------
void VulkanRenderModel::Cleanup()
{
	if ( m_pVertexBuffer != VK_NULL_HANDLE )
	{
		vkDestroyBuffer( m_pDevice, m_pVertexBuffer, nullptr );
		m_pVertexBuffer = VK_NULL_HANDLE;
	}

	if ( m_pVertexBufferMemory != VK_NULL_HANDLE )
	{
		vkFreeMemory( m_pDevice, m_pVertexBufferMemory, nullptr );
		m_pVertexBufferMemory = VK_NULL_HANDLE;
	}

	if ( m_pIndexBuffer != VK_NULL_HANDLE )
	{
		vkDestroyBuffer( m_pDevice, m_pIndexBuffer, nullptr );
		m_pIndexBuffer = VK_NULL_HANDLE;
	}

	if ( m_pIndexBufferMemory != VK_NULL_HANDLE )
	{
		vkFreeMemory( m_pDevice, m_pIndexBufferMemory, nullptr );
		m_pIndexBufferMemory = VK_NULL_HANDLE;
	}
	
	if ( m_pImage != VK_NULL_HANDLE )
	{
		vkDestroyImage( m_pDevice, m_pImage, nullptr );
		m_pImage = VK_NULL_HANDLE;
	}

	if ( m_pImageMemory != VK_NULL_HANDLE )
	{
		vkFreeMemory( m_pDevice, m_pImageMemory, nullptr );
		m_pImageMemory = VK_NULL_HANDLE;
	}
	
	if ( m_pImageView != VK_NULL_HANDLE )
	{
		vkDestroyImageView( m_pDevice, m_pImageView, nullptr );
		m_pImageView = VK_NULL_HANDLE;
	}

	if ( m_pImageStagingBuffer != VK_NULL_HANDLE )
	{
		vkDestroyBuffer( m_pDevice, m_pImageStagingBuffer, nullptr );
		m_pImageStagingBuffer = VK_NULL_HANDLE;
	}

	if ( m_pImageStagingBufferMemory != VK_NULL_HANDLE )
	{
		vkFreeMemory( m_pDevice, m_pImageStagingBufferMemory, nullptr );
		m_pImageStagingBufferMemory = VK_NULL_HANDLE;
	}

	for ( uint32_t nEye = 0; nEye < 2; nEye++ )
	{
		if ( m_pConstantBuffer[ nEye ] != VK_NULL_HANDLE )
		{
			vkDestroyBuffer( m_pDevice, m_pConstantBuffer[ nEye ], nullptr );
			m_pConstantBuffer[ nEye ] = VK_NULL_HANDLE;
		}

		if ( m_pConstantBufferMemory != VK_NULL_HANDLE )
		{
			vkFreeMemory( m_pDevice, m_pConstantBufferMemory[ nEye ], nullptr );
			m_pConstantBufferMemory[ nEye ] = VK_NULL_HANDLE;
		}
	}

	if ( m_pSampler != VK_NULL_HANDLE )
	{
		vkDestroySampler( m_pDevice, m_pSampler, nullptr );
		m_pSampler = VK_NULL_HANDLE;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Draws the render model
//-----------------------------------------------------------------------------
void VulkanRenderModel::Draw( vr::EVREye nEye, VkCommandBuffer pCommandBuffer, VkPipelineLayout pPipelineLayout, const Matrix4 &matMVP )
{
	// Update the CB with the transform
	memcpy( m_pConstantBufferData[ nEye ], &matMVP, sizeof( matMVP ) );

	// Bind the descriptor set
	vkCmdBindDescriptorSets( pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipelineLayout, 0, 1, &m_pDescriptorSets[ nEye ], 0, nullptr );
	
	// Bind the VB/IB and draw
	VkDeviceSize nOffsets[ 1 ] = { 0 };
	vkCmdBindVertexBuffers( pCommandBuffer, 0, 1, &m_pVertexBuffer, &nOffsets[ 0 ] );
	vkCmdBindIndexBuffer( pCommandBuffer, m_pIndexBuffer, 0, VK_INDEX_TYPE_UINT16 );
	vkCmdDrawIndexed( pCommandBuffer, m_unVertexCount, 1, 0, 0, 0 );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	CMainApplication *pMainApplication = new CMainApplication( argc, argv );

	if ( !pMainApplication->BInit() )
	{
		pMainApplication->Shutdown();
		return 1;
	}

	pMainApplication->RunMainLoop();

	pMainApplication->Shutdown();

	return 0;
}
