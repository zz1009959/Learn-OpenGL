#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float right;

out vec3 ourColor;
out vec4 position;
void main ()
{
	gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0f);
	ourColor =aColor;
	position=gl_Position;
}