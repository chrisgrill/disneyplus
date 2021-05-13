//
// Created by chris on 5/11/21.
//

#include "DisneySet.h"

DisneySet::DisneySet() {

}

DisneySet::~DisneySet() {
    tile_set.clear();
}

void DisneySet::rotate(const short direction) {
    int shift = direction;
    if(direction==RIGHT){
        shift += tile_set.size();
    }
    std::rotate(tile_set.begin(), tile_set.begin()+shift, tile_set.end());
}

void DisneySet::push_back(std::shared_ptr<DisneyImage> tile) {
    tile_set.push_back(tile);
}