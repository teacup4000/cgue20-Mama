#shader vertex
#version 400 core

layout(location = 0) in vec2 aPos;

out vec2 textureCoords;

uniform mat4 transformationMatrix;

void main(void) {

	gl_Position = transformationMatrix * vec4(aPos, 0.0, 1.0);
	textureCoords = vec2((aPos.x + 1.0) / 2.0, 1 - (aPos.y + 1.0) / 2.0);
}

#shader fragment
#version 400

in vec2 textureCoords;

out vec4 out_Color;

uniform sampler2D guiTexture;

void main(void) {

	out_Color = texture(guiTexture, textureCoords);

}