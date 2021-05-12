#include <SDL.h>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include "DisneyCurl.h"
#include "json/single_include/nlohmann/json.hpp"
using json = nlohmann::json;

int main(int argc, char ** argv)
{

    std::string home_json = DisneyCurl::GetJson("https://cd-static.bamgrid.com/dp-117731241344/home.json");
    json j = json::parse(home_json)["data"]["StandardCollection"]["containers"];
    std::cout << j.size() << std::endl;
    std::vector<std::shared_ptr<DisneyImage>> row_set;



    bool quit = false;
    SDL_Event event;
    DisneyImage background("../disney.jpg");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Disney+",
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    for(int i=0;i<j[0]["set"]["items"].size();i++){
        try{
            json item = j[0]["set"]["items"][i];
            std::string image_key;
            if(item["type"]=="DmcVideo"){
                image_key = "program";
            }
            else if(item["type"]=="DmcSeries"){
                image_key = "series";
            }
            std::string tile_url = j[0]["set"]["items"][i]["image"]["tile"]["1.78"][image_key]["default"]["url"];
            std::shared_ptr<DisneyImage> di = DisneyCurl::GetImage(tile_url);
            SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, di->getSurf());
            di->setTexture(texture);
            row_set.push_back(di);
        }
        catch (int e){
            std::cout << e << std::endl;
        }
        catch (...){
            std::cout << "Exception" << std::endl;
        }



    }
    //SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, di.surf);
    //SDL_Texture * texture2 = SDL_CreateTextureFromSurface(renderer, di2.getSurf());
    SDL_Texture * back_tex = SDL_CreateTextureFromSurface(renderer, background.getSurf());
    while (!quit)
    {
        SDL_WaitEvent(&event);
        SDL_Rect border = {3,3,253,112};
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym==SDLK_RIGHT){
                    border.x += 253;
                    break;
                }


        }



        SDL_Rect back_rec = {0,0,1280,720};
        SDL_RenderCopy(renderer, back_tex, NULL, &back_rec);
        //SDL_Rect border = {3,3,253,112};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderFillRect(renderer, &border);
        int x = 5;
        int y = 5;
        for(int i=0;i<row_set.size();i++){
            SDL_Rect dstrect = { x+(i*250), 5, 250, 109 };
            SDL_RenderCopy(renderer, row_set[i]->getTexture(), NULL, &dstrect);
        }
//        SDL_Rect dstrect = { 5, 5, 250, 109 };
//        SDL_Rect dstrect2 = { 263, 5, 250, 109 };
//        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
//        SDL_RenderCopy(renderer, texture2, NULL, &dstrect2);
        //SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    //SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    row_set.clear();

    /*------------------------------------------------------------------------------------------------------------*/




    curl_global_cleanup();
    return 0;
}
