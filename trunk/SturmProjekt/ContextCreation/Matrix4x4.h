#ifndef _MATRIX4X4_H_
#define _MATRIX4X4_H_

#include "GL\glew.h"
#include "Vector3D.h"
#include "Point3D.h"

class Matrix4x4
{
public:
	Matrix4x4();
	void setPerspective(float fovy,float aspect,float znear,float zfar);
	void setLookAt(const Point3D& eye,const Point3D& center,const Vector3D& up);
	bool setMatrixToProgram(GLuint program,const char* location);
private:
	float m_matrixEntries[16];
};

#endif