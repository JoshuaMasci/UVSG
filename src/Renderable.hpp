#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include "Components.hpp"
#include "Rendering/ShaderProgram.hpp"
#include "Rendering/Mesh.hpp"
#include <vector>

struct Model
{
	Mesh* mesh;
	ShaderProgram* shader;
	Transform localOffset;
	string texture;
};

class FarZoneRenderable
{
public:
	vector<Model*> models;
};

class NearZoneRenderable
{
public:
	vector<Model*> models;
};

#endif //RENDERABLE_HPP