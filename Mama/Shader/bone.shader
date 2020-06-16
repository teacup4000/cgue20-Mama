#shader vertex
#version 400 core
#define MAX_BONES 100
#define MAX_WEIGHTS 4

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 aBones;     // INT pointer
layout(location = 4) in vec4 aWeights;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 bones[MAX_BONES];

uniform bool reverse_normals;

void main()
{
	mat4 MVP = projection * view * model;
	mat4 normals_matrix = transpose(inverse(model));

	mat4 bone_transform = bones[aBones[0]] * aWeights[0];
	bone_transform += bones[aBones[1]] * aWeights[1];
	bone_transform += bones[aBones[2]] * aWeights[2];
	bone_transform += bones[aBones[3]] * aWeights[3];

	vec4 boned_position = bone_transform * vec4(aPos, 1.0); // transformed by bones

	Normal = vec3(normals_matrix * (bone_transform * vec4(aNormal, 0.0)));

	FragPos = vec3(model * boned_position);
	TexCoords = aTexCoords;
	gl_Position = MVP * boned_position;
}

#shader fragment
#version 400 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material
{
	sampler2D texture_diffuse1;

	sampler2D texture_specular1;

	float shininess;
	float transparency;
};

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform PointLight point_light;
uniform vec3 view_pos;
unicorm vec3 brightness;


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir, vec3 fragPos)
{
	vec3 light_dir = normalize(light.position - FragPos);
	//Diffuse
	float diff = max(dot(normal, light_dir), 0.0);
	//Specular 
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	//Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));


	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

void main()
{
	Normal = normalize(Normal);

	vec3 view_dir = normalize(view_pos - FragPos);

	vec4 calc_color = vec4(CalcPointLight(point_light, Normal, view_dir, FragPos), 1.0);
	calc_color.a *= material.transparency;

	FragColor = calc_color;
}