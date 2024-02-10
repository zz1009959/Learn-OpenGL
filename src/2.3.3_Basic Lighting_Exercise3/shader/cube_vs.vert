#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform float ambientStrength;//环境光
uniform float specularStrength; //高光

//out vec3 Normal;
//out vec3 FragPos;//片段的位置
out vec3 light_Color_Gouraud;

void main()
{
    //FragPos = vec3(model * vec4(aPos, 1.0));
    //移除对法向量错误缩放的影响
    //Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
     //Gouraud Shading
    vec3 Position = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

     //计算光源和片段位置之间的方向向量
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos -Position);
    //计算光源对当前片段实际的漫反射影响
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //环境光
    vec3 ambient = ambientStrength * lightColor;

    //高光
    vec3 viewDir = normalize(viewPos - Position);//视线方向向量
    vec3 reflectDir = reflect(-lightDir,norm);//沿着法线轴的反射向量

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;//镜面分量

    light_Color_Gouraud = ambient + diffuse + specular;
}