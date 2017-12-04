#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <iostream>

class CTexture
{
public:

    CTexture(SDL_Renderer* renderer, 
                TTF_Font* font,
                const std::string &textureText,
                const SDL_Color &textColor);                //  Initializes variables

    CTexture(SDL_Renderer* renderer,
                const std::string path);

    ~CTexture();

    //  Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //  Set blending
    void setBlendMode(SDL_BlendMode blending);

    //  Set alpha modulation
    void setAlpha(Uint8 alpha);

    //  Renders texture at given point
    void render(int x, int y, 
        SDL_Rect* clip  = NULL, 
        double angle = 0.0, 
        SDL_Point* center = NULL, 
        SDL_RendererFlip flip = SDL_FLIP_NONE) const;

    //  Gets image dimensions
    int getWidth() const;
    int getHeight() const;

private:
    //  The actual texture
    SDL_Texture* mTexture;
    SDL_Renderer* gaRenderer;

    //  Image dimensions
    int mWidth;
    int mHeight;

#ifdef _SDL_TTF_H
    //  Creates image from font string
    bool loadFromRenderedText(const std::string &textureText, TTF_Font* font, 
                                    const SDL_Color &textColor);
#endif

    bool loadFromFile(const std::string &path);             //  Loads image at specified path
    void free();                                            //  Deallocates texture
};
