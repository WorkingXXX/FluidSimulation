#pragma once

#ifndef FLUID_SYSTEM_H
#define FLUID_SYSTEM_H

#include <vector>

#include "SPH.h"
#include "Collision.h"
#include "FluidParticle.h"
#include "KernalSmoothFunction.h"

using namespace std;

// 粒子初始以立方体的布局分布
const int PARICLE_SCALE[3] = { 1000, 8000, 27000 };
const int SCALE_LENGTH[3] = { 10, 20, 30 };

class FluidSystem {

private:

	int m_particlesNum;
	float m_smoothRadius; // 粒子平滑半径
	float m_particleRadius; // 粒子半径
	float m_particlesInitDist; // 粒子初始间间距

	CollisionManager m_collisionMgr;

public:

	vector<FluidParticle*> generate_fluid_particles(int particleScale);

	void update_particles_position(vector<FluidParticle> & particles, void(*execute_sph_algorithm)(vector<FluidParticle>)) const;
	void update_particle_position(FluidParticle & particle);

	inline void set_smooth_radius(const float & smoothRadius) { m_smoothRadius = smoothRadius; }
	inline void set_particle_radius(const float & particleRadius) { m_particleRadius = particleRadius; }
	inline void set_particles_init_dist(const float & particlesInitDist) { m_particlesInitDist = particlesInitDist; }

	inline int get_particles_num() const { return m_particlesNum; }
	inline float get_smooth_radius() const { return m_smoothRadius; }
	inline float get_particle_radius() const { return m_particleRadius; }
	inline float get_particles_init_dist() const { return m_particlesInitDist; }
	inline CollisionManager get_collision_manager() const { return m_collisionMgr; }

};

#endif