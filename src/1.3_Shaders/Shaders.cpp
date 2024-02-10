#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<iostream>
#include<cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"// λ�ñ���������λ��ֵΪ 0
"layout (location = 1) in vec3 aColor;\n"// λ�ñ���������λ��ֵΪ 0

"out vec3 ourColor;\n" // ��Ƭ����ɫ�����һ����ɫ

"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"ourColor = aColor;\n"// ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
"}\n";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor\n;"
//"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(ourColor,1.0f);\n"
"}\n";

int main()
{
    glfwInit(); //��ʼ��GLFW

    //������Ҫ�ʹ�Ҫ�汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ʹ�õ��Ǻ���ģʽ(Core-profile)��ֻ��ʹ��OpenGL���ܵ�һ���Ӽ�

    //�������ڶ���
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();//�����߳�
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//ע�ᴰ�ڱ任��������

    /*�ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD��*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //����������ɫ����Ƭ����ɫ��
    unsigned int vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //������ɫ��Դ��
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //����Ƿ����ɹ�
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::VERTEX::COMPICATION_FAILED\n" << infoLog << std::endl;
    }
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::FRAGMENT::COMPICATION_FAILED\n" << infoLog << std::endl;
    }

    //�����������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //����ɫ�����ӵ����������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //������ɫ��
    glLinkProgram(shaderProgram);

    //���������ɫ�������Ƿ�ɹ�
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::SHADERPROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    //ɾ����ɫ������ɫ���������ӵ���������Ժ�
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //��������
    //��ʵ�ʿ�������GLfloat
    float vertices[] = {
        // λ��              // ��ɫ
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
    };

    /*unsigned int indices[] = {
        // ע��������0��ʼ!
        // ����������(0,1,2,3)���Ƕ�������vertices���±꣬
        // �����������±��������ϳɾ���

        0, 1, 3, // ��һ��������
        1, 2, 3  // �ڶ���������
    };*/
    //����VBO,VAO,EBO
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &EBO);

    //��VAO����
    glBindVertexArray(VAO);
    //�󶨻������
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // ���VBO����
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //�����������ݣ����ö���λ������ָ�룩
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //��ɫ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //���������Ƶ�������
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //���EBO
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //���
    glBindVertexArray(0);

    //��ȡ������������
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    //�����߿����ģʽ
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_FILLΪ���

    while (!glfwWindowShouldClose(window))
    {
        //����
        processInput(window);

        //��Ⱦָ��
        /*������glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ��*/
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*���Ƶڶ��������Σ���ɫ����ʱ��仯*/
        //ʹ����ɫ��
        glUseProgram(shaderProgram);

        //����uniform��ɫ
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        //��VAO
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);//--->��ӦVBO
        //���
        glBindVertexArray(0);

        //��������
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //һ����Դ��������;����ȡ������������Դ:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();//��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
    return 0;
}

//�ӿڵ������ص���������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//�����ر�
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}