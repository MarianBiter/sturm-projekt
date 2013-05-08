#include "windows.h"
#include "GL\glew.h"
#include "GL\wglew.h"
#include <tchar.h>
#include <cstdio>
#include <cstdlib>

GLuint vao;
GLuint buffer;
GLuint vertexShader;
GLuint fragmentShader;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


GLchar * textFileRead(const char* filename)
{
	FILE *fp = NULL;
	GLchar* content;
	unsigned int  count = 0;
	fp = fopen(filename,"rt");

	if(fp == NULL)
	{
		return NULL;
	}

	fseek(fp,0,SEEK_END);
	count = ftell(fp);
	rewind(fp);

	content = new GLchar[count+1];

	count = fread(content,sizeof(GLchar),count,fp);
	content[count] = '\0';

	fclose(fp);

	return content;


}
void loadShaders()
{
	GLint retVal;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSource = textFileRead("triangles.vert");
	glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&retVal);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSource = textFileRead("triangles.frag");
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&retVal);
}


void initScene()
{
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	GLfloat vertices[6][2] =
	{
		{-0.9,-0.9},
		{0.85,-0.9},
		{-0.9,0.85},
		{0.9,-0.85},
		{0.90,0.90},
		{-0.85,0.9}
	};

	glGenBuffers(1,&buffer);
	glBindBuffer(GL_VERTEX_ARRAY,buffer);
	glBufferData(GL_VERTEX_ARRAY,sizeof(vertices),vertices,GL_STATIC_DRAW);
	
	loadShaders();


}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{

	WNDCLASSEX winclass;
	USHORT width = 800;
	USHORT height = 650;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
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
    if(RegisterClass( &dummyWinclass ) == 0)
	{
		MessageBox(NULL,_T("Could not register dummy class"),_T("GLEW failed"),MB_OK);
	}

	HWND dummyHwnd=CreateWindow(_T("DUMMY"),
								_T(""),
								WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW , 
								0,0,0,0,0,0, 
								dummyWinclass.hInstance,
								0 );

	if(dummyHwnd ==  NULL)
	{
		MessageBox(NULL,_T("Could not create dummy window"),_T("GLEW failed"),MB_OK);
	}

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

	pixelformat = ChoosePixelFormat(dummyHdc,&pfd);
	
	if(pixelformat == 0)
	{
		MessageBox(NULL,_T("Could not choose dummy pixel format"),_T("GLEW failed"),MB_OK);
	}

	if(!SetPixelFormat( dummyHdc, pixelformat, &pfd ))
	{
		MessageBox(NULL,_T("Could not set dummy pixel format"),_T("GLEW failed"),MB_OK);
	}

	HGLRC dummyRc = wglCreateContext( dummyHdc );
    wglMakeCurrent(dummyHdc, dummyRc);

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
									WGL_SWAP_METHOD_ARB,WGL_SWAP_EXCHANGE_ARB,
									0};
	float fAttributes[] = { 0, 0 };
	int iPixelFormat;
	UINT numFormats;
	BOOL valid = wglChoosePixelFormatARB(hDC,attributesPixelFormat,fAttributes,1,&iPixelFormat,&numFormats);

	SetPixelFormat( hDC, iPixelFormat, NULL);


	int attributesContext[] = { WGL_CONTEXT_MAJOR_VERSION_ARB,4,
								WGL_CONTEXT_MINOR_VERSION_ARB,3,
								/*WGL_CONTEXT_FLAGS_ARB,WGL_CONTEXT_DEBUG_BIT_ARB,*/
								WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
								0};

	HGLRC hGLRC = wglCreateContextAttribsARB(hDC,NULL,attributesContext);

	wglMakeCurrent(hDC,hGLRC);

	const GLubyte* string = glGetString(GL_VERSION);

	initScene();

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd,msg,wParam,lParam);
}