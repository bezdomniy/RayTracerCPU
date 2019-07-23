#include "UtilityFunctions.h"

Texture* utilityFunctions::loadTextureFromFile(const std::string& name, const std::string& path, std::unordered_map<std::string, Texture*>* texture_map)
{
	//std::unordered_map<std::string, Texture*>::iterator texturePtr = texture_map->find(name);
	//texture;

	if (!texture_map || texture_map->find(name) == texture_map->end()) {
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		GLuint textureID;
		Texture* texture = new Texture();

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

		return texture;
	}
	else {
		return (*texture_map->find(name)).second;
	}


	//return texture;
}
