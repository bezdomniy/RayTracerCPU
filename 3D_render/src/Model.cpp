#include "Model.h"

Model::Model()
{
}

Model::Model(char* path)
{
	loadModel(path);
	std::cout << meshes.size() << "\n";
}

Model::~Model()
{
}

void Model::Draw(Shader shader)
{

	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
	
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	//for (int i = 0; i < scene->mNumMaterials; i++) {
	//	std::cout << scene->mMaterials[i]->mProperties << "\n";
	//}
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//aiString name;
	//material->Get(AI_MATKEY_NAME, name);
	//std::cout << mesh->mMaterialIndex << ": " << name.C_Str() << "\n";

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		//aiColor4D colour = mesh->mColors[0][i];
		//vertex.Colour = glm::vec4(colour.r, colour.g, colour.b, colour.a);
		loadColors(material, vertex);
		

		//// tangent
		//vector.x = mesh->mTangents[i].x;
		//vector.y = mesh->mTangents[i].y;
		//vector.z = mesh->mTangents[i].z;
		//vertex.Tangent = vector;
		//// bitangent
		//vector.x = mesh->mBitangents[i].x;
		//vector.y = mesh->mBitangents[i].y;
		//vector.z = mesh->mBitangents[i].z;
		//vertex.Bitangent = vector;

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::cout << material->mProperties << "\n";

		std::vector<Texture*> ambientMaps = loadMaterialsTexture(material, aiTextureType_AMBIENT, "texture_ambient");
		textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
		// 1. diffuse maps
		std::vector<Texture*> diffuseMaps = loadMaterialsTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture*> specularMaps = loadMaterialsTexture(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 2. specular maps
		std::vector<Texture*> emissiveMaps = loadMaterialsTexture(material, aiTextureType_EMISSIVE, "texture_emissive");
		textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
		// 3. normal maps
		std::vector<Texture*> normalMaps = loadMaterialsTexture(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture*> heightMaps = loadMaterialsTexture(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}



	return Mesh(vertices, indices, textures);
}

std::vector<Texture*> Model::loadMaterialsTexture(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;
	/*std::cout << mat->GetTextureCount(type) << "|\n";*/

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j]->path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture* texture = new Texture;
			texture->id = TextureFromFile(str.C_Str(), this->directory);
			texture->type = typeName;
			
			texture->path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}

	/*std::cout << textures_loaded.size() << "|\n";*/

	return textures;
}

void Model::loadColors(aiMaterial* mat, Vertex &vertex)
{
	//glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	//aiColor4D ambient;
	aiColor4D diffuse;
	//aiColor4D emissive;
	//aiColor4D reflective;
	//aiColor4D specular;

	

	//aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambient);
	//aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	//aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &emissive);
	//aiGetMaterialColor(mat, AI_MATKEY_COLOR_REFLECTIVE, &reflective);
	//aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular);

	//aiString name;

	mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	//mat->Get(AI_MATKEY_NAME, name);

	//
	//std::cout << name.C_Str() << "\n";
		
	//vertex.Colour = glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a);
	vertex.Colour = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	//vertex.Colour = glm::vec4(emissive.r, emissive.g, emissive.b, emissive.a);
	//vertex.Colour = glm::vec4(reflective.r, reflective.g, reflective.b, reflective.a);
	//vertex.Colour = glm::vec4(specular.r, specular.g, specular.b, specular.a);

}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory/*, bool gamma*/)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
	GLuint textureID;


	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL Error: %s\n", filename.c_str(), SDL_GetError());

	}
	else {
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

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

	return textureID;
}