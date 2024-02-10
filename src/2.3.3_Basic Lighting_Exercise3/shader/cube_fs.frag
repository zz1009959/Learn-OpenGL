#version 460 core
out vec4 FragColor;

in vec3 light_Color_Gouraud;
uniform vec3 objectColor;
/*
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//观察者的世界空间坐标
uniform float ambientStrength;//环境光
uniform float specularStrength; //高光

void main()
{
    //计算光源和片段位置之间的方向向量
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos -FragPos);
    //计算光源对当前片段实际的漫反射影响
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //环境光
    vec3 ambient = ambientStrength * lightColor;

    //高光
    vec3 viewDir = normalize(viewPos - FragPos);//视线方向向量
    vec3 reflectDir = reflect(-lightDir,norm);//沿着法线轴的反射向量

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;//镜面分量

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}*/
void main()
{
    FragColor = vec4(light_Color_Gouraud * objectColor, 1.0);
}