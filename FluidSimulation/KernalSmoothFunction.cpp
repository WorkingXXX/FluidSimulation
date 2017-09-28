
#include "KernalSmoothFunction.h"

float K_POLY6(const float &h) { return 315.0 / (64.0*pi*pow((h), 9)); }
float K_SPIKY(const float &h) { return 15.0 / (pi*pow((h), 6)); }
float K_VISCOSITY(const float &h) { return 15.0 / (2.0*pi*pow((h), 3)); }

float KSF::W_poly6_function(const float & r, const float & h) const {

	//cout << "K_POLY6(h) = " << K_POLY6(h) << endl;
	if (0 <= r && r<= h) 
		return K_POLY6(h) * pow((h * h - r * r), 3);
	else 
		return 0;

}

float KSF::W_poly6_function(Vector3d & rv, const float & h) const {

	float r = rv.get_length();

	if (0 <= r && r<= h) 
		return K_POLY6(h) * pow((h * h - r * r), 3);
	else 
		return 0;

}

float KSF::W_spiky_function(const float & r, const float & h) const {

	if (0 <= r && r<= h) 
		return K_SPIKY(h) * pow((h - r), 3);
	else 
		return 0;

}

float KSF::W_spiky_function(Vector3d & rv, const float & h) const {

	float r = rv.get_length();

	if (0 <= r && r<= h) 
		return K_SPIKY(h) * pow((h - r), 3);
	else 
		return 0;

}	

Vector3d KSF::W_spiky_function_d1(Vector3d & rv, const float & h) const {

	float r = rv.get_length();

	if (0 <= r && r<= h) 
		return -45 * pow((h - r), 2) * rv / (pi * r * pow(h, 6));
	else 
		return Vector3d(0, 0, 0);

}

float KSF::W_viscosity_function(const float & r, const float & h) const {

	if (0 <= r && r<= h) 
		return K_VISCOSITY(h) * (-0.5 * pow(r / h, 3) + pow(r / h, 2) + 0.5 * h / r - 1);
	else 
		return 0;

}

float KSF::W_viscosity_function_d2(const float & r, const float & h) const {

	if (0 <= r && r<= h) 
		return 45 * (h - r) / (pi * pow(h, 6));
	else
		return 0;

}

float KSF::W_viscosity_function_d2(Vector3d & rv, const float & h) const {

	float r = rv.get_length();

	if (0 <= r && r<= h)
		return 45 * (h - r) / (pi * pow(h, 6));
	else 
		return 0;

}