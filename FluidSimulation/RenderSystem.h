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

// 粒子属性在顶点着色器中的索引
enum VertexShaderAttribIndex {
	PARTICLES_POS = 0
};

// 粒子属性在片元着色器中的索引
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

	// 显示渲染窗口
	void display_render_window(int argc, char ** argv) const;
	// 执行渲染
	void execute_render() const;
	// 初始化渲染环境
	void init_render_context() const;
	// 编译着色器
	GLuint compiler_shader(const char * shaderPath, GLenum shaderType) const;
	// 链接着色器
	GLuint link_shader(const GLuint v, const GLuint g_, const GLuint f);
	// 创建着色器
	GLint create_shader(const char * shadersPath[]);
	// 使用着色器
	bool use_shader(const int shaderId) const;
	// 输入流体粒子位置数据
	void input_particles_pos(const vector<Vector3d> particlesPos);
	void input_particles_pos(const vector<FluidParticle> particles);
	// 绘制流体粒子
	GLuint draw_particles();

	// 绘制碰撞三角面线框
	void draw_collision_triangles_wireframe(const CollisionManager & collisionMgr);
};

#endif