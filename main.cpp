#include <SDL.h>
#include <SDL_ttf.h>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <algorithm>
#include <future>
#include "DisneyCurl.h"
#include "DisneySet.h"
#include "json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

int main(int argc, char ** argv)
{


    // Initialize SDL and SDL_ttf components needed downstream.
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window * window = SDL_CreateWindow("Disney+",
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    TTF_Font *font = TTF_OpenFont("../fonts/DejaVuSans.ttf", 40);
    if(!font) {
        printf("Unable to load font: '%s'!\n"
               "SDL2_ttf Error: %s\n", "../fonts/DejaVuSans.ttf", TTF_GetError());
        return 0;
    }


    // Load data
    std::string home_json = DisneyCurl::GetJson("https://cd-static.bamgrid.com/dp-117731241344/home.json");
    json data = json::parse(home_json)["data"]["StandardCollection"]["containers"];
    std::cout << data.size() << std::endl;

    std::vector<std::shared_ptr<DisneySet>> page_set;
    for(int i=0;i<data.size();i++){
        std::shared_ptr<DisneySet> row_set = std::make_shared<DisneySet>(data[i], renderer);
        if(row_set->GetSize() > 0) page_set.push_back(row_set);
    }

    // SDL event loop
    bool quit = false;
    SDL_Event event;
    int x = 5;
    int y = 420;
    // Load first background image
    page_set[0]->LoadBackground(renderer, font);
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
                        page_set[0]->Rotate(DisneySet::LEFT) ;
                        break;
                    case SDLK_LEFT:
                        page_set[0]->Rotate(DisneySet::RIGHT) ;
                        break;
                    case SDLK_UP:
                        std::rotate(page_set.begin(), page_set.begin()-1+page_set.size(), page_set.end());
                        break;
                    case SDLK_DOWN:

                        std::rotate(page_set.begin(), page_set.begin()+1, page_set.end());
                        break;
                }

                page_set[0]->LoadBackground(renderer, font);
                break;
        }

        // Draw white rectangle that acts as selected tile indicator
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_Rect border = {3,420,253,143};
        SDL_RenderFillRect(renderer, &border);

        // Render tile sets
        for(int i=0; i < page_set.size(); i++){
            for(int j=0; j < page_set[i]->tile_set.size(); j++){
                int width = 250;
                int height = 140;

                int b = ((int)page_set[i]->tile_set.size()*255);

                int offset = (x+(j * 255));
                SDL_Rect dstrect = { offset+b% b, y+(i*145), width, height };
                SDL_RenderCopy(renderer, page_set[i]->tile_set[j]->image->getTexture(), NULL, &dstrect);
            }
        }
        SDL_RenderPresent(renderer);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    page_set.clear();
    curl_global_cleanup();
    return 0;
}
