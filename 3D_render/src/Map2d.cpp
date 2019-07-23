#include "Map2d.h"

#define SIZE_Y 1.5
#define SIZE_X 1
#define SIZE_Z 1

Map2d::Map2d()
{
}

Map2d::Map2d(char* path)
{
	//loadModel(path);
	//std::cout << meshes.size() << "\n";
}

Map2d::~Map2d()
{
}

Mesh Map2d::addWall(int posX, int posZ, int alignment, bool facingOut, const std::string& texturePath, bool addToMeshes)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	Vertex v0;
	Vertex v1;
	Vertex v2;
	Vertex v3;

	Texture* texture;
	Mesh mesh;

	int normFlip = -1;
	if (facingOut) {
		normFlip = 1;
	}

	glm::vec3 worldPosition;

	switch (alignment) {
	case 0: //Left
		v0 = { glm::vec3(0, 0, 0) }; //baseUpper
		v1 = { glm::vec3(0, 0, SIZE_Z) }; //baseLower
		v2 = { glm::vec3(0, SIZE_Y, SIZE_Z) }; //ceilLower
		v3 = { glm::vec3(0, SIZE_Y, 0) }; //ceilUpper

		worldPosition = glm::vec3(posX * SIZE_X, 0.f, posZ * SIZE_Z);

		v0.Normal = glm::vec3(normFlip * -1.f, 0.f, 0.f);
		v1.Normal = glm::vec3(normFlip * -1.f, 0.f, 0.f);
		v2.Normal = glm::vec3(normFlip * -1.f, 0.f, 0.f);
		v3.Normal = glm::vec3(normFlip * -1.f, 0.f, 0.f);

		break;
	case 1: //Right
		v0 = { glm::vec3(0, 0, 0) }; //baseUpper
		v1 = { glm::vec3(0, 0, SIZE_Z) }; //baseLower
		v2 = { glm::vec3(0, SIZE_Y, SIZE_Z) }; //ceilLower
		v3 = { glm::vec3(0, SIZE_Y, 0) }; //ceilUpper

		worldPosition = glm::vec3((posX + 1) * SIZE_X, 0.f, posZ * SIZE_Z);

		v0.Normal = glm::vec3(normFlip * 1.f, 0.f, 0.f);
		v1.Normal = glm::vec3(normFlip * 1.f, 0.f, 0.f);
		v2.Normal = glm::vec3(normFlip * 1.f, 0.f, 0.f);
		v3.Normal = glm::vec3(normFlip * 1.f, 0.f, 0.f);

		break;
	case 2: //Top
		v0 = { glm::vec3(0, 0, 0) }; //baseLeft
		v1 = { glm::vec3(SIZE_X, 0, 0) }; //baseRight
		v2 = { glm::vec3(SIZE_X, SIZE_Y, 0) }; //ceilRight
		v3 = { glm::vec3(0, SIZE_Y, 0) }; //ceilLeft

		worldPosition = glm::vec3(posX * SIZE_X, 0.f, posZ * SIZE_Z);

		v0.Normal = glm::vec3(0.f, 0.f, normFlip * -1.f);
		v1.Normal = glm::vec3(0.f, 0.f, normFlip * -1.f);
		v2.Normal = glm::vec3(0.f, 0.f, normFlip * -1.f);
		v3.Normal = glm::vec3(0.f, 0.f, normFlip * -1.f);

		break;
	case 3: //Bottom
		v0 = { glm::vec3(0, 0, 0) }; //baseLeft
		v1 = { glm::vec3(SIZE_X, 0, 0) }; //baseRight
		v2 = { glm::vec3(SIZE_X, SIZE_Y, 0) }; //ceilRight
		v3 = { glm::vec3(0, SIZE_Y, 0) }; //ceilLeft

		worldPosition = glm::vec3(posX * SIZE_X, 0.f, (posZ + 1) * SIZE_Z);

		v0.Normal = glm::vec3(0.f, 0.f, normFlip * 1.f);
		v1.Normal = glm::vec3(0.f, 0.f, normFlip * 1.f);
		v2.Normal = glm::vec3(0.f, 0.f, normFlip * 1.f);
		v3.Normal = glm::vec3(0.f, 0.f, normFlip * 1.f);

		break;
	default:
		break;
	}

	v0.TexCoords = glm::vec2(0.0f, 0.0f);
	v1.TexCoords = glm::vec2(1.0f, 0.0f);
	v2.TexCoords = glm::vec2(1.0f, 1.0f);
	v3.TexCoords = glm::vec2(0.0f, 1.0f);

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	indices = { 0, 1, 3,
				1, 3, 2 };

	texture = utilityFunctions::loadTextureFromFile("wall", texturePath, &texture_map);
	//textures.push_back(texture);

	mesh = Mesh(vertices, indices, textures);
	mesh.type = "wall";
	texture_map[mesh.type] = texture;

	if (addToMeshes) {
		meshes.push_back(mesh);
		worldSpacePositions.push_back(worldPosition);
	}
		


	return mesh;
}

Mesh Map2d::addCorner(int posX, int posZ, int alignment, bool facingOut, const std::string& texturePath, bool addToMeshes)
{
	Mesh mesh0;
	Mesh mesh1;
	Mesh newMesh;

	glm::vec3 worldPosition = glm::vec3(posX * SIZE_X, 0.f, posZ * SIZE_Z);

	switch (alignment) {
	case 0: //Top left
		mesh0 = addWall(posX, posZ, 0, facingOut, texturePath, false);
		mesh1 = addWall(posX, posZ, 2, facingOut, texturePath, false);
		break;
	case 1: //Top Right
		mesh0 = addWall(posX, posZ, 1, facingOut, texturePath, false);
		mesh1 = addWall(posX, posZ, 2, facingOut, texturePath, false);

		for (auto& v : mesh0.vertices) {
			v.Position.x += SIZE_X;
		}

		break;
	case 2: //Bottom Right
		mesh0 = addWall(posX, posZ, 1, facingOut, texturePath, false);
		mesh1 = addWall(posX, posZ, 3, facingOut, texturePath, false);

		for (auto& v : mesh0.vertices) {
			v.Position.x += SIZE_X;
		}

		for (auto& v : mesh1.vertices) {
			v.Position.z += SIZE_Z;
		}

		break;
	case 3: //Bottom left
		mesh0 = addWall(posX, posZ, 0, facingOut, texturePath, false);
		mesh1 = addWall(posX, posZ, 3, facingOut, texturePath, false);

		for (auto& v : mesh1.vertices) {
			v.Position.z += SIZE_Z;
		}
			
		break;
	default:
		break;
	}

	newMesh = mesh0.combine(mesh1);
	if (addToMeshes) {
		meshes.push_back(newMesh);
		worldSpacePositions.push_back(worldPosition);
	}
		

	return newMesh;
}

Mesh Map2d::addBlock(int posX, int posZ, const std::string& texturePath, bool addToMeshes)
{
	Mesh mesh0;
	Mesh mesh1;
	Mesh newMesh;

	glm::vec3 worldPosition = glm::vec3(posX * SIZE_X, 0.f, posZ * SIZE_Z);

	mesh0 = addCorner(posX, posZ, 0, true, texturePath, false);
	mesh1 = addCorner(posX, posZ, 2, true, texturePath, false);

	newMesh = mesh0.combine(mesh1);

	if (addToMeshes) {
		meshes.push_back(newMesh);
		worldSpacePositions.push_back(worldPosition);
	}


	return newMesh;
}

Mesh Map2d::addFloor(int posX, int posZ, const std::string& texturePath)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	Vertex v0;
	Vertex v1;
	Vertex v2;
	Vertex v3;

	Texture* texture;
	Mesh mesh;

	v0 = { glm::vec3(0, 0, 0) }; 
	v1 = { glm::vec3(SIZE_X, 0, 0) }; 
	v3 = { glm::vec3(0, 0, SIZE_Z) }; 
	v2 = { glm::vec3(SIZE_X, 0, SIZE_Z) };

	worldSpacePositions.push_back(glm::vec3(posX * SIZE_X, 0.f, posZ * SIZE_Z));

	v0.Normal = glm::vec3(0.f, 1.f, 0.f);
	v1.Normal = glm::vec3(0.f, 1.f, 0.f);
	v2.Normal = glm::vec3(0.f, 1.f, 0.f);
	v3.Normal = glm::vec3(0.f, 1.f, 0.f);


	v0.TexCoords = glm::vec2(0.0f, 0.0f);
	v1.TexCoords = glm::vec2(1.0f, 0.0f);
	v2.TexCoords = glm::vec2(1.0f, 1.0f);
	v3.TexCoords = glm::vec2(0.0f, 1.0f);

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	indices = { 0, 1, 3,
				1, 2, 3 };

	texture = utilityFunctions::loadTextureFromFile("floor", texturePath, &texture_map);

	mesh = Mesh(vertices, indices, textures);
	mesh.type = "floor";
	texture_map[mesh.type] = texture;

	meshes.push_back(mesh);

	return mesh;
}

void Map2d::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader, texture_map[meshes[i].type], worldSpacePositions[i]);
	}
}

void Map2d::loadMap(int grid[], size_t gridSize, int width, int height, const std::string& wallTexturePath, const std::string& floorTexturePath)
{
	int index = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			index = grid[(i * width) + j];

			switch (index) {
			case 0:
				break;
			case 1:
				addWall(j, i, 0, false, wallTexturePath);
				break;
			case 2:
				addWall(j, i, 1, false, wallTexturePath);
				break;
			case 3:
				addWall(j, i, 2, false, wallTexturePath);
				break;
			case 4:
				addWall(j, i, 3, false, wallTexturePath);
				break;
			case 5:
				addCorner(j, i, 0, false, wallTexturePath);
				break;
			case 6:
				addCorner(j, i, 1, false, wallTexturePath);
				break;
			case 7:
				addCorner(j, i, 2, false, wallTexturePath);
				break;
			case 8:
				addCorner(j, i, 3, false, wallTexturePath);
				break;
			case 9:
				addBlock(j, i, wallTexturePath);
				break;
			default:
				break;
			}
			addFloor(j, i, floorTexturePath);
		}
	}
}

