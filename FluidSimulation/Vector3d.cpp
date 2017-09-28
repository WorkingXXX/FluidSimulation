
#include "Vector3d.h"

Vector3d::Vector3d(const Vector3d & v) {

	m_x = v.m_x;
	m_y = v.m_y;
	m_z = v.m_z;

}

Vector3d Vector3d::operator + (const Vector3d & v) const{

	return Vector3d(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);

}

void Vector3d::operator += (const Vector3d & v) {

	m_x += v.m_x;
	m_y += v.m_y;
	m_z += v.m_z;

}

Vector3d Vector3d::operator - (const Vector3d & v) const {

	Vector3d result(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
	return result;

}

Vector3d Vector3d::operator - () {

	m_x = -m_x;
	m_y = -m_y;
	m_z = -m_z;

	return *this;

}

void Vector3d::operator -= (const Vector3d & v) {

	m_x -= v.m_x;
	m_y -= v.m_y;
	m_z -= v.m_z;

}

Vector3d operator * (const Vector3d & v, const float & f){
	
	return Vector3d(v.get_x() * f, v.get_y() * f, v.get_z() * f);

}

void Vector3d::operator *= (const float & f){

	m_x *= f;
	m_y *= f;
	m_z *= f;

}

Vector3d operator * (const Vector3d & v, const double & f){

	return Vector3d(v.get_x() * f, v.get_y() * f, v.get_z() * f);

}

Vector3d operator * (const double & f, const Vector3d & v) {

	return v * f;

}

void Vector3d::operator *= (const double & f) {

	m_x *= f;
	m_y *= f;
	m_z *= f;

}

Vector3d Vector3d::operator / (const float & f) const {

	//if (abs(f) < EPSILON) {

	//	std::cout << "Error : denominator can't be 0" << std::endl;
	//
	//}

	return Vector3d(m_x / f, m_y / f, m_z / f);

}

bool Vector3d::operator == (const Vector3d & v) const {
	return (m_x == v.get_x()) && (m_y == v.get_y()) && (m_z == v.get_z()) && (m_w == v.get_w());
}

float Vector3d::dot(const Vector3d & v) const {

	return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;

}

Vector3d Vector3d::cross(const Vector3d & v) const {

	return Vector3d(m_y * v.m_z - m_z * v.m_y,
					m_z * v.m_x - m_x * v.m_z,
					m_x * v.m_y - m_y * v.m_x);
	
}

void Vector3d::normalize() {

	float length = get_length();

	if (length <= EPSILON) return;

	m_x /= length;
	m_y /= length;
	m_z /= length;

}

float Vector3d::distance_to(const Vector3d & v) const {

	return sqrt((m_x - v.m_x) * (m_x - v.m_x)+
		(m_y - v.m_y) * (m_y - v.m_y)+
		(m_z - v.m_z) * (m_z - v.m_z));

}

void Vector3d::print() const {

	cout << "x = " << m_x << ", "
		<< "y = " << m_y << ", "
		<< "z = " << m_z << endl;
 
}