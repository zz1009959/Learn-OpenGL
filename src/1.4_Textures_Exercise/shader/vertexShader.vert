#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
void main ()
{
	gl_Position=vec4(aPos,1.0f);
	ourColor =aColor;
	TexCoord = aTexCoord;
	//TexCoord = aTexCoord * 2.0;//将纹理坐标设置到0-2范围
	//TexCoord = aTexCoord * 2.0 - 0.5;//将纹理坐标设置到0-2范围并且居中
}