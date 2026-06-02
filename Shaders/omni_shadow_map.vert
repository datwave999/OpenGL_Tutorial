#version 330

layout (location = 0) in vec3 pos;

uniform mat4 model;

out vec4 vFragPos;

void main(){
	vFragPos = model * vec4(pos, 1.0);
	gl_Position = vFragPos;
}
