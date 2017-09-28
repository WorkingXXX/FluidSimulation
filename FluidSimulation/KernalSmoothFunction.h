#pragma once

#ifndef KERNAL_SMOOTH_FUNCTION_H
#define KERNAL_SMOOTH_FUNCTION_H

#include <cmath>
#include <vector>

#include "Vector3d.h"
#include "PhysicsConstant.h"

#define pi 3.1415926535
// Poly6��ƽ������ϵ��
extern float K_POLY6(const float &h);

// SPIKY��ƽ������ϵ��
extern float K_SPIKY(const float &h);

// VISCOSITY��ƽ������ϵ��
extern float K_VISCOSITY(const float &h);


class KSF {

public:

	KSF() {
		density_factor = Particle_Mass * 315 / (64 * pi * pow(Kernal_Radius, 9));
		pressure_factor = Particle_Mass * 45 / (pi * pow(Kernal_Radius, 6));
		viscosity_factor = mu * pressure_factor;
	}
	virtual ~KSF() {}

	float density_factor;
	float pressure_factor;
	float viscosity_factor;

	// Poly6��ƽ������
	float W_poly6_function(const float & r, const float & h) const;
	float W_poly6_function(Vector3d & rv, const float & h) const;

	// Spiky��ƽ������
	float W_spiky_function(const float & r, const float & h) const;
	float W_spiky_function(Vector3d & rv, const float & h) const;
	// Spiky��ƽ������һ�׵�����
	Vector3d W_spiky_function_d1(Vector3d & rv, const float & h) const;

	// Viscosity��ƽ������
	float W_viscosity_function(const float & r, const float & h) const;
	float W_viscosity_function(Vector3d & rv, const float & h) const;
	// Viscosity��ƽ���������׵�����
	float W_viscosity_function_d2(const float & r, const float & h) const;
	float W_viscosity_function_d2(Vector3d & rv, const float & h) const;


};

#endif
