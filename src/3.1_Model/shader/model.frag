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

uniform vec3 viewPos;//�۲��ߵ�����ռ�����
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform Light dirLight;

void main()
{    
     //������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse  = dirLight.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));  

    //������
   vec3 ambient  = dirLight.ambient  * vec3(texture(texture_diffuse1, TexCoords));

    //�߹�
    vec3 viewDir = normalize(viewPos - FragPos);//���߷�������
    vec3 reflectDir = reflect(-lightDir,norm);//���ŷ�����ķ�������

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = dirLight.specular * spec * vec3(texture(texture_specular1, TexCoords));//�������

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0f);
}