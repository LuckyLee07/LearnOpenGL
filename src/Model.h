#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>
#include "Mesh.h"

#include "assimp/scene.h"
#include "assimp/material.h"

class Model
{
private:
	float m_scale;
	std::string m_filePath;
	std::vector<Mesh> m_Meshes;

public:
	Model(const char* filePath, float scale = 1.0f);
	void Draw(Shader& shader);

	float GetScale() const { return m_scale; }

private:
	void loadModel(const std::string& filePath);
	void processNode(aiNode *pNode, const aiScene *pScene);
	Mesh processMesh(aiMesh *pMesh, const aiScene *pScene);

	vector<Texture> loadMaterialTextures(aiMaterial *pMaterial, aiTextureType cType, const char* typeName);
};

#endif