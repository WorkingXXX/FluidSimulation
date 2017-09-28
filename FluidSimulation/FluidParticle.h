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
	float mass; // 质量
	float radius;
	float density; // 密度
	float smoothRadius; // 平滑半径
	Vector3d pos; // 位置
	Vector3d prePos; // 上一刻的位置，用于碰撞检测
 	Vector3d vel; // 速度
	Vector3d acc; // 加速度
	Vector3d pressure; // 压力
	Vector3d viscosity; // 粘滞力
	Vector3d gravity; // 重力
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
