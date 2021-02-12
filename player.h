#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "main.h"
#include "Texture.h"
#include "obstacle.h"
class Player
{
public:
	Player();
	void handleEvent(SDL_Event* e);
	void update(std::vector<Obstacle> obstacles);
	void Draw();
	void setTex(Texture tex);
	bool isDead();
	void Reset();
	~Player();
private:
	Texture _texture;
	int _x = 10;
	int _y = 50;
	double _velY = 0;
	int _width;
	int _height;
	bool _isDead = false;
	Uint32 previousTicks;
};

#endif
