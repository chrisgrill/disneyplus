//
// Created by chris on 5/11/21.
//

#include "DisneySet.h"

DisneySet::DisneySet() {

}

DisneySet::DisneySet(json data, SDL_Renderer * renderer) {
    if(data["set"]["type"] == "SetRef"){
        json refSet = json::parse(DisneyCurl::GetJson("https://cd-static.bamgrid.com/dp-117731241344/sets/bd1bfb9a-bbf7-43a0-ac5e-3e3889d7224d.json"));
        parse_set(refSet["data"]["CuratedSet"], renderer);
    }
    else{
        parse_set(data["set"], renderer);
    }

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

int DisneySet::getSize(){
    return tile_set.size();
}
void DisneySet::parse_set(json data, SDL_Renderer* renderer) {
    for (int j = 0; j < data["items"].size(); j++) {
        try {

            json item = data["items"][j];
            std::string image_key;
            if (item["type"] == "DmcVideo") {
                image_key = "program";
            } else if (item["type"] == "DmcSeries") {
                image_key = "series";
            }
            std::string tile_url = data["items"][j]["image"]["tile"]["1.78"][image_key]["default"]["url"];
            std::shared_ptr<DisneyImage> di = DisneyCurl::GetImage(tile_url);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, di->getSurf());
            di->setTexture(texture);
            tile_set.push_back(di);
        }
        catch (int e) {
            std::cout << e << std::endl;
        }
        catch (...) {
            std::cout << "Exception" << std::endl;
        }
    }
}

void DisneySet::parse_ref_set(json data, SDL_Renderer* renderer){

}