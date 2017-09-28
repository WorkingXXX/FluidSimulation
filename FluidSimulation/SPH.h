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

	// �������ӵ��ܶ�
	void compute_particle_density(FluidParticle * p) const;
	// ������������ѹ��
	void compute_particle_pressure(FluidParticle * p) const;
	// ������������ճ����
	void compute_particle_viscosity(FluidParticle * p) const;
	// ����������������
	void compute_particle_gravity(FluidParticle * p) const;
	// �������ӵļ��ٶ�
	void compute_particle_acceleration(FluidParticle * p) const;
	// �������ӵ��ٶ�
	void compute_particle_velocity(FluidParticle * p) const;
	// �������ӵ�λ��
	void compute_particle_position(FluidParticle * p) const;

	void collision_detect(FluidParticle * p) const;

public:

	SPH() {}
	virtual ~SPH() {}

	// ִ��ģ���㷨
	void execute_algorithm(vector<FluidParticle*> particles) const;

	inline void set_time_step(const float & timeStep) { m_timeStep = timeStep; }
	inline float get_time_step() const { return m_timeStep; }

};

#endif