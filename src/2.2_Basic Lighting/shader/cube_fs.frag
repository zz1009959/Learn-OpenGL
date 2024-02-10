#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//�۲��ߵ�����ռ�����

void main()
{
    //�����Դ��Ƭ��λ��֮��ķ�������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos -FragPos);
    //�����Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ��
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //������
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    //�߹�
    float specularStrength = 0.5f;  //����ǿ��
    vec3 viewDir = normalize(viewPos - FragPos);//���߷�������
    vec3 reflectDir = reflect(-lightDir,norm);//���ŷ�����ķ�������

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;//�������

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}