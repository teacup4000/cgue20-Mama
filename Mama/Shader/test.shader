#shader vertex
#version 330 core
#define NR_POINT_LIGHTS 7

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 TangentLightPos[NR_POINT_LIGHTS];
out vec3 TangentViewPos[NR_POINT_LIGHTS];
out vec3 TangentFragPos[NR_POINT_LIGHTS];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 pointLights[NR_POINT_LIGHTS];

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	//Normal = transpose(inverse(mat3(model))) * aNormal;
	TexCoords = aTexCoords;

	vec3 T = normalize(Normal * aTangent);
	vec3 N = normalize(Normal * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		TangentLightPos[i] = TBN * pointLights[i].position;
		TangentViewPos[i] = TBN * viewPos;
		TangentFragPos[i] = TBN * FragPos;
	}
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

#define NR_POINT_LIGHTS 7

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

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 TangentLightPos[NR_POINT_LIGHTS];
in vec3 TangentViewPos[NR_POINT_LIGHTS];
in vec3 TangentFragPos[NR_POINT_LIGHTS];


uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

//uniform vec3 TangentLightPos;
//uniform vec3 TangentViewPos;
//uniform vec3 TangentFragPos;

vec3 CalcNormal(vec3 lightPos, vec3 TangentLightPos, vec3 TangentViewPos, vec3 TangentFragPos);
// function prototypes
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos);

void main()
{
	// properties
	vec3 normal = texture(material.normalMap, TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	//vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcNormal(pointLights[i].position, TangentLightPos[i], TangentViewPos[i], TangentFragPos[i]) + CalcPointLight(pointLights[i], normal, FragPos);
	}
	FragColor = vec4(result, 1.0);
}

vec3 CalcNormal(vec3 lightPos, vec3 TangentLightPos, vec3 TangentViewPos, vec3 TangentFragPos)
{
	// obtain normal from normal map in range [0,1]
	vec3 normal = texture(material.normalMap, TexCoords).rgb;
	// transform normal vector to range [-1,1]
	normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

	// get diffuse color
	vec3 color = texture(material.diffuse, TexCoords).rgb;
	// ambient
	vec3 ambient = 0.1 * color;
	// diffuse
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;
	// specular
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.2) * spec;
	return (ambient + diffuse + specular);
}


// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * vec3(texture(material.specular, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

