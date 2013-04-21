#include "windows.h"
#include "GL\glew.h"
#include "GL\wglew.h"
#include <tchar.h>


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{

	WNDCLASSEX winclass;
	USHORT width = 800;
	USHORT height = 650;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WndProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = L"OPENGL";
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&winclass);
	
	HWND hWnd = CreateWindowEx(	NULL,
								L"OPENGL",
								L"4.3 Context creation",
								WS_OVERLAPPEDWINDOW,
								0,
								0,
								width,
								height,
								NULL,
								NULL,
								winclass.hInstance,
								NULL);



	HDC hDC = GetDC(hWnd);

	//dummy context creation

	WNDCLASS dummyWinclass;

	ZeroMemory(&dummyWinclass, sizeof(dummyWinclass));
    dummyWinclass.lpfnWndProc = DefWindowProc;
    dummyWinclass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
    dummyWinclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    dummyWinclass.lpszClassName = _T("DUMMY");
    RegisterClass( &dummyWinclass );

	HWND dummyHwnd=CreateWindow(_T("FREEGLUT_dummy"),
								_T(""),
								WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW , 
								0,0,0,0,0,0, 
								dummyWinclass.hInstance,
								0 );


	HDC dummyHdc = GetDC(dummyHwnd);

	// will use this pfd just for the dummy opengl context
	PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    
        PFD_TYPE_RGBA,            
        32,                        
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                       
        8,                        
        0,                       
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

	int pixelformat;
	SetPixelFormat( dummyHdc, pixelformat, &pfd );

	HGLRC dummyRc = wglCreateContext( hDC );
    wglMakeCurrent(hDC, dummyRc);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		MessageBox(NULL,_T("Could not initialize GLEW"),_T("GLEW failed"),MB_OK);
	}

	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(dummyRc);
	ReleaseDC(dummyHwnd,dummyHdc);
	DestroyWindow(dummyHwnd);
	UnregisterClass(_T("DUMMY"),dummyWinclass.hInstance);


	int attributesPixelFormat[] = { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
									WGL_SUPPORT_OPENGL_ARB,	GL_TRUE,
									WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
									WGL_COLOR_BITS_ARB, 32,
									WGL_ALPHA_BITS_ARB, 0,
									WGL_DEPTH_BITS_ARB, 24,
									WGL_STENCIL_BITS_ARB, 8,
									WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
									0};
	float fAttributes[] = { 0, 0 };
	int iPixelFormat;
	UINT numFormats;
	BOOL valid = wglChoosePixelFormatARB(hDC,attributesPixelFormat,fAttributes,1,&iPixelFormat,&numFormats);

	SetPixelFormat( hDC, iPixelFormat, NULL);


	int attributesContext[] = { WGL_CONTEXT_MAJOR_VERSION_ARB,4,
								WGL_CONTEXT_MINOR_VERSION_ARB,3,
								WGL_CONTEXT_FLAGS_ARB,WGL_CONTEXT_DEBUG_BIT_ARB,
								WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
								0};

	HGLRC hGLRC = wglCreateContextAttribsARB(hDC,NULL,attributesContext);

	wglMakeCurrent(hDC,hGLRC);

	const GLubyte* string = glGetString(GL_VERSION);


}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd,msg,wParam,lParam);
}