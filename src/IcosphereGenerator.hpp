/// Seed of Andromeda Icosphere Generator
/// Written by Frank McCoy
/// Use it for whatever, but remember where you got it from.

#include <cstdint>
#include <unordered_map>
#include <vector>
#include "glmInclude.hpp"

const static float GOLDEN_RATIO = 1.61803398875f;

const static int NUM_ICOSOHEDRON_VERTICES = 12;
const static vector3 ICOSOHEDRON_VERTICES[12] = {
	vector3(-1.0f, GOLDEN_RATIO, 0.0f),
	vector3(1.0f, GOLDEN_RATIO, 0.0f),
	vector3(-1.0f, -GOLDEN_RATIO, 0.0f),
	vector3(1.0f, -GOLDEN_RATIO, 0.0f),

	vector3(0.0f, -1.0f, GOLDEN_RATIO),
	vector3(0.0f, 1.0f, GOLDEN_RATIO),
	vector3(0.0f, -1.0, -GOLDEN_RATIO),
	vector3(0.0f, 1.0f, -GOLDEN_RATIO),

	vector3(GOLDEN_RATIO, 0.0f, -1.0f),
	vector3(GOLDEN_RATIO, 0.0f, 1.0f),
	vector3(-GOLDEN_RATIO, 0.0f, -1.0f),
	vector3(-GOLDEN_RATIO, 0.0, 1.0f)
};

const static int NUM_ICOSOHEDRON_INDICES = 60;
const static unsigned int ICOSOHEDRON_INDICES[60] = {
	0, 11, 5,
	0, 5, 1,
	0, 1, 7,
	0, 7, 10,
	0, 10, 11,

	1, 5, 9,
	5, 11, 4,
	11, 10, 2,
	10, 7, 6,
	7, 1, 8,

	3, 9, 4,
	3, 4, 2,
	3, 2, 6,
	3, 6, 8,
	3, 8, 9,

	4, 9, 5,
	2, 4, 11,
	6, 2, 10,
	8, 6, 7,
	9, 8, 1
};

// Hash functions for the unordered map
class Vec3KeyFuncs {
public:
	size_t operator()(const vector3& k)const {
		return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
	}

	bool operator()(const vector3& a, const vector3& b)const {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

inline vector3 findMidpoint(vector3 vertex1, vector3 vertex2) {
	return glm::normalize(vector3((vertex1.x + vertex2.x) / 2.0f, (vertex1.y + vertex2.y) / 2.0f, (vertex1.z + vertex2.z) / 2.0f));
}

/// Generates an icosphere with radius 1.0f.
/// @param lod: Number of subdivisions
/// @param indices: Resulting indices for use with glDrawElements
/// @param vertices: Resulting vertex vertices
void generateIcosphereMesh(size_t lod, std::vector<vector3>& vertices, std::vector<vector3>& normals, std::vector<unsigned int>& indices) {
	std::vector<unsigned int> newIndices;
	newIndices.reserve(256);

	std::unordered_map<vector3, unsigned int, Vec3KeyFuncs, Vec3KeyFuncs> vertexLookup;

	indices.resize(NUM_ICOSOHEDRON_INDICES);
	for (unsigned int i = 0; i < NUM_ICOSOHEDRON_INDICES; i++) {
		indices[i] = ICOSOHEDRON_INDICES[i];
	}
	vertices.resize(NUM_ICOSOHEDRON_VERTICES);
	for (unsigned int i = 0; i < NUM_ICOSOHEDRON_VERTICES; i++) {
		vertices[i] = glm::normalize(ICOSOHEDRON_VERTICES[i]);
		vertexLookup[glm::normalize(ICOSOHEDRON_VERTICES[i])] = i;
	}

	for (size_t i = 0; i < (size_t)lod; i++) {
		for (size_t j = 0; j < indices.size(); j += 3) {
			/*
			j
			mp12   mp13
			j+1    mp23   j+2
			*/
			// Defined in counter clockwise order
			vector3 vertex1 = vertices[indices[j + 0]];
			vector3 vertex2 = vertices[indices[j + 1]];
			vector3 vertex3 = vertices[indices[j + 2]];

			vector3 midPoint12 = findMidpoint(vertex1, vertex2);
			vector3 midPoint23 = findMidpoint(vertex2, vertex3);
			vector3 midPoint13 = findMidpoint(vertex1, vertex3);

			unsigned int mp12Index;
			unsigned int mp23Index;
			unsigned int mp13Index;

			auto iter = vertexLookup.find(midPoint12);
			if (iter != vertexLookup.end()) { // It is in the map
				mp12Index = iter->second;
			}
			else { // Not in the map
				mp12Index = (unsigned int)vertices.size();
				vertices.push_back(midPoint12);
				vertexLookup[midPoint12] = mp12Index;
			}

			iter = vertexLookup.find(midPoint23);
			if (iter != vertexLookup.end()) { // It is in the map
				mp23Index = iter->second;
			}
			else { // Not in the map
				mp23Index = (unsigned int)vertices.size();
				vertices.push_back(midPoint23);
				vertexLookup[midPoint23] = mp23Index;
			}

			iter = vertexLookup.find(midPoint13);
			if (iter != vertexLookup.end()) { // It is in the map
				mp13Index = iter->second;
			}
			else { // Not in the map
				mp13Index = (unsigned int)vertices.size();
				vertices.push_back(midPoint13);
				vertexLookup[midPoint13] = mp13Index;
			}

			newIndices.push_back(indices[j]);
			newIndices.push_back(mp12Index);
			newIndices.push_back(mp13Index);

			newIndices.push_back(mp12Index);
			newIndices.push_back(indices[j + 1]);
			newIndices.push_back(mp23Index);

			newIndices.push_back(mp13Index);
			newIndices.push_back(mp23Index);
			newIndices.push_back(indices[j + 2]);

			newIndices.push_back(mp12Index);
			newIndices.push_back(mp23Index);
			newIndices.push_back(mp13Index);
		}
		indices.swap(newIndices);
		newIndices.clear();
	}
}