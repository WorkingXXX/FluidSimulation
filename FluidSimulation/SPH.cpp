
#include "SPH.h"

void SPH::compute_particle_density(FluidParticle * p) const {

	float density = 0;
	vector<FluidParticle*> nps = p->neighborsParticles;
	//cout << "nps size = " << nps.size() << endl;
	for (int i = 0; i < nps.size(); i++) {
		float dist = p->pos.distance_to(nps[i]->pos);
		//cout << "dist = " << dist << endl;
		//cout << "W_poly6 = " << m_ksf.W_poly6_function(dist, p->smoothRadius) << endl;
		density += pow(Kernal_Radius * Kernal_Radius - dist * dist, 3);
	}
	p->density = m_ksf.density_factor * density;
	//cout << p->density << endl;
}

void SPH::compute_particle_pressure(FluidParticle * p) const {

	Vector3d pressure(0, 0, 0);
	vector<FluidParticle*> nps = p->neighborsParticles;
	//cout << "density = " << p->density << endl;

	// ¼ÆËãÑ¹Ç¿
	float p_j = K * (p->density - RHO);
	float d_j = p->density;
	Vector3d r_j = p->pos;
	//cout << "p_j pos : ";
	//p->pos.print();
	//cout << "p_j neighbors size " << nps.size() << endl;
	for (int i = 0; i < nps.size(); i++) {
		float p_i = K * (nps[i]->density - RHO);
		Vector3d r_i = nps[i]->pos;
		float dist = p->pos.distance_to(nps[i]->pos);
		float d_i = nps[i]->density;
		//cout << "nps[i]->mass : " << nps[i]->mass << endl;
		//cout << "nps[i]->pos : "; nps[i]->pos.print();
		//cout << "p_i + p_j : " << (p_i + p_j) << endl;
		//cout << "W_spiky : ";
		//m_ksf.W_spiky_function_d1(p->pos - nps[i]->pos, p->smoothRadius).print();
		//cout << "nps[i]->density : " << nps[i]->density << endl;
		pressure -= 0.5 * (p_i + p_j) * (Kernal_Radius - dist) * (Kernal_Radius - dist)
			* (r_j - r_i) / (dist * d_i);
	}
	p->pressure = m_ksf.pressure_factor * pressure / d_j;
	if (abs(p->pressure.get_x()) > 0.01) p->pressure.set_x(0);
	if (abs(p->pressure.get_y()) > 0.01) p->pressure.set_y(0);
	if (abs(p->pressure.get_z()) > 0.01) p->pressure.set_z(0);
	//cout << "pressure = ";
	//p->pressure.print();
}

void SPH::compute_particle_viscosity(FluidParticle * p) const {

	Vector3d viscosity(0, 0, 0);

	vector<FluidParticle*> nps = p->neighborsParticles;
	Vector3d u_j = p->vel;
	Vector3d r_j = p->pos;
	float d_j = p->density;

	for (int i = 0; i < nps.size(); i++) {
		float d_i = nps[i]->density;
		Vector3d u_i = nps[i]->vel;
		Vector3d r_i = nps[i]->pos;
		float mass_i = nps[i]->mass;
		float dist = r_i.distance_to(r_j);
		/*cout << "u_j : "; u_j.print();
		cout << "u_i : "; u_i.print();
		cout << "distance : ";
		(u_i - u_j).print();
		cout << "W_viscosity : " << m_ksf.W_viscosity_function_d2(r_j - r_i, h) << endl;*/
		viscosity += (u_i - u_j) * (Kernal_Radius - dist) / d_i;
	}
	p->viscosity = m_ksf.viscosity_factor * viscosity / d_j;

}

void SPH::compute_particle_gravity(FluidParticle * p) const {

	//p->gravity = p->mass * G * Vector3d(0, 0, -1);

}

void SPH::compute_particle_acceleration(FluidParticle * p) const {

	//p->acc = (p->pressure + p->viscosity + p->gravity);
	p->acc = p->pressure + p->gravity;
	//p->acc.print();
	
}

void SPH::compute_particle_velocity(FluidParticle * p) const {
	p->vel += p->acc * m_timeStep;
	//cout << "acc : ";
	//p->acc.print();
	//cout << "next vel : ";
	//p->vel.print();
}

void SPH::compute_particle_position(FluidParticle * p) const {

	p->prePos = p->pos;
	p->pos += p->vel * m_timeStep;
	//cout << "next pos : ";
	//p->pos.print();

}

void SPH::collision_detect(FluidParticle * p) const {

	p->prePos = p->pos;

	bool collisionHappened = false;

	Vector3d * minP = new Vector3d(Lower_XYZ, Lower_XYZ, Lower_XYZ);
	Vector3d * maxP = new Vector3d(Upper_XYZ, Upper_XYZ, Upper_XYZ);

	/*float x = p->pos.get_x(); float y = p->pos.get_y(); float z = p->pos.get_z();
	float minX = minP->get_x(); float minY = minP->get_y(); float minZ = minP->get_z();
	float maxX = maxP->get_x(); float maxY = maxP->get_y(); float maxZ = minP->get_z();

	float diff = */

	if (p->pos.get_x() < minP->get_x()) {
		p->pos.set_x(minP->get_x());
		p->vel -= 2 * NAV[0] * p->vel.dot(NAV[0]);
		p->vel.set_x(p->vel.get_x() * Damping);
		collisionHappened = true;
	}

	if (p->pos.get_x() > maxP->get_x()) {
		p->pos.set_x(maxP->get_x());
		p->vel -= 2 * NAV[1] * p->vel.dot(NAV[1]);
		p->vel.set_x(p->vel.get_x() * Damping);
		collisionHappened = true;
	}

	if (p->pos.get_y() < minP->get_y()) {
		p->pos.set_y(minP->get_y());
		p->vel -= 2 * NAV[2] * p->vel.dot(NAV[2]);
		p->vel.set_y(p->vel.get_y() * Damping);
		collisionHappened = true;
	}

	if (p->pos.get_y() > maxP->get_y()) {
		p->pos.set_y(maxP->get_y());
		p->vel -= 2 * NAV[3] * p->vel.dot(NAV[3]);
		p->vel.set_y(p->vel.get_y() * Damping);
		collisionHappened = true;
	}

	if (p->pos.get_z() < minP->get_z()) {
		p->pos.set_z(minP->get_z());
		p->vel -= 2 * NAV[4] * p->vel.dot(NAV[4]);
		p->vel.set_z(p->vel.get_z() * Damping);
		collisionHappened = true;
	}

	if (p->pos.get_z() > maxP->get_z()) {
		p->pos.set_z(maxP->get_z());
		p->vel -= 2 * NAV[5] * p->vel.dot(NAV[5]);
		p->vel.set_z(p->vel.get_z() * Damping);
		collisionHappened = true;
	}

}

void SPH::execute_algorithm(vector<FluidParticle*> particles) const {

	int particlesNum = particles.size();

	//particles[0]->print_info();
#pragma omp parallel for
	for (int i = 0; i < particlesNum; i++) {
		FluidParticle * p = particles[i];
		m_neighborsMethod.find_neighbors(particles, p);
	}

#pragma omp parallel for
	for (int i = 0; i < particlesNum; i++) {
		FluidParticle * p = particles[i];
		compute_particle_density(p);
	}

#pragma omp parallel for
	for (int i = 0; i < particlesNum; i++) {
		FluidParticle * p = particles[i];
		compute_particle_pressure(p);
		compute_particle_viscosity(p);
		compute_particle_gravity(p);
		compute_particle_acceleration(p);
	}

#pragma omp parallel for
	for (int i = 0; i < particlesNum; i++) {
		FluidParticle * p = particles[i];
		compute_particle_velocity(p);
		compute_particle_position(p);
		collision_detect(p);
	}

}

