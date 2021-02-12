#pragma once
#ifndef OBSTACLEMANAGER_H
#define OBSTACLEMANAGER_H
#include "main.h"
#include "obstacle.h"
#include <random>
class ObstacleManager
{
public:
	void Update(std::vector<Obstacle>* obstacles, int* score);
	ObstacleManager();
private:
	Uint32 _previousTicks;
	Uint32 _previousObstacle = 0;
	Uint32 _frequency = 3000;
	std::mt19937 randomGen;
	std::uniform_int_distribution<int> yDist;
};


#endif