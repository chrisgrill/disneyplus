//
// Created by chris on 5/11/21.
//

#ifndef DISNEYPLUS_DISNEYSET_H
#define DISNEYPLUS_DISNEYSET_H

#include <memory>
#include <vector>
#include <algorithm>
#include "DisneyImage.h"

class DisneySet {
public:
    DisneySet();
    ~DisneySet();
    void rotate(const short direction);
    void push_back(std::shared_ptr<DisneyImage> tile);
    std::vector<std::shared_ptr<DisneyImage>> tile_set;
    static const short LEFT = 1;
    static const short RIGHT = -1;

};


#endif //DISNEYPLUS_DISNEYSET_H
