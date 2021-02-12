#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include "main.h"

class Texture
{
public:
	bool loadTex(std::string path);
	void render(int x, int y);
	void free();
	int getWidth();
	int getHeight();
private:
	SDL_Texture* _texture = NULL;
	int _width = 0;
	int _height = 0;
};

#endif