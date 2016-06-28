#version 440 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec4 color;

layout (std140, binding = 0) uniform TransformBlock
{
	mat4 mv_matrix;
	mat4 proj_matrix;
} transform;

out VS_OUT
{
	vec4 color;
} vs_out;

void main(void)
{
	gl_Position = transform.proj_matrix * transform.mv_matrix * vec4(vertex, 1.0);
	vs_out.color = vec4(vertex, 1.0) * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}