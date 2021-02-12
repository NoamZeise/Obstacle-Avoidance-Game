#include "main.h"
#include "Texture.h"
#include "player.h"
#include "obstacle.h"
#include "obstacleManager.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 450;

bool init();
SDL_Texture* loadTextureText(std::string text, SDL_Rect* texRect);
bool loadMedia();
void close();

void DrawScore(std::stringstream& scoreText, int score, int maxScore, SDL_Texture*& currentTextTexture, SDL_Rect& scoreDestination);

void HandleEvents(SDL_Event& e, bool& quit);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont;
SDL_Color gTextColour = { 255, 255, 255, 255 };
Player gPlayer;

bool init()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("unable to initialise sdl SDL Error: %s", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Failed to initalise window, SDL Error: %s", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Failed to initalise renderer, SDL Error: %s", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//init png loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL image could not initalize SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				if (TTF_Init() == -1)
				{
					printf("failed to initilaise SDL_TTF, SDL TTF Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

SDL_Texture* loadTextureText(std::string text, SDL_Rect* texRect)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), gTextColour);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (texture == NULL)
		{
			printf("failed to create texture from surface, SDL Error: %s\n", SDL_GetError());
		}
		texRect->w = textSurface->w;
		texRect->h = textSurface->h;
		SDL_FreeSurface(textSurface);
		textSurface = NULL;
	}
	return texture;
}


bool loadMedia()
{
	bool success = true;
	Texture tempTex;
	if (!tempTex.loadTex("Textures/player.png"))
	{
		success = false;
	}
	gPlayer.setTex(tempTex);

	gFont = TTF_OpenFont("Fonts/TitilliumWeb-Regular.ttf", 20);
	if (gFont == NULL)
	{
		printf("Failed to load TitilliumWeb-Black.ttf font, SDL TTF Error: %s", TTF_GetError());
	}

	return success;
}

void close()
{
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}




int main(int argc, char* args[])
{
	std::vector<Obstacle> obstacles(0);
	ObstacleManager obstacleManager;
	int score = 0;
	int maxScore = 0;

	struct stat buffer;
	if (stat("highscore.txt", &buffer) == 0)
	{
		std::ifstream input;
		input.open("highscore.txt");
		std::string line;
		std::getline(input, line);
		maxScore = std::stoi(line);
		input.close();
	}
	else
	{
		std::ofstream output;
		output.open("highscore.txt");
		output.close();
	}
	if (!init())
	{
		printf("Failed to inilaise\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media\n");
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			std::stringstream scoreText;
			SDL_Texture* currentTextTexture = NULL;
			SDL_Rect scoreDestination = { 10, 0, 0, 0 };
			while (!quit)
			{
				HandleEvents(e, quit);

				gPlayer.update(obstacles);
				if (gPlayer.isDead())
				{
					if (score > maxScore)
						maxScore = score;
					score = 0;
					obstacles.clear();
					gPlayer.Reset();
				}
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(gRenderer);

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x11, 0x05, 0xFF);
				obstacleManager.Update(&obstacles, &score);
				if (score > maxScore)
					maxScore = score;
				gPlayer.Draw();
				DrawScore(scoreText, score, maxScore, currentTextTexture, scoreDestination);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	std::ofstream writeHighScore;
	writeHighScore.open("highscore.txt");
	writeHighScore.clear();
	writeHighScore << maxScore;
	writeHighScore.close();

	obstacles.~vector();
	gPlayer.~Player();
	close();
	return 0;
}

void HandleEvents(SDL_Event& e, bool& quit)
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
			quit = true;
		if (e.type == SDL_KEYDOWN)
			if (e.key.keysym.sym == SDLK_ESCAPE)
				quit = true;
		gPlayer.handleEvent(&e);
	}
}

void DrawScore(std::stringstream& scoreText, int score, int maxScore, SDL_Texture*& currentTextTexture, SDL_Rect& scoreDestination)
{
	scoreText.str("");
	scoreText << "Score: " << score;
	currentTextTexture = loadTextureText(scoreText.str().c_str(), &scoreDestination);
	if (currentTextTexture == NULL)
	{
		printf("Failed to load texture from rendered tex\n");
	}
	SDL_RenderCopy(gRenderer, currentTextTexture, NULL, &scoreDestination);
	SDL_DestroyTexture(currentTextTexture);
	currentTextTexture = NULL;
	scoreText.str("");
	scoreText << "Highscore: " << maxScore;
	currentTextTexture = loadTextureText(scoreText.str().c_str(), &scoreDestination);
	scoreDestination.x = SCREEN_WIDTH - scoreDestination.w - 10;
	if (currentTextTexture == NULL)
	{
		printf("Failed to load texture from rendered tex\n");
	}
	SDL_RenderCopy(gRenderer, currentTextTexture, NULL, &scoreDestination);
	SDL_DestroyTexture(currentTextTexture);
	currentTextTexture = NULL;
	scoreDestination.x = 10;
}
