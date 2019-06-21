#include "SpriteSheet.h"


SpriteSheet::SpriteSheet()
{

}

SpriteSheet::SpriteSheet(std::string const& sp, std::string const& cp, bool json)
{
	spritesPath = sp;
	//loadText(cp);
	if (json)
		loadJSON("resources/platformer/adventurer.json");
	else
		loadText(cp);
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

std::vector<SDL_Rect*>* SpriteSheet::operator[](std::string const& spriteName)
{
	//std::cout << gSpriteClips[spriteName].size() << "\n";
	return &gSpriteClips[spriteName];
}

void SpriteSheet::loadText(std::string coordsPath)
{
	std::ifstream infile(coordsPath);

	std::string line;
	std::string name;

	int frames = 0;

	SDL_Rect* nextRect = nullptr;
	while (std::getline(infile, line)) {
		std::stringstream s(line);

		s >> name;

		nextRect = new SDL_Rect();
		s >> nextRect->x;
		s >> nextRect->y;
		s >> nextRect->w;
		s >> nextRect->h;
		s >> frames;

		for (int i = 0; i < frames; i++) {
			nextRect = new SDL_Rect({ nextRect->x + i * nextRect->w, nextRect->y, nextRect->w, nextRect->h });
			gSpriteClips[name].push_back(nextRect);
		}
	}
}

void SpriteSheet::loadJSON(std::string coordsPath)
{
	std::ifstream i(coordsPath);
	json j = json::parse(i);
	i.close();

	std::string name;
	int posBuffer = 0;
	int rectIndex = 0;

	// range-based for
	for (auto it = j["frames"].begin(); it != j["frames"].end(); ++it) {
		SDL_Rect* nextRect = new SDL_Rect();

		nextRect->x = it.value()["frame"]["x"];
		nextRect->y = it.value()["frame"]["y"];
		nextRect->w = it.value()["frame"]["w"];
		nextRect->h = it.value()["frame"]["h"];

		posBuffer = it.key().find_last_of("-");
		name = it.key().substr(0, posBuffer);

		posBuffer = it.key().find_last_of(".");
		rectIndex = stoi(it.key().substr(short(posBuffer) - 2, posBuffer));

		gSpriteClips[name].insert(gSpriteClips[name].begin()+ rectIndex, nextRect);

		//std::cout << name << " ";
		//for (auto e : gSpriteClips[name]) {
		//	std::cout << e->x << " ";
		//}
		//std::cout << "\n";
	}


	//exit(0);
}



