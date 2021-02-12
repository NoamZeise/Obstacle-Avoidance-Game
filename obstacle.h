#pragma once
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "main.h"

class Obstacle
{
public:
	SDL_Rect* getRect();
	void Draw();
	void Update();
	Obstacle(SDL_Rect* rectangle);
	Obstacle();
	bool IsRemoved();
private:
	SDL_Rect _rectangle;
	double _speed = 2;
	Uint32 _previousTicks;
	bool _removed = false;
};

#endif