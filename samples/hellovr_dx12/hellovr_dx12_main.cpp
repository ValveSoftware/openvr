//========= Copyright Valve Corporation ============//

#include <windows.h>
#include "d3dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <D3Dcompiler.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <stdio.h>
#include <string>
#include <cstdlib>

#include <openvr.h>

#include "shared/lodepng.h"
#include "shared/Matrices.h"
#include "shared/pathtools.h"

using Microsoft::WRL::ComPtr;

void ThreadSleep( unsigned long nMilliseconds )
{
	::Sleep( nMilliseconds );
}

// Slots in the RenderTargetView descriptor heap
enum RTVIndex_t
{
	RTV_LEFT_EYE = 0,
	RTV_RIGHT_EYE,
	RTV_SWAPCHAIN0,
	RTV_SWAPCHAIN1,
	NUM_RTVS
};

// Slots in the ConstantBufferView/ShaderResourceView descriptor heap
enum CBVSRVIndex_t
{
	CBV_LEFT_EYE = 0,
	CBV_RIGHT_EYE,
	SRV_LEFT_EYE,
	SRV_RIGHT_EYE,
	SRV_TEXTURE_MAP,
	// Slot for texture in each possible render model
	SRV_TEXTURE_RENDER_MODEL0,
	SRV_TEXTURE_RENDER_MODEL1,
	SRV_TEXTURE_RENDER_MODEL2,
	SRV_TEXTURE_RENDER_MODEL3,
	SRV_TEXTURE_RENDER_MODEL4,
	SRV_TEXTURE_RENDER_MODEL5,
	SRV_TEXTURE_RENDER_MODEL6,
	SRV_TEXTURE_RENDER_MODEL7,
	SRV_TEXTURE_RENDER_MODEL8,
	SRV_TEXTURE_RENDER_MODEL9,
	SRV_TEXTURE_RENDER_MODEL10,
	SRV_TEXTURE_RENDER_MODEL11,
	SRV_TEXTURE_RENDER_MODEL12,
	SRV_TEXTURE_RENDER_MODEL13,
	SRV_TEXTURE_RENDER_MODEL14,
	SRV_TEXTURE_RENDER_MODEL15,
	// Slot for transform in each possible rendermodel
	CBV_LEFT_EYE_RENDER_MODEL0,
	CBV_LEFT_EYE_RENDER_MODEL1,
	CBV_LEFT_EYE_RENDER_MODEL2,
	CBV_LEFT_EYE_RENDER_MODEL3,
	CBV_LEFT_EYE_RENDER_MODEL4,
	CBV_LEFT_EYE_RENDER_MODEL5,
	CBV_LEFT_EYE_RENDER_MODEL6,
	CBV_LEFT_EYE_RENDER_MODEL7,
	CBV_LEFT_EYE_RENDER_MODEL8,
	CBV_LEFT_EYE_RENDER_MODEL9,
	CBV_LEFT_EYE_RENDER_MODEL10,
	CBV_LEFT_EYE_RENDER_MODEL11,
	CBV_LEFT_EYE_RENDER_MODEL12,
	CBV_LEFT_EYE_RENDER_MODEL13,
	CBV_LEFT_EYE_RENDER_MODEL14,
	CBV_LEFT_EYE_RENDER_MODEL15,
	CBV_RIGHT_EYE_RENDER_MODEL0,
	CBV_RIGHT_EYE_RENDER_MODEL1,
	CBV_RIGHT_EYE_RENDER_MODEL2,
	CBV_RIGHT_EYE_RENDER_MODEL3,
	CBV_RIGHT_EYE_RENDER_MODEL4,
	CBV_RIGHT_EYE_RENDER_MODEL5,
	CBV_RIGHT_EYE_RENDER_MODEL6,
	CBV_RIGHT_EYE_RENDER_MODEL7,
	CBV_RIGHT_EYE_RENDER_MODEL8,
	CBV_RIGHT_EYE_RENDER_MODEL9,
	CBV_RIGHT_EYE_RENDER_MODEL10,
	CBV_RIGHT_EYE_RENDER_MODEL11,
	CBV_RIGHT_EYE_RENDER_MODEL12,
	CBV_RIGHT_EYE_RENDER_MODEL13,
	CBV_RIGHT_EYE_RENDER_MODEL14,
	CBV_RIGHT_EYE_RENDER_MODEL15,
	NUM_SRV_CBVS
};


class DX12RenderModel
{
public:
	DX12RenderModel( const std::string & sRenderModelName );
	~DX12RenderModel();

	bool BInit( ID3D12Device *pDevice, ID3D12GraphicsCommandList *pCommandList, ID3D12DescriptorHeap *pCBVSRVHeap, vr::TrackedDeviceIndex_t unTrackedDeviceIndex, const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture );
	void Cleanup();
	void Draw( vr::EVREye nEye, ID3D12GraphicsCommandList *pCommandList, UINT nCBVSRVDescriptorSize, const Matrix4 &matMVP );
	const std::string & GetName() const { return m_sModelName; }

private:
	ComPtr< ID3D12Resource > m_pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	ComPtr< ID3D12Resource > m_pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	ComPtr< ID3D12Resource > m_pTexture;
	ComPtr< ID3D12Resource > m_pTextureUploadHeap;
	ComPtr< ID3D12Resource > m_pConstantBuffer;
	UINT8 *m_pConstantBufferData[ 2 ];
	size_t m_unVertexCount;
	vr::TrackedDeviceIndex_t m_unTrackedDeviceIndex;
	ID3D12DescriptorHeap *m_pCBVSRVHeap;
	std::string m_sModelName;
};

static bool g_bPrintf = true;
static const int g_nFrameCount = 2; // Swapchain depth

//-----------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------------
class CMainApplication
{
public:
	CMainApplication( int argc, char *argv[] );
	virtual ~CMainApplication();

	bool BInit();
	bool BInitD3D12();
	bool BInitCompositor();

	void SetupRenderModels();

	void Shutdown();

	void RunMainLoop();
	bool HandleInput();
	void ProcessVREvent( const vr::VREvent_t & event );
	void RenderFrame();

	bool SetupTexturemaps();
	static void GenMipMapRGBA( const UINT8 *pSrc, UINT8 **ppDst, int nSrcWidth, int nSrcHeight, int *pDstWidthOut, int *pDstHeightOut );

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

	void SetupRenderModelForTrackedDevice( vr::TrackedDeviceIndex_t unTrackedDeviceIndex );
	DX12RenderModel *FindOrLoadRenderModel( vr::TrackedDeviceIndex_t unTrackedDeviceIndex, const char *pchRenderModelName );

private: 
	bool m_bDebugD3D12;
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

	// D3D12 members
	UINT m_nFrameIndex;
	HANDLE m_fenceEvent;
	ComPtr< ID3D12Fence > m_pFence;
	UINT64 m_nFenceValues[ g_nFrameCount ];
	ComPtr< ID3D12Device > m_pDevice;
	ComPtr< IDXGISwapChain3 > m_pSwapChain;
	ComPtr< ID3D12Resource > m_pSwapChainRenderTarget[ g_nFrameCount ];
	ComPtr< ID3D12CommandQueue > m_pCommandQueue;
	ComPtr< ID3D12CommandAllocator > m_pCommandAllocators[ g_nFrameCount ];
	ComPtr< ID3D12GraphicsCommandList > m_pCommandList;
	ComPtr< ID3D12DescriptorHeap > m_pCBVSRVHeap;
	ComPtr< ID3D12DescriptorHeap > m_pRTVHeap;
	ComPtr< ID3D12DescriptorHeap > m_pDSVHeap;
	ComPtr< ID3D12RootSignature > m_pRootSignature;
	ComPtr< ID3D12PipelineState > m_pScenePipelineState;
	ComPtr< ID3D12PipelineState > m_pCompanionPipelineState;
	ComPtr< ID3D12PipelineState > m_pAxesPipelineState;
	ComPtr< ID3D12PipelineState > m_pRenderModelPipelineState;
	ComPtr< ID3D12Resource > m_pSceneConstantBuffer;
	D3D12_CPU_DESCRIPTOR_HANDLE m_sceneConstantBufferView[ 2 ];
	UINT8 *m_pSceneConstantBufferData[ 2 ];
	UINT m_nRTVDescriptorSize;
	UINT m_nDSVDescriptorSize;
	UINT m_nCBVSRVDescriptorSize;

	ComPtr< ID3D12Resource > m_pSceneVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_sceneVertexBufferView;
	ComPtr< ID3D12Resource > m_pTexture;
	ComPtr< ID3D12Resource > m_pTextureUploadHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_textureShaderResourceView;
	ComPtr< ID3D12Resource > m_pCompanionWindowVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_companionWindowVertexBufferView;
	ComPtr< ID3D12Resource > m_pCompanionWindowIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_companionWindowIndexBufferView;
	ComPtr< ID3D12Resource > m_pControllerAxisVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_controllerAxisVertexBufferView;
	

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
		ComPtr< ID3D12Resource > m_pTexture;
		CD3DX12_CPU_DESCRIPTOR_HANDLE m_renderTargetViewHandle;
		ComPtr< ID3D12Resource > m_pDepthStencil;
		CD3DX12_CPU_DESCRIPTOR_HANDLE m_depthStencilViewHandle;
	};
	FramebufferDesc m_leftEyeDesc;
	FramebufferDesc m_rightEyeDesc;

	bool CreateFrameBuffer( int nWidth, int nHeight, FramebufferDesc &framebufferDesc, RTVIndex_t nRTVIndex );
	
	uint32_t m_nRenderWidth;
	uint32_t m_nRenderHeight;

	std::vector< DX12RenderModel * > m_vecRenderModels;
	DX12RenderModel *m_rTrackedDeviceToRenderModel[ vr::k_unMaxTrackedDeviceCount ];
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
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMainApplication::CMainApplication( int argc, char *argv[] )
	: m_pCompanionWindow(NULL)
	, m_nCompanionWindowWidth( 640 )
	, m_nCompanionWindowHeight( 320 )
	, m_pHMD( NULL )
	, m_pRenderModels( NULL )
	, m_bDebugD3D12( false )
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
	, m_nFrameIndex( 0 )
	, m_fenceEvent( NULL )
	, m_nRTVDescriptorSize( 0 )
	, m_nCBVSRVDescriptorSize( 0 )
	, m_nDSVDescriptorSize( 0 )
{
	memset( m_pSceneConstantBufferData, 0, sizeof( m_pSceneConstantBufferData ) );

	for( int i = 1; i < argc; i++ )
	{
		if( !stricmp( argv[i], "-dxdebug" ) )
		{
			m_bDebugD3D12 = true;
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

	m_pCompanionWindow = SDL_CreateWindow( "hellovr [D3D12]", nWindowPosX, nWindowPosY, m_nCompanionWindowWidth, m_nCompanionWindowHeight, unWindowFlags );
	if (m_pCompanionWindow == NULL)
	{
		dprintf( "%s - Window could not be created! SDL Error: %s\n", __FUNCTION__, SDL_GetError() );
		return false;
	}

	m_strDriver = "No Driver";
	m_strDisplay = "No Display";

	m_strDriver = GetTrackedDeviceString( m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String );
	m_strDisplay = GetTrackedDeviceString( m_pHMD, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String );

	std::string strWindowTitle = "hellovr [D3D12] - " + m_strDriver + " " + m_strDisplay;
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
 
	if ( !BInitD3D12() )
	{
		dprintf( "%s - Unable to initialize D3D12!\n", __FUNCTION__ );
		return false;
	}

	if ( !BInitCompositor() )
	{
		dprintf( "%s - Failed to initialize VR Compositor!\n", __FUNCTION__ );
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Initialize DX12. Returns true if DX12 has been successfully
//          initialized, false if shaders could not be created.
//          If failure occurred in a module other than shaders, the function
//          may return true or throw an error. 
//-----------------------------------------------------------------------------
bool CMainApplication::BInitD3D12()
{
	UINT nDXGIFactoryFlags = 0;

	// Debug layers if -dxdebug is specified
	if ( m_bDebugD3D12 )
	{
		ComPtr< ID3D12Debug > pDebugController;
		if ( SUCCEEDED( D3D12GetDebugInterface( IID_PPV_ARGS( &pDebugController ) ) ) )
		{
			pDebugController->EnableDebugLayer();
			nDXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}

	ComPtr< IDXGIFactory4 > pFactory;
	if ( FAILED( CreateDXGIFactory2( nDXGIFactoryFlags, IID_PPV_ARGS( &pFactory ) ) ) )
	{
		dprintf( "CreateDXGIFactory2 failed.\n");
		return false;
	}

	// Query OpenVR for the output adapter index
	int32_t nAdapterIndex = 0;
	m_pHMD->GetDXGIOutputInfo( &nAdapterIndex );

	ComPtr< IDXGIAdapter1 > pAdapter;
	if ( FAILED( pFactory->EnumAdapters1( nAdapterIndex, &pAdapter ) ) )
	{
		dprintf( "Error enumerating DXGI adapter.\n") ;
	}
	DXGI_ADAPTER_DESC1 adapterDesc;
	pAdapter->GetDesc1( &adapterDesc );

	if ( FAILED( D3D12CreateDevice( pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS( &m_pDevice ) ) ) )
	{
		dprintf( "Failed to create D3D12 device with D3D12CreateDevice.\n" );
		return false;
	}

	// Create the command queue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	if ( FAILED( m_pDevice->CreateCommandQueue( &queueDesc, IID_PPV_ARGS( &m_pCommandQueue ) ) ) )
	{
		printf( "Failed to create D3D12 command queue.\n" );
		return false;
	}

	// Create the swapchain
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = g_nFrameCount;
	swapChainDesc.Width = m_nCompanionWindowWidth;
	swapChainDesc.Height = m_nCompanionWindowHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	// Determine the HWND from SDL
	struct SDL_SysWMinfo wmInfo; 
	SDL_VERSION( &wmInfo.version ); 
	SDL_GetWindowWMInfo( m_pCompanionWindow, &wmInfo );
	HWND hWnd = wmInfo.info.win.window;

	ComPtr< IDXGISwapChain1 > pSwapChain;
	if ( FAILED( pFactory->CreateSwapChainForHwnd( m_pCommandQueue.Get(), hWnd, &swapChainDesc, nullptr, nullptr, &pSwapChain ) ) )
	{
		dprintf( "Failed to create DXGI swapchain.\n" );
		return false;
	}

	pFactory->MakeWindowAssociation( hWnd, DXGI_MWA_NO_ALT_ENTER );
	pSwapChain.As( &m_pSwapChain );
	m_nFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	// Create descriptor heaps
	{
		m_nRTVDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
		m_nDSVDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_DSV );
		m_nCBVSRVDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );
		
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = NUM_RTVS;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_pDevice->CreateDescriptorHeap( &rtvHeapDesc, IID_PPV_ARGS( &m_pRTVHeap ) );
		
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = NUM_RTVS;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_pDevice->CreateDescriptorHeap( &rtvHeapDesc, IID_PPV_ARGS( &m_pDSVHeap ) );

		D3D12_DESCRIPTOR_HEAP_DESC cbvSrvHeapDesc = {};
		cbvSrvHeapDesc.NumDescriptors = NUM_SRV_CBVS;
		cbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		m_pDevice->CreateDescriptorHeap( &cbvSrvHeapDesc, IID_PPV_ARGS( &m_pCBVSRVHeap ) );
	}

	// Create per-frame resources
	for ( int nFrame = 0; nFrame < g_nFrameCount; nFrame++ )
	{
		if ( FAILED( m_pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS( &m_pCommandAllocators[ nFrame ] ) ) ) )
		{
			dprintf( "Failed to create command allocators.\n" );
			return false;
		}

		// Create swapchain render targets
		m_pSwapChain->GetBuffer( nFrame, IID_PPV_ARGS( &m_pSwapChainRenderTarget[ nFrame ] ) );

		// Create swapchain render target views
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle( m_pRTVHeap->GetCPUDescriptorHandleForHeapStart() );
		rtvHandle.Offset( RTV_SWAPCHAIN0 + nFrame, m_nRTVDescriptorSize );
		m_pDevice->CreateRenderTargetView( m_pSwapChainRenderTarget[ nFrame ].Get(), nullptr, rtvHandle );
	}

	// Create constant buffer
	{
		m_pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer( 1024 * 64 ),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS( &m_pSceneConstantBuffer ) );

		// Keep as persistently mapped buffer, store left eye in first 256 bytes, right eye in second
		UINT8 *pBuffer;
		CD3DX12_RANGE readRange( 0, 0 );
		m_pSceneConstantBuffer->Map( 0, &readRange, reinterpret_cast<void**>( &pBuffer ) );
		// Left eye to first 256 bytes, right eye to second 256 bytes
		m_pSceneConstantBufferData[ 0 ] = pBuffer;
		m_pSceneConstantBufferData[ 1 ] = pBuffer + 256;

		// Left eye CBV
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvLeftEyeHandle( m_pCBVSRVHeap->GetCPUDescriptorHandleForHeapStart() );
		cbvLeftEyeHandle.Offset( CBV_LEFT_EYE, m_nCBVSRVDescriptorSize );
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_pSceneConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ( sizeof( Matrix4 ) + 255 ) & ~255; // Pad to 256 bytes
		m_pDevice->CreateConstantBufferView( &cbvDesc, cbvLeftEyeHandle );
		m_sceneConstantBufferView[ 0 ] = cbvLeftEyeHandle;

		// Right eye CBV
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvRightEyeHandle( m_pCBVSRVHeap->GetCPUDescriptorHandleForHeapStart() );
		cbvRightEyeHandle.Offset( CBV_RIGHT_EYE, m_nCBVSRVDescriptorSize );
		cbvDesc.BufferLocation += 256;
		m_pDevice->CreateConstantBufferView( &cbvDesc, cbvRightEyeHandle );
		m_sceneConstantBufferView[ 1 ] = cbvRightEyeHandle;
	}

	// Create fence
	{
		memset( m_nFenceValues, 0, sizeof( m_nFenceValues ) );
		m_pDevice->CreateFence( m_nFenceValues[ m_nFrameIndex ], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &m_pFence ) );
		m_nFenceValues[ m_nFrameIndex ]++;

		m_fenceEvent = CreateEvent( nullptr, FALSE, FALSE, nullptr );
	}

	if( !CreateAllShaders() )
		return false;

	// Create command list
	m_pDevice->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocators[ m_nFrameIndex ].Get(), m_pScenePipelineState.Get(), IID_PPV_ARGS( &m_pCommandList ) );

	SetupTexturemaps();
	SetupScene();
	SetupCameras();
	SetupStereoRenderTargets();
	SetupCompanionWindow();
	SetupRenderModels();

	// Do any work that was queued up during loading
	m_pCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists( _countof(ppCommandLists), ppCommandLists );

	// Wait for it to finish
	m_pCommandQueue->Signal( m_pFence.Get(), m_nFenceValues[ m_nFrameIndex ] );
	m_pFence->SetEventOnCompletion( m_nFenceValues[ m_nFrameIndex], m_fenceEvent );
	WaitForSingleObjectEx( m_fenceEvent, INFINITE, FALSE );
	m_nFenceValues[ m_nFrameIndex ]++;

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
	if( m_pHMD )
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}

	for( std::vector< DX12RenderModel * >::iterator i = m_vecRenderModels.begin(); i != m_vecRenderModels.end(); i++ )
	{
		delete (*i);
	}
	m_vecRenderModels.clear();

	if( m_pCompanionWindow )
	{
		SDL_DestroyWindow(m_pCompanionWindow);
		m_pCompanionWindow = NULL;
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
		m_pCommandAllocators[ m_nFrameIndex ]->Reset();

		m_pCommandList->Reset( m_pCommandAllocators[ m_nFrameIndex ].Get(), m_pScenePipelineState.Get() );
		m_pCommandList->SetGraphicsRootSignature( m_pRootSignature.Get() );

		ID3D12DescriptorHeap *ppHeaps[] = { m_pCBVSRVHeap.Get() };
		m_pCommandList->SetDescriptorHeaps( _countof( ppHeaps ), ppHeaps );

		UpdateControllerAxes();
		RenderStereoTargets();
		RenderCompanionWindow();

		m_pCommandList->Close();

		// Execute the command list.
		ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
		m_pCommandQueue->ExecuteCommandLists( _countof( ppCommandLists ), ppCommandLists );

		vr::VRTextureBounds_t bounds;
		bounds.uMin = 0.0f;
		bounds.uMax = 1.0f;
		bounds.vMin = 0.0f;
		bounds.vMax = 1.0f;

		vr::D3D12TextureData_t d3d12LeftEyeTexture = { m_leftEyeDesc.m_pTexture.Get(), m_pCommandQueue.Get(), 0 };
		vr::Texture_t leftEyeTexture = { ( void * ) &d3d12LeftEyeTexture, vr::TextureType_DirectX12, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit( vr::Eye_Left, &leftEyeTexture, &bounds, vr::Submit_Default );

		vr::D3D12TextureData_t d3d12RightEyeTexture = { m_rightEyeDesc.m_pTexture.Get(), m_pCommandQueue.Get(), 0 };
		vr::Texture_t rightEyeTexture = { ( void * ) &d3d12RightEyeTexture, vr::TextureType_DirectX12, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit( vr::Eye_Right, &rightEyeTexture, &bounds, vr::Submit_Default );
	}

	// Present
	m_pSwapChain->Present( 0, 0 );

	// Wait for completion
	{
		const UINT64 nCurrentFenceValue = m_nFenceValues[ m_nFrameIndex ];
		m_pCommandQueue->Signal( m_pFence.Get(), nCurrentFenceValue );

		m_nFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
		if ( m_pFence->GetCompletedValue() < m_nFenceValues[ m_nFrameIndex ] )
		{
			m_pFence->SetEventOnCompletion( m_nFenceValues[ m_nFrameIndex ], m_fenceEvent );
			WaitForSingleObjectEx( m_fenceEvent, INFINITE, FALSE );
		}
		
		m_nFenceValues[ m_nFrameIndex ] = nCurrentFenceValue + 1;
	}

	// Spew out the controller and pose count whenever they change.
	if ( m_iTrackedControllerCount != m_iTrackedControllerCount_Last || m_iValidPoseCount != m_iValidPoseCount_Last )
	{
		m_iValidPoseCount_Last = m_iValidPoseCount;
		m_iTrackedControllerCount_Last = m_iTrackedControllerCount;
		
		dprintf( "PoseCount:%d(%s) Controllers:%d\n", m_iValidPoseCount, m_strPoseClasses.c_str(), m_iTrackedControllerCount );
	}

	UpdateHMDMatrixPose();
}

//-----------------------------------------------------------------------------
// Purpose: Creates all the shaders used by HelloVR DX12
//-----------------------------------------------------------------------------
bool CMainApplication::CreateAllShaders()
{
	std::string sExecutableDirectory = Path_StripFilename( Path_GetExecutablePath() );
	std::string strFullPath = Path_MakeAbsolute( "../cube_texture.png", sExecutableDirectory );
	
	// Root signature
	{
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		if ( FAILED( m_pDevice->CheckFeatureSupport( D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof( featureData ) ) ) )
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}
		
		CD3DX12_DESCRIPTOR_RANGE1 ranges[2];
		CD3DX12_ROOT_PARAMETER1 rootParameters[2];
		
		ranges[0].Init( D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC );
		ranges[1].Init( D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0 );
		rootParameters[0].InitAsDescriptorTable( 1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX );
		rootParameters[1].InitAsDescriptorTable( 1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL );
		
		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1( _countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags );
		ComPtr< ID3DBlob > signature;
		ComPtr< ID3DBlob > error;
		D3DX12SerializeVersionedRootSignature( &rootSignatureDesc, featureData.HighestVersion, &signature, &error );
		m_pDevice->CreateRootSignature( 0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS( &m_pRootSignature ) );
	}

	// Scene shader
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;
		UINT compileFlags = 0;

		std::string shaderPath =  Path_MakeAbsolute( "../shaders/scene.hlsl", sExecutableDirectory );
		std::wstring shaderPathW = std::wstring( shaderPath.begin(), shaderPath.end() );
		ComPtr< ID3DBlob > error;
		if ( FAILED( D3DCompileFromFile( shaderPathW.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, &error ) ) )
		{
			dprintf( "Failed compiling vertex shader '%s':\n%s\n", shaderPath.c_str(), ( char* )error->GetBufferPointer() );
			return false;
		}
		if ( FAILED( D3DCompileFromFile( shaderPathW.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, &error ) ) )
		{
			dprintf( "Failed compiling pixel shader '%s':\n%s\n", shaderPath.c_str(), ( char* )error->GetBufferPointer() );
			return false;
		}

		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof( inputElementDescs ) };
		psoDesc.pRootSignature = m_pRootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE( vertexShader.Get() );
		psoDesc.PS = CD3DX12_SHADER_BYTECODE( pixelShader.Get() );
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC( D3D12_DEFAULT );
		psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
		psoDesc.RasterizerState.MultisampleEnable = TRUE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC( D3D12_DEFAULT );
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC( D3D12_DEFAULT );
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = m_nMSAASampleCount;
		psoDesc.SampleDesc.Quality = 0;
		if ( FAILED( m_pDevice->CreateGraphicsPipelineState( &psoDesc, IID_PPV_ARGS( &m_pScenePipelineState ) ) ) )
		{
			dprintf( "Error creating D3D12 pipeline state.\n" );
			return false;
		}
	}

	// Companion shader
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;
		UINT compileFlags = 0;

		std::string shaderPath =  Path_MakeAbsolute( "../shaders/companion.hlsl", sExecutableDirectory );
		std::wstring shaderPathW = std::wstring( shaderPath.begin(), shaderPath.end() );
		ComPtr< ID3DBlob > error;
		if ( FAILED( D3DCompileFromFile( shaderPathW.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, &error ) ) )
		{
			dprintf( "Failed compiling vertex shader '%s':\n%s\n", shaderPath.c_str(), ( char* )error->GetBufferPointer() );
			return false;
		}
		if ( FAILED( D3DCompileFromFile( shaderPathW.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, &error ) ) )
		{
			dprintf( "Failed compiling pixel shader '%s':\n%s\n", shaderPath.c_str(), ( char* )error->GetBufferPointer() );
			return false;
		}

		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof( inputElementDescs ) };
		psoDesc.pRootSignature = m_pRootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE( vertexShader.Get() );
		psoDesc.PS = CD3DX12_SHADER_BYTECODE( pixelShader.Get() );
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC( D3D12_DEFAULT );
		psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC( D3D12_DEFAULT );
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC( D3D12_DEFAULT );
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		if ( FAILED( m_pDevice->CreateGraphicsPipelineState( &psoDesc, IID_PPV_ARGS( &m_pCompanionPipelineState ) ) ) )
		{
			dprintf( "Error creating D3D12 pipeline state.\n" );
			return false;
		}
	}

	// Axes shader
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;
		UINT compileFlags = 0;

		std::string shaderPath =  Path_MakeAbsolute( "../shaders/axes.hlsl", sExecutableDirectory );
		std::wstring shaderPathW = std::wstring( shaderPath.begin(), shaderPath.end() );
		ComPtr< ID3DBlob > error;
		if ( FAILED( D3DCompileFromFile( shaderPathW.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, &error ) ) )
		{
			dprintf( "Failed compiling vertex shader '%s':\n%s\n", shaderPath.c_str(), ( char* )error->GetBufferPointer() );
			return false;
		}
		if ( FAILED( D3DCompileFromFile( shaderPathW.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, &error ) ) )
		{
			dprintf( "Failed compiling pixel shader '%s':\n%s\n", shaderPath.c_str(), ( char* )error->GetBufferPointer() );
			return false;
		}

		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof( inputElementDescs ) };
		psoDesc.pRootSignature = m_pRootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE( vertexShader.Get() );
		psoDesc.PS = CD3DX12_SHADER_BYTECODE( pixelShader.Get() );
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC( D3D12_DEFAULT );
		psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
		psoDesc.RasterizerState.MultisampleEnable = TRUE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC( D3D12_DEFAULT );
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC( D3D12_DEFAULT );
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = m_nMSAASampleCount;
		psoDesc.SampleDesc.Quality = 0;
		if ( FAILED( m_pDevice->CreateGraphicsPipelineState( &psoDesc, IID_PPV_ARGS( &m_pAxesPipelineState ) ) ) )
		{
			dprintf( "Error creating D3D12 pipeline state.\n" );
			return false;
		}
	}

	// Render Model shader
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;
		UINT compileFlags = 0;

		std::string shaderPath =  Path_MakeAbsolute( "../shaders/rendermodel.hlsl", sExecutableDirectory );
		std::wstring shaderPathW = std::wstring( shaderPath.begin(), shaderPath.end() );
		ComPtr< ID3DBlob > error;
		if ( FAILED( D3DCompileFromFile( shaderPathW.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, &error ) ) )
		{
			dprintf( "Failed compiling vertex shader '%s':\n%s\n", shaderPath.c_str(), ( char* )error->GetBufferPointer() );
			return false;
		}
		if ( FAILED( D3DCompileFromFile( shaderPathW.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, &error ) ) )
		{
			dprintf( "Failed compiling pixel shader '%s':\n%s\n", shaderPath.c_str(), ( char* )error->GetBufferPointer() );
			return false;
		}

		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		// Describe and create the graphics pipeline state object (PSO).
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, _countof( inputElementDescs ) };
		psoDesc.pRootSignature = m_pRootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE( vertexShader.Get() );
		psoDesc.PS = CD3DX12_SHADER_BYTECODE( pixelShader.Get() );
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC( D3D12_DEFAULT );
		psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
		psoDesc.RasterizerState.MultisampleEnable = TRUE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC( D3D12_DEFAULT );
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC( D3D12_DEFAULT );
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = m_nMSAASampleCount;
		psoDesc.SampleDesc.Quality = 0;
		if ( FAILED( m_pDevice->CreateGraphicsPipelineState( &psoDesc, IID_PPV_ARGS( &m_pRenderModelPipelineState ) ) ) )
		{
			dprintf( "Error creating D3D12 pipeline state.\n" );
			return false;
		}
	}

	return true;
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

	// Store level 0
	std::vector< D3D12_SUBRESOURCE_DATA > mipLevelData;
	UINT8 *pBaseData = new UINT8[ nImageWidth * nImageHeight * 4 ];
	memcpy( pBaseData, &imageRGBA[0], sizeof( UINT8 ) * nImageWidth * nImageHeight * 4 );
	
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = &pBaseData[ 0 ];
	textureData.RowPitch = nImageWidth * 4;
	textureData.SlicePitch = textureData.RowPitch * nImageHeight;
	mipLevelData.push_back( textureData );

	// Generate mipmaps for the image
	int nPrevImageIndex = 0;
	int nMipWidth = nImageWidth;
	int nMipHeight = nImageHeight;

	while( nMipWidth > 1 && nMipHeight > 1 )
	{
		UINT8 *pNewImage;
		GenMipMapRGBA( ( UINT8* )mipLevelData[ nPrevImageIndex ].pData, &pNewImage, nMipWidth, nMipHeight, &nMipWidth, &nMipHeight );
		
		D3D12_SUBRESOURCE_DATA mipData = {};
		mipData.pData = pNewImage;
		mipData.RowPitch = nMipWidth * 4;
		mipData.SlicePitch = textureData.RowPitch * nMipHeight;
		mipLevelData.push_back( mipData );

		nPrevImageIndex++;
	}

	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.MipLevels = ( UINT16 ) mipLevelData.size();
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Width = nImageWidth;
	textureDesc.Height = nImageHeight;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	m_pDevice->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_DEFAULT ),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS( &m_pTexture ) );

	// Create shader resource view
	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle( m_pCBVSRVHeap->GetCPUDescriptorHandleForHeapStart() );
	srvHandle.Offset( SRV_TEXTURE_MAP, m_nCBVSRVDescriptorSize );
	m_pDevice->CreateShaderResourceView( m_pTexture.Get(), nullptr, srvHandle );
	m_textureShaderResourceView = srvHandle;
	
	const UINT64 nUploadBufferSize = GetRequiredIntermediateSize( m_pTexture.Get(), 0, textureDesc.MipLevels );

	// Create the GPU upload buffer.
	m_pDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer( nUploadBufferSize ),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS( &m_pTextureUploadHeap ) );

	UpdateSubresources( m_pCommandList.Get(), m_pTexture.Get(), m_pTextureUploadHeap.Get(), 0, 0, mipLevelData.size(), &mipLevelData[0] );
	m_pCommandList->ResourceBarrier( 1, &CD3DX12_RESOURCE_BARRIER::Transition( m_pTexture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE ) );

	// Free mip pointers
	for ( size_t nMip = 0; nMip < mipLevelData.size(); nMip++ )
	{
		delete [] mipLevelData[ nMip ].pData;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: generate next level mipmap for an RGBA image
//-----------------------------------------------------------------------------
void CMainApplication::GenMipMapRGBA( const UINT8 *pSrc, UINT8 **ppDst, int nSrcWidth, int nSrcHeight, int *pDstWidthOut, int *pDstHeightOut )
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

	*ppDst = new UINT8[ 4 * ( *pDstWidthOut ) * ( *pDstHeightOut ) ];
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
			( *ppDst ) [ ( y * ( *pDstWidthOut ) + x ) * 4 ] = ( UINT8 ) ( r );
			( *ppDst ) [ ( y * ( *pDstWidthOut ) + x ) * 4 + 1] = ( UINT8 ) ( g );
			( *ppDst ) [ ( y * ( *pDstWidthOut ) + x ) * 4 + 2] = ( UINT8 ) ( b );
			( *ppDst ) [ ( y * ( *pDstWidthOut ) + x ) * 4 + 3] = ( UINT8 ) ( a );
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
	
	m_pDevice->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer( sizeof(float) * vertdataarray.size() ), 
		D3D12_RESOURCE_STATE_GENERIC_READ, 
		nullptr, 
		IID_PPV_ARGS( &m_pSceneVertexBuffer ) );

	UINT8 *pMappedBuffer;
	CD3DX12_RANGE readRange( 0, 0 );
	m_pSceneVertexBuffer->Map( 0, &readRange, reinterpret_cast< void** >( &pMappedBuffer ) );
	memcpy( pMappedBuffer, &vertdataarray[0], sizeof( float ) * vertdataarray.size() );
	m_pSceneVertexBuffer->Unmap( 0, nullptr );

	m_sceneVertexBufferView.BufferLocation = m_pSceneVertexBuffer->GetGPUVirtualAddress();
	m_sceneVertexBufferView.StrideInBytes = sizeof( VertexDataScene );
	m_sceneVertexBufferView.SizeInBytes =  sizeof( float ) * vertdataarray.size();
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
	if ( m_pControllerAxisVertexBuffer == nullptr && vertdataarray.size() > 0 )
	{
		// Make big enough to hold up to the max number
		size_t nSize = sizeof(float) * vertdataarray.size();
		nSize *= vr::k_unMaxTrackedDeviceCount;
		
		m_pDevice->CreateCommittedResource( 
			&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ), 
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer( nSize ), 
			D3D12_RESOURCE_STATE_GENERIC_READ, 
			nullptr, 
			IID_PPV_ARGS( &m_pControllerAxisVertexBuffer ) );

		m_controllerAxisVertexBufferView.BufferLocation = m_pControllerAxisVertexBuffer->GetGPUVirtualAddress();
		m_controllerAxisVertexBufferView.StrideInBytes = sizeof( float ) * 6;
		m_controllerAxisVertexBufferView.SizeInBytes =  sizeof( float ) * vertdataarray.size();
	}

	// Update the VB data
	if ( m_pControllerAxisVertexBuffer )
	{
		UINT8 *pMappedBuffer;
		CD3DX12_RANGE readRange( 0, 0 );
		m_pControllerAxisVertexBuffer->Map( 0, &readRange, reinterpret_cast< void** >( &pMappedBuffer ) );
		memcpy( pMappedBuffer, &vertdataarray[0], sizeof( float ) * vertdataarray.size() );
		m_pControllerAxisVertexBuffer->Unmap( 0, nullptr );
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
bool CMainApplication::CreateFrameBuffer( int nWidth, int nHeight, FramebufferDesc &framebufferDesc, RTVIndex_t nRTVIndex )
{
	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	textureDesc.Width = nWidth;
	textureDesc.Height = nHeight;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.SampleDesc.Count = m_nMSAASampleCount;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Create color target
	m_pDevice->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_DEFAULT ),
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&CD3DX12_CLEAR_VALUE( DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor ),
			IID_PPV_ARGS( &framebufferDesc.m_pTexture ) );

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle( m_pRTVHeap->GetCPUDescriptorHandleForHeapStart() );
	rtvHandle.Offset( nRTVIndex, m_nRTVDescriptorSize );
	m_pDevice->CreateRenderTargetView( framebufferDesc.m_pTexture.Get(), nullptr, rtvHandle );
	framebufferDesc.m_renderTargetViewHandle = rtvHandle;

	// Create shader resource view
	CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle( m_pCBVSRVHeap->GetCPUDescriptorHandleForHeapStart() );
	srvHandle.Offset( SRV_LEFT_EYE + nRTVIndex, m_nCBVSRVDescriptorSize );
	m_pDevice->CreateShaderResourceView( framebufferDesc.m_pTexture.Get(), nullptr, srvHandle );

	// Create depth
	D3D12_RESOURCE_DESC depthDesc = textureDesc;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	m_pDevice->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_DEFAULT ),
			D3D12_HEAP_FLAG_NONE,
			&depthDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&CD3DX12_CLEAR_VALUE( DXGI_FORMAT_D32_FLOAT, 1.0f, 0 ),
			IID_PPV_ARGS( &framebufferDesc.m_pDepthStencil ) );

	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle( m_pDSVHeap->GetCPUDescriptorHandleForHeapStart() );
	dsvHandle.Offset( nRTVIndex, m_nDSVDescriptorSize );
	m_pDevice->CreateDepthStencilView( framebufferDesc.m_pDepthStencil.Get(), nullptr, dsvHandle );
	framebufferDesc.m_depthStencilViewHandle = dsvHandle;
	return true;
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

	CreateFrameBuffer( m_nRenderWidth, m_nRenderHeight, m_leftEyeDesc, RTV_LEFT_EYE );
	CreateFrameBuffer( m_nRenderWidth, m_nRenderHeight, m_rightEyeDesc, RTV_RIGHT_EYE );
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
	
	m_pDevice->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ), 
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer( sizeof( VertexDataWindow ) * vVerts.size() ), 
		D3D12_RESOURCE_STATE_GENERIC_READ, 
		nullptr, 
		IID_PPV_ARGS( &m_pCompanionWindowVertexBuffer ) );

	UINT8 *pMappedBuffer;
	CD3DX12_RANGE readRange( 0, 0 );
	m_pCompanionWindowVertexBuffer->Map( 0, &readRange, reinterpret_cast< void** >( &pMappedBuffer ) );
	memcpy( pMappedBuffer, &vVerts[0], sizeof( VertexDataWindow ) * vVerts.size() );
	m_pCompanionWindowVertexBuffer->Unmap( 0, nullptr );

	m_companionWindowVertexBufferView.BufferLocation = m_pCompanionWindowVertexBuffer->GetGPUVirtualAddress();
	m_companionWindowVertexBufferView.StrideInBytes = sizeof( VertexDataWindow );
	m_companionWindowVertexBufferView.SizeInBytes =  sizeof( VertexDataWindow ) * vVerts.size();


	UINT16 vIndices[] = { 0, 1, 3,   0, 3, 2,   4, 5, 7,   4, 7, 6};
	m_uiCompanionWindowIndexSize = _countof(vIndices);

	m_pDevice->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ), 
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer( sizeof( vIndices ) ),
		D3D12_RESOURCE_STATE_GENERIC_READ, 
		nullptr, 
		IID_PPV_ARGS( &m_pCompanionWindowIndexBuffer ) );

	m_pCompanionWindowIndexBuffer->Map( 0, &readRange, reinterpret_cast< void** >( &pMappedBuffer ) );
	memcpy( pMappedBuffer, &vIndices[0], sizeof( vIndices ) );
	m_pCompanionWindowIndexBuffer->Unmap( 0, nullptr );

	m_companionWindowIndexBufferView.BufferLocation = m_pCompanionWindowIndexBuffer->GetGPUVirtualAddress();
	m_companionWindowIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	m_companionWindowIndexBufferView.SizeInBytes = sizeof( vIndices );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RenderStereoTargets()
{
	D3D12_VIEWPORT viewport = { 0.0f, 0.0f, ( FLOAT ) m_nRenderWidth, ( FLOAT ) m_nRenderHeight, 0.0f, 1.0f };
	D3D12_RECT scissor = { 0, 0, ( LONG ) m_nRenderWidth, ( LONG )m_nRenderHeight };

	m_pCommandList->RSSetViewports( 1, &viewport );
	m_pCommandList->RSSetScissorRects( 1, &scissor );

	//----------//
	// Left Eye //
	//----------//
	// Transition to RENDER_TARGET
	m_pCommandList->ResourceBarrier( 1, &CD3DX12_RESOURCE_BARRIER::Transition( m_leftEyeDesc.m_pTexture.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET ) );
	m_pCommandList->OMSetRenderTargets( 1, &m_leftEyeDesc.m_renderTargetViewHandle, FALSE, &m_leftEyeDesc.m_depthStencilViewHandle );

	const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pCommandList->ClearRenderTargetView( m_leftEyeDesc.m_renderTargetViewHandle, clearColor, 0, nullptr );
	m_pCommandList->ClearDepthStencilView( m_leftEyeDesc.m_depthStencilViewHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr );

	RenderScene( vr::Eye_Left );
	
	// Transition to SHADER_RESOURCE to submit to SteamVR
	m_pCommandList->ResourceBarrier( 1, &CD3DX12_RESOURCE_BARRIER::Transition( m_leftEyeDesc.m_pTexture.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE ) );


	//-----------//
	// Right Eye //
	//-----------//
	// Transition to RENDER_TARGET
	m_pCommandList->ResourceBarrier( 1, &CD3DX12_RESOURCE_BARRIER::Transition( m_rightEyeDesc.m_pTexture.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET ) );
	m_pCommandList->OMSetRenderTargets( 1, &m_rightEyeDesc.m_renderTargetViewHandle, FALSE, &m_rightEyeDesc.m_depthStencilViewHandle );

	m_pCommandList->ClearRenderTargetView( m_rightEyeDesc.m_renderTargetViewHandle, clearColor, 0, nullptr );
	m_pCommandList->ClearDepthStencilView( m_rightEyeDesc.m_depthStencilViewHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0, 0, 0, nullptr );

	RenderScene( vr::Eye_Right );
	
	// Transition to SHADER_RESOURCE to submit to SteamVR
	m_pCommandList->ResourceBarrier( 1, &CD3DX12_RESOURCE_BARRIER::Transition( m_rightEyeDesc.m_pTexture.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE ) );
}

//-----------------------------------------------------------------------------
// Purpose: Renders a scene with respect to nEye.
//-----------------------------------------------------------------------------
void CMainApplication::RenderScene( vr::Hmd_Eye nEye )
{
	if( m_bShowCubes )
	{
		m_pCommandList->SetPipelineState( m_pScenePipelineState.Get() );

		// Select the CBV (left or right eye)
		CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle( m_pCBVSRVHeap->GetGPUDescriptorHandleForHeapStart() );
		cbvHandle.Offset( nEye, m_nCBVSRVDescriptorSize );
		m_pCommandList->SetGraphicsRootDescriptorTable( 0, cbvHandle );
		
		// SRV is just after the left eye 
		CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle( m_pCBVSRVHeap->GetGPUDescriptorHandleForHeapStart() );
		srvHandle.Offset( SRV_TEXTURE_MAP, m_nCBVSRVDescriptorSize );
		m_pCommandList->SetGraphicsRootDescriptorTable( 1, srvHandle );

		// Update the persistently mapped pointer to the CB data with the latest matrix
		memcpy( m_pSceneConstantBufferData[ nEye ], GetCurrentViewProjectionMatrix( nEye ).get(), sizeof( Matrix4 ) );

		// Draw
		m_pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		m_pCommandList->IASetVertexBuffers( 0, 1, &m_sceneVertexBufferView );
		m_pCommandList->DrawInstanced( m_uiVertcount, 1, 0, 0 );
	}

	bool bIsInputCapturedByAnotherProcess = m_pHMD->IsInputFocusCapturedByAnotherProcess();

	if( !bIsInputCapturedByAnotherProcess && m_pControllerAxisVertexBuffer )
	{
		// draw the controller axis lines
		m_pCommandList->SetPipelineState( m_pAxesPipelineState.Get() );

		m_pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_LINELIST );
		m_pCommandList->IASetVertexBuffers( 0, 1, &m_controllerAxisVertexBufferView );
		m_pCommandList->DrawInstanced( m_uiControllerVertcount, 1, 0, 0 );
	}

	// ----- Render Model rendering -----
	m_pCommandList->SetPipelineState( m_pRenderModelPipelineState.Get() );
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
		
		m_rTrackedDeviceToRenderModel[ unTrackedDevice ]->Draw( nEye, m_pCommandList.Get(), m_nCBVSRVDescriptorSize, matMVP );
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RenderCompanionWindow()
{
	m_pCommandList->SetPipelineState( m_pCompanionPipelineState.Get() );

	// Transition swapchain image to RENDER_TARGET
	m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition( m_pSwapChainRenderTarget[ m_nFrameIndex ].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET ) );

	// Bind current swapchain image
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle( m_pRTVHeap->GetCPUDescriptorHandleForHeapStart() );
	rtvHandle.Offset( RTV_SWAPCHAIN0 + m_nFrameIndex, m_nRTVDescriptorSize );
	m_pCommandList->OMSetRenderTargets( 1, &rtvHandle, 0, nullptr );

	D3D12_VIEWPORT viewport = { 0.0f, 0.0f, ( FLOAT ) m_nCompanionWindowWidth, ( FLOAT ) m_nCompanionWindowHeight, 0.0f, 1.0f };
	D3D12_RECT scissor = { 0, 0, ( LONG ) m_nCompanionWindowWidth, ( LONG )m_nCompanionWindowHeight };

	m_pCommandList->RSSetViewports( 1, &viewport );
	m_pCommandList->RSSetScissorRects( 1, &scissor );

	
	// render left eye (first half of index array)
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandleLeftEye( m_pCBVSRVHeap->GetGPUDescriptorHandleForHeapStart() );
	srvHandleLeftEye.Offset( SRV_LEFT_EYE, m_nCBVSRVDescriptorSize );
	m_pCommandList->SetGraphicsRootDescriptorTable( 1, srvHandleLeftEye );

	m_pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	m_pCommandList->IASetVertexBuffers( 0, 1, &m_companionWindowVertexBufferView );
	m_pCommandList->IASetIndexBuffer( &m_companionWindowIndexBufferView );
	m_pCommandList->DrawIndexedInstanced( m_uiCompanionWindowIndexSize / 2, 1, 0, 0, 0 );

	// render right eye (second half of index array)
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandleRightEye( m_pCBVSRVHeap->GetGPUDescriptorHandleForHeapStart() );
	srvHandleRightEye.Offset( SRV_RIGHT_EYE, m_nCBVSRVDescriptorSize );
	m_pCommandList->SetGraphicsRootDescriptorTable( 1, srvHandleRightEye );
	m_pCommandList->DrawIndexedInstanced( m_uiCompanionWindowIndexSize / 2, 1, ( m_uiCompanionWindowIndexSize / 2 ), 0, 0 );

	// Transition swapchain image to PRESENT
	m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition( m_pSwapChainRenderTarget[ m_nFrameIndex ].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT ) );
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
DX12RenderModel *CMainApplication::FindOrLoadRenderModel( vr::TrackedDeviceIndex_t unTrackedDeviceIndex, const char *pchRenderModelName )
{
	DX12RenderModel *pRenderModel = NULL;
	// To simplify the D3D12 rendering code, create an instance of the model for each model name.  This is less efficient
	// memory wise, but simplifies the rendering code so we can store the transform in a constant buffer associated with
	// the model itself.  You would not want to do this in a production application.
	//for( std::vector< DX12RenderModel * >::iterator i = m_vecRenderModels.begin(); i != m_vecRenderModels.end(); i++ )
	//{
		//if( !stricmp( (*i)->GetName().c_str(), pchRenderModelName ) )
		//{
		//	pRenderModel = *i;
		//	break;
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

		pRenderModel = new DX12RenderModel( pchRenderModelName );
		if ( !pRenderModel->BInit( m_pDevice.Get(), m_pCommandList.Get(),  m_pCBVSRVHeap.Get(), unTrackedDeviceIndex, *pModel, *pTexture ) )
		{
			dprintf( "Unable to create D3D12 model from render model %s\n", pchRenderModelName );
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
// Purpose: Create/destroy D3D12 a Render Model for a single tracked device
//-----------------------------------------------------------------------------
void CMainApplication::SetupRenderModelForTrackedDevice( vr::TrackedDeviceIndex_t unTrackedDeviceIndex )
{
	if( unTrackedDeviceIndex >= vr::k_unMaxTrackedDeviceCount )
		return;

	// try to find a model we've already set up
	std::string sRenderModelName = GetTrackedDeviceString( m_pHMD, unTrackedDeviceIndex, vr::Prop_RenderModelName_String );
	DX12RenderModel *pRenderModel = FindOrLoadRenderModel( unTrackedDeviceIndex, sRenderModelName.c_str() );
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
// Purpose: Create/destroy D3D12 Render Models
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
// Purpose: Create/destroy D3D12 Render Models
//-----------------------------------------------------------------------------
DX12RenderModel::DX12RenderModel( const std::string & sRenderModelName )
	: m_sModelName( sRenderModelName )
{
	memset( m_pConstantBufferData, 0, sizeof( m_pConstantBufferData ) );
}

DX12RenderModel::~DX12RenderModel()
{
	Cleanup();
}

//-----------------------------------------------------------------------------
// Purpose: Allocates and populates the D3D12 resources for a render model
//-----------------------------------------------------------------------------
bool DX12RenderModel::BInit( ID3D12Device *pDevice, ID3D12GraphicsCommandList *pCommandList, ID3D12DescriptorHeap *pCBVSRVHeap, vr::TrackedDeviceIndex_t unTrackedDeviceIndex, const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture )
{
	m_unTrackedDeviceIndex = unTrackedDeviceIndex;
	m_pCBVSRVHeap = pCBVSRVHeap;

	// Create and populate the vertex buffer
	{
		pDevice->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(  sizeof( vr::RenderModel_Vertex_t ) * vrModel.unVertexCount ), 
			D3D12_RESOURCE_STATE_GENERIC_READ, 
			nullptr, 
			IID_PPV_ARGS( &m_pVertexBuffer ) );

		UINT8 *pMappedBuffer;
		CD3DX12_RANGE readRange( 0, 0 );
		m_pVertexBuffer->Map( 0, &readRange, reinterpret_cast< void** >( &pMappedBuffer ) );
		memcpy( pMappedBuffer,  vrModel.rVertexData, sizeof( vr::RenderModel_Vertex_t ) * vrModel.unVertexCount );
		m_pVertexBuffer->Unmap( 0, nullptr );

		m_vertexBufferView.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof( vr::RenderModel_Vertex_t );
		m_vertexBufferView.SizeInBytes =   sizeof( vr::RenderModel_Vertex_t ) * vrModel.unVertexCount;
	}

	// Create and populate the index buffer
	{
		pDevice->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ), 
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer( sizeof( uint16_t ) * vrModel.unTriangleCount * 3 ),
			D3D12_RESOURCE_STATE_GENERIC_READ, 
			nullptr, 
			IID_PPV_ARGS( &m_pIndexBuffer ) );

		UINT8 *pMappedBuffer;
		CD3DX12_RANGE readRange( 0, 0 );
		m_pIndexBuffer->Map( 0, &readRange, reinterpret_cast< void** >( &pMappedBuffer ) );
		memcpy( pMappedBuffer, vrModel.rIndexData, sizeof( uint16_t ) * vrModel.unTriangleCount * 3 );
		m_pIndexBuffer->Unmap( 0, nullptr );

		m_indexBufferView.BufferLocation = m_pIndexBuffer->GetGPUVirtualAddress();
		m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		m_indexBufferView.SizeInBytes = sizeof( uint16_t ) * vrModel.unTriangleCount * 3;
	}

	// create and populate the texture
	{
		int nImageWidth = vrDiffuseTexture.unWidth;
		int nImageHeight = vrDiffuseTexture.unHeight;
		std::vector< D3D12_SUBRESOURCE_DATA > mipLevelData;
 	
		UINT8 *pBaseData = new UINT8[ nImageWidth * nImageHeight * 4 ];
		memcpy( pBaseData,vrDiffuseTexture.rubTextureMapData, sizeof( UINT8 ) * nImageWidth * nImageHeight * 4 );
		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = &pBaseData[ 0 ];
		textureData.RowPitch = nImageWidth * 4;
		textureData.SlicePitch = textureData.RowPitch * nImageHeight;
		mipLevelData.push_back( textureData );

		// Generate mipmaps for the image
		int nPrevImageIndex = 0;
		int nMipWidth = nImageWidth;
		int nMipHeight = nImageHeight;

		while( nMipWidth > 1 && nMipHeight > 1 )
		{
			UINT8 *pNewImage;
			CMainApplication::GenMipMapRGBA( ( UINT8* )mipLevelData[ nPrevImageIndex ].pData, &pNewImage, nMipWidth, nMipHeight, &nMipWidth, &nMipHeight );
		
			D3D12_SUBRESOURCE_DATA mipData = {};
			mipData.pData = pNewImage;
			mipData.RowPitch = nMipWidth * 4;
			mipData.SlicePitch = textureData.RowPitch * nMipHeight;
			mipLevelData.push_back( mipData );

			nPrevImageIndex++;
		}
	
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = ( UINT16 ) mipLevelData.size();
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.Width = nImageWidth;
		textureDesc.Height = nImageHeight;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		pDevice->CreateCommittedResource( &CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_DEFAULT ),
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS( &m_pTexture ) );

		// Create shader resource view
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle( pCBVSRVHeap->GetCPUDescriptorHandleForHeapStart() );
		srvHandle.Offset( SRV_TEXTURE_RENDER_MODEL0 + unTrackedDeviceIndex, pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ) );
		pDevice->CreateShaderResourceView( m_pTexture.Get(), nullptr, srvHandle );

		const UINT64 nUploadBufferSize = GetRequiredIntermediateSize( m_pTexture.Get(), 0, textureDesc.MipLevels );

		// Create the GPU upload buffer.
		pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer( nUploadBufferSize ),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS( &m_pTextureUploadHeap ) );

		UpdateSubresources( pCommandList, m_pTexture.Get(), m_pTextureUploadHeap.Get(), 0, 0, mipLevelData.size(), &mipLevelData[0] );
		pCommandList->ResourceBarrier( 1, &CD3DX12_RESOURCE_BARRIER::Transition( m_pTexture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE ) );

		// Free mip pointers
		for ( size_t nMip = 0; nMip < mipLevelData.size(); nMip++ )
		{
			delete [] mipLevelData[ nMip ].pData;
		}
	}

	// Create a constant buffer to hold the transform (one for each eye)
	{
		pDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES( D3D12_HEAP_TYPE_UPLOAD ),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer( 1024 * 64 ),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS( &m_pConstantBuffer ) );

		// Keep as persistently mapped buffer, store left eye in first 256 bytes, right eye in second
		UINT8 *pBuffer;
		CD3DX12_RANGE readRange( 0, 0 );
		m_pConstantBuffer->Map( 0, &readRange, reinterpret_cast<void**>( &pBuffer ) );
		// Left eye to first 256 bytes, right eye to second 256 bytes
		m_pConstantBufferData[ 0 ] = pBuffer;
		m_pConstantBufferData[ 1 ] = pBuffer + 256;

		// Left eye CBV
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvLeftEyeHandle( m_pCBVSRVHeap->GetCPUDescriptorHandleForHeapStart() );
		cbvLeftEyeHandle.Offset( CBV_LEFT_EYE_RENDER_MODEL0 + m_unTrackedDeviceIndex, pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ) );
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_pConstantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ( sizeof( Matrix4 ) + 255 ) & ~255; // Pad to 256 bytes
		pDevice->CreateConstantBufferView( &cbvDesc, cbvLeftEyeHandle );
		
		// Right eye CBV
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvRightEyeHandle( m_pCBVSRVHeap->GetCPUDescriptorHandleForHeapStart() );
		cbvRightEyeHandle.Offset( CBV_RIGHT_EYE_RENDER_MODEL0 + m_unTrackedDeviceIndex,  pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV ) );
		cbvDesc.BufferLocation += 256;
		pDevice->CreateConstantBufferView( &cbvDesc, cbvRightEyeHandle );
	}

	m_unVertexCount = vrModel.unTriangleCount * 3;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Frees the D3D12 resources for a render model
//-----------------------------------------------------------------------------
void DX12RenderModel::Cleanup()
{
}

//-----------------------------------------------------------------------------
// Purpose: Draws the render model
//-----------------------------------------------------------------------------
void DX12RenderModel::Draw( vr::EVREye nEye, ID3D12GraphicsCommandList *pCommandList, UINT nCBVSRVDescriptorSize, const Matrix4 &matMVP )
{
	// Update the CB with the transform
	memcpy( m_pConstantBufferData[ nEye ], &matMVP, sizeof( matMVP ) );

	// Bind the CB
	int nStartOffset = ( nEye == vr::Eye_Left ) ? CBV_LEFT_EYE_RENDER_MODEL0 : CBV_RIGHT_EYE_RENDER_MODEL0;
	CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle( m_pCBVSRVHeap->GetGPUDescriptorHandleForHeapStart() );
	cbvHandle.Offset( nStartOffset + m_unTrackedDeviceIndex, nCBVSRVDescriptorSize );
	pCommandList->SetGraphicsRootDescriptorTable( 0, cbvHandle );

	// Bind the texture
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle( m_pCBVSRVHeap->GetGPUDescriptorHandleForHeapStart() );
	srvHandle.Offset( SRV_TEXTURE_RENDER_MODEL0 + m_unTrackedDeviceIndex, nCBVSRVDescriptorSize );
	pCommandList->SetGraphicsRootDescriptorTable( 1, srvHandle );

	// Bind the VB/IB and draw
	pCommandList->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	pCommandList->IASetVertexBuffers( 0, 1, &m_vertexBufferView );
	pCommandList->IASetIndexBuffer( &m_indexBufferView );
	pCommandList->DrawIndexedInstanced( m_unVertexCount, 1, 0, 0, 0 );
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
