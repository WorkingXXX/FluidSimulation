
#include "Collision.h"

/* CollisionTriangle */

bool CollisionTriangle::create_triangle(const Vector3d & a, const Vector3d & b, const Vector3d & c) {

	// 若存在重合的点，则创建失败
	if (a == b || b == c || c == a) {
		cout << "Error : Fail to create CollisionTriangle" << endl;
		cout << "Caused by : Existing coincident points" << endl;
		return false;
	}

	// 若三点共线，则创建失败
	if (!is_triangle(a, b, c)) {
		cout << "Error : Fail to create CollisionTriangle" << endl;
		cout << "Caused by : Three points collinear" << endl;
		return false;
	}

	m_a = a;
	m_b = b;
	m_c = c;
	
	adjust_vertexes_to_CCW();

	float ax = a.get_x(), ay = a.get_y(), az = a.get_z();
	float bx = b.get_x(), by = b.get_y(), bz = b.get_z();
	float cx = c.get_x(), cy = c.get_y(), cz = c.get_z();

	A = (cy - ay) * (cz - az) - (bz - az) * (cy - ay);
	B = (cx - ax) * (bz - az) - (bx - ax) * (cz - az);
	C = (bx - ax) * (cy - ay) - (cx - ax) * (by - ay);
	D = -(A*ax + B*ay + C*az);

	return true;
}

AxisPlane CollisionTriangle::can_project_to(const Vector3d & a, const Vector3d & b, const Vector3d & c) const {

	float ax = a.get_x(), ay = a.get_y(), az = a.get_z();
	float bx = b.get_x(), by = b.get_y(), bz = b.get_z();
	float cx = c.get_x(), cy = c.get_y(), cz = c.get_z();

	if (abs(((cx - ax) * (by - ay)) - ((cy - ay) * (bx - ax))) < EPSILON) 
		return XoY;
	if (abs(((cz - az) * (by - ay)) - ((cy - ay) * (bz - az))) < EPSILON) 
		return YoZ;
	if (abs(((cx - ax) * (bz - az)) - ((cz - az) * (bx - ax))) < EPSILON) 
		return ZoX;

}

// 空间中的直线方程为：(x-x1)/(x2-x1) = (y-y1)/(y2-y1) = (z-z1)/(z2-z1)
bool CollisionTriangle::is_triangle(const Vector3d & a, const Vector3d & b, const Vector3d & c) const {

	float ax = a.get_x(), ay = a.get_y(), az = a.get_z();
	float bx = b.get_x(), by = b.get_y(), bz = b.get_z();
	float cx = c.get_x(), cy = c.get_y(), cz = c.get_z();

	return !(abs((cx - ax)*(by - ay) - (bx - ax)*(cy - ay)) < EPSILON
		&& abs((cy - ay)*(bz - az) - (cz - az)*(by - ay)) < EPSILON);

}

void CollisionTriangle::adjust_vertexes_to_CCW() {

	float ax = m_a.get_x(), ay = m_a.get_y(), az = m_a.get_z();
	float bx = m_b.get_x(), by = m_b.get_y(), bz = m_b.get_z();
	float cx = m_c.get_x(), cy = m_c.get_y(), cz = m_c.get_z();

	Vector3d a, b, c;

	switch (can_project_to(m_a, m_b, m_c)) {
	case XoY:
		if (ax <= bx <= cx) { a = m_b; b = m_c; c = m_a; }
		else if (ax <= cx <= bx) { a = m_c; b = m_b; a = m_a; }
		else if (bx <= ax <= cx) { a = m_a; b = m_c; c = m_b; }
		else if (bx <= cx <= ax) { a = m_c; b = m_a; c = m_a; }
		else if (cx <= ax <= bx) { a = m_a; b = m_b; c = m_c; }
		else if (cx <= bx <= ax) { a = m_b; b = m_a; a = m_c; }	
		break;
	case YoZ:
		if (ay <= by <= cy) { a = m_b; b = m_c; c = m_a; }
		else if (ay <= cy <= by) { a = m_c; b = m_b; a = m_a; }
		else if (by <= ay <= cy) { a = m_a; b = m_c; c = m_b; }
		else if (by <= cy <= ay) { a = m_c; b = m_a; c = m_a; }
		else if (cy <= ay <= by) { a = m_a; b = m_b; c = m_c; }
		else if (cy <= by <= ay) { a = m_b; b = m_a; a = m_c; }
		break;
	case ZoX:
		if (az <= bz <= cz) { a = m_b; b = m_c; c = m_a; }
		else if (az <= cz <= bz) { a = m_c; b = m_b; a = m_a; }
		else if (bz <= az <= cz) { a = m_a; b = m_c; c = m_b; }
		else if (bz <= cz <= az) { a = m_c; b = m_a; c = m_a; }
		else if (cz <= az <= bz) { a = m_a; b = m_b; c = m_c; }
		else if (cz <= bz <= az) { a = m_b; b = m_a; a = m_c; }
		break;
	default:
		break;
	}

	m_a = a;
	m_b = b;
	m_c = c;

}

PosRelation CollisionTriangle::judge_pos_relation(const Vector3d & p) const {

	float value = A * p.get_x() + B * p.get_y() + C * p.get_z() + D;

	if (value > EPSILON) return Above;
	else if (value < -EPSILON) return Under;
	else return On;

}

float CollisionTriangle::distance_to_point(const Vector3d & p) const {

	float v1 = A * p.get_x() + B * p.get_y() + C * p.get_z() + D;
	float v2 = sqrt(A * A + B * B + C * C);

	return v1 / v2;
}

/* CollisionBox */

CollisionBox::CollisionBox(const Vector3d & minP, const Vector3d & maxP) {

	float x1 = minP.get_x(), y1 = minP.get_y(), z1 = minP.get_z();
	float x2 = maxP.get_x(), y2 = maxP.get_y(), z2 = maxP.get_z();

	Vector3d v1 = minP; m_vertexes.push_back(v1);
	Vector3d v2(x2, y1, z1); m_vertexes.push_back(v2);
	Vector3d v3(x2, y2, z1); m_vertexes.push_back(v3);
	Vector3d v4(x1, y2, z1); m_vertexes.push_back(v4);
	Vector3d v5(x1, y1, z2); m_vertexes.push_back(v5);
	Vector3d v6(x2, y1, z2); m_vertexes.push_back(v6);
	Vector3d v7 = maxP; m_vertexes.push_back(v7);
	Vector3d v8(x1, y2, z2); m_vertexes.push_back(v8);

}

bool CollisionBox::collision_detect(FluidParticle & p) const{

	p.prePos = p.pos;

	bool collisionHappened = false;

	Vector3d minP = m_vertexes[0];
	Vector3d maxP = m_vertexes[6];

	if (p.pos.get_x() < minP.get_x()) {
		p.pos.set_x(minP.get_x());
		p.vel -= 2 * NAV[0] * p.vel.dot(NAV[0]);
		p.vel.set_x(p.vel.get_x() * Damping);
		collisionHappened = true;
	}

	if (p.pos.get_x() > maxP.get_x()) {
		p.pos.set_x(maxP.get_x());
		p.vel -= 2 * NAV[1] * p.vel.dot(NAV[1]);
		p.vel.set_x(p.vel.get_x() * Damping);
		collisionHappened = true;
	}

	if (p.pos.get_y() < minP.get_y()) {
		p.pos.set_y(minP.get_y());
		p.vel -= 2 * NAV[2] * p.vel.dot(NAV[2]);
		p.vel.set_y(p.vel.get_y() * Damping);
		collisionHappened = true;
	}

	if (p.pos.get_y() > maxP.get_y()) {
		p.pos.set_y(maxP.get_y());
		p.vel -= 2 * NAV[3] * p.vel.dot(NAV[3]);
		p.vel.set_y(p.vel.get_y() * Damping);
		collisionHappened = true;
	}

	if (p.pos.get_z() < minP.get_z()) {
		p.pos.set_z(minP.get_z());
		p.vel -= 2 * NAV[4] * p.vel.dot(NAV[4]);
		p.vel.set_z(p.vel.get_z() * Damping);
		collisionHappened = true;
	}

	if (p.pos.get_z() > maxP.get_z()) {
		p.pos.set_x(maxP.get_z());
		p.vel -= 2 * NAV[5] * p.vel.dot(NAV[5]);
		p.vel.set_z(p.vel.get_z() * Damping);
		collisionHappened = true;
	}
	
	return collisionHappened;
}

/* CollisionManager */

void CollisionManager::collision_detect(FluidParticle & p) const {

	for (int i = 0; i < m_collisionTriangles.size(); i++) {
		if (triangle_collision_detect(m_collisionTriangles[i], p)) {
			adjust_particle_velocity(m_collisionTriangles[i], p);
			adjust_particle_position(m_collisionTriangles[i], p);
		}
	}

}

bool CollisionManager::triangle_collision_detect(const CollisionTriangle & ctri, const FluidParticle & p) const {

	// 判断点此时刻与上一时刻与三角面所在平面是否位于同一侧
	int posRelFlag = ctri.judge_pos_relation(p.pos) * ctri.judge_pos_relation(p.prePos);

	// 标志同号或为0说明没有发生碰撞
	if (posRelFlag == 1 || posRelFlag == 0) return false;

	return true;

}

void CollisionManager::adjust_particle_velocity(const CollisionTriangle & ctri, FluidParticle & p) const {
	


}

void CollisionManager::adjust_particle_position(const CollisionTriangle & ctri, FluidParticle & p) const {

}