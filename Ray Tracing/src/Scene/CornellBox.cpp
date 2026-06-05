#include "CornellBox.h"
#include "Utils/MeshLoader.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

CornellBox::CornellBox(std::unique_ptr<ICameraController> cameraController)
{
	// World setup
	Mesh worldMesh;

	worldMesh.vertices = {
		// Walls
		glm::vec3(-5.0f, -5.0f, -10.0f),
		glm::vec3(-5.0f, -5.0f, +5.0f),
		glm::vec3(+5.0f, -5.0f, -10.0f),
		glm::vec3(+5.0f, -5.0f, +5.0f),
		glm::vec3(-5.0f, +5.0f, -10.0f),
		glm::vec3(-5.0f, +5.0f, +5.0f),
		glm::vec3(+5.0f, +5.0f, -10.0f),
		glm::vec3(+5.0f, +5.0f, +5.0f),

		// Light source (a small quad on the ceiling)
		glm::vec3(-1.0f, 4.9f, -3.0f),
		glm::vec3(-1.0f, 4.9f, +3.0f),
		glm::vec3(+1.0f, 4.9f, -3.0f),
		glm::vec3(+1.0f, 4.9f, +3.0f),
	};
	worldMesh.indices = {
		0, 1, 4, 1, 5, 4, // Left face
		1, 3, 5, 3, 7, 5, // Front face
		3, 2, 7, 2, 6, 7, // Right face
		0, 2, 3, 0, 3, 1, // Bottom face
		4, 7, 6, 4, 5, 7, // Top face
		0, 6, 2, 0, 4, 6, // Back face
		8, 9, 10, 9, 11, 10, // Light source
	};
	worldMesh.materials = {
		Material{ glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f, 0.0f }, // Red
		Material{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f, 0.0f }, // Green
		Material{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f, 0.0f }, // White
		Material{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 5.0f, 1.0f, 0.0f }, // Light
	};
	worldMesh.materialIds = {
		1, 1, // Left face
		2, 2, // Front face
		0, 0, // Right face
		2, 2, // Bottom face
		2, 2, // Top face
		2, 2, // Back face
		3, 3, // Light source
	};

	// Dragon setup
	Mesh dragonMesh = MeshLoader::ParsePLY("assets/stanford_dragon.ply");

	constexpr float angleRadians = glm::radians(180.0f);
	glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
	glm::quat rotationQuat = glm::angleAxis(angleRadians, yAxis);

	for (size_t i = 0; i < dragonMesh.vertices.size(); i++)
	{
		dragonMesh.vertices[i] *= 40.0f; // Scale the dragon to make it more visible in the scene
		dragonMesh.vertices[i] -= glm::vec3(0.0f, 6.0f, 0.0f); // Move the dragon down to be centered on the ground
		dragonMesh.vertices[i] = rotationQuat * dragonMesh.vertices[i]; // Rotate the dragon
	}

	dragonMesh.materials.push_back(Material{ glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.2f, 0.8f }); // A simple white material

	for (size_t i = 0; i < dragonMesh.indices.size() / 3; i++)
	{
		dragonMesh.materialIds[i] = 0; // Set all triangles to the same material for simplicity
	}

	// Add to scene
	m_Scene.AddMesh(worldMesh);
	m_Scene.AddMesh(dragonMesh);

	// Camera setup
	std::unique_ptr<Camera> camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, -9.5f), glm::vec2(0.0f, 0.0f));

	m_Scene.SetCamera(std::move(camera));
	m_Scene.SetCameraController(std::move(cameraController));
}