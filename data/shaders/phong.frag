#version 440 core

layout (std140, binding = 1) uniform MaterialProperties
{
	vec3 ambient
	vec3 diffuse;
	vec3 specular;
	float specularIntensity;
	float dissolve;
} material;

in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
} fs_in;

out vec4 color;

void main(void)
{
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);

	vec3 R = reflect(-L, N);

	vec3 ambient = material.ambient;
	vec3 diffuse = max(dot(N, L), 0.0) * material.diffuse;
	vec3 specular = pow(max(dot(R, V), 0.0), material.specularIntensity) * material.specular;

	color = vec4(ambient + diffuse + specular, 1.0);
}