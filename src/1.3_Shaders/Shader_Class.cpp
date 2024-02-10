#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<shader.h>

#include<iostream>
#include<cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


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

	Shader ourShader("./src/1.3_Shaders/shader/vertexShader.vert", "./src/1.3_Shaders/shader/fragmentShader.frag");


	//��������
	//��ʵ�ʿ�������GLfloat
	float vertices[] = {
		// λ��              // ��ɫ
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
	};

	//����VBO,VAO
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

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

		/*����������*/
		//ʹ����ɫ��
		ourShader.use();

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