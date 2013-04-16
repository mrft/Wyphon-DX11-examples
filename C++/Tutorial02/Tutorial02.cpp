//--------------------------------------------------------------------------------------
// File: Tutorial02.cpp
//
// This application displays a triangle using Direct3D 11
//
// http://msdn.microsoft.com/en-us/library/windows/apps/ff729719.aspx
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <d3d11.h>
#include <d3d10.h>
#include <d3d10_1.h>
#include <d3d10misc.h>
#include <d3d9.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "resource.h"

#include "Wyphon.h"
#include "WyphonUtils.h"


using namespace DirectX;

using namespace Wyphon;
using namespace WyphonUtils;


//--------------------------------------------------------------------------------------
// Stuff for Wyphon
//--------------------------------------------------------------------------------------

//	Define the Functions in the DLL for reuse. This is just prototyping the dll's function. 
//	A mock of it. Use "stdcall" for maximum compatibility.

//typedef void ( * LPWyphonPartnerJoinedCALLBACK)( HANDLE wyphonPartnerHandle, unsigned int sendingWyphonPartnerId, LPCTSTR description, void * customData );
//typedef void ( * LPWyphonPartnerLeftCALLBACK)( HANDLE wyphonPartnerHandle, unsigned int sendingWyphonPartnerId, void * customData );
//
//typedef void ( * LPD3DTextureSharingStartedCALLBACK)( HANDLE wyphonPartnerHandle, unsigned int sendingPartnerId, HANDLE sharedTextureHandle, unsigned int width, unsigned int height, DWORD format, DWORD usage, LPTSTR description );
//typedef void ( * LPD3DTextureSharingStoppedCALLBACK)( HANDLE wyphonPartnerHandle, unsigned int sendingPartnerId, HANDLE sharedTextureHandle, unsigned int width, unsigned int height, DWORD format, DWORD usage, LPTSTR description );
//
//
//typedef bool (/*WINAPI*/ /*__stdcall*/ * pShareD3DTexture)(HANDLE wyphonPartnerHandle, HANDLE sharedTextureHandle, unsigned int width, unsigned int height, DWORD format, DWORD usage, LPTSTR description);
//typedef bool (/*WINAPI*/ /*__stdcall*/ * pUnshareD3DTexture)(HANDLE wyphonPartnerHandle, HANDLE sharedTextureHandle);
//
//typedef bool (/*WINAPI*/ /*__stdcall*/ * pDestroyWyphonPartner)(HANDLE wyphonPartnerHandle);
//
//typedef HANDLE (/*WINAPI*/ /*__stdcall*/ * pCreateWyphonPartner)( LPTSTR applicationName
//							, void * pCallbackFuncCustomData
//							, LPWyphonPartnerJoinedCALLBACK pPartnerJoinedCallbackFunc
//							, LPWyphonPartnerLeftCALLBACK pPartnerLeftCallbackFunc
//							, LPD3DTextureSharingStartedCALLBACK pD3DTextureSharingStartedCallbackFunc 
//							, LPD3DTextureSharingStoppedCALLBACK pD3DTextureSharingStoppedCallbackFunc 
//	 						);
//
//typedef LPCTSTR (/*WINAPI*/ /*__stdcall*/ * pGetWyphonPartnerName)(HANDLE wyphonPartnerHandle, unsigned int wyphonPartnerId);
//
//typedef bool (/*WINAPI*/ /*__stdcall*/ * pGetD3DTextureInfo)(HANDLE wyphonPartnerHandle, HANDLE sharedTextureHandle, unsigned int & wyphonPartnerId, unsigned int & width, unsigned int & height, DWORD & format, DWORD & usage, LPTSTR description, int maxDescriptionLength );
//
//typedef unsigned int (/*WINAPI*/ /*__stdcall*/ * pGetPartnerId)(HANDLE wyphonPartnerHandle);
//
//HINSTANCE hLocalMessageBroadcastDll = LoadLibrary(TEXT("Wyphon.dll"));
//
//// get pointers to the functions in the dll
/////////////////////////////////////////////
//
////if ( hLocalMessageBroadcastDll != NULL ) {
//FARPROC lpfnShareD3DTexture = GetProcAddress(HMODULE (hLocalMessageBroadcastDll),"ShareD3DTexture");
//FARPROC lpfnUnshareD3DTexture = GetProcAddress(HMODULE (hLocalMessageBroadcastDll),"UnshareD3DTexture");
//FARPROC lpfnDestroyWyphonPartner = GetProcAddress(HMODULE (hLocalMessageBroadcastDll),"DestroyWyphonPartner");
//FARPROC lpfnCreateWyphonPartner = GetProcAddress(HMODULE (hLocalMessageBroadcastDll),"CreateWyphonPartner");
//FARPROC lpfnGetWyphonPartnerName = GetProcAddress(HMODULE (hLocalMessageBroadcastDll),"GetWyphonPartnerName");
//FARPROC lpfnGetD3DTextureInfo = GetProcAddress(HMODULE (hLocalMessageBroadcastDll),"GetD3DTextureInfo");
//FARPROC lpfnGetPartnerId = GetProcAddress(HMODULE (hLocalMessageBroadcastDll),"GetPartnerId");
//
//pShareD3DTexture ShareD3DTexture = (pShareD3DTexture)lpfnShareD3DTexture;
//pUnshareD3DTexture UnshareD3DTexture = (pUnshareD3DTexture)lpfnUnshareD3DTexture;
//pDestroyWyphonPartner DestroyWyphonPartner = (pDestroyWyphonPartner)lpfnDestroyWyphonPartner;
//pCreateWyphonPartner CreateWyphonPartner = (pCreateWyphonPartner)lpfnCreateWyphonPartner;
//pGetWyphonPartnerName GetWyphonPartnerName = (pGetWyphonPartnerName)lpfnGetWyphonPartnerName;
//pGetD3DTextureInfo GetD3DTextureInfo = (pGetD3DTextureInfo)lpfnGetD3DTextureInfo;
//pGetPartnerId GetPartnerId = (pGetPartnerId)lpfnGetPartnerId;

//}



















//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
    XMFLOAT3 Pos;
};


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;
ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;

ID3D11Texture2D*		g_pSharedTexture = NULL; //A texture we want to share with the rest of the world !!!
HANDLE					g_hWyphonPartner;
float					g_ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
HWND                    g_hWnd_D3D9Ex = NULL;

bool					is64bit = ( sizeof(int*) == 8 );


//D3DX11SaveTextureToFile(g_pImmediateContext,texture_to_save,D3DX11_IFF_PNG,"c:\\data_local\texture.png");

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

	///////////////////
	//// SETUP Wyphon
	///////////////////

	//no callbacks since we only want to share textures, we're not interested in info of other partners
	g_hWyphonPartner = CreateWyphonPartner( is64bit ? TEXT( "Wyphon sender test 64 BIT" ) : TEXT( "Wyphon sender test 32 BIT" )
							, NULL
							, NULL
							, NULL
							, NULL
							, NULL
	 						);
	//WyphonUtils
	InitDX9Ex();



    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Render();
        }
    }


	/////////////////////
	//// TEARDOWN Wyphon
	/////////////////////

	//no callbacks since we only want to share textures, we're not interested in info of other partners
	if (g_hWyphonPartner != NULL) {
		DestroyWyphonPartner( g_hWyphonPartner );
	}

	//WyphonUtils
	ReleaseDX9Ex();


    CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 320, 240 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", is64bit ? L"Wyphon SENDER test 64bit" : L"Wyphon SENDER test 32bit",
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //DXGI_FORMAT_B8G8R8A8_UNORM; //DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
	

	//void * dxgiFactory1 = NULL;
	//IID dxgiFactoryIID;
	//CreateDXGIFactory1(dxgiFactoryIID, & dxgiFactory1);


    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial02.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return hr;
	}

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
	UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
	hr = g_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &g_pVertexLayout );
	pVSBlob->Release();
	if( FAILED( hr ) )
        return hr;

    // Set the input layout
    g_pImmediateContext->IASetInputLayout( g_pVertexLayout );

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial02.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        XMFLOAT3( 0.0f, 0.5f, 0.5f ),
        XMFLOAT3( 0.5f, -0.5f, 0.5f ),
        XMFLOAT3( -0.5f, -0.5f, 0.5f ),
    };
    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

    // Set primitive topology
    g_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_pImmediateContext ) g_pImmediateContext->ClearState();

    if( g_pVertexBuffer ) g_pVertexBuffer->Release();
    if( g_pVertexLayout ) g_pVertexLayout->Release();
    if( g_pVertexShader ) g_pVertexShader->Release();
    if( g_pPixelShader ) g_pPixelShader->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}



D3DFORMAT ConvertDXGIToD3D9Format(DXGI_FORMAT format)
{
    switch (format)
    {
    case DXGI_FORMAT_B8G8R8A8_UNORM:
        return D3DFMT_A8R8G8B8;
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: 
        return D3DFMT_A8R8G8B8;
    case DXGI_FORMAT_B8G8R8X8_UNORM:
        return D3DFMT_X8R8G8B8;
    case DXGI_FORMAT_R8G8B8A8_UNORM: 
        return D3DFMT_A8B8G8R8;
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: 
        return D3DFMT_A8B8G8R8;
    default:
        return D3DFMT_UNKNOWN;
    };
}









	/// Utility function that takes a given hWnd as a parameter. It will create a D3D9ex device first 
	/// and then will try to create a D3D9Ex shared texture. the returned handle can be used 
	/// by D3D10 or higher to open the texture and do whatever needs to be done
	///
	/// This way D3D10 or higher apps can make sure their texture can be opened by D3D9Ex applications
	///
	/// Usage:	if you need a texture for copying data from main memory to, use D3DUSAGE_DYNAMIC (will be in AGP memory instead of GPU memory)
	///			if you need a texture te render another 3D scene to, use D3DUSAGE_RENDERTARGET (will be in GPU memory)
	///
	
	//don't forget to cleanup these at the end !!!
	IDirect3D9Ex * g_pDirect3D9Ex = NULL;
	IDirect3DDevice9Ex * g_pDeviceD3D9ex = NULL;
	

	HRESULT MyCreateDX9ExTexture(UINT width, UINT height, D3DFORMAT format, DWORD usage, PDIRECT3DTEXTURE9 * out_pD3D9Texture, HANDLE * out_SharedTextureHandle) {
		HRESULT hr = S_OK;

		//create Direct3D instance if necessary
		if ( g_pDirect3D9Ex == NULL ) {
			hr = Direct3DCreate9Ex( D3D_SDK_VERSION,  /*_Out_*/ &g_pDirect3D9Ex );
			if ( hr != S_OK ) {
				return hr;
			}
		}

		//create device if necessary
		if ( g_pDeviceD3D9ex == NULL ) {

			//seems to work if no window !
			//g_hWnd_D3D9Ex = NULL;


			// Do we support hardware vertex processing? if so, use it. 
			// If not, downgrade to software.
			D3DCAPS9 d3dCaps;
			hr = g_pDirect3D9Ex->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps );
			if ( hr != S_OK ) {
				// TO DO: Respond to failure of GetDeviceCaps
				return hr;
			}

			DWORD dwBehaviorFlags = 0;
			if ( d3dCaps.VertexProcessingCaps != 0 ) {
				dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

				//usage = usage XOR D3DUSAGE_SOFTWAREPROCESSING;
			}
			else {
				dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

				usage = usage | D3DUSAGE_SOFTWAREPROCESSING;
			}

			usage = usage | D3DUSAGE_NONSECURE;
			
			D3DDISPLAYMODE displayMode;

			hr = g_pDirect3D9Ex->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &displayMode);

			if ( hr != S_OK ) {
				return hr;
			}

			//D3DPRESENT_PARAMETERS * presentParameters
			D3DPRESENT_PARAMETERS presentParameters = {0};
			ZeroMemory( &presentParameters, sizeof(presentParameters) );
			presentParameters.Windowed = true;
			presentParameters.hDeviceWindow = NULL; //g_hWnd; //NULL; //g_hWnd_D3D9Ex;
			presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			presentParameters.BackBufferWidth = 64;
			presentParameters.BackBufferHeight = 64;
			presentParameters.BackBufferFormat = displayMode.Format; //D3DFMT_A8R8G8B8; //format
			presentParameters.EnableAutoDepthStencil = FALSE;
			presentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
			presentParameters.BackBufferCount = 1;
			//present_parameters.Flags = 0;
			//present_parameters.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;


			hr = g_pDirect3D9Ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd_D3D9Ex, dwBehaviorFlags, &presentParameters, NULL, &g_pDeviceD3D9ex);
			
			if ( hr != S_OK ) {
				return hr;
			}
		}

		//create texture
		hr = g_pDeviceD3D9ex->CreateTexture(width, height, 1, usage, format, D3DPOOL_DEFAULT, out_pD3D9Texture, out_SharedTextureHandle);

		return hr;
	}








//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
    // Clear the back buffer
    //float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha

	//make sure the texture changes the whole time
	g_ClearColor[0] = ( g_ClearColor[0] + 0.0010f ); if (g_ClearColor[0] >= 1) g_ClearColor[0] -= 1.0f;
	g_ClearColor[1] = ( g_ClearColor[1] + 0.0009f ); if (g_ClearColor[1] >= 1) g_ClearColor[1] -= 1.0f;
	g_ClearColor[2] = ( g_ClearColor[2] + 0.0008f ); if (g_ClearColor[2] >= 1) g_ClearColor[2] -= 1.0f;



			///////////////////////////////////////////////////////////////////////////////////////////
			//// COPY THE BACKBUFFER TO OUR (global) SHARED TEXTURE
			///////////////////////////////////////////////////////////////////////////////////////////
			ID3D11Texture2D* pSourceBuffer; //will be used to copy the backbuffer to our sharedTexture

			////g_pImmediateContext->
			////g_pSwapChain->
			g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pSourceBuffer);

			//if ( g_pSharedTexture == NULL && pSourceBuffer != NULL ) {
			//	//IDirect3D9Ex_CreateDevice(p,a, b ,c,d,e,f);
			//	


			D3D11_TEXTURE2D_DESC td;
			pSourceBuffer->GetDesc(&td);
			//make it a shared texture, so add a flag
			td.MiscFlags |= D3D11_RESOURCE_MISC_SHARED;
			td.MipLevels = 1;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.ArraySize = 1;
			td.BindFlags = 0; //D3D11_BIND_RENDER_TARGET;
			td.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //DXGI_FORMAT_R8G8B8A8_UNORM;
			//CD3D11_TEXTURE2D_DESC td2 = new CD3D11_TEXTURE2D_DESC(...);



			// I think we should create a D3D9ex shared texture and share that. Using the same handle we should open our D3D11 copy ourselves... 
			// Some hints here http://xboxforums.create.msdn.com/forums/t/103939.aspx


			if (g_pSharedTexture == NULL) {
				//Ideally end up with 1 simple function somewhat like this: CreateD3D10SharedTexture(pSourceBuffer, g_pd3dDevice, &g_pSharedTexture);

#define USE_DLL			true
#define USE_FUNCTION	true		//only relevant if USE_DLL = false


				HANDLE d3d9exShareHandle = NULL;
				PDIRECT3DTEXTURE9 pD3D9Texture = NULL;
				D3DFORMAT format = D3DFMT_A32B32G32R32F; //D3DFMT_A8R8G8B8; //D3DFMT_X8R8G8B8; 

				if ( USE_DLL ) {
						PDIRECT3DTEXTURE9 pD3D9Texture = NULL;

						//unsigned __int32 sharedTextureHandleInt;
						HRESULT hr = CreateDX9ExTexture( td.Width, td.Height, D3DUSAGE_RENDERTARGET, format, &pD3D9Texture, &d3d9exShareHandle );

						//d3d9exShareHandle = (HANDLE)sharedTextureHandleInt;
				}
				else if ( ! USE_FUNCTION ) {
						IDirect3D9Ex * pDirect3D9Ex;
						IDirect3DDevice9 * pDeviceD3D9;
						IDirect3DDevice9Ex * pDeviceD3D9ex;
						ID3D10Device1 * pDeviceD3D10_1;


						//Get adapter of the current D3D11 device. Our D3D10 will run on the same adapter.
						IDXGIDevice* pDXGIDevice;
						IDXGIAdapter* pAdapter;
						g_pd3dDevice->QueryInterface<IDXGIDevice>(&pDXGIDevice);
						//g_pd3dDevice->QueryInterface<IDirect3DDevice9Ex>(&pDeviceD3D9ex);
						pDXGIDevice->GetAdapter(&pAdapter);
						pDXGIDevice->Release();


						//////// If we add the lib-file to the linker there is no need to load this dll dynamically
						///////////////////////////////////////////////////////////////////////////////////////////
						////Get address of the function D3D10CreateDevice1 dynamically.
						////Load D3D10.DLL
						////HMODULE m_hD3D9 = LoadLibrary(L"d3d9.dll");
						//HMODULE m_hD3D10 = LoadLibrary(L"d3d10_1.dll");
						//typedef HRESULT (WINAPI* FN_D3D10CreateDevice1)(IDXGIAdapter *pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software,
						//												UINT Flags, D3D10_FEATURE_LEVEL1 HardwareLevel, UINT SDKVersion, ID3D10Device1 **ppDevice );
						//FN_D3D10CreateDevice1 fnCreate = (FN_D3D10CreateDevice1)GetProcAddress(m_hD3D10, "D3D10CreateDevice1");
						////Call D3D10CreateDevice1 dynamically.
						//fnCreate( pAdapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_BGRA_SUPPORT | D3D10_CREATE_DEVICE_DEBUG, 
						//		  D3D10_FEATURE_LEVEL_10_1, D3D10_1_SDK_VERSION, &pDeviceD3D10_1);

						D3D10CreateDevice1(pAdapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_BGRA_SUPPORT
								| D3D10_CREATE_DEVICE_DEBUG, D3D10_FEATURE_LEVEL_10_1, D3D10_1_SDK_VERSION, &pDeviceD3D10_1);



						//Now we have a D3D10 device: m_pInterD3D10 (which means the Intermediate Direct3D10 device).
						//Now let’s share it with D3D11. Do this on your Swapchain Resizing event. (Because size of the Render Target changes):

						D3DPRESENT_PARAMETERS present_parameters = {0};
						//IDirect3DDevice9Ex *device;
						//D3DDISPLAYMODEEX mode; //, *m;

						// WE DON'T NEED A SEPARATE WINDOW TO CREATE OUR D3D9ex device !!!
						//////////////////////////////////////////////////////////////////
						// Create window
						//RECT rc = { 0, 0, 640, 480 };
						//AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
						//if (g_hWnd_D3D9Ex == NULL) {
						//	g_hWnd_D3D9Ex = CreateWindow( L"TutorialWindowClass", L"D3D9Ex window - Direct3D 11 Tutorial 2: Rendering a Triangle",
						//								   WS_OVERLAPPEDWINDOW,
						//								   CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, g_hInst,
						//								   NULL );
						//	ShowWindow( g_hWnd_D3D9Ex, SW_SHOWNA );
						//}

						ZeroMemory( &present_parameters, sizeof(present_parameters) );
						present_parameters.Windowed = true;
						present_parameters.hDeviceWindow = NULL; //g_hWnd; //g_hWnd_D3D9Ex;
						present_parameters.SwapEffect = D3DSWAPEFFECT_COPY;
						present_parameters.BackBufferWidth = td.Width;
						present_parameters.BackBufferHeight = td.Height;
						present_parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
						present_parameters.EnableAutoDepthStencil = FALSE;
						present_parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
						present_parameters.BackBufferCount = 1;
						//present_parameters.Flags = 0;
						//present_parameters.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

						//mode.Size = sizeof(mode);
						//mode.Width = td.Width;
						//mode.Height = td.Height;
						//mode.RefreshRate = 0;
						//mode.Format = D3DFMT_A8R8G8B8;
						//mode.ScanLineOrdering = D3DSCANLINEORDERING_UNKNOWN;

						////Create a D3D10.1 render target texture and share it with our D3D11.
						//D3D10_TEXTURE2D_DESC tDesc;
						//tDesc.Width = td.Width;
						//tDesc.Height = td.Height;
						//tDesc.MipLevels = 1;
						//tDesc.ArraySize = 1;
						//tDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //DXGI_FORMAT_R8G8B8A8_UNORM;
						//tDesc.SampleDesc.Count = 1;
						//tDesc.SampleDesc.Quality = 0;
						//tDesc.Usage = D3D10_USAGE_DEFAULT;

						////EVEN IF YOU WON'T USE AS SHADER RESOURCE, SET THIS BIND FLAGS:
						//tDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
						//tDesc.CPUAccessFlags = 0;
						//tDesc.MiscFlags = D3D10_RESOURCE_MISC_SHARED;//


						Direct3DCreate9Ex( D3D_SDK_VERSION,  
												//_Out_ 
												&pDirect3D9Ex);

						HRESULT hr;

						//This works
						D3DDISPLAYMODE d3ddm = {0};
						pDirect3D9Ex->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
						
						//This doesn't work
						D3DDISPLAYMODEEX d3ddmex;
						d3ddmex.Size = sizeof(d3ddmex);
						d3ddmex.Width = d3ddm.Width;
						d3ddmex.Height = d3ddm.Height;
						d3ddmex.Format = d3ddm.Format;
						d3ddmex.ScanLineOrdering = D3DSCANLINEORDERING_UNKNOWN;

						//D3DDISPLAYROTATION d3ddrot;
						//if( S_OK != ( hr = pDirect3D9Ex->GetAdapterDisplayModeEx( D3DADAPTER_DEFAULT, &d3ddmex, &d3ddrot ) ) )
						//{
						//	// TO DO: Respond to failure of GetAdapterDisplayMode
						//	return;
						//}

						if ( S_OK != ( hr = pDirect3D9Ex->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
																	d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
																	D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
						{
							if( hr == D3DERR_NOTAVAILABLE )
								// POTENTIAL PROBLEM: We need at least a 16-bit z-buffer!
								return;
						}


						// Do we support hardware vertex processing? if so, use it. 
						// If not, downgrade to software.
						D3DCAPS9 d3dCaps;
						if( S_OK != pDirect3D9Ex->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps ) ) {
							// TO DO: Respond to failure of GetDeviceCaps
							return;
						}
						DWORD dwBehaviorFlags = 0;
						if( d3dCaps.VertexProcessingCaps != 0 )
							dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
						else
							dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;


						DWORD d3d9ex_usage = D3DUSAGE_RENDERTARGET | D3DUSAGE_NONSECURE | (dwBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING ? 0 : D3DUSAGE_SOFTWAREPROCESSING);

						//HRESULT createDeviceHR = pDirect3D9Ex->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd_D3D9Ex, dwBehaviorFlags, &present_parameters, &pDeviceD3D9);
						//if (createDeviceHR == S_OK) {
						//	hr = pDeviceD3D9->CreateTexture(td.Width, td.Height, 1
						//					, D3DUSAGE_RENDERTARGET //| D3DUSAGE_SOFTWAREPROCESSING
						//					, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pD3D9Texture, &d3d9exShareHandle);
						//	if ( pD3D9Texture == NULL ) {
						//		Sleep(1000);
						//	}
						//}
						//else if (D3DERR_DEVICELOST == createDeviceHR)
						//	Sleep(1000);
						//else if (D3DERR_INVALIDCALL == createDeviceHR)
						//	Sleep(1000);
						//else if (D3DERR_WRONGTEXTUREFORMAT == createDeviceHR)
						//	Sleep(1000);

						HRESULT createDeviceExHR = pDirect3D9Ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd_D3D9Ex, dwBehaviorFlags, &present_parameters, NULL, &pDeviceD3D9ex);
						if (createDeviceExHR == S_OK) {
							hr = pDeviceD3D9ex->CreateTexture(td.Width, td.Height, 1, d3d9ex_usage, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pD3D9Texture, &d3d9exShareHandle);
						}
						else if (D3DERR_DEVICELOST == createDeviceExHR)
							Sleep(1000);
						else if (D3DERR_INVALIDCALL == createDeviceExHR)
							Sleep(1000);
						else if (D3DERR_WRONGTEXTUREFORMAT == createDeviceExHR)
							Sleep(1000);

						////Create the RT texture on D3D10
						//ID3D10Texture2D * pD3D10Texture;
						//pDeviceD3D10_1->CreateTexture2D( &tDesc, NULL, &pD3D10Texture);

						////Get DXGI Resource and retrieve the sharing handle.
						//IDXGISurface * pDXGISurf;
						//IDXGIResource * pDXGIRes;
						//HANDLE d3d10ShareHandle;
						//pD3D10Texture->QueryInterface<IDXGISurface>(&pDXGISurf);
						//pDXGISurf->QueryInterface<IDXGIResource>(&pDXGIRes);
						//pDXGIRes->GetSharedHandle( &d3d9exShareHandle );
						//pDXGIRes->Release();
						//pDXGISurf->Release();
				}
				else { //IF USE_FUNCTION
						//DWORD usage = D3DUSAGE_RENDERTARGET;
						
						PDIRECT3DTEXTURE9 pD3D9Texture = NULL;

						HRESULT hr = MyCreateDX9ExTexture(td.Width, td.Height, format /*D3DFMT_A8R8G8B8*/, D3DUSAGE_RENDERTARGET, &pD3D9Texture, &d3d9exShareHandle);
				}

				if (d3d9exShareHandle != NULL) {
					////Call D3D 11 to open shared resource (m_pDevice is the D3D11 device).
					ID3D11Resource * pD3D11Res;
					//ID3D11Texture2D * m_pInterRTD3D11;

					g_pd3dDevice->OpenSharedResource(d3d9exShareHandle, __uuidof(ID3D11Resource), (void**)&pD3D11Res);
					pD3D11Res->QueryInterface<ID3D11Texture2D>(&g_pSharedTexture);
					pD3D11Res->Release();
		
					g_pImmediateContext->CopyResource(g_pSharedTexture, pSourceBuffer);

					ShareD3DTexture(g_hWyphonPartner, d3d9exShareHandle, td.Width, td.Height, format, D3DUSAGE_RENDERTARGET, TEXT("screen output"));

					//SimpleVertex vertices[] =
					//{
					//	{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT2( 1.0f, 0.0f ) },
					//	{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
					//	{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT2( 0.0f, 1.0f ) },
					//	{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT2( 1.0f, 1.0f ) },
					//};
				}
				else {
					Sleep(1000);
				}
			}


			//if ( g_pSharedTexture == NULL ) {
				//Render to SCREEN

				g_pImmediateContext->OMSetRenderTargets( 1,  &g_pRenderTargetView, NULL );

				g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, g_ClearColor );

				// Render a triangle
				g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
				g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
				g_pImmediateContext->Draw( 3, 0 );


				// Present the information rendered to the back buffer to the front buffer (the screen)
				g_pSwapChain->Present( 0, 0 );
			//}
			if ( g_pSharedTexture != NULL ) {
				//Render to TEXTURE

				// If screen and shared texture have the same format, we could CopyResource the backbuffer to the texture
				/////////////////////////////////////////////////////////////////////////////////////////////////////////
				//g_pImmediateContext->CopyResource(g_pSharedTexture, pSourceBuffer);

				// Otherwise, we need to setup the texture as the rendertarget, and render the scene again
				//////////////////////////////////////////////////////////////////////////////////////////
				//g_pImmediateContext->Flush();


				HRESULT result;
				D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

				ID3D11RenderTargetView* m_renderTargetView = NULL;
				ID3D11ShaderResourceView* m_shaderResourceView = NULL;

				// Setup the description of the render target view.
				D3D11_TEXTURE2D_DESC textureDesc;
				g_pSharedTexture->GetDesc(&textureDesc);

				renderTargetViewDesc.Format = textureDesc.Format;
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc.Texture2D.MipSlice = 0;

				// Create the render target view.
				result = g_pd3dDevice->CreateRenderTargetView(g_pSharedTexture, &renderTargetViewDesc, &m_renderTargetView);
				if(FAILED(result))
				{
				}
				else {
					// Setup the description of the shader resource view.
					shaderResourceViewDesc.Format = textureDesc.Format;
					shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
					shaderResourceViewDesc.Texture2D.MipLevels = 1;

					// Create the shader resource view.
					//result = g_pd3dDevice->CreateShaderResourceView(g_pSharedTexture, &shaderResourceViewDesc, &m_shaderResourceView);
					//if(FAILED(result))
					//{
					//}
					//else {
						//ID3D11DepthStencilView* m_depthStencilView;

						g_pImmediateContext->OMSetRenderTargets( 1, &m_renderTargetView, NULL );

						g_pImmediateContext->ClearRenderTargetView( m_renderTargetView, g_ClearColor );

						// Render a triangle
						g_pImmediateContext->VSSetShader( g_pVertexShader, NULL, 0 );
						g_pImmediateContext->PSSetShader( g_pPixelShader, NULL, 0 );
						g_pImmediateContext->Draw( 3, 0 );

						//g_pImmediateContext->Flush();

						// Present the information rendered to the back buffer to the front buffer (the screen)
						//g_pSwapChain->Present( 0, 0 );

						//cleanup
						if ( m_shaderResourceView != NULL ) m_shaderResourceView->Release();
						if ( m_renderTargetView != NULL ) m_renderTargetView->Release();

					//}
				}

/* Something similar in D3D9Ex

				//set new render target
				g_pd3dDevice->SetRenderTarget(0,pRenderSurface);
				//clear texture
				g_App.GetDevice()->Clear(0,
										   NULL,
										   D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
										   D3DCOLOR_XRGB(100,100,100),
										   1.0f,
										   0);
				g_App.GetDevice()->BeginScene();

				g_App.GetDevice()->SetTexture(0,pPyramideTexture);

				D3DXMatrixRotationY(&matRotationY,fRotation);
				D3DXMatrixTranslation(&matTranslation,0.0f,0.0f,5.0f);
				g_App.GetDevice()->SetTransform(D3DTS_WORLD, &(matRotationY * matTranslation));
				//set projection matrix
				g_App.GetDevice()->SetTransform(D3DTS_PROJECTION,&matProjection);

				g_App.GetDevice()->SetStreamSource(0,pTriangleVB,0,sizeof(D3DVERTEX));
				g_App.GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,0,4);

				g_App.GetDevice()->EndScene();
*/

			}




	Sleep(10);


//	ID3D11Texture2D* pSurface;
//	HRESULT hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &pSurface ) );
//	if( pSurface )
//	{
//		//ft: added m_dpi myself
//		int m_dpi = 96;
//		
//	    const int width = static_cast<int>(m_window->Bounds.Width * m_dpi / 96.0f);
//	    const int height = static_cast<int>(m_window->Bounds.Height * m_dpi / 96.0f);
//	    unsigned int size = width * height;
//	    if( m_captureData )
//	    {
//	        freeFramebufferData( m_captureData );
//	    }
//	    m_captureData = new unsigned char[ width * height * 4 ];
//	
//	    ID3D11Texture2D* pNewTexture = NULL;
//	
//	    D3D11_TEXTURE2D_DESC description;
//	    pSurface->GetDesc( &description );
//	    description.BindFlags = 0;
//	    description.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
//	    description.Usage = D3D11_USAGE_STAGING;
//	
//	    HRESULT hr = m_d3dDevice->CreateTexture2D( &description, NULL, &pNewTexture );
//	    if ( pNewTexture )
//	    {
//	        m_d3dContext->CopyResource( pNewTexture, pSurface );
//	        D3D11_MAPPED_SUBRESOURCE resource;
//	        unsigned int subresource = D3D11CalcSubresource( 0, 0, 0 );
//	        HRESULT hr = m_d3dContext->Map( pNewTexture, subresource, D3D11_MAP_READ_WRITE, 0, &resource );
//	        //resource.pData; // TEXTURE DATA IS HERE
//	
//	        const int pitch = width << 2;
//	        const unsigned char* source = static_cast< const unsigned char* >( resource.pData );
//	        unsigned char* dest = m_captureData;
//	        for( int i = 0; i < height; ++i )
//	        {
//	            memcpy( dest, source, width * 4 );
//	            source += pitch;
//	            dest += pitch;
//	        }
//	
//	        m_captureSize = size;
//	        m_captureWidth = width;
//	        m_captureHeight = height;
//	
//	        return;
//	    }
//	
//	    freeFramebufferData( m_captureData );
//	}
}
