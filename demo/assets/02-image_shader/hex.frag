#version 330 core

uniform sampler2D tex;

in vec2 texCoord;

out vec4 color;

void main () {
	float x = texCoord.x;

	color = texture (tex, texCoord) * vec4(0.2f, 0.5f, 0.3f, 1.f);
}
