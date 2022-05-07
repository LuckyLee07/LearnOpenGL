#include "Mesh.h"
#include "GL/glew.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

Mesh::Mesh(vector<VertexData> vertices, vector<uint> indices, vector<Texture> textures)
	: m_VAO(0), m_VBO(0), m_IBO(0)
{
	this->m_Indices = indices;
	this->m_Vertices = vertices;
	this->m_Textures = textures;
	
	this->setupMesh();
}

void Mesh::Draw(Shader shader)
{
	uint diffuseNr = 1, specularNr = 1;
	for (uint idx = 0; idx < m_Textures.size(); idx++)
	{
		m_Textures[idx].Bind(idx);//设置纹理单元槽
		std::string texNumber;
		std::string cType = m_Textures[idx].GetType();
		if (cType == "texture_diffuse")
			texNumber = std::to_string(diffuseNr++);
		else if (cType == "texture_specular")
			texNumber = std::to_string(specularNr++);
		if (texNumber.empty()) continue;
		shader.SetUniform1i(cType + texNumber, idx);
		m_Textures[idx].Active(); //激活纹理单元
	}
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(VertexData), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
		&m_Indices[0], GL_STATIC_DRAW);

	// 顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	// 顶点法线
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Normal));
	// 顶点纹理坐标
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, TexCoords));

	glBindVertexArray(0);
}