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

	int animationSlowdown = 1;

	SDL_Rect* currentSprite;

	void nextFrame();
	void setAnimationSlowdown(int slowdown);
	
private:

};

