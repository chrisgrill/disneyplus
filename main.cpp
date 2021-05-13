#include <SDL.h>
#include <SDL_ttf.h>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <algorithm>
#include "DisneyCurl.h"
#include "DisneySet.h"
#include "json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

int main(int argc, char ** argv)
{
    std::string home_json = DisneyCurl::GetJson("https://cd-static.bamgrid.com/dp-117731241344/home.json");
    json data = json::parse(home_json)["data"]["StandardCollection"]["containers"];
    std::cout << data.size() << std::endl;

    std::vector<DisneySet*> page_set;

    bool quit = false;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window * window = SDL_CreateWindow("Disney+",
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    /*--------------------------------- Text Setup------------------------------------------------------------------------*/
    TTF_Font *font = TTF_OpenFont("../fonts/DejaVuSans.ttf", 40);
    if(!font) {
        printf("Unable to load font: '%s'!\n"
               "SDL2_ttf Error: %s\n", "../fonts/DejaVuSans.ttf", TTF_GetError());
        return 0;
    }
    SDL_Color textColor           = { 0x00, 0x00, 0x00, 0xFF };
    SDL_Color textBackgroundColor = { 0xFF, 0xFF, 0xFF, 0xFF };
    SDL_Texture *text = NULL;
    SDL_Rect textRect = {5,5,200,140};
    SDL_Surface *textSurface = TTF_RenderText_Shaded(font, "Rated PG-13", textColor, textBackgroundColor);
    if(!textSurface) {
        printf("Unable to render text surface!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
    } else {
        // Create texture from surface pixels
        text = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!text) {
            printf("Unable to create texture from rendered text!\n"
                   "SDL2 Error: %s\n", SDL_GetError());
            return 0;
        }
    }
    /*--------------------------------------------------------- End Text -------------------------------------------------*/
    for(int i=0;i<data.size();i++){
        DisneySet* row_set = new DisneySet(data[i], renderer);
        if(row_set->getSize() > 0) page_set.push_back(row_set);
    }

    int x = 5;
    int y = 280;
    int selected_row = 0;
    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym){
                    case SDLK_RIGHT:
                        page_set[selected_row]->rotate(DisneySet::LEFT) ;
                        break;
                    case SDLK_LEFT:
                        page_set[selected_row]->rotate(DisneySet::RIGHT) ;
                        break;
                    case SDLK_UP:
                        std::rotate(page_set.begin(), page_set.begin()-1+page_set.size(), page_set.end());
                        break;
                    case SDLK_DOWN:

                        std::rotate(page_set.begin(), page_set.begin()+1, page_set.end());
                        break;
                }
                break;
        }

        SDL_Rect back_rec = {0,0,1280,720};
        //SDL_RenderCopy(renderer, back_tex, NULL, &back_rec);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_Rect border = {3,280+((int)selected_row*145),253,143};
        SDL_RenderFillRect(renderer, &border);

        for(int i=0; i < page_set.size(); i++){
            for(int j=0; j < page_set[i]->tile_set.size(); j++){
                int width = 250;
                int height = 140;
//                if(i==selected_row){
//                    if(j==0){
//                        width = 375;
//                        height = 210;
//                    }
//                }

                int b = ((int)page_set[i]->tile_set.size()*255);

                int offset = (x+(j * 255));
                SDL_Rect dstrect = { offset+b% b, y+(i*145), width, height };
                SDL_RenderCopy(renderer, page_set[i]->tile_set[j]->getTexture(), NULL, &dstrect);
            }
        }
        SDL_RenderCopy(renderer, text, NULL, &textRect);
        SDL_RenderPresent(renderer);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    page_set.clear();
    curl_global_cleanup();
    return 0;
}
