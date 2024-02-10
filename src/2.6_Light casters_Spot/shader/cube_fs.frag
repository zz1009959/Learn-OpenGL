#version 460 core

//����
struct Material {
    //vec3 ambient;  //����������ɫ����һ��
    sampler2D diffuse; //��������ͼ
   sampler2D specular;
    float shininess;
}; 

//�ƹ�
struct Light {
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

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
    vec3 lightDir = normalize(light.position -FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));  

     //������
     vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));

     //�߹�
     vec3 viewDir = normalize(viewPos - FragPos);//���߷�������
     vec3 reflectDir = reflect(-lightDir,norm);//���ŷ�����ķ�������

     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
     vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));//�������

     // �۹�(soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

     vec3 result = ambient + diffuse + specular;
     FragColor = vec4(result, 1.0);

}

//��������Ƿ��ھ۹��׶����
    /*float theta = dot(lightDir, normalize(-light.direction)); 
    if(theta > light.cutOff)
    {
        //������         
         //������
        //�߹�
    }
    else
    {
        //������ھ۹�Ʒ�Χ�ھ�ʹ�û�����
         FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
    }*/