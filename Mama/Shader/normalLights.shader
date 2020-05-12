#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

#define NR_POINT_LIGHTS 7

out VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos[NR_POINT_LIGHTS];
	vec3 TangentViewPos[NR_POINT_LIGHTS];
	vec3 TangentFragPos[NR_POINT_LIGHTS];
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
uniform PointLight pointLights[NR_POINT_LIGHTS];

//uniform vec3 lightPos;
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
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		vs_out.TangentLightPos[i] = TBN * pointLights[i].position;
		vs_out.TangentViewPos[i] = TBN * viewPos;
		vs_out.TangentFragPos[i] = TBN * vs_out.FragPos;
	}
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

#define NR_POINT_LIGHTS 7

in VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos[NR_POINT_LIGHTS];
	vec3 TangentViewPos[NR_POINT_LIGHTS];
	vec3 TangentFragPos[NR_POINT_LIGHTS];
} fs_in;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normalMap;
	float shininess;
};

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
uniform sampler2D normalMap;


uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

vec3 CalcPointLight(PointLight light, int i);


void main()
{
	vec3 result;

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], i);
	// phase 3: spot light

	FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, int i) {


	// obtain normal from normal map in range [0,1]
	vec3 normal = texture(material.normalMap, fs_in.TexCoords).rgb;
	// transform normal vector to range [-1,1]
	normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	float distance = length(light.position - fs_in.FragPos);

	// get diffuse color
	//vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
	// diffuse
	vec3 lightDir = normalize(fs_in.TangentLightPos[i] - fs_in.TangentFragPos[i]);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
	// specular
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	vec3 viewDir = normalize(fs_in.TangentViewPos[i] - fs_in.TangentFragPos[i]);
	vec3 reflectDir = reflect(-lightDir, normal);
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	//float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	vec3 specular = light.specular * vec3(texture(material.specular, fs_in.TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}