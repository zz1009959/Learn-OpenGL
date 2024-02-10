#ifndef MESH_H
#define MESH_H

#include<glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include<shader.h>
#include<vector>
#include<string>

using namespace std;
#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 Position;//����λ��
	glm::vec3 Normal;//���㷨����
	glm::vec2 TexCoords;//��������
	glm::vec3 Tangent;//��������
	glm::vec3 Bitangent;//������/����
	int m_BoneIDs[MAX_BONE_INFLUENCE];//Ӱ��ö���Ĺ�������
	float m_Weights[MAX_BONE_INFLUENCE];//ÿ��������Ȩ��
};

struct  Texture {
	unsigned int id;//����id
	string type;//��������
	string path;//����·��
};

class Mesh {
public:
	//��������
	vector<Vertex> vertices; //����
	vector<unsigned int> indices; //�±�
	vector<Texture> textures; //����
	unsigned int VAO;

	//���캯��
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	 {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();//��ʼ������
	 }
	//��Ⱦ
	void Draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			//�ڰ�֮ǰ������Ӧ������Ԫ
			glActiveTexture(GL_TEXTURE0 + i);
			// ��ȡ������ţ�diffuse_textureN �е� N��
			string number;
			string name = textures[i].type;
			if(name == "texture_diffuse")
				number = std::to_string(diffuseNr++);//��unsigned int ת��Ϊstring
			else if(name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); 
			else if (name == "texture_height")
				number = std::to_string(heightNr++); 

			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);

			// ��������
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			//һ��������ɣ��������������û�Ĭ��ֵ���Ǻõ�����
			glActiveTexture(GL_TEXTURE0);
		}
	}
private:
	unsigned int VBO, EBO;
	//��ʼ�����庯��
	void setupMesh()
	{
		//���ɶ���
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//��VAO
		glBindVertexArray(VAO);

		//�󶨻��壬��������
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//����λ��
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		//���㷨��
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);
		//������������
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(2);
		//��������
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		glEnableVertexAttribArray(3);
		// ���㸱����
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		glEnableVertexAttribArray(4);
		// ����id
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
		glEnableVertexAttribArray(5);
		// ����Ȩ��
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
		glEnableVertexAttribArray(6);

		//���VAO
		glBindVertexArray(0);
	}
};
#endif