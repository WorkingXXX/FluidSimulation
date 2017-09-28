
#include "FluidSystem.h"

vector<FluidParticle*> FluidSystem::generate_fluid_particles(int particleScale){

	if (particleScale < 1) particleScale = 1;
	if (particleScale > 3) particleScale = 3;
	
	int particlesNum = PARICLE_SCALE[particleScale - 1];
	m_particlesNum = particlesNum;
	int imax = SCALE_LENGTH[particleScale - 1] - 1;

	vector<FluidParticle*> new_particles;

	int ix = 0, iy = 0, iz = 0;
	Vector3d zero(0, 0, 0);

	for (int i = 0; i < particlesNum; i++) {

		FluidParticle *p = new FluidParticle;
		p->index = i;
		p->init_();
		p->pos += Vector3d(m_particlesInitDist * ix, m_particlesInitDist * iy, m_particlesInitDist * iz);
		
		if (ix != imax) ix++;
		else if (ix == imax && iy != imax) { iy++; ix = 0; }
		else if (ix == imax && iy == imax) { iz++; ix = iy = 0; }

		new_particles.push_back(p);
	}

	return new_particles;
}

void FluidSystem::update_particles_position(vector<FluidParticle> & particles, void(*execute_sph_algorithm)(vector<FluidParticle>))const {

	execute_sph_algorithm(particles);

	for (int i = 0; i < particles.size(); i++) {

		m_collisionMgr.collision_detect(particles[i]);

	}

}
