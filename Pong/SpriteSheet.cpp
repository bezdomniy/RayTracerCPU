#include "SpriteSheet.h"

SpriteSheet::SpriteSheet()
{

}

SpriteSheet::SpriteSheet(std::string sp, std::string cp)
{
	spritesPath = sp;
	load(cp);
}

SpriteSheet::~SpriteSheet()
{
}

void SpriteSheet::destroy()
{
	delete this;
}

void SpriteSheet::print()
{
	for (auto element : gSpriteClips) {
		std::printf("%s ", element.first.c_str());

		std::printf("%d ", element.second.first->x);
		std::printf("%d ", element.second.first->y);
		std::printf("%d ", element.second.first->w);
		std::printf("%d ", element.second.first->h);
		std::printf("%d\n", element.second.second);
	}
}

std::pair<SDL_Rect*, int> SpriteSheet::operator[](std::string spriteName)
{
	return gSpriteClips[spriteName];
}

void SpriteSheet::load(std::string coordsPath)
{
	std::ifstream infile(coordsPath);

	std::string line;
	std::string word;

	while (std::getline(infile, line)) {
		std::stringstream s(line);

		s >> word;

		SDL_Rect* nextRect = new SDL_Rect();
		s >> nextRect->x;
		s >> nextRect->y;
		s >> nextRect->w;
		s >> nextRect->h;
		gSpriteClips[word].first = nextRect;
		s >> gSpriteClips[word].second;
	}
}



