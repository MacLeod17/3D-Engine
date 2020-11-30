#version 430 core

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec3 vs_normal;

out vec3 fs_color;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
};

struct Light
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
};

uniform Material material;

uniform Light light;

uniform mat4 mvp;
uniform mat4 model_view;

void main()
{
	//Ambient
	vec3 ambient = material.ambient * light.ambient;

	//Diffuse
	vec3 normal = mat3(model_view) * vs_normal;
	vec4 position = model_view * vec4(vs_position, 1);
	vec3 direction_to_light = normalize(vec3(light.position - position));

	//Intensity
	float lDotN = max(dot(direction_to_light, normal), 0);
	vec3 diffuse = material.diffuse * light.diffuse * lDotN;

	fs_color = ambient + diffuse;
	gl_Position = mvp * vec4(vs_position, 1.0);
}