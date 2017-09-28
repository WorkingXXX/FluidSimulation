#pragma once

#ifndef FLUID_PARTICLE_H
#define FLUID_PARTICLE_H

#include <vector>

#include "Vector3d.h"
#include "PhysicsConstant.h"

using namespace std;

class FluidParticle {

public:

	int index;
	float mass; // ����
	float radius;
	float density; // �ܶ�
	float smoothRadius; // ƽ���뾶
	Vector3d pos; // λ��
	Vector3d prePos; // ��һ�̵�λ�ã�������ײ���
 	Vector3d vel; // �ٶ�
	Vector3d acc; // ���ٶ�
	Vector3d pressure; // ѹ��
	Vector3d viscosity; // ճ����
	Vector3d gravity; // ����
	vector<FluidParticle*> neighborsParticles;

	FluidParticle() {}

	FluidParticle(const float & mass_, const float & density_, const float & smoothRadius_,
		const Vector3d & pos_, const Vector3d & vel_, const Vector3d & acc_,
		const Vector3d & pressure_, const Vector3d & viscosity_, const Vector3d & gravity_)
		:mass(mass_), density(density_), smoothRadius(smoothRadius_), pos(pos_), vel(vel_), acc(acc),
		pressure(pressure_), viscosity(viscosity_), gravity(gravity_) {}

	FluidParticle(const FluidParticle & p) {

		mass = p.mass;
		density = p.density;
		smoothRadius = p.smoothRadius;
		pos = p.pos;
		vel = p.vel;
		acc = p.acc;
		pressure = p.pressure;
		viscosity = p.viscosity;
		gravity = p.gravity;

	}

	void init_();

	void print_info();
};

#endif
