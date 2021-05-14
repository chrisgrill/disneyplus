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

void DisneySet::RenderText(std::string text, SDL_Rect rect, TTF_Font* font, SDL_Renderer* renderer) {
    SDL_Color textColor           = { 0xFF, 0xFF, 0xFF, 0xFF };
    SDL_Texture *text_tex = NULL;
    SDL_Surface *text_surf = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if(!text_surf) {
        printf("Unable to render text surface!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
    } else {
        // Create texture from surface pixels
        text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
        if (!text_tex) {
            printf("Unable to create texture from rendered text!\n"
                   "SDL2 Error: %s\n", SDL_GetError());

        }
        SDL_FreeSurface(text_surf);
    }
    SDL_RenderCopy(renderer, text_tex, NULL, &rect);
}
void DisneySet::LoadBackground(SDL_Renderer* renderer, TTF_Font* font) {
    std::shared_ptr<DisneyImage> background = DisneyCurl::GetImage(tile_set[0]->background_url);
    SDL_Texture * back_tex = SDL_CreateTextureFromSurface(renderer, background->getSurf());
    SDL_Rect back_rec = {0,0,1280,720};
    SDL_RenderCopy(renderer, back_tex, NULL, &back_rec);

    SDL_Rect rating_rect = {5,17,(int)tile_set[0]->rating.size()*12,16};
    SDL_Rect title_rect = {5,5,(int)tile_set[0]->title.size()*12,16};
    RenderText(tile_set[0]->title, title_rect, font, renderer);
    RenderText(tile_set[0]->rating, rating_rect, font, renderer);
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
