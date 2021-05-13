//
// Created by chris on 5/11/21.
//

#ifndef DISNEYPLUS_DISNEYSET_H
#define DISNEYPLUS_DISNEYSET_H

#include <memory>
#include <vector>
#include <algorithm>
#include <SDL.h>
#include "DisneyImage.h"
#include "DisneyCurl.h"
#include "json.hpp"
using json = nlohmann::json;

class DisneySet {
public:
    DisneySet();
    DisneySet(json data, SDL_Renderer* renderer);
    ~DisneySet();
    void rotate(const short direction);
    void push_back(std::shared_ptr<DisneyImage> tile);
    int getSize();
    std::vector<std::shared_ptr<DisneyImage>> tile_set;
    static const short LEFT = 1;
    static const short RIGHT = -1;
private:
    void parse_set(json data, SDL_Renderer* renderer);
    void parse_ref_set(json data, SDL_Renderer* renderer);

};


#endif //DISNEYPLUS_DISNEYSET_H
