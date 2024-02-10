#version 460 core

//����
struct Material {
    //vec3 ambient;  //����������ɫ����һ��
    sampler2D diffuse; //��������ͼ
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

uniform vec3 viewPos;//�۲��ߵ�����ռ�����

uniform Material material;  //����
uniform Light light;//�ƹ�

void main()
{
    //������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position -FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));  

    //������
   vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));

    //�߹�
    vec3 viewDir = normalize(viewPos - FragPos);//���߷�������
    vec3 reflectDir = reflect(-lightDir,norm);//���ŷ�����ķ�������

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));//�������

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}