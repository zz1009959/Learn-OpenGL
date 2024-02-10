#ifndef MODEL_H
#define MODEL_H
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    // ģ������
    vector<Texture> textures_loaded;	// �洢��ĿǰΪֹ���ص����������������Ż���ȷ���������μ���
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // ���캯��
    Model(string const& path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // ��������
    void Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    //���ļ��м���֧�� ASSIMP ��չ����ģ�ͣ��������ɵ�����洢������������
    void loadModel(string const& path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        //��ȡ�ļ�·����Ŀ¼·��
        directory = path.substr(0, path.find_last_of('/'));

        //�ݹ鴦�� ASSIMP �ĸ��ڵ�
        processNode(scene->mRootNode, scene);
    }

    // �Եݹ鷽ʽ����ڵ㡣����ڵ��ϵ�ÿ�����񣬲������ӽڵ㣨����У����ظ���һ����
    void processNode(aiNode* node, const aiScene* scene)
    {
        // ����ڵ����е���������еĻ���
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // �ڵ����ֻ�����������������������е�ʵ�ʶ���
            // �����а����������ݣ��ڵ�ֻ�������������ݣ���ڵ�֮��Ĺ�ϵ����
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        //�������������ӽڵ��ظ���һ����
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // ����
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // ���������ÿ������
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            /*����������һ��ռλ����������Ϊ assimp ʹ���Լ���������
            ����ֱ��ת��Ϊ glm �� vec3 �࣬���������Ƚ����ݴ��䵽���ռλ�� glm::vec3*/
            glm::vec3 vector; 
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // �ж������Ƿ������������
            {
                glm::vec2 vec;
                /*һ�����������԰��� 8 ����ͬ���������꣬��ˣ����Ǽ��費��
                 ʹ��һ����������ж�����������ģ�ͣ������������ȡ��һ�飨0��*/
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // ���ڣ����������ÿ���棨��������������Σ�������ȡ��Ӧ�Ķ�������
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // ���������������������洢������������
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // �������
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
       // ���Ǽٶ���ɫ���в��������Ƶ�Լ���׳ɡ�ÿ������������Ӧ����Ϊ
        // "texture_diffuseN"������ N ��һ���� 1 �� MAX_SAMPLER_NUMBER ��˳���
        // ��������Ҳ����ˣ����±���ʾ��
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // ���ظ�����ȡ���������ݴ������������
        return Mesh(vertices, indices, textures);
    }

    //���������͵����в������������δ���أ��������Щ����
    // �������Ϣ��������ṹ����ʽ����
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // ���֮ǰ�Ƿ���ع���������ǣ��������һ�ε����� ��������������
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // ����Ѿ������˾�����ͬ�ļ�·�������������������һ�����Ż���
                    break;
                }
            }
            if (!skip)
            {    // �������û�б����أ��Ǿͼ�����
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture); // ����洢Ϊ����ģ�͵�������أ���ȷ�����ǲ��᲻��Ҫ�ؼ����ظ�������
            }
        }
        return textures;
    }
};

//��������
unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif