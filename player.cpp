#include "player.h"


Player::Player()
{
	_height = 0;
	_width = 0;
	_x = 10;
	_y = SCREEN_HEIGHT;
	_velY = 3;
	previousTicks = SDL_GetTicks();
	_isDead = false;
}

Player::~Player()
{
	_texture.free();
}

void Player::Draw()
{
	_texture.render(_x, _y);
}

void Player::handleEvent(SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN)
	{
		if (e->key.keysym.sym == SDLK_SPACE)
		{
			_velY = -_velY;
		}
	}
}

void Player::setTex(Texture tex)
{
	_texture = tex;
	_width = _texture.getWidth();
	_height = _texture.getHeight();
}

void Player::update(std::vector<Obstacle> obstacles)
{
	_y += _velY * ((SDL_GetTicks() - previousTicks) / 4);
	if (_y < 0)
	{
		_y = 0;
		_velY = -_velY;
	}
	if (_y > SCREEN_HEIGHT - _height)
	{
		_y = SCREEN_HEIGHT - _height;
		_velY = -_velY;
	}
	for (Uint32 i = 0; i < obstacles.size(); i++)
	{
		const SDL_Rect* obsRect = obstacles[i].getRect();
		const SDL_Rect playerRect = { _x, _y, _width, _height };
		SDL_Rect result;
		if (SDL_IntersectRect(obsRect, &playerRect, &result))
		{
			_isDead = true;
		}
	}
	previousTicks = SDL_GetTicks();
}

bool Player::isDead()
{
	return _isDead;
}

void Player::Reset()
{
	_y = SCREEN_HEIGHT - _width;
	_velY = 3;
	_isDead = false;
}