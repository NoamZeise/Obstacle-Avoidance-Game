#include "obstacle.h"

SDL_Rect* Obstacle::getRect()
{
	return &_rectangle;
}

Obstacle::Obstacle(SDL_Rect* rectangle)
{
	_rectangle = *rectangle;
	if(_rectangle.x < SCREEN_WIDTH)
		_rectangle.x = SCREEN_WIDTH;
	_previousTicks = SDL_GetTicks();
}

Obstacle::Obstacle()
{
	SDL_Rect temp = { 0, 0, 0, 0 };
	_rectangle = temp;
	_previousTicks = SDL_GetTicks();
}

void Obstacle::Draw()
{
	const SDL_Rect* tempRect = &_rectangle;
	SDL_RenderFillRect(gRenderer, tempRect);
	tempRect = NULL;
}

void Obstacle::Update()
{
	_rectangle.x -= _speed * (SDL_GetTicks() - _previousTicks) / 6;
	if (_rectangle.y < 0)
		_rectangle.y = 0;
	if (_rectangle.y > SCREEN_HEIGHT - _rectangle.h)
		_rectangle.y = SCREEN_HEIGHT - _rectangle.h;
	if (_rectangle.x < 0 - _rectangle.w)
		_removed = true;
	_previousTicks = SDL_GetTicks();
}

bool Obstacle::IsRemoved()
{
	return _removed;
}
