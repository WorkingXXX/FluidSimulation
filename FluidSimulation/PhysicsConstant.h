#pragma once

#ifndef PHYSICS_CONSTANT_H
#define PHYSICS_CONSTANT_H

#include "Vector3d.h"

// ��������
const float G = 9.8;

// ����ľ�̬�ܶ�
const float RHO = 1000.0;

// ��������״̬���̳���
const float K = 1.0;

// ����ճ��ϵ��
const float mu = 1.0;

// ������ײ����߽���ٶȵ�˥��ϵ��
const float Damping = 0.5;

const float Particle_Radius = 0.01;

const float Internal_Stiffness = 1000.0;

const float Boundary_Stiffness = 25000.0;

const float Boundary_Damp = 100.0;

const float Particle_Mass = 0.001;

const float Kernal_Radius = 0.03;

const float Lower_XYZ = -0.05;

const float Upper_XYZ = 0.3;

#endif
