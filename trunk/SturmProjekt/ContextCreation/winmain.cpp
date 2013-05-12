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
GLuint program;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

GLchar* getShaderCompileError(GLuint shader)
{
	GLint length;
	glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&length);
	GLchar* errorMessage;
	errorMessage = new GLchar[length+1];
	glGetShaderInfoLog(shader,length,NULL,errorMessage);
	return errorMessage;
}

GLchar* getProgramLinkError(GLuint program)
{
	GLint length;
	glGetProgramiv(program,GL_INFO_LOG_LENGTH,&length);
	GLchar* errorMessage;
	errorMessage =  new GLchar[length+1];
	glGetProgramInfoLog(program,length,NULL,errorMessage);

	return errorMessage;
}

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
	if(retVal != GL_TRUE)
	{
		GLchar* errorMsg = getShaderCompileError(vertexShader);
		delete[] errorMsg;
	}
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSource = textFileRead("triangles.frag");
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&retVal);
	if(retVal != GL_TRUE)
	{
		GLchar* errorMsg = getShaderCompileError(fragmentShader);
		delete[] errorMsg;
	}


	program = glCreateProgram();

	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);

	glLinkProgram(program);
	glGetProgramiv(program,GL_LINK_STATUS,&retVal);
	if(retVal!= GL_TRUE)
	{
		GLchar* errorMsg = getProgramLinkError(program);
		delete[] errorMsg;
	}

	
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
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	
	loadShaders();

	glUseProgram(program);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)0);
	glEnableVertexAttribArray(0);

	//glClearColor(1.0,0.0,0.0,1.0);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,6);

	glFlush();

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

	ShowWindow(hWnd,SW_SHOW);

	MSG msg;

    // Start main loop
	while (1) 
    {
        // Did we recieve a message, or are we idling ?
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
        {
			if (msg.message == WM_QUIT) break;
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		} 
        else 
        {
			display();
			SwapBuffers(hDC);

		} // End If messages waiting
	
    } // Until quit message is receieved


}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
    {		
        case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	
	return 0;
}