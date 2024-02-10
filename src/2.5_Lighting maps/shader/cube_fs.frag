#version 460 core

//材质
struct Material {
    //vec3 ambient;  //与漫反射颜色几乎一致
    sampler2D diffuse; //漫反射贴图
   // vec3 specular;
   sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;//观察者的世界空间坐标

uniform Material material;  //材质
uniform Light light;//灯光

void main()
{
    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position -FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));  

    //环境光
   vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));

    //高光
    vec3 viewDir = normalize(viewPos - FragPos);//视线方向向量
    vec3 reflectDir = reflect(-lightDir,norm);//沿着法线轴的反射向量

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));//镜面分量

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}