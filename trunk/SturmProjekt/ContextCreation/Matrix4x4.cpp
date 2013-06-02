#include "Matrix4x4.h"
#include <cmath>
#include <memory>

Matrix4x4::Matrix4x4()
{}


void Matrix4x4::setLookAt(const Point3D& eye,const Point3D& center,const Vector3D& up)
{
	Vector3D f = center - eye;
	f.normalize();
	Vector3D u = up.normalized();
	Vector3D s = f^u;
	s.normalize();
	u = s^f;

	memset(m_matrixEntries,0,sizeof(float)*16);

	m_matrixEntries[0] = 1.0;
	m_matrixEntries[1*4+1] = 1.0;
	m_matrixEntries[2*4+2] = 1.0;
	m_matrixEntries[3*4+3] = 1.0;
	
	m_matrixEntries[0] = s.x;
	m_matrixEntries[1*4+0] = s.y;
	m_matrixEntries[2*4+0] = s.z;
	m_matrixEntries[0*4+1] = u.x;
	m_matrixEntries[1*4+1] = u.y;
	m_matrixEntries[2*4+1] = u.z;
	m_matrixEntries[0*4+2] = -f.x;
	m_matrixEntries[1*4+2] = -f.y;
	m_matrixEntries[2*4+2] = -f.z;
	m_matrixEntries[3*4+0] = -(s*eye);
	m_matrixEntries[3*4+1] = -(u*eye);
	m_matrixEntries[3*4+2] = f*eye;



}

void Matrix4x4::setPerspective(float fovy,float aspect,float znear,float zfar)
{
	float const radianFOV = fovy*(3.1415926535897932384626433832795/180.0);
	float range = tan(radianFOV/2.0)*znear;
	float left = -range *aspect;
	float right = range*aspect;
	float bottom = - range;
	float top = range;
	
	memset(m_matrixEntries,0,sizeof(float)*16);

	m_matrixEntries[0] = (2.0*znear)/(right-left);
	m_matrixEntries[1*4+1] = (2.0*znear)/(top - bottom);
	m_matrixEntries[2*4+2] = -(zfar + znear)/(zfar-znear);
	m_matrixEntries[2*4+3] = -1.0;
	m_matrixEntries[2*3+2] = - (2.0 * zfar * znear) / (zfar- znear);

}


bool Matrix4x4::setMatrixToProgram(GLuint program,const char* location)
{
	GLuint matrixLocation = glGetUniformLocation(program,location);

	//glUseProgram(program);

	glUniformMatrix4fv(matrixLocation,1,GL_FALSE,m_matrixEntries);



	return true;
}