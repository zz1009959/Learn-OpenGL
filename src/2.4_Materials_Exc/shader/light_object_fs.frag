#version  460 core
out vec4 FragColor;

uniform vec3 lightColor1;//�ƹ�

void main()
{
	FragColor=vec4(lightColor1,1.0f);
}