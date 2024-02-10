#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

/*
    model变换：从本地坐标到世界坐标
    view变换：从世界坐标到观察坐标
    projection变换：投影到屏幕坐标系
*/
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

out vec3 Normal;
out vec3 FragPos;//片段的位置
out vec3 LightPos_view;//光源位置

void main()
{
    //FragPos = vec3(model * vec4(aPos, 1.0));//将片段位置变换到世界空间坐标
    FragPos = vec3( view * model * vec4(aPos, 1.0));//将片段位置（本地坐标）变换到观察空间坐标
    LightPos_view = vec3(view * vec4(lightPos, 1.0));

    //移除对法向量错误缩放的影响
    //Normal = mat3(transpose(inverse(model))) * aNormal;
    Normal = mat3(transpose(inverse(view * model))) * aNormal;//变换到观察坐标

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}