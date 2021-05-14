//
// Created by chris on 5/11/21.
//


#include "DisneySet.h"

DisneySet::DisneySet() {

}

DisneySet::DisneySet(json data, SDL_Renderer * renderer) {
    if(data["set"]["type"] == "SetRef"){
        json refSet = json::parse(DisneyCurl::GetJson("https://cd-static.bamgrid.com/dp-117731241344/sets/" + (std::string)data["set"]["refId"] + ".json"));
        ParseSet(refSet["data"]["CuratedSet"], renderer);
    }
    else{
        ParseSet(data["set"], renderer);
    }

}

DisneySet::~DisneySet() {
    tile_set.clear();
}

void DisneySet::Rotate(const short direction) {
    int shift = direction;
    if(direction==RIGHT){
        shift += tile_set.size();
    }
    std::rotate(tile_set.begin(), tile_set.begin()+shift, tile_set.end());
}

void DisneySet::PushBack(std::shared_ptr<DisneyTile> tile) {
    tile_set.push_back(tile);
}

int DisneySet::GetSize(){
    return tile_set.size();
}
void DisneySet::ParseSet(json data, SDL_Renderer* renderer) {
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
            std::string background_url = data["items"][j]["image"]["hero_collection"]["1.78"][image_key]["default"]["url"];
            std::string title = data["items"][j]["text"]["title"]["full"][image_key]["default"]["content"];
            std::string rating = data["items"][j]["ratings"][0]["value"];

            std::shared_ptr<DisneyImage> di = DisneyCurl::GetImage(tile_url);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, di->getSurf());
            di->setTexture(texture);
            std::shared_ptr<DisneyTile> tile = std::make_shared<DisneyTile>(di, background_url, rating, title);
            tile_set.push_back(tile);
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