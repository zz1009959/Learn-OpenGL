#version 460 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;//观察者的世界空间坐标
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform Light dirLight;

void main()
{    
     //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse  = dirLight.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));  

    //环境光
   vec3 ambient  = dirLight.ambient  * vec3(texture(texture_diffuse1, TexCoords));

    //高光
    vec3 viewDir = normalize(viewPos - FragPos);//视线方向向量
    vec3 reflectDir = reflect(-lightDir,norm);//沿着法线轴的反射向量

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = dirLight.specular * spec * vec3(texture(texture_specular1, TexCoords));//镜面分量

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0f);
}