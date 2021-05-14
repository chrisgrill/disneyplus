//
// Created by chris on 5/11/21.
//

#ifndef DISNEYPLUS_DISNEYSET_H
#define DISNEYPLUS_DISNEYSET_H

#include <memory>
#include <vector>
#include <algorithm>
#include <SDL.h>
#include <SDL_ttf.h>
#include "DisneyImage.h"
#include "DisneyCurl.h"
#include "DisneyTile.h"
#include "json.hpp"
using json = nlohmann::json;

class DisneySet {
public:
    DisneySet();
    DisneySet(json data, SDL_Renderer* renderer);
    ~DisneySet();
    void Rotate(const short direction);
    void PushBack(std::shared_ptr<DisneyTile> tile);
    void LoadBackground(SDL_Renderer* renderer, TTF_Font* font);
    int GetSize();
    std::vector<std::shared_ptr<DisneyTile>> tile_set;
    static const short LEFT = 1;
    static const short RIGHT = -1;
private:
    void ParseSet(json data, SDL_Renderer* renderer);
    void RenderText(std::string text, SDL_Rect rect, TTF_Font* font, SDL_Renderer* renderer);

};


#endif //DISNEYPLUS_DISNEYSET_H
