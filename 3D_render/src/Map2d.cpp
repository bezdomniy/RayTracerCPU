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

	switch (alignment) {
	case 0: //Left
		v0 = {glm::vec3(posX * SIZE_X, 0, posZ * SIZE_Z)}; //baseUpper
		v1 = { glm::vec3(posX * SIZE_X, 0, (posZ + 1) * SIZE_Z) }; //baseLower
		v3 = { glm::vec3(posX * SIZE_X, SIZE_Y, posZ * SIZE_Z) }; //ceilUpper
		v2 = { glm::vec3(posX * SIZE_X, SIZE_Y, (posZ + 1) * SIZE_Z) }; //ceilLower

		v0.Normal = glm::vec3(normFlip * -1.f, 0.f, 0.f);
		v1.Normal = glm::vec3(normFlip * -1.f, 0.f, 0.f);
		v2.Normal = glm::vec3(normFlip * -1.f, 0.f, 0.f);
		v3.Normal = glm::vec3(normFlip * -1.f, 0.f, 0.f);

		break;
	case 1: //Right
		v0 = { glm::vec3((posX + 1) * SIZE_X, 0, posZ * SIZE_Z) }; //baseUpper
		v1 = { glm::vec3((posX + 1) * SIZE_X, 0, (posZ + 1) * SIZE_Z) }; //baseLower
		v3 = { glm::vec3((posX + 1) * SIZE_X, SIZE_Y, posZ * SIZE_Z) }; //ceilUpper
		v2 = { glm::vec3((posX + 1) * SIZE_X, SIZE_Y, (posZ + 1) * SIZE_Z) }; //ceilLower

		v0.Normal = glm::vec3(normFlip * 1.f, 0.f, 0.f);
		v1.Normal = glm::vec3(normFlip * 1.f, 0.f, 0.f);
		v2.Normal = glm::vec3(normFlip * 1.f, 0.f, 0.f);
		v3.Normal = glm::vec3(normFlip * 1.f, 0.f, 0.f);

		break;
	case 2: //Top
		v0 = { glm::vec3(posX * SIZE_X, 0, posZ * SIZE_Z) }; //baseLeft
		v1 = { glm::vec3((posX + 1) * SIZE_X, 0, posZ * SIZE_Z) }; //baseRight
		v3 = { glm::vec3(posX * SIZE_X, SIZE_Y, posZ * SIZE_Z) }; //ceilLeft
		v2 = { glm::vec3((posX + 1) * SIZE_X, SIZE_Y, posZ * SIZE_Z) }; //ceilRight

		v0.Normal = glm::vec3(0.f, 0.f, normFlip * -1.f);
		v1.Normal = glm::vec3(0.f, 0.f, normFlip * -1.f);
		v2.Normal = glm::vec3(0.f, 0.f, normFlip * -1.f);
		v3.Normal = glm::vec3(0.f, 0.f, normFlip * -1.f);

		break;
	case 3: //Bottom
		v0 = { glm::vec3(posX * SIZE_X, 0, (posZ + 1) * SIZE_Z) }; //baseLeft
		v1 = { glm::vec3((posX + 1) * SIZE_X, 0, (posZ + 1) * SIZE_Z) }; //baseRight
		v3 = { glm::vec3(posX * SIZE_X, SIZE_Y, (posZ + 1) * SIZE_Z) }; //ceilLeft
		v2 = { glm::vec3((posX + 1) * SIZE_X, SIZE_Y, (posZ + 1) * SIZE_Z) }; //ceilRight

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

	texture = loadTextureFromFile("wall", texturePath);
	//textures.push_back(texture);

	mesh = Mesh(vertices, indices, textures);
	mesh.type = "wall";
	texture_map[mesh.type] = texture;

	if (addToMeshes)
		meshes.push_back(mesh);


	return mesh;
}

Mesh Map2d::addCorner(int posX, int posZ, int alignment, bool facingOut, const std::string& texturePath, bool addToMeshes)
{
	Mesh mesh0;
	Mesh mesh1;
	Mesh newMesh;

	switch (alignment) {
	case 0: //Top left
		mesh0 = addWall(posX, posZ, 0, facingOut, texturePath, false);
		mesh1 = addWall(posX, posZ, 2, facingOut, texturePath, false);
		break;
	case 1: //Top Right
		mesh0 = addWall(posX, posZ, 1, facingOut, texturePath, false);
		mesh1 = addWall(posX, posZ, 2, facingOut, texturePath, false);
		break;
	case 2: //Bottom Right
		mesh0 = addWall(posX, posZ, 1, facingOut, texturePath, false);
		mesh1 = addWall(posX, posZ, 3, facingOut, texturePath, false);
		break;
	case 3: //Bottom left
		mesh0 = addWall(posX, posZ, 0, facingOut, texturePath, false);
		mesh1 = addWall(posX, posZ, 3, facingOut, texturePath, false);
		break;
	default:
		break;
	}

	newMesh = mesh0.combine(mesh1);
	if (addToMeshes)
		meshes.push_back(newMesh);

	return newMesh;
}

Mesh Map2d::addBlock(int posX, int posZ, const std::string& texturePath, bool addToMeshes)
{
	Mesh mesh0;
	Mesh mesh1;
	Mesh newMesh;

	mesh0 = addCorner(posX, posZ, 0, true, texturePath, false);
	mesh1 = addCorner(posX, posZ, 2, true, texturePath, false);

	newMesh = mesh0.combine(mesh1);

	if (addToMeshes)
		meshes.push_back(newMesh);

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

	v0 = { glm::vec3(posX * SIZE_X, 0, posZ * SIZE_Z) }; 
	v1 = { glm::vec3((posX + 1) * SIZE_X, 0, posZ * SIZE_Z) }; 
	v3 = { glm::vec3(posX * SIZE_X, 0, (posZ + 1) * SIZE_Z) }; 
	v2 = { glm::vec3((posX + 1) * SIZE_X, 0, (posZ + 1) * SIZE_Z) };

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

	texture = loadTextureFromFile("floor", texturePath);

	mesh = Mesh(vertices, indices, textures);
	mesh.type = "floor";
	texture_map[mesh.type] = texture;

	meshes.push_back(mesh);

	return mesh;
}

void Map2d::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader, texture_map[meshes[i].type]);
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

Texture* Map2d::loadTextureFromFile(const std::string& name, const std::string& path)
{
	std::unordered_map<std::string, Texture*>::iterator texturePtr = texture_map.find(name);
	Texture* texture;

	if (texturePtr == texture_map.end()) {
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		GLuint textureID;
		texture = new Texture();

		if (loadedSurface == nullptr)
		{
			printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());

		}
		else {
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			texture->id = textureID;
			texture->type = "texture_diffuse";
			texture->path = path.c_str();

			GLenum format;
			if (loadedSurface->format->BytesPerPixel == 1)
				format = GL_RED;
			else if (loadedSurface->format->BytesPerPixel == 3)
				format = GL_RGB;
			else if (loadedSurface->format->BytesPerPixel == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, format, loadedSurface->w, loadedSurface->h, 0, format, GL_UNSIGNED_BYTE, loadedSurface->pixels);
			glGenerateMipmap(GL_TEXTURE_2D);

			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		//Get rid of old loaded surface 
		SDL_FreeSurface(loadedSurface);
	}
	else {
		texture = (*texturePtr).second;
	}


	return texture;
}