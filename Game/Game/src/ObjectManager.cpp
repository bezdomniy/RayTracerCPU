#include "ObjectManager.h"


ObjectManager::ObjectManager(SDL_Renderer* renderer)
{
	indexToSprite.push_back("");
	indexToSprite.push_back("wall_mid");
	rendererPtr = renderer;
}

ObjectManager::~ObjectManager()
{
}

std::string ObjectManager::getFilenameFromPath(std::string const& path)
{
	std::size_t last_slash = path.find_last_of("/");
	return path.substr(last_slash + 1);
}

void ObjectManager::loadMapFromFile(std::string const path, std::string textureName)
{
	std::ifstream infile(path);
	std::string line;
	std::string gridBuffer;
	std::string tileName;
	std::string spriteName;

	int i = 0;
	int j = 0;

	while (std::getline(infile, line)) {
		j = 0;
		for (char& c: line) {
			gridBuffer = c;
			if (gridBuffer.compare(",") == 0)
				continue;
			if (stoi(gridBuffer) == 0) {
				j++;
				continue;
			}

			spriteName = indexToSprite[stoi(gridBuffer)];

			tileName = "tile" + std::to_string(i) + std::to_string(j);

			//std::cout << "|";
			//for (auto& c : tiles) {
			//	std::cout << c.first << " ";
			//}
			//std::cout << "|\n\n";

			tiles[tileName] = Entity(tileName, getSpriteSheet(spriteName), spriteName, j * gridWidth, i * gridHeight, rendererPtr, &tiles);
			tiles[tileName].setTexture(texturesByName[textureName]);

			j++;
		}

		i++;
	}

	infile.close();
}

void ObjectManager::addSpriteSheet(std::string const& sp, std::string const& cp, bool json)
{
	SpriteSheet* newSpriteSheet = new SpriteSheet(sp, cp, json);
	spriteSheets[ObjectManager::getFilenameFromPath(sp)] = newSpriteSheet;
}

void ObjectManager::addTexture(SDL_Renderer* renderer, SpriteSheet* spriteSheet)
{
	Texture* newTexture = new Texture(renderer);
	newTexture->loadFromFile(spriteSheet->spritesPath);
	texturesByIndex.push_back(newTexture);
	texturesByName[ObjectManager::getFilenameFromPath(spriteSheet->spritesPath)] = newTexture;
}

SpriteSheet* ObjectManager::getSpriteSheet(std::string const spriteName)
{
	for (auto& spriteSheet : spriteSheets) {
		for (auto& sprite : spriteSheet.second->gSpriteClips) {
			if (sprite.first == spriteName) {
				return spriteSheet.second;
			}
		}
	}
	return nullptr;
}

