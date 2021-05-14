//
// Created by chris on 5/10/21.
//

#ifndef DISNEYPLUS_DISNEYIMAGE_H
#define DISNEYPLUS_DISNEYIMAGE_H
#include <SDL.h>
#include <string>
#include <iostream>


class DisneyImage{
public:
    DisneyImage(unsigned char* data, int len);
    ~DisneyImage();


private:
    SDL_Surface* surf;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
public:
    SDL_Texture *getTexture() const;
    void setTexture(SDL_Texture *texture);
    SDL_Renderer *getRenderer() const;
    void setRenderer(SDL_Renderer *renderer);
    SDL_Surface *getSurf() const;
    void setSurf(SDL_Surface *surf);




};
#endif //DISNEYPLUS_DISNEYIMAGE_H

