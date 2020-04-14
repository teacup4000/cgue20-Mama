#shader vertex
#version 400 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 texCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	texCoords = aTexCoords;
	normal = aNormal;
	fragPos = vec3(model * vec4(aPos, 1.0));
}

#shader fragment
#version 400 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D texture_diffuse1;

void main()
{
	vec3 ambient = texture(texture_diffuse1, texCoords).rgb;
	vec3 color = ambient * texture(texture_diffuse1, texCoords).rgb;
	fragColor = vec4(color, 0.5f);
}


