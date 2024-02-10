#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<shader.h>
#include<camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<imgui/imgui.h>
#include<imgui/imgui_impl_glfw.h>
#include<imgui/imgui_impl_opengl3.h>

#include<model.h>

#include<iostream>
#include<cmath>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
const char* glsl_version = "#version 460";

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	glfwInit(); //��ʼ��GLFW

	//������Ҫ�ʹ�Ҫ�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ʹ�õ��Ǻ���ģʽ(Core-profile)��ֻ��ʹ��OpenGL���ܵ�һ���Ӽ�

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();//�����߳�
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//ע�ᴰ�ڱ任��������
	glfwSetCursorPosCallback(window, mouse_callback);//����ƶ�
	glfwSetScrollCallback(window, scroll_callback);//�����ֹ���

	// ���ع��
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/*�ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD��*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(false);

	//------------------------------
	//����imgui������
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	//����imgui���
	ImGui::StyleColorsDark();

	//����ƽ̨����Ⱦ��
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	Shader ourShader("./src/3.1_Model/shader/model.vert", "./src/3.1_Model/shader/model.frag");
	Model ourModel("./model/elysia/elysia.obj");

	//������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//imgui����ֵ
	ImVec4 clear_color = ImVec4(0.1, 0.1, 0.1, 1.0);
	float light_diff = 0.5f;
	float light_amb = 0.2f;
	while (!glfwWindowShouldClose(window))
	{
		//ÿ֡ʱ���߼�
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//����
		processInput(window);
		//����imgui���
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("ImGui");
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		ImGui::SliderFloat("light_diff", &light_diff, 0.0f, 1.0f);
		ImGui::SliderFloat("light_amb", &light_amb, 0.0f, 1.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		//��Ⱦָ��
		/*������glClear�����������ɫ����֮��������ɫ���嶼�ᱻ���ΪglClearColor�������õ���ɫ��*/
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		ourShader.use();
		// ���ù���
		ourShader.setVec3("dirLight.ambient", light_amb, light_amb, light_amb);
		ourShader.setVec3("dirLight.diffuse", light_diff, light_diff, light_diff);
		ourShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		ourShader.setVec3("viewPos", camera.Position);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// ��Ⱦ
		float rotateAngle = glfwGetTime();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(rotateAngle*20), glm::vec3(0.0f, 1.0f, 0.0f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		//��Ⱦgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//��������
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// ����ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();//��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	return 0;
}

//�ӿڵ������ص���������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//�����ر�
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // ��ת����Ϊ y ������µ���

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}