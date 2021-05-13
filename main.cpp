#include <SDL.h>
#include <curl/curl.h>
#include <iostream>
#include <memory>
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
    DisneyImage background("../disney.jpg");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Disney+",
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    for(int i=0;i<data.size();i++){
        DisneySet* row_set = new DisneySet();
        //std::vector<std::shared_ptr<DisneyImage>> row_set;
        for(int j=0; j < data[i]["set"]["items"].size(); j++){
            try{

                json item = data[0]["set"]["items"][j];
                std::string image_key;
                if(item["type"]=="DmcVideo"){
                    image_key = "program";
                }
                else if(item["type"]=="DmcSeries"){
                    image_key = "series";
                }
                std::string tile_url = data[i]["set"]["items"][j]["image"]["tile"]["1.78"][image_key]["default"]["url"];
                std::shared_ptr<DisneyImage> di = DisneyCurl::GetImage(tile_url);
                SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, di->getSurf());
                di->setTexture(texture);
                row_set->push_back(di);
            }
            catch (int e){
                std::cout << e << std::endl;
            }
            catch (...){
                std::cout << "Exception" << std::endl;
            }

        }
        page_set.push_back(row_set);
    }


    SDL_Texture * back_tex = SDL_CreateTextureFromSurface(renderer, background.getSurf());
    int x = 5;
    int y = 5;
    unsigned int selected_row = 0;
    while (!quit)
    {
        SDL_WaitEvent(&event);
        SDL_Rect border = {3,3,253,143};
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
                        selected_row -= 1;
                        break;
                    case SDLK_DOWN:
                        selected_row += 1;
                        break;
                }
                break;
        }



        SDL_Rect back_rec = {0,0,1280,720};
        SDL_RenderCopy(renderer, back_tex, NULL, &back_rec);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderFillRect(renderer, &border);

        for(int i=0; i < page_set.size(); i++){
            for(int j=0; j < page_set[i]->tile_set.size(); j++){
                int b = ((int)page_set[i]->tile_set.size()*255);
                int offset = (x+(j * 255));
                SDL_Rect dstrect = { offset+b% b, y+(i*145), 250, 140 };
                SDL_RenderCopy(renderer, page_set[i]->tile_set[j]->getTexture(), NULL, &dstrect);
            }
        }


        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    page_set.clear();

    /*------------------------------------------------------------------------------------------------------------*/




    curl_global_cleanup();
    return 0;
}
