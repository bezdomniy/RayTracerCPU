#include "SpriteSheet.h"


SpriteSheet::SpriteSheet()
{

}

SpriteSheet::SpriteSheet(std::string const& sp, std::string const& cp, bool json)
{
	spritesPath = sp;
	//loadText(cp);
	if (json)
		loadJSON(cp);
	else
		loadText(cp);
}

SpriteSheet::SpriteSheet(std::string const& sp, int spriteWidth, int spriteHeight, int tilesHorizontal, int tilesVertical)
{
	spritesPath = sp;
	loadSequentialImage(spriteWidth, spriteHeight, tilesHorizontal, tilesVertical);
}

SpriteSheet::~SpriteSheet()
{
}

void SpriteSheet::destroy()
{
	delete this;
}

//void SpriteSheet::print()
//{
//	for (auto element : gSpriteClips) {
//		std::printf("%s ", element.first.c_str());
//
//		std::printf("%d ", element.second.first->x);
//		std::printf("%d ", element.second.first->y);
//		std::printf("%d ", element.second.first->w);
//		std::printf("%d ", element.second.first->h);
//		std::printf("%d\n", element.second.second);
//	}
//}

std::vector<Sprite*>* SpriteSheet::operator[](std::string const& spriteName)
{
	//std::cout << gSpriteClips[spriteName].size() << "\n";
	return &spriteClips[spriteName];
}

Sprite* SpriteSheet::operator[](int const index)
{
	return sprites[index];
}

void SpriteSheet::loadText(std::string coordsPath)
{
	std::ifstream infile(coordsPath);

	std::string line;
	std::string name;

	int frames = 1;

	Sprite* nextRect = nullptr;
	while (std::getline(infile, line)) {
		std::stringstream s(line);

		s >> name;

		nextRect = new Sprite();
		s >> nextRect->x;
		s >> nextRect->y;
		s >> nextRect->w;
		s >> nextRect->h;
		s >> frames;

		for (int i = 0; i < frames; i++) {
			nextRect = new Sprite(nextRect->x + i * nextRect->w, nextRect->y, nextRect->w, nextRect->h);

			sprites.push_back(nextRect);
			spriteClips[name].push_back(nextRect);
		}
	}
	infile.close();
}

void SpriteSheet::loadJSON(std::string coordsPath)
{
	std::ifstream i(coordsPath);
	if (!i.is_open()) {
		std::cerr << "Error opening file at: " << coordsPath << "\n";
		//exit(1);
	}
	json j = json::parse(i);
	i.close();

	std::string name;
	size_t posBuffer = 0;
	int rectIndex = 0;

	// range-based for
	for (auto it = j["frames"].begin(); it != j["frames"].end(); ++it) {
		Sprite* nextRect = new Sprite();


		nextRect->x = it.value()["frame"]["x"];
		nextRect->y = it.value()["frame"]["y"];
		nextRect->w = it.value()["frame"]["w"];
		nextRect->h = it.value()["frame"]["h"];

		sprites.push_back(nextRect);

		posBuffer = it.key().find_last_of("-");
		name = it.key().substr(0, posBuffer);

		posBuffer = it.key().find_last_of(".");
		rectIndex = stoi(it.key().substr(posBuffer - 2, posBuffer));

		spriteClips[name].insert(spriteClips[name].begin() + rectIndex, nextRect);
	}

	//for (auto& e : gSpriteClips) {
	//	std::cout << e.first << ": " << e.second.size()  << "\n";
	//}

	//exit(0);
}

void SpriteSheet::loadSequentialImage(int spriteWidth, int spriteHeight, int tilesHorizontal, int tilesVertical)
{
	spriteClips.clear();
	Sprite* nextRect = nullptr;
	sprites.push_back(nextRect);
	for (int i = 0; i < tilesVertical; i++) {
		for (int j = 0; j < tilesHorizontal; j++) {
			Sprite* nextRect = new Sprite(j * spriteWidth, i * spriteHeight, spriteWidth, spriteHeight);
			//std::cout << nextRect->x << nextRect->y << "\n";
			sprites.push_back(nextRect);
		}
	}
	//std::cout << sprites.size() << "\n";
//	for (auto& e : sprites) {
//	std::cout << e->x << ", " << e->y  << "\n";
//}

//exit(0);
}



