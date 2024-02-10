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
	glm::vec3 Position;//顶点位置
	glm::vec3 Normal;//顶点法向量
	glm::vec2 TexCoords;//纹理坐标
	glm::vec3 Tangent;//顶点切线
	glm::vec3 Bitangent;//副切线/法线
	int m_BoneIDs[MAX_BONE_INFLUENCE];//影响该顶点的骨骼索引
	float m_Weights[MAX_BONE_INFLUENCE];//每个骨骼的权重
};

struct  Texture {
	unsigned int id;//纹理id
	string type;//纹理类型
	string path;//纹理路径
};

class Mesh {
public:
	//网格数据
	vector<Vertex> vertices; //顶点
	vector<unsigned int> indices; //下标
	vector<Texture> textures; //纹理
	unsigned int VAO;

	//构造函数
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	 {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();//初始化缓冲
	 }
	//渲染
	void Draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			//在绑定之前激活相应的纹理单元
			glActiveTexture(GL_TEXTURE0 + i);
			// 获取纹理序号（diffuse_textureN 中的 N）
			string number;
			string name = textures[i].type;
			if(name == "texture_diffuse")
				number = std::to_string(diffuseNr++);//将unsigned int 转换为string
			else if(name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); 
			else if (name == "texture_height")
				number = std::to_string(heightNr++); 

			glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);

			// 绘制网格
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			//一旦配置完成，将所有内容设置回默认值总是好的做法
			glActiveTexture(GL_TEXTURE0);
		}
	}
private:
	unsigned int VBO, EBO;
	//初始化缓冲函数
	void setupMesh()
	{
		//生成对象
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//绑定VAO
		glBindVertexArray(VAO);

		//绑定缓冲，传入数据
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//顶点位置
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		//顶点法线
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);
		//顶点纹理坐标
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(2);
		//顶点切线
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		glEnableVertexAttribArray(3);
		// 顶点副切线
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		glEnableVertexAttribArray(4);
		// 骨骼id
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
		glEnableVertexAttribArray(5);
		// 骨骼权重
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
		glEnableVertexAttribArray(6);

		//解绑VAO
		glBindVertexArray(0);
	}
};
#endif