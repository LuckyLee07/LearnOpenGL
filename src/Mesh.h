#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "glm/glm.hpp"
#include "Texture.h"
#include "Shader.h"

using namespace std;

typedef unsigned int uint;

struct VertexData
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	std::vector<VertexData> m_Vertices;
	std::vector<uint> m_Indices;
	std::vector<Texture> m_Textures;

public:
	Mesh(vector<VertexData> vertices, vector<uint> indices, vector<Texture> textures);

	void Draw(Shader shader);

private:
	uint m_VAO, m_VBO, m_IBO;

	void setupMesh();
};

#endif
