#include <fstream> 
#include <unordered_map>
#include <string>
#include <sstream>
#include <utility>
#include <SDL.h>

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(std::string sp, std::string cp);
	~SpriteSheet();
	void destroy();
	void print();

	std::pair<SDL_Rect*, int> operator[] (std::string spriteName);

	std::string spritesPath;
	std::unordered_map<std::string, std::pair<SDL_Rect*, int>> gSpriteClips;

private:
	void load(std::string coordsPath);
};

