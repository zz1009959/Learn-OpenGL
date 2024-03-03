#version 460 core
out vec4 FragColor;

float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    // ����1
    // float depth1 = (-ViewPos.z - near) / (far - near);

    // ����2
    // float ndc = gl_FragCoord.z * 2.0 - 1.0;  // NDC����ռ�
    // float clip = ndc / gl_FragCoord.w;       // �ü��ռ�
    // float depth2 = (clip + near) / (near + far); //z���� -near �� +far

    float depth = LinearizeDepth(gl_FragCoord.z) / far; // Ϊ����ʾ���� far
    FragColor = vec4(vec3(depth), 1.0);
}