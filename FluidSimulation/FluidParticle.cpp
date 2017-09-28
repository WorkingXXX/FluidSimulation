
#include "FluidParticle.h"

void FluidParticle::init_() {
	density = 0;
	mass = Particle_Mass;
	smoothRadius = Kernal_Radius;
	pos = prePos = vel = acc = pressure = viscosity =  Vector3d(0, 0, 0);
	gravity = Vector3d(0, 0, -G);
}

void FluidParticle::print_info() {

	cout << "mass : " << mass << endl;
	cout << "density : " << density << endl;
	cout << "position : "; pos.print(); 
	cout << "prePosition : "; prePos.print(); 
	cout << "velcoity : "; vel.print();
	cout << "pressure : "; pressure.print();
	cout << "viscosity : "; viscosity.print();
	cout << "gravity : "; gravity.print(); 
	cout << "size of neighbors : " << neighborsParticles.size() << endl;

}