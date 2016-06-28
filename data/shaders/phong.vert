#version 440 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

layout (std140, binding = 0) uniform TransformBlock
{
	mat4 mv_matrix;
	mat4 view_matrix;
	mat4 proj_matrix;
} transform;

uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);

out VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
} vs_out;

void main(void)
{
	// Calculate view-space coordinates.
	vec4 P = mv_matrix * vec4(vertex, 1.0);

	// Calc normal in view-space.
	vs_out.N = mat3(mv_matrix) * normal

	// Calc light vector.
	vs_out.L = light_pos * P.xyz;

	// Calc view vector.
	vs_out.V = -P.xyz;

	// Calc clip-space position.
	gl_position = proj_matrix * P;
}