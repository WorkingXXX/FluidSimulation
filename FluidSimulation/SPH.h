#pragma once

#ifndef SPH_H
#define SPH_H

#include <omp.h>

#include "Collision.h"
#include "Neighbors.h"
#include "FluidParticle.h"
#include "PhysicsConstant.h"
#include "KernalSmoothFunction.h"

class SPH {

private:

	float m_timeStep;

	Neighbors m_neighborsMethod;
	KSF m_ksf;

	// 计算粒子的密度
	void compute_particle_density(FluidParticle * p) const;
	// 计算粒子所受压力
	void compute_particle_pressure(FluidParticle * p) const;
	// 计算粒子所受粘滞力
	void compute_particle_viscosity(FluidParticle * p) const;
	// 计算粒子所受重力
	void compute_particle_gravity(FluidParticle * p) const;
	// 计算粒子的加速度
	void compute_particle_acceleration(FluidParticle * p) const;
	// 计算粒子的速度
	void compute_particle_velocity(FluidParticle * p) const;
	// 计算粒子的位置
	void compute_particle_position(FluidParticle * p) const;

	void collision_detect(FluidParticle * p) const;

public:

	SPH() {}
	virtual ~SPH() {}

	// 执行模拟算法
	void execute_algorithm(vector<FluidParticle*> particles) const;

	inline void set_time_step(const float & timeStep) { m_timeStep = timeStep; }
	inline float get_time_step() const { return m_timeStep; }

};

#endif