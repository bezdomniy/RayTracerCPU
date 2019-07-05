#pragma once
#include <SDL.h>

class Sprite :
	public SDL_Rect
{
public:
	Sprite();
	Sprite(int x, int y, int w, int h);



};