#version 460 core
out vec4 FragColor;
in vec3 ourColor;
in vec4 position;

void main ()
{
	//FragColor=vec4(ourColor,1.0f);
	FragColor=position;
}