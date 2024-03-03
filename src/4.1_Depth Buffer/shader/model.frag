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
    // 方法1
    // float depth1 = (-ViewPos.z - near) / (far - near);

    // 方法2
    // float ndc = gl_FragCoord.z * 2.0 - 1.0;  // NDC坐标空间
    // float clip = ndc / gl_FragCoord.w;       // 裁剪空间
    // float depth2 = (clip + near) / (near + far); //z落在 -near 到 +far

    float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    FragColor = vec4(vec3(depth), 1.0);
}