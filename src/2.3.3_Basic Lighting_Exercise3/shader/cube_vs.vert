#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform float ambientStrength;//������
uniform float specularStrength; //�߹�

//out vec3 Normal;
//out vec3 FragPos;//Ƭ�ε�λ��
out vec3 light_Color_Gouraud;

void main()
{
    //FragPos = vec3(model * vec4(aPos, 1.0));
    //�Ƴ��Է������������ŵ�Ӱ��
    //Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
     //Gouraud Shading
    vec3 Position = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

     //�����Դ��Ƭ��λ��֮��ķ�������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos -Position);
    //�����Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ��
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //������
    vec3 ambient = ambientStrength * lightColor;

    //�߹�
    vec3 viewDir = normalize(viewPos - Position);//���߷�������
    vec3 reflectDir = reflect(-lightDir,norm);//���ŷ�����ķ�������

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;//�������

    light_Color_Gouraud = ambient + diffuse + specular;
}