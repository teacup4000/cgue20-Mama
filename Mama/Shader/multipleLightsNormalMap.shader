#shader vertex
#version 330 core
#define NR_PT_LT 8

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos[NR_PT_LT];
	vec3 TangentViewPos[NR_PT_LT];
	vec3 TangentFragPos[NR_PT_LT];
} vs_out; 

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform PointLight pointLights[NR_PT_LT];
uniform vec3 viewPos;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));
	for (int i = 0; i < NR_PT_LT; i++)
	{
		vs_out.TangentLightPos[i] = TBN * pointLights[i].position;
		vs_out.TangentViewPos[i] = TBN * viewPos;
		vs_out.TangentFragPos[i] = TBN * vs_out.FragPos;
	}

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
#define NR_PT_LT 8

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos[NR_PT_LT];
	vec3 TangentViewPos[NR_PT_LT];
	vec3 TangentFragPos[NR_PT_LT];
} fs_in;

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D diffuseMap;
uniform sampler2D specMap;
uniform sampler2D normalMap;

uniform PointLight pointLights[NR_PT_LT];
uniform vec3 viewPos;

vec3 CalcLight(PointLight light, int i);

void main()
{
	vec3 sum;
	for (int i = 0; i < NR_PT_LT; i++)
	{
		sum += CalcLight(pointLights[i], i);
	}
	float brightness = dot(sum, vec3(0.2126, 0.7152, 0.0722));

	if (brightness > 1.0)
		BrightColor = vec4(sum, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	FragColor = vec4(sum, sum);
}

vec3 CalcLight(PointLight light, int i)
{
	vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);  

	// get diffuse color
	vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
	vec3 spec = texture(specMap, fs_in.TexCoords).rgb;
	// ambient
	vec3 ambient = light.ambient * color;
	// diffuse
	vec3 lightDir = normalize(fs_in.TangentLightPos[i] - fs_in.TangentFragPos[i]);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * color;
	// specular
	vec3 viewDir = normalize(fs_in.TangentViewPos[i] - fs_in.TangentFragPos[i]);

	vec3 specular = light.specular * spec;

	float distance = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}