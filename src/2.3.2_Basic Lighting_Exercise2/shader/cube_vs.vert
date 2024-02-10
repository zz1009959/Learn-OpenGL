#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

/*
    model�任���ӱ������굽��������
    view�任�����������굽�۲�����
    projection�任��ͶӰ����Ļ����ϵ
*/
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

out vec3 Normal;
out vec3 FragPos;//Ƭ�ε�λ��
out vec3 LightPos_view;//��Դλ��

void main()
{
    //FragPos = vec3(model * vec4(aPos, 1.0));//��Ƭ��λ�ñ任������ռ�����
    FragPos = vec3( view * model * vec4(aPos, 1.0));//��Ƭ��λ�ã��������꣩�任���۲�ռ�����
    LightPos_view = vec3(view * vec4(lightPos, 1.0));

    //�Ƴ��Է������������ŵ�Ӱ��
    //Normal = mat3(transpose(inverse(model))) * aNormal;
    Normal = mat3(transpose(inverse(view * model))) * aNormal;//�任���۲�����

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}