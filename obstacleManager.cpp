#include "obstacleManager.h"

ObstacleManager::ObstacleManager()
{
	_previousTicks = SDL_GetTicks();
	std::random_device rd{};
	std::mt19937 engine{ rd() };
	randomGen = engine;
	std::uniform_int_distribution<int> tempDist(0, SCREEN_HEIGHT);
	yDist = tempDist;
}

void ObstacleManager::Update(std::vector<Obstacle>* obstacles, int* score)
{
	for (Uint32 i = 0; i < obstacles->size(); i++)
	{
		obstacles->at(i).Update();
		obstacles->at(i).Draw();
		if (obstacles->at(i).IsRemoved())
		{
			*score += 10;
			obstacles->erase(obstacles->begin() + i--);
		}
	}
	
	if (_frequency < SDL_GetTicks() - _previousObstacle)
	{
		std::uniform_int_distribution<int> sideLength(10, 100);
		std::uniform_int_distribution<int> separation(100, 500);
		_previousObstacle = SDL_GetTicks();
		SDL_Rect enemyRect1 = { 0, yDist(randomGen), sideLength(randomGen), sideLength(randomGen) };
		SDL_Rect enemyRect2 = { SCREEN_WIDTH + separation(randomGen), yDist(randomGen), sideLength(randomGen), sideLength(randomGen) };
		Obstacle enemyobs1(&enemyRect1);
		Obstacle enemyobs2(&enemyRect2);
		obstacles->push_back(enemyobs1);
		obstacles->push_back(enemyobs2);
		if((*score * 10) / 2 < 2000)
			_frequency = 3000 - ((*score * 10) / 2);
	}

	_previousTicks = SDL_GetTicks();
}