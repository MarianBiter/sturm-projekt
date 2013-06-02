#include <cmath>

#include "Vector3D.h"
#include "Point3D.h"

Vector3D::Vector3D(void): x(0.0), y(0.0), z(0.0){}

Vector3D::Vector3D(double a): x(a), y(a), z(a){}

Vector3D::Vector3D(double _x, double _y, double _z): x(_x), y(_y), z(_z){}

Vector3D::Vector3D(const Vector3D& vector): x(vector.x), y(vector.y), z(vector.z){}

Vector3D::Vector3D(const Point3D& p): x(p.x), y(p.y), z(p.z){}

Vector3D::~Vector3D (void) {}

Vector3D& Vector3D::operator= (const Vector3D& rhs) 
{
	if (this == &rhs)
		return (*this);

	x = rhs.x; y = rhs.y; z = rhs.z;

	return (*this);
}


Vector3D& Vector3D::operator= (const Point3D& rhs) 
{
	x = rhs.x; y = rhs.y; z = rhs.z;
	return (*this);
}


double Vector3D::length(void) 
{
	return (sqrt(x * x + y * y + z * z));
}

void Vector3D::normalize(void) 
{	
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}

Vector3D& Vector3D::normalized() const
{
	double length = sqrt(x * x + y * y + z * z);
	Vector3D rez(x/length,y/length,z/length);
	return rez;
}

Vector3D& Vector3D::hat(void) 
{	
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
	return (*this);
} 


std::string Vector3D::toString()
{
	char buffer[50];
	sprintf_s(buffer,"%f",x);
	char buffer2[50];
	sprintf_s(buffer2,"%f",y);
	char buffer3[50];
	sprintf_s(buffer3,"%f",z);
	return "("+std::string(buffer)+","+std::string(buffer2)+","+std::string(buffer3)+")";
}
