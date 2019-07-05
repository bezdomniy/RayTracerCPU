#include "Sprite.h"

Sprite::Sprite() : SDL_Rect()
{
}

Sprite::Sprite(int x, int y, int w, int h) : SDL_Rect({x,y,w,h})
{

}
