#pragma once

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <iostream>
#include <vector>
#include <fstream>

#include "OpenGL\glew.h"
#include "OpenGL\freeglut.h"

#include "Vector3d.h"
#include "Collision.h"

using namespace std;

#define BUFFER_OFFSET(offset) ((void*)(offset))

// ���������ڶ�����ɫ���е�����
enum VertexShaderAttribIndex {
	PARTICLES_POS = 0
};

// ����������ƬԪ��ɫ���е�����
enum FragmentShaderAttribIndex {

};

class RenderSystem {

private:

	vector<GLuint> m_shaders;
	int m_shadersNum = 0;

	GLfloat ** m_particlesPos;
	int m_particlesNum;

	GLint m_shaderProgram;

public:

	RenderSystem();
	~RenderSystem();

	// ��ʾ��Ⱦ����
	void display_render_window(int argc, char ** argv) const;
	// ִ����Ⱦ
	void execute_render() const;
	// ��ʼ����Ⱦ����
	void init_render_context() const;
	// ������ɫ��
	GLuint compiler_shader(const char * shaderPath, GLenum shaderType) const;
	// ������ɫ��
	GLuint link_shader(const GLuint v, const GLuint g_, const GLuint f);
	// ������ɫ��
	GLint create_shader(const char * shadersPath[]);
	// ʹ����ɫ��
	bool use_shader(const int shaderId) const;
	// ������������λ������
	void input_particles_pos(const vector<Vector3d> particlesPos);
	void input_particles_pos(const vector<FluidParticle> particles);
	// ������������
	GLuint draw_particles();

	// ������ײ�������߿�
	void draw_collision_triangles_wireframe(const CollisionManager & collisionMgr);
};

#endif