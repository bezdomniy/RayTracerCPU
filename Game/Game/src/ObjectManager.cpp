#include "ObjectManager.h"


ObjectManager::ObjectManager(SDL_Renderer* renderer)
{
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
	std::ifstream inStream(path);
	if (!inStream.is_open()) {
		std::cerr << "Error opening file at: " << path << "\n";
		exit(1);
	}
	json mapJSON = json::parse(inStream);
	inStream.close();

	int width = mapJSON["layers"][0]["width"];
	int height = mapJSON["layers"][0]["height"];
	int index = 0;
	std::string tileName;

	//for (int i : j["layers"][0]["data"]) {
	//	std::cout << i << ", ";
	//}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			index = mapJSON["layers"][0]["data"][(i * width) + j];
			
			//std::cout << (i * height) + j << ", ";
			if (spriteSheets[textureName]->operator[](index) != nullptr) {
				//std::cout << (i * height) + j << ": ";
				//std::cout << mapJSON["layers"][0]["data"][(i * height) + j] << "\n";
				tileName = "tile" + std::to_string(i) + std::to_string(j);
				tiles[tileName] = Entity(tileName, spriteSheets[textureName], index, j * gridWidth, i * gridHeight, rendererPtr, &tiles);
				tiles[tileName].setTexture(texturesByName[textureName]);
			}

		}
	}
	//std::cout << "\n";
	//exit(1);

}

//void ObjectManager::loadMapFromFile(std::string const path, std::string textureName)
//{
//	std::ifstream infile(path);
//	std::string line;
//	std::string gridBuffer;
//	std::string tileName;
//	//std::string spriteName;
//
//	int i = 0;
//	int j = 0;
//	int index = 0;
//
//	while (std::getline(infile, line)) {
//		j = 0;
//		for (char& c: line) {
//			gridBuffer = c;
//			if (gridBuffer.compare(",") == 0)
//				continue;
//			if (stoi(gridBuffer) == 0) {
//				j++;
//				continue;
//			}
//
//			tileName = "tile" + std::to_string(i) + std::to_string(j);
//
//			index = stoi(gridBuffer);
//
//			tiles[tileName] = Entity(tileName, spriteSheets[textureName], index, j * gridWidth, i * gridHeight, rendererPtr, &tiles);
//			tiles[tileName].setTexture(texturesByName[textureName]);
//
//			j++;
//		}
//
//		i++;
//	}
//
//	infile.close();
//}

void ObjectManager::addSpriteSheetWithCoordinatesFile(std::string const& sp, std::string const& cp, bool json)
{
	SpriteSheet* newSpriteSheet = new SpriteSheet(sp, cp, json);
	spriteSheets[ObjectManager::getFilenameFromPath(sp)] = newSpriteSheet;
}

void ObjectManager::addSpriteSheet(std::string const& sp, int spriteWidth, int spriteHeight, int tilesHorizontal, int tilesVertical)
{
	SpriteSheet* newSpriteSheet = new SpriteSheet(sp, spriteWidth, spriteHeight, tilesHorizontal, tilesVertical);
	spriteSheets[ObjectManager::getFilenameFromPath(sp)] = newSpriteSheet;
}

void ObjectManager::addTexture(SDL_Renderer* renderer, SpriteSheet* spriteSheet)
{
	Texture* newTexture = new Texture(renderer);
	newTexture->loadFromFile(spriteSheet->spritesPath);
	//texturesByIndex.push_back(newTexture);
	texturesByName[ObjectManager::getFilenameFromPath(spriteSheet->spritesPath)] = newTexture;
}

SpriteSheet* ObjectManager::getSpriteSheet(std::string const spriteName)
{
	for (auto& spriteSheet : spriteSheets) {
		for (auto& sprite : spriteSheet.second->spriteClips) {
			if (sprite.first == spriteName) {
				return spriteSheet.second;
			}
		}
	}
	return nullptr;
}

