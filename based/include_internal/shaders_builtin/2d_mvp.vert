R"(
#version 330 core

uniform mat4 mvp;

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 in_texCoord;

out vec2 texCoord;

void main () {
	gl_Position = mvp * vec4 (pos, 0.0f, 1.0f);
	texCoord = vec2 (in_texCoord.s, 1.0f - in_texCoord.t);
}
)"
