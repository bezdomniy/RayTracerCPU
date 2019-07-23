#include "SpriteSheet.h"


SpriteSheet::SpriteSheet()
{

}

SpriteSheet::SpriteSheet(float sheetWidth, float sheetHeight, std::string const& sp, std::string const& cp, bool json)
{
	width = sheetWidth;
	height = sheetHeight;

	spritesPath = sp;
	//loadText(cp);
	if (json)
		loadJSON(cp);
	else
		loadText(cp);
}

SpriteSheet::SpriteSheet(float sheetWidth, float sheetHeight, std::string const& sp, int spriteWidth, int spriteHeight, int tilesHorizontal, int tilesVertical)
{
	width = sheetWidth;
	height = sheetHeight;
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

std::vector<Rectangle*>* SpriteSheet::operator[](std::string const& spriteName)
{
	//std::cout << gSpriteClips[spriteName].size() << "\n";
	return &spriteClips[spriteName];
}

Rectangle* SpriteSheet::operator[](int const index)
{
	return sprites[index];
}

void SpriteSheet::loadText(std::string coordsPath)
{
	//std::cout << "called\n";
	std::ifstream infile(coordsPath);

	std::string line;
	std::string name;

	int frames = 1;

	Rectangle* nextRect = nullptr;
	while (std::getline(infile, line)) {
		std::stringstream s(line);
		frames = 1;

		s >> name;

		nextRect = new Rectangle();
		s >> nextRect->x;
		s >> nextRect->y;
		s >> nextRect->w;
		s >> nextRect->h;
		s >> frames;



		for (int i = 0; i < frames; i++) {
			Rectangle* nextFrame = new Rectangle({ nextRect->x + ((float)i * nextRect->w), nextRect->y, nextRect->w, nextRect->h });
			sprites.push_back(nextFrame);
			spriteClips[name].push_back(nextFrame);
		}

		
	}
	infile.close();


	for (auto& anim : spriteClips) {
		for (auto& frame : anim.second) {
			frame->x = frame->x / width;
			frame->w = frame->w / width;
			frame->y = frame->y / height;
			frame->h = frame->h / height;
		}
		
	}
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
		Rectangle* nextRect = new Rectangle();


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

}

void SpriteSheet::loadSequentialImage(float spriteWidth, float spriteHeight, int tilesHorizontal, int tilesVertical)
{
	spriteClips.clear();
	Rectangle* nextRect = nullptr;
	sprites.push_back(nextRect);
	for (int i = 0; i < tilesVertical; i++) {
		for (int j = 0; j < tilesHorizontal; j++) {
			Rectangle* nextRect = new Rectangle({ float(j) * spriteWidth, float(i) * spriteHeight, spriteWidth, spriteHeight });
			//std::cout << nextRect->x << nextRect->y << "\n";
			sprites.push_back(nextRect);
		}
	}

}



