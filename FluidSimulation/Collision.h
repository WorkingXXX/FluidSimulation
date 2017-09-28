#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include <vector>
#include <cmath>

#include "Vector3d.h"
#include "FluidParticle.h"
#include "PhysicsConstant.h"

using namespace std;

// ����������ƽ��
enum AxisPlane {
	XoY = 1,
	YoZ = 2,
	ZoX = 3
};

// ��ײ����ļ�������
enum CollisionType {
	Triangle,
	Box
};

// ����ƽ���λ�ù�ϵ
enum PosRelation {
	Above = 1,
	On = 0,
	Under = -1
};

// ��λ��������
const Vector3d NAV[6] = {
	Vector3d(1, 0, 0),  // ��x��������
	Vector3d(-1, 0, 0), // ��x�Ḻ����
	Vector3d(0, 1, 0),  // ��y��������
	Vector3d(0, -1, 0), // ��y�Ḻ����
	Vector3d(0, 0, 1),  // ��z��������
	Vector3d(0, 0, -1)  // ��z�Ḻ����
};

////////////////////////////////////////////////////////

/* CollisionObject */

class CollisionObject {

public:

	CollisionObject() {}
	~CollisionObject() {}

	virtual bool collision_detect(FluidParticle & p) const = 0;

};

///////////////////////////////////////////////////////

/* CollisionTriangle */

class CollisionTriangle {

private:

	Vector3d m_a, m_b, m_c;

	CollisionType type = Triangle;
	
	// ��������������άƽ�淽�̣�Ax + By + Cz + D = 0
	float A, B, C, D;

public:

	CollisionTriangle() {}
	CollisionTriangle(const CollisionTriangle & ctri) {
		m_a = ctri.get_a();
		m_b = ctri.get_b();
		m_c = ctri.get_c();
	}
	~CollisionTriangle() {}

	inline void set_a(const Vector3d & a) { m_a = a; }
	inline void set_b(const Vector3d & b) { m_b = b; }
	inline void set_c(const Vector3d & c) { m_c = c; }
	
	inline Vector3d get_a() const { return m_a; }
	inline Vector3d get_b() const { return m_b; }
	inline Vector3d get_c() const { return m_c; }

	inline bool is_coincident_with(const CollisionTriangle & ct) const {
		return (m_a == ct.get_a()) && (m_b == ct.get_b()) && (m_c == ct.get_c());
	}

	// ����������
	bool create_triangle(const Vector3d & a, const Vector3d & b, const Vector3d & c);

	// �ж�3�������ͶӰ���ĸ�ƽ��
	AxisPlane can_project_to(const Vector3d & a, const Vector3d & b, const Vector3d & c) const;

	// �ж�3�����Ƿ��ܹ���������
	bool is_triangle(const Vector3d & a, const Vector3d & b, const Vector3d & c) const;

	// �������λ��˳�����Ϊ��ʱ��˳��
	void adjust_vertexes_to_CCW();

	// �жϵ�������������ƽ���λ�ù�ϵ
	PosRelation judge_pos_relation(const Vector3d & p) const;

	// ����ƽ�浽��ľ���
	float distance_to_point(const Vector3d & p) const;

	virtual bool collision_detect(FluidParticle & p) const { return true; }

};

//////////////////////////////////////////////////////

/* CollisionBox */

class CollisionBox : public CollisionObject{

private:

	vector<Vector3d> m_vertexes;

public:

	CollisionBox(const Vector3d & minP, const Vector3d & maxP);
	virtual ~CollisionBox() {}

	virtual bool collision_detect(FluidParticle & p) const;

};

/////////////////////////////////////////////////////

/* CollisionManager */

class CollisionManager {

private:

	vector<CollisionTriangle> m_collisionTriangles;
	vector<CollisionBox> m_collisionBoxes;

public:

	CollisionManager() {}
	~CollisionManager() {}

	void collision_detect(FluidParticle & p) const;

	// ��������ײ���
	bool triangle_collision_detect(const CollisionTriangle & ctri, const FluidParticle & p) const;

	// ������ײʱ�����������ٶ�
	void adjust_particle_velocity(const CollisionTriangle & ctri, FluidParticle & p) const;

	// ������ײʱ����������λ��
	void adjust_particle_position(const CollisionTriangle & ctri, FluidParticle & p) const;

	inline void add_collision_triangle(const CollisionTriangle & ctri) { 
		m_collisionTriangles.push_back(ctri); 
	}

	inline void add_collision_box(const CollisionBox & cbox) {
		m_collisionBoxes.push_back(cbox);
	}

	inline vector<CollisionTriangle> get_collision_triangles() const { return m_collisionTriangles; }
	inline vector<CollisionBox> get_collision_boxes() const { return m_collisionBoxes; }
};

#endif