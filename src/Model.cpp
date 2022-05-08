#include "Model.h"
#include "Common.h"
#include "FileManager.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

Model::Model(const char* filePath)
{
	m_filePath = filePath;

	this->loadModel(filePath);
}

void Model::Draw(Shader& shader)
{
	for (uint ii = 0; ii < m_Meshes.size(); ii++)
	{
		m_Meshes[ii].Draw(shader);
	}
}

void Model::loadModel(const std::string& filePath)
{
	Assimp::Importer importer;
	const aiScene *pScene = importer.ReadFile(filePath, aiProcess_Triangulate);

	if (pScene == NULL || pScene->mFlags&AI_SCENE_FLAGS_INCOMPLETE || pScene->mRootNode == NULL)
	{
		LOG_INFO("ERROR:ASSIPM:: %s", importer.GetErrorString());
	}
	
	processNode(pScene->mRootNode, pScene);
}

void Model::processNode(aiNode *pNode, const aiScene *pScene)
{
	// 处理节点所有的网格（如果有的话）
	for (uint i = 0; i < pNode->mNumMeshes; i++)
	{
		aiMesh *pMesh = pScene->mMeshes[pNode->mMeshes[i]];
		m_Meshes.push_back(processMesh(pMesh, pScene));
	}
	// 接下来对它的子节点重复这一过程
	for (uint i = 0; i < pNode->mNumChildren; i++)
	{
		processNode(pNode->mChildren[i], pScene);
	}
}

Mesh Model::processMesh(aiMesh *pMesh, const aiScene *pScene)
{
	std::vector<VertexData> vertices;
	std::vector<uint> indices;
	std::vector<Texture> textures;

	// 处理顶点位置、法线、纹理坐标
	for (uint i = 0; i < pMesh->mNumVertices; i++)
	{
		VertexData vertex;

		if (pMesh->HasPositions()) // 顶点位置
		{
			vertex.Position.x = pMesh->mVertices[i].x;
			vertex.Position.y = pMesh->mVertices[i].y;
			vertex.Position.z = pMesh->mVertices[i].z;
		}
		
		if (pMesh->HasNormals()) // 法向量数据
		{
			vertex.Normal.x = pMesh->mNormals[i].x;
			vertex.Normal.y = pMesh->mNormals[i].y;
			vertex.Normal.z = pMesh->mNormals[i].z;
		}
		
		// Assimp允许最多8个纹理 目前只处理0号纹理
		if (pMesh->mTextureCoords[0])
		{
			vertex.TexCoords.x = pMesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = pMesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	// 处理索引数据
	for (uint i = 0; i < pMesh->mNumFaces; i++)
	{
		aiFace face = pMesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	
	// 处理材质数据
	if (pMesh->mMaterialIndex >= 0)
	{
		aiMaterial *pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(pMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> speculaMaps = loadMaterialTextures(pMaterial, aiTextureType_SPECULAR, "texture_specula");
		textures.insert(textures.end(), speculaMaps.begin(), speculaMaps.end());
	}
	if (textures.size() == 0) //无材质则取同名jpg图片
	{
		std::size_t findPos = m_filePath.rfind(".");
		std::string filePath = m_filePath.replace(findPos+1, 3, "png");

		if (FileAutoClose::isFileExist(filePath.c_str()))
		{
			Texture texture(filePath.c_str(), "texture_diffuse");
			textures.push_back(texture);
		}
        else
		{
			LOG_INFO("Fxkk====>>>File not found:%s", filePath.c_str());
		}
	}
	
	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *pMaterial, aiTextureType cType, const char* typeName)
{
	vector<Texture> textures;
	std::size_t pos = m_filePath.find_last_of("/");
	std::string dirPath = m_filePath.substr(0, pos);

	uint textureCount = pMaterial->GetTextureCount(cType);
	for (uint i = 0; i < textureCount; i++)
	{
		aiString strPath;
		pMaterial->GetTexture(cType, i, &strPath);

		std::string filePath = dirPath + "/" + strPath.C_Str();
		
		Texture texture(filePath.c_str(), typeName);
		textures.push_back(texture);
	}
	return textures;
}