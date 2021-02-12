#include "Texture.h"

bool Texture::loadTex(std::string path)
{
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            _width = loadedSurface->w;
            _height = loadedSurface->h;
        }
        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    _texture = newTexture;
    return newTexture != NULL;
}

void Texture::free()
{
	if (_texture != NULL)
	{
		SDL_DestroyTexture(_texture);
		_texture = NULL;
		_width = 0;
		_height = 0;
	}
}

void Texture::render(int x, int y)
{
	SDL_Rect destination = { x, y, _width, _height };
    SDL_RenderCopy(gRenderer, _texture, NULL, &destination);
    
}

int Texture::getWidth() {
    return _width;
}
int Texture::getHeight() {
    return _height;
}