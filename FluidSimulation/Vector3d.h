#pragma once

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <cmath>

#define EPSILON 0.00000001

using namespace std;

class Vector3d {

private:

	float m_x;
	float m_y;
	float m_z;
	float m_w = 0.0;

	float m_length = -1;

public:

	Vector3d() :m_x(0), m_y(0), m_z(0) {}
	Vector3d(const float & x, const float & y, const float & z) :m_x(x), m_y(y), m_z(z) {}
	Vector3d(const Vector3d & v);

	~Vector3d() {}

	inline void set_x(const float & x) { m_x = x; }
	inline void set_y(const float & y) { m_y = y; }
	inline void set_z(const float & z) { m_z = z; }
	inline void set_w(const float & w) { m_w = w; }
	inline void set(const float & x, const float & y, const float & z) {
		m_x = x; m_y = y; m_z = z;
	}

	inline float get_x() const { return m_x; }
	inline float get_y() const { return m_y; }
	inline float get_z() const { return m_z; }
	inline float get_w() const { return m_w; }

	// 运算符 +、+=、-、-=、*、/ 重载
	Vector3d operator + (const Vector3d & v) const;
	void operator += (const Vector3d & v);
	Vector3d operator - (const Vector3d & v) const;
	Vector3d operator - ();
	void operator -= (const Vector3d & v);
	friend Vector3d operator * (const Vector3d & v, const float & f);
	void operator *= (const float & f);
	friend Vector3d operator * (const Vector3d & v, const double & f);
	friend Vector3d operator * (const double & f, const Vector3d & v);
	void operator *= (const double & f);
	Vector3d operator / (const float & f) const;
	bool operator == (const Vector3d & v) const;

	// 向量点乘、叉乘运算
	float dot(const Vector3d & v) const;
	Vector3d cross(const Vector3d & v) const;

	// 向量单位化
	void normalize();

	// 获取向量模长
	inline float get_length(){ 

		if (m_length == -1) m_length = sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
		return m_length;

	}

	// 计算到另一个向量的距离
	float distance_to(const Vector3d & v) const;

	void print() const;

};

#endif
