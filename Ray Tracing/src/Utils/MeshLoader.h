#pragma once

#include <string>
#include "Scene/Mesh.h"

class MeshLoader
{
public:
	static Mesh ParsePLY(std::string path);
};