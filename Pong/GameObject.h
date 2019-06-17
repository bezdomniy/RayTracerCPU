#include <SDL.h>
#include <vector>

class GameObject
{
public:
	GameObject();
	GameObject(SDL_Rect* initialRect, int frames);
	~GameObject();

	std::vector<SDL_Rect*> sprites;
	int currentSpriteIndex = 0;
	int numberOfFrames = 0;

	SDL_Rect* currentSprite;

	void nextFrame();
	
private:

};

