// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

// define the screen resolution
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// global declarations
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;

// function prototypes
void initD3D(HWND hWnd);
void render_frame(HWND);
void cleanD3D(void);
void init_graphics(void);
LPCWSTR getEnumName(int num);

struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

D3DXFONT_DESC d3dFont;
ID3DXFont* font = 0;
int enumIndex = 0;

 float recentSinIndex = 0;

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
		WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

ShowWindow(hWnd, nCmdShow);

// set up and initialize Direct3D
initD3D(hWnd);

// enter the main loop:

MSG msg;

while (TRUE)
{
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
		break;

	render_frame(hWnd);
}

cleanD3D();

return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	init_graphics();    // call the function to initialize the cube

	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);    // turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
	//do not cull back face
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	//font init

	
	memset(&d3dFont, 0, sizeof(d3dFont));
	d3dFont.Height = 25; // in logical units
	d3dFont.Width = 12;  // in logical units
	d3dFont.Weight = 500;// boldness, range 0(light) - 1000(bold)
	d3dFont.Italic = FALSE;
	d3dFont.CharSet = DEFAULT_CHARSET;
	//may cause problem
	wcscpy(d3dFont.FaceName, L"Times New Roman");
	
	D3DXCreateFontIndirect(d3ddev, &d3dFont, &font);

	
	
	


	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);    // turn on the color blending
	d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);    // set the operation
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);    // set source factor
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // set dest factor
}


// this is the function used to render a single frame
void render_frame(HWND hWnd)
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//font rect init
	RECT rect;
	GetClientRect(hWnd, &rect);

	d3ddev->BeginScene();

	// select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);

	// set an ever-increasing float value
	static float index = 0.0f; index += 0.03f;
	
	if (recentSinIndex <0 &&((float)sin(index)>0)){
		
			d3ddev->SetRenderState(D3DRS_DESTBLEND, enumIndex%14);
			enumIndex++;
	}
	else {

	}

	getEnumName(enumIndex);
	//font init
	font->DrawText(NULL, L"  MUNODevelop", -1, &rect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	rect.top += 20;
	font->DrawText(NULL, getEnumName(enumIndex%14+1), -1, &rect,
		DT_SINGLELINE | DT_NOCLIP | DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

	//actually i need do it final 
	
	recentSinIndex = (float)sin(index);

	// set the view transform
	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3((float)sin(index) * 20.0f,  2.0f, (float)cos(index) * 20.0f),    // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),    // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction
	d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

												   // set the projection transform
	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),    // the horizontal field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
		1.0f,    // the near view-plane
		100.0f);    // the far view-plane
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection

															   // set the stream source
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

	// set the first world transform
	D3DXMATRIX matTranslate;
	D3DXMatrixTranslation(&matTranslate, 0.0f, 0.0f, -10.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));    // set the world transform

														   // draw the first square
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// set the second world transform
	D3DXMatrixTranslation(&matTranslate, 0.0f, 0.0f, 0.0f);
	d3ddev->SetTransform(D3DTS_WORLD, &(matTranslate));    // set the world transform

														   // draw the second square
	d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	v_buffer->Release();    // close and release the vertex buffer
	d3ddev->Release();    // close and release the 3D device
	d3d->Release();    // close and release Direct3D
}


// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		// square 1
		{ -3.0f, 3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ -3.0f, -3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 255, 0), },
		{ 3.0f, 3.0f, 3.0f, D3DCOLOR_ARGB(255, 255, 0, 0), },
		{ 3.0f, -3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 255, 255), },

		// square 2
		{ -6.0f, 6.0f, 4.0f, D3DCOLOR_ARGB(111, 0, 0, 255), },
		{ -6.0f, -6.0f, 4.0f, D3DCOLOR_ARGB(111, 0, 255, 0), },
		{ 6.0f, 6.0f, 4.0f, D3DCOLOR_ARGB(111, 255, 0, 0), },
		{ 6.0f, -6.0f, 4.0f, D3DCOLOR_ARGB(111, 0, 255, 255), },
	};

	// create a vertex buffer interface called v_buffer
	d3ddev->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer

					// lock v_buffer and load the vertices into it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();
}


LPCWSTR getEnumName(int num)
{
	switch (num)
	{
	case 1: return L"D3DBLEND_ZERO";
	case 2: return L"D3DBLEND_ONE";
	case 3: return L"D3DBLEND_SRCCOLOR";
	case 4: return L"D3DBLEND_INVSRCCOLOR";
	case 5: return L"D3DBLEND_SRCALPHA";
	case 6: return L"D3DBLEND_INVSRCALPHA";
	case 7: return L"D3DBLEND_DESTALPHA";
	case 8: return L"D3DBLEND_INVDESTALPHA";
	case 9: return L"D3DBLEND_DESTCOLOR";
	case 10: return L"D3DBLEND_INVDESTCOLOR";
	case 11: return L"D3DBLEND_SRCALPHASAT";
	case 12: return L"D3DBLEND_BOTHSRCALPHA";
	case 13: return L"D3DBLEND_BOTHINVSRCALPHA";
	}
}