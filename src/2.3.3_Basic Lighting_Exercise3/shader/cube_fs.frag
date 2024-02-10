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
uniform vec3 viewPos;//�۲��ߵ�����ռ�����
uniform float ambientStrength;//������
uniform float specularStrength; //�߹�

void main()
{
    //�����Դ��Ƭ��λ��֮��ķ�������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos -FragPos);
    //�����Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ��
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //������
    vec3 ambient = ambientStrength * lightColor;

    //�߹�
    vec3 viewDir = normalize(viewPos - FragPos);//���߷�������
    vec3 reflectDir = reflect(-lightDir,norm);//���ŷ�����ķ�������

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;//�������

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}*/
void main()
{
    FragColor = vec4(light_Color_Gouraud * objectColor, 1.0);
}