//
// Created by chris on 5/10/21.
//

#ifndef DISNEYPLUS_DISNEYIMAGE_H
#define DISNEYPLUS_DISNEYIMAGE_H

#endif //DISNEYPLUS_DISNEYIMAGE_H
#include <SDL.h>
#include <string>


class DisneyImage{
public:
    DisneyImage();
    DisneyImage(std::string filename);
    DisneyImage(unsigned char* data, int len);
    ~DisneyImage();

    SDL_Surface* surf;
private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;

};
