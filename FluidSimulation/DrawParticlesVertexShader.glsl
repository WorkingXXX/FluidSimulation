#version 450 core

in vec4 particlesPos;

void main()
{
	gl_Position = particlesPos;	
}