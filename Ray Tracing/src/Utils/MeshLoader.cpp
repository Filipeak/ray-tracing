#include "MeshLoader.h"
#include "Core/Logger.h"
#include <fstream>
#include <sstream>

Mesh MeshLoader::ParsePLY(std::string path)
{
	Mesh mesh;
	std::ifstream file(path);

	if (!file.is_open())
	{
		spdlog::error("Could not open file {}", path);
		return mesh;
	}

	std::string line;
	int vertexCount = 0;
	int faceCount = 0;
	bool headerEnded = false;

	// 1. Parse the Header
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string token;
		iss >> token;

		if (token == "element")
		{
			std::string type;
			iss >> type;
			if (type == "vertex")
			{
				iss >> vertexCount;
			}
			else if (type == "face")
			{
				iss >> faceCount;
			}
		}
		else if (token == "end_header")
		{
			headerEnded = true;
			break;
		}
	}

	if (!headerEnded)
	{
		spdlog::error("Invalid PLY file, no end_header found.");
		return mesh;
	}

	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(faceCount * 3); // Rough estimate assuming triangles
	mesh.materialIds.reserve(faceCount); // One material ID per face

	// 2. Parse the Vertices
	for (int i = 0; i < vertexCount; ++i)
	{
		if (std::getline(file, line))
		{
			std::istringstream iss(line);
			glm::vec3 vertex;

			// Note: This assumes the first 3 properties are x, y, z.
			// A more complex parser would map properties found in the header.
			iss >> vertex.x >> vertex.y >> vertex.z;
			mesh.vertices.push_back(vertex);
		}
	}

	// 3. Parse the Faces (Indices)
	for (int i = 0; i < faceCount; ++i)
	{
		if (std::getline(file, line))
		{
			std::istringstream iss(line);
			int numVerticesInFace;
			iss >> numVerticesInFace;

			if (numVerticesInFace >= 3)
			{
				unsigned int i0, i1, i2;
				iss >> i0 >> i1 >> i2;

				mesh.indices.push_back(i0);
				mesh.indices.push_back(i1);
				mesh.indices.push_back(i2);

				mesh.materialIds.push_back(0); // Placeholder material ID
			}
		}
	}

	spdlog::info("Successfully loaded mesh from {} ({} vertices, {} triangles)", path, mesh.vertices.size(), mesh.indices.size() / 3);

	return mesh;
}