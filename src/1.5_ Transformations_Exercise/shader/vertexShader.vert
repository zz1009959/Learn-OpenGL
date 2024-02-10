#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float rotateAngle;
uniform mat4 transform;

mat4 rotate_x_3d(float angle)
{
	 return mat4(1,0,0,0,
	 0,cos(angle),-sin(angle),0,
	 0,sin(angle),cos(angle),0,
	 0,0,0,1);
}

mat4 rotate_y_3d(float angle)
{
	 return mat4(cos(angle),0.0f,sin(angle),0.0f,
	0,1,0.0f,0.0f,
	 -sin(angle),0,cos(angle),0,
	 0,0,0,1);
}

mat4 rotate_z_3d(float angle)
{
	 return mat4(cos(angle),-sin(angle),0.0f,0.0f,
	sin(angle),cos(angle),0.0f,0.0f,
	 0,0,1,0,
	 0,0,0,1);
}

void main ()
{
	//gl_Position=vec4(aPos,1.0f);
	//gl_Position=vec4(rotate_x_3d(rotateAngle)*vec4(aPos,1.0f));
	gl_Position=transform*vec4(aPos,1.0f);
	ourColor =aColor;
	TexCoord = aTexCoord;
}