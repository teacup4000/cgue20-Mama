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
	if (reverse_normals)
		Normal = vec3(normals_matrix * (bone_transform * (-1.0 * vec4(aNormal, 0.0))));
	else
		Normal = vec3(normals_matrix * (bone_transform * vec4(aNormal, 0.0)));

	FragPos = vec3(model * boned_position);
	TexCoords = aTexCoords;
	gl_Position = MVP * boned_position;
}

#shader fragment
#version 400 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
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

#define NR_POINT_LIGHTS 8

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

uniform float far_plane;
uniform bool shadows;
uniform vec3 lightPos;

uniform samplerCube depthMap;
uniform vec3 brightness;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
	);

// function prototypes
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 CalcShadow(vec3 LightDir, vec3 normal, vec3 fragPos);
float ShadowCalculation(vec3 fragPos);


void main()
{
	// properties
	vec3 norm = normalize(Normal);

	// == =====================================================
	// Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
	// For each phase, a calculate function is defined that calculates the corresponding color
	// per lamp. In the main() function we take all the calculated colors and sum them up for
	// this fragment's final color.
	// == =====================================================
	vec3 result;

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos);

	result += CalcShadow(lightPos, norm, FragPos);

	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));

	if (brightness > 1.0f)
		BrightColor = vec4(result, 1.0f);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	FragColor = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	//vec3 reflectDir = reflect(-lightDir, normal);
	//float spec = pow(max(dot(viewDirw, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = (light.ambient * vec3(texture(material.diffuse, TexCoords))) * brightness;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * vec3(texture(material.specular, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcShadow(vec3 lightPos, vec3 normal, vec3 fragPos)
{
	vec3 color = texture(material.diffuse, TexCoords).rgb;
	vec3 n = normalize(normal);
	vec3 lightColor = vec3(0.1);
	// ambient
	//vec3 ambient = 0.9 * color;
	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, n), 0.0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fragPos);

	// calculate shadow
	float shadow = shadows ? ShadowCalculation(fragPos) : 0.0;
	vec3 lighting = (1.0 - shadow) * (diffuse)* color;
	return lighting;
}

float ShadowCalculation(vec3 fragPos)
{
	// get vector between fragment position and light position
	vec3 fragToLight = fragPos - lightPos;
	// use the fragment to light vector to sample from the depth map    
	// float closestDepth = texture(depthMap, fragToLight).r;
	// it is currently in linear range between [0,1], let's re-transform it back to original depth value
	// closestDepth *= far_plane;
	// now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
	// test for shadows
	// float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
	// float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	// PCF
	float shadow = 0.0;
	float bias = 0.05;
	float samples = 4.0;
	float offset = 0.1;
	for (float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
		for (float y = -offset; y < offset; y += offset / (samples * 0.5))
		{
			for (float z = -offset; z < offset; z += offset / (samples * 0.5))
			{
				float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; // use lightdir to lookup cubemap
				closestDepth *= far_plane;   // Undo mapping [0;1]
				if (currentDepth - bias > closestDepth)
					shadow += 10.0;
			}
		}
	}
	shadow /= (samples * samples * samples);
	return shadow;
}
